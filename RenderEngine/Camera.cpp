#include "Camera.hpp"

Camera::Camera(void)
: mDirty(true)
{
}

Camera::~Camera(void)
{
}

void Camera::Translate(D3DXVECTOR3 translation)
{
	mDirty = true;

	D3DMATRIX rotationMatrix;
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), 0.0f, 0.0f);

	D3DXVECTOR3 shift;
	D3DXVec3TransformCoord(&shift, &translation, &rotationMatrix);
	D3DXVec3Add(&mPos, &mPos, &shift);
}

void Camera::Rotate(float yaw, float pitch)
{
	mDirty = true;

	mYaw = ( mYaw + yaw ) % 360;
	mPitch = max(min(mPitch + pitch, 75.0f), -75.0f);
}

D3DMATRIX * Camera::GetViewMatrix()
{
	if ( mDirty )
	{
		mDirty = false;

		D3DMATRIX rotationMatrix = D3DXMatrixRotationYawPitchRoll(D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

		D3DXVECTOR3 forward, target;
		D3DXVec3TransformCoord(&forward, {0,0,1}, &rotationMatrix);
		D3DXVec3Add(&target, &mPos, &forward);

		return D3DXMatrixLookAtLH(&mViewMat, &mPos, &target, {0,1,0});
	} else {
		return &mViewMat;
	}
}