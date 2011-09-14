#include "RenderEngine.hpp"

#include "Camera.hpp"
#include "RenderObject.hpp"

#include "WorldDefs.hpp"

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


    mCamera = new Camera(1.0f, 1000.0f, 70.0f, 800.0f/600.0f);
    mDevice->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)mCamera->GetProjMatrix());

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

    mSceneTree = new Common::Trees::Octree(0.0f, WORLD_SIZE);
}

RenderEngine::~RenderEngine(void)
{
    delete mCamera;
    delete mSceneTree;

    mVertDecl->Release();
    mDevice->Release();
    mObject->Release();
}

void RenderEngine::CreateOcclusionData()
{

    D3DXCreateTexture(mDevice, 256, 256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &mOcclusionTexture);
    
    D3DSURFACE_DESC desc;
    mOcclusionTexture->GetSurfaceLevel(0, &mOcclusionSurface);
    mOcclusionSurface->GetDesc(&desc);

    D3DXCreateRenderToSurface(mDevice, desc.Width, desc.Height, D3DFMT_X8R8G8B8, TRUE, D3DFMT_D24S8, &mOcclusionRTS);

    }

RenderObject * RenderEngine::CreateRenderObject()
{
    return new RenderObject(mDevice, mVertDecl);
}

void RenderEngine::DestroyRenderObject(RenderObject * object)
{
//     std::list<RenderObject*>::iterator ittr = mRenderObjects.begin();
//     while ( ittr != mRenderObjects.end() )
//     {
//         if ( (*ittr) == object )
//         {
//             mRenderObjects.erase(ittr);
//             return;
//         }
//         ++ittr;
//     }
}

void RenderEngine::AddRenderObject(RenderObject * object)
{
    mSceneTree->AddItem((GenericObject*)object);
    //mRenderObjects.push_back(object);
}

void RenderEngine::RemoveRenderObject(RenderObject * object)
{
    //mRenderObjects.remove_if([&](RenderObject * obj){ return (obj == object); });
    mSceneTree->RemoveItem((GenericObject*)object);
}

Camera * RenderEngine::GetCamera()
{
    return mCamera;
}

float RenderEngine::GetFrameDelta()
{
    return mFrameTime;
}

fvec3 camPos;
DWORD sortOps;
bool DistanceSort(RenderObject * a, RenderObject * b)
{ 
    using namespace Common::Math;

    ++sortOps;

    float adist = DistSq(a->GetPosition(), camPos);
    float bdist = DistSq(b->GetPosition(), camPos);
    return ( adist < bdist );
}

char msg[256];
DWORD fpsTicks, frames;
D3DXMATRIX lastPos;

void RenderEngine::Render()
{
    using namespace Common::Math;

    mDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
    mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    mDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

    int sortTime = -1, occlTime = -1;
    DWORD cullTime = 0, occlOps = 0, drawOps = 0; sortOps = 0;

    camPos = mCamera->GetPosition();
    D3DXMATRIX * nowPos = (D3DXMATRIX*)mCamera->GetViewMatrix();

    if ( lastPos != *nowPos )
    {
        mDevice->SetTransform(D3DTS_VIEW, nowPos);

        // Get the octree objects
        cullTime = GetTickCount();
        mSceneTree->Cull(mCamera, (std::list<GenericObject*>*)&mRenderObjects);
        cullTime = GetTickCount() - cullTime;

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

            tris += (*objIttr)->GetVertCount(0) / 3;

            D3DXMATRIX mvp = 
                ((D3DXMATRIX)(*objIttr)->GetTransform()) * 
                (*(D3DXMATRIX*)(mCamera->GetViewMatrix())) * 
                (*(D3DXMATRIX*)(mCamera->GetProjMatrix()));

            mDefaultShader->SetMatrix(mShader_MVPMatrix, (D3DXMATRIX*)&mvp);

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
        sprintf(msg, "Box Game [Native DirectX]; %u tris @ %.2f FPS\nTiming (ticks/ops): %u cull; %d/%u sort; %d/%u occl; %u/%u draw\0", tris, trueFPS, cullTime, sortTime, sortOps, occlTime, occlOps, drawTime, drawOps);

        frames = 0;
        fpsTicks = mTicks + 1000;
    }
}
