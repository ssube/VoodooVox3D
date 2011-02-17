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
	pp.BackBufferWidth = 640;
	pp.BackBufferHeight = 480;
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
		{0, 36, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
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

	mShader_MVPMatrix = mDefaultShader->GetParameterByName(NULL, "ModelViewProj");
	mShader_BaseTexture = mDefaultShader->GetParameterByName(NULL, "BaseTexture");

	mLastTicks = mTicks = GetTickCount();
}

RenderEngine::~RenderEngine(void)
{
	delete mCamera;

	mVertDecl->Release();
	mDevice->Release();
	mObject->Release();
}


RenderObject * RenderEngine::CreateRenderObject()
{
	RenderObject * obj = new RenderObject(mDevice, mVertDecl);
	mRenderObjects.push_back(obj);
	return obj;
}

void RenderEngine::DestroyRenderObject(RenderObject * object)
{
	vector<RenderObject*>::iterator ittr = mRenderObjects.begin();
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
	return mCamera;
}

float RenderEngine::GetFrameDelta()
{
	return mFrameTime;
}

char msg[256];
DWORD fpsTicks, frames;

void RenderEngine::Render()
{
	D3DXVECTOR3 camPos = mCamera->GetPosition();

	mDevice->SetTransform(D3DTS_VIEW, mCamera->GetViewMatrix());

	mDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(32, 64, 72), 1.0f, 0);	

	mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	mDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	mDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//mDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//mDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//mDevice->SetTexture(0, mLandTexture);
	mDefaultShader->SetTexture(mShader_BaseTexture, mLandTexture);

	mDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	size_t tris = 0;

	UINT passes;

	vector<RenderObject*>::iterator ittr = mRenderObjects.begin();
	while ( ittr != mRenderObjects.end() )
	{
		// Find distance from the camera
		D3DXVECTOR3 objPos = (*ittr)->GetPosition();
		D3DXVECTOR3 diff;
		float dist = D3DXVec3LengthSq(D3DXVec3Subtract(&diff, &camPos, &objPos));
		int lod = min(dist / ( 1000000 / LOD_COUNT ), LOD_COUNT - 1);

		if ( lod < 0 )
		{
			// skip
		} else {
			tris += (*ittr)->GetVertCount(lod);

			D3DXMATRIX mvp = *(*ittr)->GetTransform();
			D3DXMatrixMultiply(&mvp, &mvp, (mCamera->GetViewMatrix()));
			D3DXMatrixMultiply(&mvp, &mvp, &mProj);

			mDefaultShader->SetMatrix(mShader_MVPMatrix, &mvp);

			mDefaultShader->Begin(&passes, NULL);
			mDefaultShader->BeginPass(0);

			(*ittr)->Render(lod);

			mDefaultShader->EndPass();
			mDefaultShader->End();
		}

		++ittr;
	}

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
		sprintf(msg, "Box Game [Native DirectX]; %u tris @ %u FPS\0", tris, frames);

		frames = 0;
		fpsTicks = mTicks + 1000;
	}
}
