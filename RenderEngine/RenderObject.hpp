#pragma once

#define LOD_COUNT 3
#define OCCL_THRESHOLD 8

using namespace Common;

#ifdef RENDERENGINE_INTERFACE
#include "CommonTypes.hpp"

class RenderObject
{
public:
	virtual void Render(size_t lod);

	virtual size_t GetVertCount(size_t lod);
	virtual void SetGeometry(size_t total, size_t * offsets, size_t * counts, Vertex * verts);

	virtual void SetPosition(fvec3 position);
	virtual fvec3 GetPosition();

	virtual bool GetVisible();
	virtual void UpdateOcclusion();

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
	virtual void SetGeometry(size_t total, size_t * offsets, size_t * counts, Vertex * verts);

	virtual void SetPosition(fvec3 pos);
	virtual fvec3 GetPosition();

	virtual bool GetVisible();
	virtual void UpdateOcclusion();

	virtual D3DXMATRIX * GetTransform();
	virtual void SetOcclusionData(LPDIRECT3DVERTEXBUFFER9 buffer, LPDIRECT3DVERTEXDECLARATION9 decl);


private:
	LPDIRECT3DDEVICE9 mDevice;
	LPDIRECT3DVERTEXDECLARATION9 mVertDecl;

	LPDIRECT3DQUERY9 mOcclQuery;
	LPDIRECT3DVERTEXDECLARATION9 mVertOcclDecl;
	LPDIRECT3DVERTEXBUFFER9 mOcclGeometry;

	size_t mVertCount[LOD_COUNT];
	size_t mVertOffset[LOD_COUNT];
	LPDIRECT3DVERTEXBUFFER9 mVertBuffer;

	D3DXVECTOR3 mPosition;
	D3DXMATRIX mTransform;

	bool mHasGeometry, mOccluded;
};
#endif
