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

#define BOOL_NULL   (bool*)   NULL
#define INT_NULL    (int*)    NULL
#define FLOAT_NULL  (float*)  NULL
#define STRING_NULL (string*) NULL