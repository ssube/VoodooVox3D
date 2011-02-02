#pragma once

#include "Includes.hpp"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Translate(float x, float y, float z);
	void Rotate(float yaw, float pitch);

	D3DMATRIX GetViewMatrix();

private:
	D3DMATRIX mViewMat;
	D3DXVECTOR3 mPos;
	float mYaw, mPitch;
	bool mDirty;
};
