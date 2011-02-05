
#include "Includes.hpp"
#include "RenderObject.hpp"

RenderObject::RenderObject(LPDIRECT3DDEVICE9 device, LPDIRECT3DVERTEXDECLARATION9 vertDecl)
	: mDevice(device), mVertBuffer(NULL), mVertDecl(vertDecl)
{
	mDevice->AddRef();
	mVertDecl->AddRef();

	D3DXMatrixIdentity(&mTransform);
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
	if ( mDevice )
	{
		mDevice->Release();
	}
}

void RenderObject::Render()
{
	mDevice->SetTransform(D3DTS_WORLD, &mTransform);

	if ( SUCCEEDED(mDevice->BeginScene()) )
	{
		mDevice->SetStreamSource(0, mVertBuffer, 0, sizeof(Vertex));
		mDevice->SetVertexDeclaration(mVertDecl);

		mDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, mVertCount / 3);

		mDevice->EndScene();
	}
}

void RenderObject::SetGeometry(size_t vertCount, Vertex * verts)
{
	mVertCount = vertCount;

	UINT vbSize = sizeof(Vertex) * vertCount;

	mDevice->CreateVertexBuffer(vbSize, 0, NULL, D3DPOOL_DEFAULT, &mVertBuffer, NULL);

	VOID * vbVerts;

	HRESULT hrVB = mVertBuffer->Lock(0, vbSize, &vbVerts, 0);

	if ( FAILED(hrVB) )
	{
		MessageBox(NULL, L"Error locking vertex buffer.", L"D3D Error", MB_ICONERROR|MB_OK);
		return;
	}

	memcpy(vbVerts, verts, vbSize);

	mVertBuffer->Unlock();
}
