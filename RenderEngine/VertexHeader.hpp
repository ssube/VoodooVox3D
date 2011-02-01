#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

struct Vertex
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

extern LPDIRECT3DVERTEXDECLARATION9 VertexDecl;
