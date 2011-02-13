#pragma once

#include "Includes.hpp"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Translate(D3DXVECTOR3 translate);
	void TranslateRaw(D3DXVECTOR3 translate);

	void Rotate(float yaw, float pitch);

	D3DXVECTOR3 Transform(D3DXVECTOR3 shift);

	D3DXVECTOR3 GetPosition();
	void SetPosition(D3DXVECTOR3 pos);

	D3DXMATRIX * GetViewMatrix();

private:
	D3DXVECTOR3 mPos, mForward, mUp;
	float mYaw, mPitch;

	bool mDirty;
	D3DXMATRIX mViewMat;
};
