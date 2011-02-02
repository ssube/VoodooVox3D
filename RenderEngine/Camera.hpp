#pragma once

#include "Includes.hpp"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Translate(D3DXVECTOR3 translate);
	void Rotate(float yaw, float pitch);

	D3DMATRIX * GetViewMatrix();

private:
	D3DXVECTOR3 mPos, mForward, mUp;
	float mYaw, mPitch;

	bool mDirty;
	D3DMATRIX mViewMat;
};
