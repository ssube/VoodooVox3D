#pragma once

#define LOD_COUNT 4

using namespace Common;

#ifdef RENDERENGINE_INTERFACE
#include "CommonTypes.hpp"

class RenderObject
{
public:
	virtual void Render(size_t lod);

	virtual size_t GetVertCount(size_t lod);
	virtual void SetGeometry(size_t lod, size_t vertCount, Vertex * verts);

	virtual void SetPosition(fvec3 position);
	virtual fvec3 GetPosition();

};
#else
#include "Includes.hpp"

class RenderObject
{
public:
	RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl);
	~RenderObject(void);

	virtual void Render(size_t lod);

	virtual size_t GetVertCount(size_t lod);
	virtual void SetGeometry(size_t lod, size_t vertCount, Vertex * verts);

	virtual void SetPosition(fvec3 pos);
	virtual fvec3 GetPosition();

	virtual D3DXMATRIX * GetTransform();


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
