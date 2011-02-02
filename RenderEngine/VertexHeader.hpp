#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

struct Vertex
{
	FLOAT x, y, z;
	FLOAT nx, ny, nz;
	FLOAT tu, tv, tw;
	DWORD color;
};

extern LPDIRECT3DVERTEXDECLARATION9 VertexDecl;
