#pragma once

#define LOD_COUNT 4

using namespace Common;

#ifdef RENDERENGINE_INTERFACE
#include "CommonTypes.hpp"

class RenderObject
{
public:
	void SetGeometry(size_t lod, size_t vertCount, Vertex * verts);
	void SetPosition(Vector3<float> position);
	Vector3<float> GetPosition();
};
#else
#include "Includes.hpp"

class RenderObject
{
public:
	RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
	~RenderObject(void);

	void Render(size_t lod);

	void SetGeometry(size_t lod, size_t vertCount, Vertex * verts);

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition();

private:
	LPDIRECT3DDEVICE9 mDevice;
	LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

	size_t mVertCount[LOD_COUNT];
	LPDIRECT3DVERTEXBUFFER9 mVertBuffer[LOD_COUNT];

	D3DXVECTOR3 mPosition;
	D3DXMATRIX mTransform;

	bool mHasGeometry;
};
#endif
