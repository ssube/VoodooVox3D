#pragma once

#include "CommonDefs.hpp"

#include "CommonTypes.hpp"
#include "Math.hpp"

namespace Common
{
    namespace Math
    {
        template<typename T>
        Vector2<T> Vector2Add(Vector2<T> x, Vector2<T> y)
        {
            return Vector2<T>(x.x + y.x, x.y + y.y);
        }

        template<typename T>
        Vector3<T> Vector3Add(Vector3<T> x, Vector3<T> y)
        {
            return Vector3<T>(x.x + y.x, x.y + y.y, x.z + y.z);
        }

        template<typename T>
        Vector4<T> Vector4Add(Vector4<T> x, Vector4<T> y)
        {
            return Vector4<T>(x.x + y.x, x.y + y.y, x.z + y.z, x.w + y.w);
        }

        template<typename T>
        Vector2<T> Vector2Mult(Vector2<T> x, T y)
        {
            return Vector2<T>(x.x * y, x.y * y);
        }

        template<typename T>
        Vector3<T> Vector3Mult(Vector3<T> x, T y)
        {
            return Vector3<T>(x.x * y, x.y * y, x.z * y);
        }

        template<typename T>
        Vector4<T> Vector4Mult(Vector4<T> x, T y)
        {
            return Vector4<T>(x.x * y, x.y * y, x.z * y, x.w * y);
        }

        template<typename T>
        Vector2<T> Vector2Div(Vector2<T> x, T y)
        {
            return Vector2<T>(x.x / y, x.y / y);
        }

        template<typename T>
        Vector3<T> Vector3Div(Vector3<T> x, T y)
        {
            return Vector3<T>(x.x / y, x.y / y, x.z / y);
        }

        template<typename T>
        Vector4<T> Vector4Div(Vector4<T> x, T y)
        {
            return Vector4<T>(x.x / y, x.y / y, x.z / y, x.w / y);
        }

        float COMMON_API Luminance(Vector3<float> x);

        float COMMON_API Luminance(Vector4<float> x);

        Vector2<int> COMMON_API Floor(Vector2<float> x);

        Vector3<int> COMMON_API Floor(Vector3<float> x);

        Vector4<int> COMMON_API Floor(Vector4<float> x);

        template<typename T>
        bool Any(Vector2<T> x, T y)
        {
            return ( x.x > y || x.y > y );
        }

        template<typename T>
        bool Any(Vector3<T> x, T y)
        {
            return ( x.x > y || x.y > y || x.z > y );
        }

        template<typename T>
        bool Any(Vector4<T> x, T y)
        {
            return ( x.x > y || x.y > y || x.z > y || x.w > y );
        }

        template<typename T>
        bool All(Vector2<T> x, T y)
        {
            return ( x.x > y && x.y > y );
        }

        template<typename T>
        bool All(Vector3<T> x, T y)
        {
            return ( x.x > y && x.y > y && x.z > y );
        }

        template<typename T>
        bool All(Vector4<T> x, T y)
        {
            return ( x.x > y && x.y > y && x.z > y && x.w > y );
        }

        template<typename T>
        bool Within(T bottom, T top, Vector2<T> x)
        {
            return ( 
                x.x >= bottom && x.x < top && 
                x.y >= bottom && x.y < top );
        }

        template<typename T>
        bool Within(T bottom, T top, Vector3<T> x)
        {
            return ( 
                x.x >= bottom && x.x < top && 
                x.y >= bottom && x.y < top && 
                x.z >= bottom && x.z < top );
        }

        template<typename T>
        bool Within(T bottom, T top, Vector4<T> x)
        {
            return ( 
                x.x >= bottom && x.x < top && 
                x.y >= bottom && x.y < top && 
                x.z >= bottom && x.z < top &&
                x.w >= bottom && x.w < top );
        }

        unsigned long COMMON_API PackColor(fvec4 Color);
        fvec4 COMMON_API UnpackColor(unsigned long Color);

        float COMMON_API LengthSq(fvec2 a);

        float COMMON_API LengthSq(fvec3 a);

        float COMMON_API LengthSq(fvec4 a);

        template<typename T>
        T COMMON_API Dot(Vector3<T> a, Vector3<T> b)
        {
            return ( a.x * b.x + a.y * b.y + a.z * b.z );
        }

        template<typename T>
        float DistSq(T a, T b)
        {
            T diff = a - b;
            return LengthSq(diff);
        }
    }
}
