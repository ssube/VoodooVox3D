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

    virtual void      Rotate(float yaw, float pitch);
    virtual float     GetYaw();
    virtual void      SetYaw(float yaw);
    virtual float     GetPitch();
    virtual void      SetPitch(float pitch);
    virtual fvec2     GetRotation();
    virtual void      SetRotation(fvec2 rotation);
    virtual void      SetRotation(float yaw, float pitch);

    virtual void      Translate(fvec3 translate);
    virtual void      TranslateRaw(fvec3 translate);
    virtual fvec3     Transform(fvec3 shift);

    virtual fvec3     GetPosition();
    virtual void      SetPosition(fvec3 pos);

    virtual fmat4x4 * GetViewMatrix();

private:
    fvec3 mPos, mForward, mUp;
    float mYaw, mPitch;

    bool mDirty;
    fmat4x4 mViewMat;
};
