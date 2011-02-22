#include "RenderEngine.hpp"


RenderEngine::RenderEngine(HWND hWnd)
    : mFrameTime(0.0f)
{
    mObject = Direct3DCreate9(D3D_SDK_VERSION);
    if ( !mObject )
    {
        throw std::runtime_error("Error creating D3D9 object.");
    }

    D3DPRESENT_PARAMETERS pp;
    ZeroMemory(&pp, sizeof(pp));
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferCount = 1;
    pp.BackBufferWidth = 800;
    pp.BackBufferHeight = 600;
    pp.BackBufferFormat = D3DFMT_X8R8G8B8;
    pp.AutoDepthStencilFormat = D3DFMT_D24S8;
    pp.EnableAutoDepthStencil = TRUE;
    pp.hDeviceWindow = hWnd;
    pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    pp.MultiSampleType = D3DMULTISAMPLE_NONE;
    pp.MultiSampleQuality = 0;

    HRESULT hrDev = mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &mDevice);
    if ( FAILED(hrDev) )
    {
        throw std::runtime_error("Error creating D3D9 device.");
    }

    D3DVERTEXELEMENT9 vertElems[] = 
    {
        {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
        {0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
        {0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
        D3DDECL_END()
    };

    HRESULT hrVD = mDevice->CreateVertexDeclaration(vertElems, &mVertDecl);

    if ( FAILED(hrVD) )
    {
        throw std::runtime_error("Error creating vertex decl.");
    }

    D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(70.0f), 640.0f/480.0f, 1.0f, 1000.0f);
    mDevice->SetTransform(D3DTS_PROJECTION, &mProj);

    mCamera = new Camera();

    D3DXCreateFontA(mDevice, 10, 8, 1, 0, FALSE, NULL, NULL, NULL, NULL, "Courier", &mFont);
    LONG left = 5;
    LONG top = 5;
    mTextRect.left = left; mTextRect.right = left + 630;
    mTextRect.top = top; mTextRect.bottom = top + 150;

    HRESULT hrTex = D3DXCreateTextureFromFile(mDevice, L"texture.png", &mLandTexture);

    LPD3DXBUFFER shaderErrors;
    HRESULT hrShader = D3DXCreateEffectFromFile(mDevice, L"render.fx", NULL, NULL, NULL, NULL, &mDefaultShader, &shaderErrors); 
    if ( FAILED(hrShader) )
    {
        MessageBoxA(hWnd, (char*)shaderErrors->GetBufferPointer(), "Shader Error", NULL);
    }

    HRESULT hrValid = mDefaultShader->FindNextValidTechnique(NULL, &mShader_Technique);
    mDefaultShader->SetTechnique(mShader_Technique);

    mShader_MVPMatrix = mDefaultShader->GetParameterBySemantic(NULL, "MATRIX_MVP");
    mShader_BaseTexture = mDefaultShader->GetParameterBySemantic(NULL, "TEXTURE_BASE");

    mLastTicks = mTicks = GetTickCount();

    this->CreateOcclusionData();
}

RenderEngine::~RenderEngine(void)
{
    delete mCamera;

    mVertDecl->Release();
    mDevice->Release();
    mObject->Release();
}

void RenderEngine::CreateOcclusionData()
{
    STEP_IN;

    /*
    D3DVERTEXELEMENT9 vertElemsOccl[] = 
    {
        {0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
        D3DDECL_END()
    };

    HRESULT hrVD = mDevice->CreateVertexDeclaration(vertElemsOccl, &mVertOcclDecl);

    if ( FAILED(hrVD) )
    {
        throw std::runtime_error("Error creating occlusion vertex decl.");
    }

    HRESULT hrOG = mDevice->CreateVertexBuffer(36 * sizeof(fvec3), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &mOcclGeometry, NULL);

    float CHUNK_SIZE = 80.0f;

    fvec3 occlGeom[] = 
    {
        // Face 1
        fvec3(      0.0f,       0.0f,       0.0f),
        fvec3(      0.0f,       0.0f, CHUNK_SIZE),
        fvec3(      0.0f, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(      0.0f, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(      0.0f, CHUNK_SIZE,       0.0f),
        fvec3(      0.0f,       0.0f,       0.0f),
        // Face 2
        fvec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(CHUNK_SIZE,       0.0f, CHUNK_SIZE),
        fvec3(CHUNK_SIZE,       0.0f,       0.0f),
        fvec3(CHUNK_SIZE,       0.0f,       0.0f),
        fvec3(CHUNK_SIZE, CHUNK_SIZE,       0.0f),
        fvec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE),
        // Face 3
        fvec3(CHUNK_SIZE,       0.0f,       0.0f),
        fvec3(CHUNK_SIZE,       0.0f, CHUNK_SIZE),
        fvec3(      0.0f,       0.0f, CHUNK_SIZE),
        fvec3(      0.0f,       0.0f, CHUNK_SIZE),
        fvec3(      0.0f,       0.0f,       0.0f),
        fvec3(CHUNK_SIZE,       0.0f,       0.0f),
        // Face 4
        fvec3(      0.0f, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(CHUNK_SIZE, CHUNK_SIZE,       0.0f),
        fvec3(CHUNK_SIZE, CHUNK_SIZE,       0.0f),
        fvec3(      0.0f, CHUNK_SIZE,       0.0f),
        fvec3(      0.0f, CHUNK_SIZE, CHUNK_SIZE),
        // Face 5
        fvec3(CHUNK_SIZE, CHUNK_SIZE,       0.0f),
        fvec3(CHUNK_SIZE,       0.0f,       0.0f),
        fvec3(      0.0f,       0.0f,       0.0f),
        fvec3(      0.0f,       0.0f,       0.0f),
        fvec3(      0.0f, CHUNK_SIZE,       0.0f),
        fvec3(CHUNK_SIZE, CHUNK_SIZE,       0.0f),
        // Face 6
        fvec3(      0.0f,       0.0f, CHUNK_SIZE),
        fvec3(CHUNK_SIZE,       0.0f, CHUNK_SIZE),
        fvec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(      0.0f, CHUNK_SIZE, CHUNK_SIZE),
        fvec3(      0.0f,       0.0f, CHUNK_SIZE),
    };

    VOID * vbVerts;

    HRESULT hrVB = mOcclGeometry->Lock(0, 36 * sizeof(fvec3), &vbVerts, 0);

    if ( FAILED(hrVB) )
    {
        throw std::runtime_error("Error locking occlusion vertex buffer.");
    }

    memcpy(vbVerts, occlGeom, 36 * sizeof(fvec3));

    mOcclGeometry->Unlock();
    */

    D3DXCreateTexture(mDevice, 256, 256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &mOcclusionTexture);
    
    D3DSURFACE_DESC desc;
    mOcclusionTexture->GetSurfaceLevel(0, &mOcclusionSurface);
    mOcclusionSurface->GetDesc(&desc);

    D3DXCreateRenderToSurface(mDevice, desc.Width, desc.Height, D3DFMT_X8R8G8B8, TRUE, D3DFMT_D24S8, &mOcclusionRTS);

    STEP_OUT;
}

RenderObject * RenderEngine::CreateRenderObject()
{
    RenderObject * obj = new RenderObject(mDevice, mVertDecl);
    //obj->SetOcclusionData(mOcclGeometry, mVertOcclDecl);
    mRenderObjects.push_back(obj);
    return obj;
}

void RenderEngine::DestroyRenderObject(RenderObject * object)
{
    std::list<RenderObject*>::iterator ittr = mRenderObjects.begin();
    while ( ittr != mRenderObjects.end() )
    {
        if ( (*ittr) == object )
        {
            mRenderObjects.erase(ittr);
            return;
        }
        ++ittr;
    }
}

Camera * RenderEngine::GetCamera()
{
    STEP_IN;

    return mCamera;

    STEP_OUT;
}

float RenderEngine::GetFrameDelta()
{
    STEP_IN;

    return mFrameTime;

    STEP_OUT;
}

fvec3 camPos;
DWORD sortOps;
bool DistanceSort(RenderObject * a, RenderObject * b)
{
    STEP_IN;

    using namespace Common::Math;

    ++sortOps;

    float adist = DistSq(a->GetPosition(), camPos);
    float bdist = DistSq(b->GetPosition(), camPos);
    return ( adist < bdist );

    STEP_OUT;
}

char msg[256];
DWORD fpsTicks, frames;
D3DXMATRIX lastPos;

void RenderEngine::Render()
{
    STEP_IN;

    using namespace Common::Math;

    mDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
    mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    mDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

    int sortTime = -1, occlTime = -1;
    DWORD occlOps = 0, drawOps = 0; sortOps = 0;

    camPos = mCamera->GetPosition();
    D3DXMATRIX * nowPos = (D3DXMATRIX*)mCamera->GetViewMatrix();

    if ( lastPos != *nowPos )
    {
        mDevice->SetTransform(D3DTS_VIEW, nowPos);

        // Depth-sort objects
        sortTime = GetTickCount();
        mRenderObjects.sort(DistanceSort);
        sortTime = GetTickCount() - sortTime;

        std::list<RenderObject*>::iterator lodIttr = mRenderObjects.begin();
        while ( lodIttr != mRenderObjects.end() )
        {
            float dist = DistSq((*lodIttr)->GetPosition(), camPos);
            (*lodIttr)->SetLOD(dist / ( 1000000 / LOD_COUNT ));
            ++lodIttr;
        }

        // Perform occlusion checking
        occlOps = 0;
        occlTime = GetTickCount();
        if ( SUCCEEDED( mOcclusionRTS->BeginScene(mOcclusionSurface, NULL) ) )
        {
            mDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);    

            std::list<RenderObject*>::iterator occlIttr = mRenderObjects.begin();
            while ( occlIttr != mRenderObjects.end() )
            {
                (*occlIttr)->UpdateOcclusion();

                ++occlOps;
                ++occlIttr;
            }

            mOcclusionRTS->EndScene(NULL);
        }
        occlTime = GetTickCount() - occlTime;

        lastPos = *nowPos;
    }

    DWORD drawTime = GetTickCount();
    mDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(32, 64, 72), 1.0f, 0);    

    mDefaultShader->SetTexture(mShader_BaseTexture, mLandTexture);

    size_t tris = 0;

    UINT passes;

    mDevice->BeginScene();

    std::list<RenderObject*>::iterator objIttr = mRenderObjects.begin();
    while ( objIttr != mRenderObjects.end() )
    {
        // Find distance from the camera
        if ( (*objIttr)->GetVisible() )
        {
            int32 lod = (*objIttr)->GetLOD();

            tris += (*objIttr)->GetVertCount(lod) / 3;

            D3DXMATRIX mvp = ((D3DXMATRIX)(*objIttr)->GetTransform()) * (*(D3DXMATRIX*)(mCamera->GetViewMatrix())) * mProj;

            mDefaultShader->SetMatrix(mShader_MVPMatrix, &mvp);

            mDefaultShader->Begin(&passes, NULL);
            mDefaultShader->BeginPass(0);

            (*objIttr)->Render(lod);

            mDefaultShader->EndPass();
            mDefaultShader->End();

            ++drawOps;
        }

        ++objIttr;
    }
    drawTime = GetTickCount() - drawTime;

    mDevice->EndScene();

    mDevice->BeginScene();
    mFont->DrawTextA(NULL, msg, strlen(msg), &mTextRect, DT_LEFT, D3DCOLOR_ARGB(0xFF, 0xFF, 0xFF, 0xFF));
    mDevice->EndScene();

    mDevice->Present(NULL, NULL, NULL, NULL);

    mLastTicks = mTicks;
    mTicks = GetTickCount();
    mFrameTime = ( mTicks - mLastTicks ) / 1000.0f;

    ++frames;
    if ( mTicks > fpsTicks )
    {        
        float trueFPS = frames / ( ( 1000.0f + ( mTicks - fpsTicks ) ) / 1000.0f );
        sprintf(msg, "Box Game [Native DirectX]; %u tris @ %.2f FPS\nTiming (ticks/ops): %d/%u sort; %d/%u occl; %u/%u draw\0", tris, trueFPS, sortTime, sortOps, occlTime, occlOps, drawTime, drawOps);

        frames = 0;
        fpsTicks = mTicks + 1000;
    }

    STEP_OUT;
}
