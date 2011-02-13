#pragma once

#include "Includes.hpp"
#include "VertexHeader.hpp"

class RenderObject
{
public:
	RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
	~RenderObject(void);

	void Render(size_t lod);

	void SetGeometry(size_t lod, size_t vertCount, Vertex * verts);
	//void SetTransform(D3DXMATRIX trans);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition();

private:
	size_t mVertCount[LOD_COUNT];

	LPDIRECT3DDEVICE9 mDevice;
	LPDIRECT3DVERTEXBUFFER9 mVertBuffer[LOD_COUNT];
	LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

	D3DXVECTOR3 mPosition;
	D3DXMATRIX mTransform;

	bool mHasGeometry;
};
