#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

class RenderObject
{
public:
	RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
	~RenderObject(void);

	void Render();

	void SetGeometry(size_t vertCount, Vertex * verts);

private:
	size_t mVertCount;

	LPDIRECT3DDEVICE9 mDevice;
	LPDIRECT3DVERTEXBUFFER9 mVertBuffer;
	LPDIRECT3DVERTEXDECLARATION9 mVertDecl;
	D3DXMATRIX mTransform;
};
