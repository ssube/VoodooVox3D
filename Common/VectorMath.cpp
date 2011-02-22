#include "VectorMath.hpp"

namespace Common
{
    namespace Math
    {
        float Luminance(Vector3<float> x)
        {
            return ( x.x * 0.27f + x.y * 0.67f + x.z * 0.06f );
        }

        float Luminance(Vector4<float> x)
        {
            return ( x.x * 0.27f + x.y * 0.67f + x.z * 0.06f );
        }

        Vector2<int> Floor(Vector2<float> x)
        {
            return Vector2<int>((int)floor(x.x), (int)floor(x.y));
        }

        Vector3<int> Floor(Vector3<float> x)
        {
            return Vector3<int>((int)floor(x.x), (int)floor(x.y), (int)floor(x.z));
        }

        Vector4<int> Floor(Vector4<float> x)
        {
            return Vector4<int>((int)floor(x.x), (int)floor(x.y), (int)floor(x.z), (int)floor(x.w));
        }

        unsigned long PackColor(fvec4 Color)
        {
            unsigned long color = 0;
            unsigned char r = (size_t)(Color.r * 255.0f);
            unsigned char g = (size_t)(Color.g * 255.0f);
            unsigned char b = (size_t)(Color.b * 255.0f);
            unsigned char a = (size_t)(Color.a * 255.0f);
            color = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
            return color;
        }

        fvec4 UnpackColor(unsigned long Color)
        {
            fvec4 color = 0.0f;
            color.a = ( (Color & 0xFF000000 ) >> 24 ) / 255.0f;
            color.r = ( (Color & 0x00FF0000 ) >> 16 ) / 255.0f;
            color.g = ( (Color & 0x0000FF00 ) >>  8 ) / 255.0f;
            color.b = ( (Color & 0x000000FF ) >>  0 ) / 255.0f;
            return color;
        }

        float LengthSq(fvec2 a)
        {
            return ( a.x * a.x + a.y * a.y );
        }

        float LengthSq(fvec3 a)
        {
            return ( a.x * a.x + a.y * a.y + a.z * a.z );
        }

        float LengthSq(fvec4 a)
        {
            return ( a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w );
        }
    }
}
