#include "Camera.hpp"

#include "Math.hpp"
#include "VectorMath.hpp"

Camera::Camera(float nearlimit, float farlimit, float fov, float aspect)
    : mDirty(true), mYaw(0.0f), mPitch(0.0f), mPos(0.0f), mForward(UnitZ), mUp(UnitY)
{
    D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&mProjMat, D3DXToRadian(fov), aspect, nearlimit, farlimit);
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
        this->UpdateInternals();
    }

    return &mViewMat;
}

fmat4x4 * Camera::GetProjMatrix()
{
    return &mProjMat;
}

int32 Camera::Clip(fvec3 aabbMin, fvec3 aabbMax)
{
    if ( mDirty ) 
    {
        this->UpdateInternals();
    }

    // Clip - sphere
    fvec3 center = ( aabbMax + aabbMin ) / 2;
    fvec3 nearest;
    if ( mPos.x < center.x ) { nearest.x = aabbMin.x; } else { nearest.x = aabbMax.x; }
    if ( mPos.y < center.y ) { nearest.y = aabbMin.y; } else { nearest.y = aabbMax.y; }
    if ( mPos.z < center.z ) { nearest.z = aabbMin.z; } else { nearest.z = aabbMax.z; }
    
    if ( Math::DistSq(mPos, nearest) > ( 1000.0f * 1000.0f ) )
    {
        return 1;
    } else {
        return 1;
    }

/*
    // Full frustrum - busted
    bool intersects = false;
    D3DXVECTOR3 minExtreme, maxExtreme;

    for ( uint32 plane = 0; plane < 6; ++plane )
    {
        if (mFrustrumPlanes[plane].a <= 0)
        {
            minExtreme.x = aabbMin.x;
            maxExtreme.x = aabbMax.x;
        }
        else
        {
            minExtreme.x = aabbMax.x;
            maxExtreme.x = aabbMin.x;
        }

        if (mFrustrumPlanes[plane].b <= 0)
        {
            minExtreme.y = aabbMin.y;
            maxExtreme.y = aabbMax.y;
        }
        else
        {
            minExtreme.y = aabbMax.y;
            maxExtreme.y = aabbMin.y;
        }

        if (mFrustrumPlanes[plane].c <= 0)
        {
            minExtreme.z = aabbMin.z;
            maxExtreme.z = aabbMax.z;
        }
        else
        {
            minExtreme.z = aabbMax.z;
            maxExtreme.z = aabbMin.z; 
        }
        
        if ( D3DXPlaneDotCoord(&mFrustrumPlanes[plane], &minExtreme) + mFrustrumPlanes[plane].d >  0)
        {
            return -1;
        }

        if ( D3DXPlaneDotCoord(&mFrustrumPlanes[plane], &maxExtreme) + mFrustrumPlanes[plane].d >= 0)
        {
            intersects = TRUE;
        }
    }

    if ( intersects )
    {
        return 0;
    } else {
        return 1;
    }
*/
}

void Camera::UpdateInternals()
{
    if ( !mDirty ) return;

    mDirty = false;

    this->UpdateViewMatrix();
    this->UpdateFrustrumPlanes();
}

void Camera::UpdateViewMatrix()
{
    D3DXMATRIX rotationMatrix;
    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(mYaw), D3DXToRadian(mPitch), 0.0f);

    D3DXVECTOR3 forward, target;
    D3DXVec3TransformCoord(&forward, (D3DXVECTOR3*)&mForward, &rotationMatrix);
    D3DXVec3Add(&target, (D3DXVECTOR3*)&mPos, &forward);

    D3DXMatrixLookAtLH((D3DXMATRIX*)&mViewMat, (D3DXVECTOR3*)&mPos, &target, (D3DXVECTOR3*)&mUp);
}

void Camera::UpdateFrustrumPlanes()
{
    D3DXMATRIX viewproj;
    D3DXMatrixMultiply(&viewproj, (D3DXMATRIX*)&mViewMat, (D3DXMATRIX*)&mProjMat);

    // Left plane
    mFrustrumPlanes[0].a = viewproj._14 + viewproj._11;
    mFrustrumPlanes[0].b = viewproj._24 + viewproj._21;
    mFrustrumPlanes[0].c = viewproj._34 + viewproj._31;
    mFrustrumPlanes[0].d = viewproj._44 + viewproj._41;

    // Right plane
    mFrustrumPlanes[1].a = viewproj._14 - viewproj._11;
    mFrustrumPlanes[1].b = viewproj._24 - viewproj._21;
    mFrustrumPlanes[1].c = viewproj._34 - viewproj._31;
    mFrustrumPlanes[1].d = viewproj._44 - viewproj._41;

    // Top plane
    mFrustrumPlanes[2].a = viewproj._14 - viewproj._12;
    mFrustrumPlanes[2].b = viewproj._24 - viewproj._22;
    mFrustrumPlanes[2].c = viewproj._34 - viewproj._32;
    mFrustrumPlanes[2].d = viewproj._44 - viewproj._42;

    // Bottom plane
    mFrustrumPlanes[3].a = viewproj._14 + viewproj._12;
    mFrustrumPlanes[3].b = viewproj._24 + viewproj._22;
    mFrustrumPlanes[3].c = viewproj._34 + viewproj._32;
    mFrustrumPlanes[3].d = viewproj._44 + viewproj._42;

    // Near plane
    mFrustrumPlanes[4].a = viewproj._13;
    mFrustrumPlanes[4].b = viewproj._23;
    mFrustrumPlanes[4].c = viewproj._33;
    mFrustrumPlanes[4].d = viewproj._43;

    // Far plane
    mFrustrumPlanes[5].a = viewproj._14 - viewproj._13;
    mFrustrumPlanes[5].b = viewproj._24 - viewproj._23;
    mFrustrumPlanes[5].c = viewproj._34 - viewproj._33;
    mFrustrumPlanes[5].d = viewproj._44 - viewproj._43;

    // Normalize planes
    for ( int i = 0; i < 6; i++ )
    {
        D3DXPlaneNormalize( &mFrustrumPlanes[i], &mFrustrumPlanes[i] );
    }
}
