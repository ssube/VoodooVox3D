#pragma  once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <windows.h>

#ifdef _DEBUG
#	define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>

// Common includes
#define D3DX_CONVERSION_FUNCS
#include "CommonTypes.hpp"

//#define TOD3DXVEC2(vec) D3DXVECTOR2(vec.x, vec.y)
//#define TOD3DXVEC3(vec) D3DXVECTOR3(vec.x, vec.y, vec.z)
//#define TOD3DXVEC4(vec) D3DXVECTOR4(vec.x, vec.y, vec.z, vec.w)
