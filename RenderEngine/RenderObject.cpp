
#include "Includes.hpp"
#include "RenderObject.hpp"

RenderObject::RenderObject
(
	LPDIRECT3DDEVICE9 device, 
	LPDIRECT3DVERTEXDECLARATION9 vertDecl
)
	: mDevice(device), mVertDecl(vertDecl), mVertBuffer(NULL)
{
	ZeroMemory(mVertOffset, sizeof(size_t) * 4);
	ZeroMemory(mVertCount, sizeof(size_t) * 4);

	mDevice->AddRef();
	mVertDecl->AddRef();

	D3DXMatrixIdentity(&mTransform);

	HRESULT hrOQ = mDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &mOcclQuery);
}

RenderObject::~RenderObject(void)
{
	//mEngine->DestroyRenderObject(this);

	if ( mVertBuffer )
	{
		mVertBuffer->Release();
	}
	if ( mVertDecl )
	{
		mVertDecl->Release();
	}
	if ( mVertOcclDecl )
	{
		mVertOcclDecl->Release();
	}
	if ( mOcclQuery )
	{
		mOcclQuery->Release();
	}
	if ( mOcclGeometry )
	{
		mOcclGeometry->Release();
	}
	if ( mDevice )
	{
		mDevice->Release();
	}
}

void RenderObject::Render(size_t lod)
{
	if ( ! mHasGeometry ) return;
	if ( ! mVertBuffer ) return;

	mDevice->SetTransform(D3DTS_WORLD, &mTransform);

	if ( SUCCEEDED(mDevice->BeginScene()) )
	{
		mDevice->SetStreamSource(0, mVertBuffer, sizeof(Vertex) * mVertOffset[lod], sizeof(Vertex));
		mDevice->SetVertexDeclaration(mVertDecl);

		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mVertCount[lod] / 3);

		mDevice->EndScene();
	}
}

void RenderObject::SetGeometry(size_t total, size_t * offsets, size_t * counts, Vertex * verts)
{
	memcpy(mVertOffset, offsets, sizeof(size_t) * LOD_COUNT);
	memcpy(mVertCount,  counts,  sizeof(size_t) * LOD_COUNT);

	// Check for counts
	if ( total == 0 )
	{
		mHasGeometry = false;
	} else {
		mHasGeometry = true;

		UINT vbSize = sizeof(Vertex) * total;

		if ( FAILED(mDevice->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &mVertBuffer, NULL) ) )
		{
			throw std::runtime_error("Unable to create vertex buffer.");
		}

		VOID * vbVerts;

		HRESULT hrVB = mVertBuffer->Lock(0, vbSize, &vbVerts, 0);

		if ( FAILED(hrVB) )
		{
			throw std::runtime_error("Error locking vertex buffer.");
		}

		memcpy(vbVerts, verts, vbSize);

		mVertBuffer->Unlock();
	}
}

void RenderObject::SetOcclusionData(LPDIRECT3DVERTEXBUFFER9 buffer, LPDIRECT3DVERTEXDECLARATION9 decl)
{
	mOcclGeometry = buffer;
	mVertOcclDecl = decl;

	mOcclGeometry->AddRef();
	mVertOcclDecl->AddRef();
}

/*void RenderObject::SetTransform(D3DXMATRIX trans)
{
	mTransform = trans;
}*/


void RenderObject::SetPosition(fvec3 pos)
{
	mPosition = pos;
	D3DXMatrixTranslation(&mTransform, pos.x, pos.y, pos.z);
}

fvec3 RenderObject::GetPosition()
{
	return mPosition;
}

size_t RenderObject::GetVertCount(size_t lod)
{
	return mVertCount[lod];
}

D3DXMATRIX * RenderObject::GetTransform()
{
	return &mTransform;
}

bool RenderObject::GetVisible()
{
	return ( mHasGeometry && !mOccluded );
}

void RenderObject::UpdateOcclusion()
{
	if ( !mHasGeometry )
	{
		mOccluded = true;
		return;
	}

	DWORD pixels;
	HRESULT queryDone = mOcclQuery->GetData(&pixels, sizeof(DWORD), 0);

	if ( queryDone == S_OK )
	{
		// Process
		mOccluded = ( pixels < 4 );
	}

	mOcclQuery->Issue(D3DISSUE_BEGIN);

	mDevice->SetTransform(D3DTS_WORLD, &mTransform);

	//if ( SUCCEEDED(mDevice->BeginScene()) )
	//{
		mDevice->SetStreamSource(0, mOcclGeometry, 0, sizeof(fvec3));
		mDevice->SetVertexDeclaration(mVertOcclDecl);

		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 12);

	//	mDevice->EndScene();
	//}

	mOcclQuery->Issue(D3DISSUE_END);

	return;
}
