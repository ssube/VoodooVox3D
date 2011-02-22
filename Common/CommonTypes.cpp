
#include "CommonTypes.hpp"
#include "VectorMath.hpp"

namespace Common
{
    const COMMON_API fvec3 UnitX(1.0f, 0.0f, 0.0f);
    const COMMON_API fvec3 UnitY(0.0f, 1.0f, 0.0f);
    const COMMON_API fvec3 UnitZ(0.0f, 0.0f, 1.0f);

    int32 Plane::Clip(fvec3 minextreme, fvec3 maxextreme)
    {
        using namespace Math;
        
        if ( Dot(Normal, minextreme) + Offset > 0 )
        {
            return -1; // Out entirely
        } else if ( Dot(Normal, maxextreme) + Offset >= 0 ) {
            return 0; // In partially
        } else {
            return 1; // In entirely
        }
    }

    ViewFrustrum::ViewFrustrum(fmat4x4 viewprojMatrix)
        : CurrentMatrix(viewprojMatrix)
    { 
        this->Update(0);
    };

    void ViewFrustrum::Update(fmat4x4 * viewprojMatrix)
    {
        if ( viewprojMatrix )
        {
            CurrentMatrix = *viewprojMatrix;
        }
        // Left
        Planes[0].Normal.x = CurrentMatrix.Data[1][4] + CurrentMatrix.Data[1][1];
        Planes[0].Normal.y = CurrentMatrix.Data[2][4] + CurrentMatrix.Data[2][1];
        Planes[0].Normal.z = CurrentMatrix.Data[3][4] + CurrentMatrix.Data[3][1];
        Planes[0].Offset   = CurrentMatrix.Data[4][4] + CurrentMatrix.Data[4][1];
        // Right
        Planes[1].Normal.x = CurrentMatrix.Data[1][4] - CurrentMatrix.Data[1][1];
        Planes[1].Normal.y = CurrentMatrix.Data[2][4] - CurrentMatrix.Data[2][1];
        Planes[1].Normal.z = CurrentMatrix.Data[3][4] - CurrentMatrix.Data[3][1];
        Planes[1].Offset   = CurrentMatrix.Data[4][4] - CurrentMatrix.Data[4][1];
        // Top
        Planes[2].Normal.x = CurrentMatrix.Data[1][4] - CurrentMatrix.Data[1][2];
        Planes[2].Normal.y = CurrentMatrix.Data[2][4] - CurrentMatrix.Data[2][2];
        Planes[2].Normal.z = CurrentMatrix.Data[3][4] - CurrentMatrix.Data[3][2];
        Planes[2].Offset   = CurrentMatrix.Data[4][4] - CurrentMatrix.Data[4][2];
        // Bottom
        Planes[3].Normal.x = CurrentMatrix.Data[1][4] + CurrentMatrix.Data[1][2];
        Planes[3].Normal.y = CurrentMatrix.Data[2][4] + CurrentMatrix.Data[2][2];
        Planes[3].Normal.z = CurrentMatrix.Data[3][4] + CurrentMatrix.Data[3][2];
        Planes[3].Offset   = CurrentMatrix.Data[4][4] + CurrentMatrix.Data[4][2];
        // Near
        Planes[4].Normal.x = CurrentMatrix.Data[1][3];
        Planes[4].Normal.y = CurrentMatrix.Data[2][3];
        Planes[4].Normal.z = CurrentMatrix.Data[3][3];
        Planes[4].Offset   = CurrentMatrix.Data[4][3];
        // Far
        Planes[5].Normal.x = CurrentMatrix.Data[1][4] - CurrentMatrix.Data[1][3];
        Planes[5].Normal.y = CurrentMatrix.Data[2][4] - CurrentMatrix.Data[2][3];
        Planes[5].Normal.z = CurrentMatrix.Data[3][4] - CurrentMatrix.Data[3][3];
        Planes[5].Offset   = CurrentMatrix.Data[4][4] - CurrentMatrix.Data[4][3];
    }

    int32 ViewFrustrum::Clip(fvec3 a, fvec3 b)
    {
        bool intersect;

        fvec3 minextreme, maxextreme;

        for ( uint32 plane = 0; plane < 6; ++plane )
        {
            if ( Planes[plane].Normal.x <= 0 )
            {
                minextreme.x = a.x;
                maxextreme.x = b.x;
            } else {
                minextreme.x = b.x;
                maxextreme.x = a.x;
            }
            if ( Planes[plane].Normal.y <= 0 )
            {
                minextreme.y = a.y;
                maxextreme.y = b.y;
            } else {
                minextreme.y = b.y;
                maxextreme.y = a.y;
            }
            if ( Planes[plane].Normal.z <= 0 )
            {
                minextreme.z = a.z;
                maxextreme.z = b.z;
            } else {
                minextreme.z = b.z;
                maxextreme.z = a.z;
            }

            int32 result = Planes[plane].Clip(minextreme, maxextreme);
            if ( result == -1 )
            {
                return -1;
            } else if ( result == 0 ) {
                intersect = true;
            }
        }

        if ( intersect )
        {
            return 0;
        } else {
            return 1;
        }
    }
}
