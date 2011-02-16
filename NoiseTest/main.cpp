
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <queue>
#include <string>

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

#define D3DX_CONVERSION_FUNCS
#include "CommonTypes.hpp"
#include "CoherentNoise.hpp"

using namespace std;

LPDIRECT3D9 mObject;
LPDIRECT3DDEVICE9 mDevice;
size_t threadsDone;
std::queue<wstring> msgQueue;
//fstream cout;

Common::fvec3 heat[] =
{
	Common::fvec3(0.00f, 0.00f, 0.00f), // Seabed
	Common::fvec3(0.12f, 0.15f, 0.39f),
	Common::fvec3(0.21f, 0.28f, 0.49f),
	Common::fvec3(0.31f, 0.38f, 0.59f),
	Common::fvec3(0.32f, 0.46f, 0.58f),
	Common::fvec3(0.33f, 0.54f, 0.57f),
	Common::fvec3(0.56f, 0.68f, 0.61f), // Water end - 7
	Common::fvec3(0.53f, 0.52f, 0.42f), 
	Common::fvec3(0.65f, 0.63f, 0.54f), // Sand - 9
	Common::fvec3(0.63f, 0.64f, 0.55f),
	Common::fvec3(0.52f, 0.70f, 0.60f), // Grass - 11
	Common::fvec3(0.17f, 0.35f, 0.16f), // 
	Common::fvec3(0.17f, 0.35f, 0.16f), // Forest - 13
	Common::fvec3(0.41f, 0.47f, 0.39f), // Hill - 14
	Common::fvec3(0.57f, 0.58f, 0.56f), // 
	Common::fvec3(0.69f, 0.71f, 0.71f), // Mountain - 16
	Common::fvec3(0.80f, 0.85f, 0.85f), // Snow - 17
};

struct NoiseDesc
{
	wchar_t name[16];
	Common::Noise::CoherentNoise * cnoise, * pnoise;
	float sPower, slice;
	size_t nOctaves, sOctaves;
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

	float nval = desc->cnoise->GetOctavePoint(Common::fvec3(pTexCoord->x, pTexCoord->y, desc->slice), desc->nOctaves);
	float sval = desc->pnoise->GetOctavePoint(Common::fvec3(pTexCoord->x, pTexCoord->y, desc->slice), desc->sOctaves);
	sval = pow(sval, desc->sPower+1.0f);
	//nval += sval;

	float index = nval * 17.0f;

	size_t coldIndex = (size_t)floor(index);
	size_t hotIndex = (coldIndex<16) ? coldIndex + 1 : coldIndex;

	Common::fvec3 smooth = Common::Math::Lerp(heat[coldIndex], heat[hotIndex], (float)(index - coldIndex));
	float shift = abs(cos((float)(index - coldIndex) * 20.0f));
	Common::fvec3 ridged = Common::Math::Lerp(heat[hotIndex], heat[coldIndex], shift);
	
	*pOut = Common::Math::CosInterp(smooth, ridged, sval);
}

DWORD WINAPI MakeTexture(LPVOID param)
{
	cout << "Thread " << GetCurrentThreadId() << ": Starting thread callback.\n";

	NoiseDesc * desc = (NoiseDesc*)param;

	LPDIRECT3DTEXTURE9 noiseTex;
	HRESULT retv = D3DXCreateTexture(mDevice, 1024, 1024, 1, D3DUSAGE_DYNAMIC, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &noiseTex);

	if ( FAILED(retv) )
	{
		cout << "Thread " << GetCurrentThreadId() << ": Error creating texture.\n";
		return 1;
	}

	cout << "Thread " << GetCurrentThreadId() << ": Starting texture fill.\n";

	retv = D3DXFillTexture(noiseTex, &DrawNoise, desc);

	if ( FAILED(retv) )
	{
		cout << "Thread " << GetCurrentThreadId() << ": Error filling texture.\n";
		return 1;
	}

	retv = D3DXSaveTextureToFile(desc->name, D3DXIFF_PNG, noiseTex, NULL);

	if ( FAILED(retv) )
	{
		cout << "Thread " << GetCurrentThreadId() << ": Error saving texture.\n";
		return 1;
	}

	delete desc->pnoise;
	delete desc->cnoise;
	delete desc;

	cout << "Thread " << GetCurrentThreadId() << ": Completed successfully.\n";
	return 0;
}

int main(int argc, char * argv[])
{
	//cout.open(L"noisetest.log");

	mObject = Direct3DCreate9(D3D_SDK_VERSION);
	if ( !mObject )
	{
		cout << "Error creating D3D9 object.\n";
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
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.MultiSampleQuality = 0;

	HRESULT hrDev = mObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &mDevice);
	if ( FAILED(hrDev) )
	{
		cout << "Error creating D3D9 device.\n";
	}

	cout << "Starting texture generation.\n";

	CreateDirectory(L"maps", 0);
	SetCurrentDirectory(L"maps");

	size_t fileslice = 0;
	srand(GetTickCount());

	for ( size_t batch = 0; batch < 16; ++batch )
	{
		cout << "Starting batch " << batch << "\n";

		threadsDone = 0;
		HANDLE threads[4];

		for ( size_t slice = 0; slice < 4; ++slice )
		{
			NoiseDesc * desc = new NoiseDesc();

			wsprintf(desc->name, L"slice_%d.png", ++fileslice);
			desc->cnoise = new Common::Noise::CoherentNoise(rand());
			desc->pnoise = new Common::Noise::CoherentNoise(rand());
			desc->sPower = (float)( rand() % 3 );
			desc->nOctaves = 4 + ( rand() % 5 );
			desc->sOctaves = 1 + ( rand() % 4 );
			desc->slice = ( rand() % 64 ) / 64.0f;

			DWORD tID;
			threads[slice] = CreateThread(NULL, 0, &MakeTexture, desc, 0, &tID);

			cout << "Thread " << tID << " processing image \"" << desc->name << "\".\n";
		}

		WaitForMultipleObjects(4, threads, TRUE, INFINITE);

		for ( size_t slice = 0; slice < 4; ++slice )
		{
			CloseHandle(threads[slice]);
		}
	}

	//cout.close();
}
