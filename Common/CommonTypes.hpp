#pragma once

#include "CommonDefs.hpp"

namespace Common
{
#    define INDEX2(o, n) o[n.x][n.y]
#    define INDEX3(o, n) o[n.x][n.y][n.z]
#    define INDEX4(o, n) o[n.x][n.y][n.z][n.w]

    typedef unsigned __int8   uint8;
    typedef __int8            int8;
    typedef unsigned __int16  uint16;
    typedef __int16           int16;
    typedef unsigned __int32  uint32;
    typedef __int32           int32;
    typedef unsigned __int64  uint64;
    typedef __int64           int64;

    /**
     * Simple 2-component vector. When declared with Vector2<float>, this is compatible
     * with D3DXVECTOR2.
     */
    template<typename T>
    struct Vector2
    {
        union
        {
            struct { T x, y; };
            struct { T r, g; };
            struct { T u, v; };
        };

        Vector2()
            : x(), y()
        { };

        Vector2(T val)
            : x(val), y(val)
        { };

        Vector2(const T * val)
            : x(val[0]), y(val[1])
        { };

        Vector2(T x, T y)
            : x(x), y(y)
        { };

        Vector2<T> operator=(const Vector2<T> other)
        {
            this->x = other.x;
            this->y = other.y;

            return *this;
        };

        Vector2<T> operator=(const T other)
        {
            this->x = this->y = other;
        }

        bool operator!=(const Vector2<T> other)
        {
            return !( *this == other );
        }

        bool operator==(const Vector2<T> other)
        {
            return ( this->x == other.x && this->y == other.y );
        };

        bool operator<(const Vector2<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y );
            T omag = ( other.x * other.x ) + ( other.y * other.y );
            return ( tmag < omag );
        };

        bool operator>(const Vector2<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y );
            T omag = ( other.x * other.x ) + ( other.y * other.y );
            return ( tmag > omag );
        };

        Vector2<T> operator+(const Vector2<T> other)
        {
            return Vector2<T>(this->x + other.x, this->y + other.y);
        };

        Vector2<T> operator-(const Vector2<T> other)
        {
            return Vector2<T>(this->x - other.x, this->y - other.y);
        };

        Vector2<T> operator*(const float value)
        {
            return Vector2<T>(this->x * value, this->y * value);
        };

        Vector2<T> operator/(const float value)
        {
            return Vector2<T>(this->x / value, this->y / value);
        };

        template<typename S>
        operator Vector2<S>()
        {
            return Vector2<S>((S)this->x, (S)this->y);
        }

#ifdef D3DX_CONVERSION_FUNCS
        Vector2(D3DXVECTOR2 v)
        {
            this->x = v.x;
            this->y = v.y;
        }

        Vector2<T> & operator=(const D3DXVECTOR2 & other)
        {
            memcpy(this, other, sizeof(float)*2);
        };

        operator D3DXVECTOR2()
        {
            return *(D3DXVECTOR2*)this;
        };

        operator D3DXVECTOR3()
        {
            return D3DXVECTOR3((float)x, (float)y, 0.0f);
        };

        operator D3DXVECTOR4()
        {
            return D3DXVECTOR4((float)x, (float)y, 0.0f, 1.0f);
        };
#endif
    };

    /**
     * Simple 3-component vector. When declared with Vector3<float>, this is compatible
     * with D3DXVECTOR3.
     */
    template<typename T>
    struct Vector3
    {
        union
        {
            struct { T x, y, z; };
            struct { T r, g, b; };
            struct { T u, v, s; };
        };

        Vector3()
            : x(), y(), z()
        { };

        Vector3(T val)
            : x(val), y(val), z(val)
        { };

        Vector3(const T * val)
            : x(val[0]), y(val[1]), z(val[2])
        { };

        Vector3(T x, T y, T z)
            : x(x), y(y), z(z)
        { };

        Vector3<T> operator=(const Vector3<T> other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;

            return *this;
        };

        Vector3<T> operator=(const T other)
        {
            this->x = this->y = this->z = other;
        }

        bool operator!=(const Vector3<T> other)
        {
            return !( *this == other );
        }

        bool operator==(const Vector3<T> other)
        {
            return ( this->x == other.x && this->y == other.y && this->z == other.z );
        };

        bool operator<(const Vector3<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y ) + ( this->z * this->z );
            T omag = ( other.x * other.x ) + ( other.y * other.y ) + ( other.z * other.z );
            return ( tmag < omag );
        };

        bool operator>(const Vector3<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y ) + ( this->z * this->z );
            T omag = ( other.x * other.x ) + ( other.y * other.y ) + ( other.z * other.z );
            return ( tmag > omag );
        };

        Vector3<T> operator+(const Vector3<T> other)
        {
            return Vector3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
        };

        Vector3<T> operator-(const Vector3<T> other)
        {
            return Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
        };

        Vector3<T> operator*(const float value)
        {
            return Vector3<T>(this->x * value, this->y * value, this->z * value);
        };

        Vector3<T> operator/(const float value)
        {
            return Vector3<T>(this->x / value, this->y / value, this->z / value);
        };

        template<typename S>
        operator Vector3<S>()
        {
            return Vector3<S>((S)this->x, (S)this->y, (S)this->z);
        }

#ifdef D3DX_CONVERSION_FUNCS
        Vector3(D3DXVECTOR3 v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }

        Vector3<T> & operator=(const D3DXVECTOR3 & other)
        {
            memcpy(this, other, sizeof(float)*3);
        };

        operator D3DXVECTOR3()
        {
            return *(D3DXVECTOR3*)this;
        };

        operator D3DXVECTOR4()
        {
            return D3DXVECTOR4((float)x, (float)y, (float)z, 1.0f);
        };
#endif
    };

    /**
     * Simple 4-component vector. When declared with Vector4<float>, this is compatible
     * with D3DXVECTOR4.
     */
    template<typename T>
    struct Vector4
    {
        union
        {
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            struct { T u, v, s, t; };
        };

        Vector4()
            : x(), y(), z(), w()
        { };

        Vector4(T val)
            : x(val), y(val), z(val), w(val)
        { };

        Vector4(const T * val)
            : x(val[0]), y(val[1]), z(val[2]), w(val[3])
        { };

        Vector4(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        { };

        Vector4<T> operator=(const Vector4<T> other)
        {
            this->x = other.x;
            this->y = other.y;
            this->z = other.z;
            this->w = other.w;

            return *this;
        };

        Vector4<T> operator=(const T other)
        {
            this->x = this->y = this->z = this->w = other;
        }

        bool operator!=(const Vector4<T> other)
        {
            return !( *this == other );
        }

        bool operator==(const Vector4<T> other)
        {
            return ( this->x == other.x && this->y == other.y && this->z == other.z && this->w == other.w );
        };

        bool operator<(const Vector4<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y ) + ( this->z * this->z ) + ( this->w * this->w );
            T omag = ( other.x * other.x ) + ( other.y * other.y ) + ( other.z * other.z ) + ( other.w * other.w );
            return ( tmag < omag );
        };

        bool operator>(const Vector4<T> other)
        {
            T tmag = ( this->x * this->x ) + ( this->y * this->y ) + ( this->z * this->z ) + ( this->w * this->w );
            T omag = ( other.x * other.x ) + ( other.y * other.y ) + ( other.z * other.z ) + ( other.w * other.w );
            return ( tmag > omag );
        };

        Vector4<T> operator+(Vector4<T> other)
        {
            return Vector4<T>(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
        };

        Vector4<T> operator-(Vector4<T> other)
        {
            return Vector4<T>(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
        };

        Vector4<T> operator*(float value)
        {
            return Vector4<T>(this->x * value, this->y * value, this->z * value, this->w * value);
        };

        Vector4<T> operator/(float value)
        {
            return Vector4<T>(this->x / value, this->y / value, this->z / value, this->w / value);
        };

        template<typename S>
        operator Vector4<S>()
        {
            return Vector4<S>((S)this->x, (S)this->y, (S)this->z, (S)this->w);
        };

#ifdef D3DX_CONVERSION_FUNCS
        Vector4(D3DXVECTOR4 v)
        {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;
        }

        Vector4<T> & operator=(const D3DXVECTOR4 & other)
        {
            memcpy(this, other, sizeof(float)*4);
        };

        operator D3DXVECTOR4()
        {
            return *(D3DXVECTOR3*)this;
        };
#endif
    };


    typedef Vector2<float>    fvec2;
    typedef Vector3<float>    fvec3;
    typedef Vector4<float>    fvec4;

    typedef Vector2<uint32>   uvec2;
    typedef Vector3<uint32>   uvec3;
    typedef Vector4<uint32>   uvec4;

    typedef Vector2<int32>    ivec2;
    typedef Vector3<int32>    ivec3;
    typedef Vector4<int32>    ivec4;

    extern COMMON_API const fvec3 UnitX;
    extern COMMON_API const fvec3 UnitY;
    extern COMMON_API const fvec3 UnitZ;

    template<typename T>
    struct Matrix4x4
    {
        union
        {
            T Cells[16];
            T Data[4][4];
        };

        Matrix4x4()
        {
            memset(Data, 0, sizeof(T)*16);
        }

        Matrix4x4(T value)
        {
            memset(Data, value, sizeof(T)*16);
        }

        Matrix4x4(T * values)
        {
            memcpy(Data, values, sizeof(T)*16);
        }

        Matrix4x4(Vector4<T> * vecs)
        {
            memcpy(Data, vecs, sizeof(T)*16);
        }

        T * operator()(uint32 row)
        {
            return Data[row];
        }

        T operator()(uint32 row, uint32 col)
        {
            return Data[row][col];
        }

        Matrix4x4<T> operator=(const Matrix4x4<T> other)
        {
            memcpy(this->Data, other.Data, sizeof(T)*16);

            return *this;
        }

        Vector4<T> Row(uint32 row)
        {
            return Vector4<T>(Data[row][0], Data[row][1], Data[row][2], Data[row][3]);
        }

        Vector4<T> Col(uint32 col)
        {
            return Vector4<T>(Data[0][col], Data[1][col], Data[2][col], Data[3][col]);
        }

#ifdef D3DX_CONVERSION_FUNCS
        Matrix4x4(D3DXMATRIX v)
        {
            memcpy(Data, v, sizeof(float)*16);
        };

        operator D3DXMATRIX()
        {
            return *(D3DXMATRIX*)this;
        };

        Matrix4x4<T> & operator=(const D3DXMATRIX & other)
        {
            memcpy(this, &other, sizeof(float)*16);
        };
#endif
    };


    typedef Matrix4x4<float>  fmat4x4;
    typedef Matrix4x4<uint32> umat4x4;
    typedef Matrix4x4<int32>  imat4x4;

    /**
     * Standard vertex format. Contains position, normal, texture and color.
     * Not compatible with DirectX without a shader.
     */
    struct COMMON_API Vertex
    {
        fvec3 Position;
        fvec3 Normal;
        fvec3 Texture;
        fvec4 Color;

        Vertex()
            : Position(), Normal(), Texture(), Color()
        { };

        Vertex(fvec3 pos, fvec3 norm, fvec3 tex, fvec4 color)
            : Position(pos), Normal(norm), Texture(tex), Color(color)
        { };

        Vertex
        (
            float x, float y, float z,
            float nx, float ny, float nz, 
            float tu, float tv, float tw, 
            float cr, float cg, float cb, float ca
        )
            : Position(x, y, z), Normal(nx, ny, nz), Texture(tu, tv, tw), Color(cr, cg, cb, ca)
        { };
    };

    class GenericCamera
    {
    public:
        virtual void      Rotate(float yaw, float pitch) = 0;
        virtual float     GetYaw() = 0;
        virtual void      SetYaw(float yaw) = 0;
        virtual float     GetPitch() = 0;
        virtual void      SetPitch(float pitch) = 0;
        virtual fvec2     GetRotation() = 0;
        virtual void      SetRotation(fvec2 rotation) = 0;
        virtual void      SetRotation(float yaw, float pitch) = 0;

        virtual void      Translate(fvec3 translate) = 0;
        virtual void      TranslateRaw(fvec3 translate) = 0;
        virtual fvec3     Transform(fvec3 shift) = 0;

        virtual fvec3     GetPosition() = 0;
        virtual void      SetPosition(fvec3 pos) = 0;

        virtual fmat4x4 * GetViewMatrix() = 0;
        virtual fmat4x4 * GetProjMatrix() = 0;

        virtual int32     Clip(fvec3 aabbMin, fvec3 aabbMax) = 0;
    };

#   define OBJECT_TYPE_UNKNOWN    0x00

    class GenericObject
    {
    public:
        virtual uint32 GetObjectType() = 0;

        virtual fvec3 GetPosition() = 0;
        virtual fvec3 GetSize() = 0;
    };
}
