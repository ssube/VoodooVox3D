#pragma once

#ifndef RENDERENGINE_INTERFACE
#    include "Includes.hpp"
#endif

#include "CommonTypes.hpp"
using namespace Common;

class Camera
{
public:
    Camera(void);
    ~Camera(void);

    void Translate(fvec3 translate);
    void TranslateRaw(fvec3 translate);

    void Rotate(float yaw, float pitch);

    fvec3 Transform(fvec3 shift);

    fvec3 GetPosition();
    void SetPosition(fvec3 pos);

    fmat4x4 * GetViewMatrix();

private:
    fvec3 mPos, mForward, mUp;
    float mYaw, mPitch;

    bool mDirty;
    fmat4x4 mViewMat;
};
