#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

class RenderObject
{
public:
	RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
	~RenderObject(void);

	void Render();

	void SetGeometry(int vertCount, Vertex * verts);

private:
	int mVertCount;

	LPDIRECT3DDEVICE9 mDevice;
	LPDIRECT3DVERTEXBUFFER9 mVertBuffer;
	LPDIRECT3DVERTEXDECLARATION9 mVertDecl;
};
