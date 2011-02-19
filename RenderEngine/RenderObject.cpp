
#include "Includes.hpp"
#include "RenderObject.hpp"

RenderObject::RenderObject
(
	LPDIRECT3DDEVICE9 device, 
	LPDIRECT3DVERTEXDECLARATION9 vertDecl
)
	: mDevice(device), mVertDecl(vertDecl)
{
	ZeroMemory(mVertBuffer, sizeof(LPDIRECT3DVERTEXBUFFER9) * 4);
	ZeroMemory(mVertCount, sizeof(size_t) * 4);

	mDevice->AddRef();
	mVertDecl->AddRef();

	D3DXMatrixIdentity(&mTransform);

	HRESULT hrOQ = mDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION, &mOcclQuery);
}

RenderObject::~RenderObject(void)
{
	//mEngine->DestroyRenderObject(this);

	for ( size_t lod = 0; lod < LOD_COUNT; ++lod )
	{
		if ( mVertBuffer[lod] )
		{
			mVertBuffer[lod]->Release();
		}
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
	if ( ! mVertBuffer[lod] ) return;

	mDevice->SetTransform(D3DTS_WORLD, &mTransform);

	if ( SUCCEEDED(mDevice->BeginScene()) )
	{
		mDevice->SetStreamSource(0, mVertBuffer[lod], 0, sizeof(Vertex));
		mDevice->SetVertexDeclaration(mVertDecl);

		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mVertCount[lod] / 3);

		mDevice->EndScene();
	}
}

void RenderObject::SetGeometry(size_t lod, size_t vertCount, Vertex * verts)
{
	if ( vertCount == 0 )
	{
		mHasGeometry = false;
	} else {
		mHasGeometry = true;

		mVertCount[lod] = vertCount;

		UINT vbSize = sizeof(Vertex) * vertCount;

		mDevice->CreateVertexBuffer(vbSize, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &(mVertBuffer[lod]), NULL);

		VOID * vbVerts;

		HRESULT hrVB = mVertBuffer[lod]->Lock(0, vbSize, &vbVerts, 0);

		if ( FAILED(hrVB) )
		{
			throw std::runtime_error("Error locking vertex buffer.");
		}

		memcpy(vbVerts, verts, vbSize);

		mVertBuffer[lod]->Unlock();
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
	if ( mHasGeometry )
	{
		mOccluded = false;
		return;
	}

	DWORD pixels;
	HRESULT queryDone = mOcclQuery->GetData(&pixels, sizeof(DWORD), 0);

	if ( queryDone == S_OK )
	{
		// Process
		mOccluded = ( pixels > OCCL_THRESHOLD );
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
