#pragma  once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <windows.h>

#ifdef _DEBUG
#    define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9effect.h>

#include <dinput.h>

// Common includes
#define COMMON_IMPORT
#define D3DX_CONVERSION_FUNCS
#include "CommonTypes.hpp"
#include "Math.hpp"
#include "VectorMath.hpp"
