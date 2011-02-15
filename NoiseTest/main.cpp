
#include <stdlib.h>
#include <windows.h>

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

#define D3DX_CONVERSION_FUNCS
#include "CommonTypes.hpp"
#include "CoherentNoise.hpp"

LPDIRECT3D9 mObject;
LPDIRECT3DDEVICE9 mDevice;
Common::Noise::CoherentNoise * cnoise, * pnoise;

Common::fvec3 heat[] =
{
	Common::fvec3(0.00f, 0.00f, 0.00f), // Seabed
	Common::fvec3(0.00f, 0.00f, 0.20f),
	Common::fvec3(0.00f, 0.00f, 0.50f),
	Common::fvec3(0.00f, 0.20f, 0.70f),
	Common::fvec3(0.00f, 0.50f, 1.00f), // 5
	Common::fvec3(0.10f, 0.70f, 1.00f), // Water end
	Common::fvec3(0.80f, 0.80f, 0.70f), 
	Common::fvec3(0.80f, 0.80f, 0.70f), // Sand
	Common::fvec3(0.73f, 0.82f, 0.64f),
	Common::fvec3(0.52f, 0.80f, 0.60f), // Grass
	Common::fvec3(0.2f, 0.5f, 0.3f),
	Common::fvec3(0.7f, 0.3f, 0.2f),
	Common::fvec3(1.0f, 1.0f, 1.0f)
};

struct NoiseDesc
{
	float sPower;
	size_t nOctaves, sOctaves;
	bool lerpMode;
};

VOID WINAPI DrawNoise
(
 D3DXVECTOR4 * pOut,
 CONST D3DXVECTOR2* pTexCoord, 
 CONST D3DXVECTOR2* pTexelSize, 
 LPVOID pData
 )
{
	NoiseDesc * desc = (NoiseDesc*)pData;

	float nval = cnoise->GetOctavePoint(Common::fvec3(pTexCoord->x, pTexCoord->y, 0.0f), desc->nOctaves);
	float sval = pnoise->GetOctavePoint(Common::fvec3(pTexCoord->x, pTexCoord->y, 0.0f), desc->sOctaves);
	sval = pow(sval, desc->sPower);

	float index = nval * 5.0f;

	size_t coldIndex = (size_t)floor(index);
	size_t hotIndex = (coldIndex<4) ? coldIndex + 1 : coldIndex;

	Common::fvec3 smooth = Common::Math::Lerp(heat[coldIndex], heat[hotIndex], (float)(index - coldIndex));
	float shift = abs(cos((float)(index - coldIndex) * 20.0f));
	Common::fvec3 ridged = Common::Math::Lerp(heat[hotIndex], heat[coldIndex], shift);

	if ( desc->lerpMode )
	{
		*pOut = Common::Math::CosLerp(smooth, ridged, sval);
	} else {
		*pOut = Common::Math::Lerp(smooth, ridged, sval);
	}
}


LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	// Register the window class
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"NoiseTest", NULL};

	RegisterClassEx(&wc);

	// Create the window
	HWND hWnd = CreateWindow(L"NoiseTest", L"Box Game [DirectX Native]", WS_OVERLAPPEDWINDOW, 100, 300, 640, 480, GetDesktopWindow(), NULL, wc.hInstance, NULL);

	mObject = Direct3DCreate9(D3D_SDK_VERSION);
	if ( !mObject )
	{
		MessageBox(NULL, L"Error creating D3D9 object.", L"Error", NULL);
	}

	D3DPRESENT_PARAMETERS pp;
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferCount = 1;
	pp.BackBufferWidth = 640;
	pp.BackBufferHeight = 480;
	pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	pp.EnableAutoDepthStencil = FALSE;
	//pp.hDeviceWindow = hWnd;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.MultiSampleQuality = 0;

	HRESULT hrDev = mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &mDevice);
	if ( FAILED(hrDev) )
	{
		MessageBox(NULL, L"Error creating D3D9 device.", L"Error", NULL);
	}

	srand(GetTickCount());

	LPDIRECT3DTEXTURE9 noiseTex;
	HRESULT retv = D3DXCreateTexture(mDevice, 1024, 1024, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &noiseTex);

	if ( FAILED(retv) )
	{
		MessageBox(NULL, L"Error creating texture.", L"Error", NULL);
	}

	for ( size_t slice = 0; slice < 16; ++slice )
	{
		wchar_t name[16];
		wsprintf(name, L"slice_%d.png", slice);

		cnoise = new Common::Noise::CoherentNoise(rand());
		pnoise = new Common::Noise::CoherentNoise(rand());
		NoiseDesc * desc = new NoiseDesc();

		desc->sPower = ( ( rand() % 16 ) / 4.0f ) + 0.5f;
		desc->nOctaves = 2 + ( rand() % 8 );
		desc->sOctaves = 1 + ( rand() % 4 );
		desc->lerpMode = ( rand() % 3 ) != 0;

		retv = D3DXFillTexture(noiseTex, &DrawNoise, desc);

		if ( FAILED(retv) )
		{
			MessageBox(NULL, L"Error filling texture.", L"Error", NULL);
		}

		retv = D3DXSaveTextureToFile(name, D3DXIFF_PNG, noiseTex, NULL);

		if ( FAILED(retv) )
		{
			MessageBox(NULL, L"Error saving texture.", L"Error", NULL);
		}

		delete desc;
		delete cnoise;
		delete pnoise;
	}
}
