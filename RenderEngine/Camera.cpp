#include "Camera.hpp"

Camera::Camera(void)
	: mDirty(true)
{
	mYaw = mPitch = 0;
	mPos = D3DXVECTOR3(0, 0, 0);
	mForward = D3DXVECTOR3(0, 0, 1);
	mUp = D3DXVECTOR3(0, 1, 0);
}

Camera::~Camera(void)
{
}

void Camera::Translate(D3DXVECTOR3 translation)
{
	mDirty = true;

	D3DXMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), 0.0f, 0.0f);

	D3DXVECTOR3 shift;
	D3DXVec3TransformCoord(&shift, &translation, &rotationMatrix);
	D3DXVec3Add(&mPos, &mPos, &shift);
}

void Camera::Rotate(float yaw, float pitch)
{
	mDirty = true;

	mYaw = ( mYaw + yaw );
	while ( mYaw >= 360) { mYaw -= 360; }
	while ( mYaw < 0 ) { mYaw += 360; }

	mPitch = max(min(mPitch + pitch, 75.0f), -75.0f);
}

D3DMATRIX * Camera::GetViewMatrix()
{
	if ( mDirty )
	{
		mDirty = false;

		D3DXMATRIX rotationMatrix;
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

		D3DXVECTOR3 forward, target;
		D3DXVec3TransformCoord(&forward, &mForward, &rotationMatrix);
		D3DXVec3Add(&target, &mPos, &forward);

		D3DXMatrixLookAtLH((D3DXMATRIX*)&mViewMat, &mPos, &target, &mUp);
	}

	return &mViewMat;
}