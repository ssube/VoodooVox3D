#include "Camera.hpp"

Camera::Camera(void)
    : mDirty(true), mYaw(0.0f), mPitch(0.0f), mPos(0.0f), mForward(UnitZ), mUp(UnitY)
{
}

Camera::~Camera(void)
{
}

float Camera::GetYaw()
{
    return mYaw;
}

void Camera::SetYaw(float yaw)
{
    mDirty = true;

    mYaw = yaw;
}

float Camera::GetPitch()
{
    return mPitch;
}

void Camera::SetPitch(float pitch)
{
    mDirty = true;

    mPitch = pitch;
}

fvec2 Camera::GetRotation()
{
    return fvec2(mYaw, mPitch);
}

void Camera::SetRotation(fvec2 rotation)
{
    using namespace Common::Math;

    mDirty = true;

    mYaw   = rotation.x;
    while ( mYaw <   0.0f ) mYaw += 360.0f;
    while ( mYaw > 360.0f ) mYaw -= 360.0f;

    mPitch = Clamp(-75.0f, 75.0f, rotation.y);
}

void Camera::SetRotation(float yaw, float pitch)
{
    using namespace Common::Math;

    mDirty = true;

    mYaw   = yaw;
    while ( mYaw <   0.0f ) mYaw += 360.0f;
    while ( mYaw > 360.0f ) mYaw -= 360.0f;

    mPitch = Clamp(-75.0f, 75.0f, pitch);
}

void Camera::Rotate(float yaw, float pitch)
{
    using namespace Common::Math;

    mDirty = true;

    mYaw += yaw;
    mPitch += pitch;

    while ( mYaw <   0.0f ) mYaw += 360.0f;
    while ( mYaw > 360.0f ) mYaw -= 360.0f;
    mPitch = Clamp(-75.0f, 75.0f, mPitch);
}

void Camera::Translate(fvec3 translation)
{
    mDirty = true;

    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

    D3DXVECTOR3 shift;
    D3DXVec3TransformCoord(&shift, (D3DXVECTOR3*)&translation, &rotationMatrix);
    D3DXVec3Add((D3DXVECTOR3*)&mPos, (D3DXVECTOR3*)&mPos, &shift);
}

void Camera::TranslateRaw(fvec3 translation)
{
    mDirty = true;

    D3DXVec3Add((D3DXVECTOR3*)&mPos, (D3DXVECTOR3*)&mPos, (D3DXVECTOR3*)&translation);
}

fvec3 Camera::Transform(fvec3 shift)
{
    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

    D3DXVECTOR3 final;
    D3DXVec3TransformCoord(&final, (D3DXVECTOR3*)&shift, &rotationMatrix);

    return final;
}

fvec3 Camera::GetPosition()
{
    return mPos;
}

void Camera::SetPosition(fvec3 pos)
{
    if ( mPos != pos )
    {
        mDirty = true;
        mPos = pos;
    }
}

fmat4x4 * Camera::GetViewMatrix()
{
    if ( mDirty )
    {
        mDirty = false;

        D3DXMATRIX rotationMatrix;
        D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

        D3DXVECTOR3 forward, target;
        D3DXVec3TransformCoord(&forward, (D3DXVECTOR3*)&mForward, &rotationMatrix);
        D3DXVec3Add(&target, (D3DXVECTOR3*)&mPos, &forward);

        D3DXMatrixLookAtLH((D3DXMATRIX*)&mViewMat, (D3DXVECTOR3*)&mPos, &target, (D3DXVECTOR3*)&mUp);
    }

    return &mViewMat;
}
