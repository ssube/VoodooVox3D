#pragma once

#include "CommonDefs.hpp"

namespace Common
{
    /**
     * Simple 2-component vector. When declared with Vector2<float>, this is compatible
     * with D3DXVECTOR2.
     */
    template<typename T>
    struct Vector2
    {
        T x, y;

        Vector2()
            : x(), y()
        { };

        Vector2(T val)
            : x(val), y(val)
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

#ifdef D3DX_CONVERSION_FUNCS
        operator D3DXVECTOR2()
        {
            //return D3DXVECTOR2((float)x, (float)y);
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

        Vector2(D3DXVECTOR2 v)
            : x(v.x), y(v.y)
        {

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
        T x, y, z;

        Vector3()
            : x(), y(), z()
        { };

        Vector3(T val)
            : x(val), y(val), z(val)
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

#ifdef D3DX_CONVERSION_FUNCS
        operator D3DXVECTOR3()
        {
            //return D3DXVECTOR3((float)x, (float)y, (float)z);
            return *(D3DXVECTOR3*)this;
        };

        operator D3DXVECTOR4()
        {
            return D3DXVECTOR4((float)x, (float)y, (float)z, 1.0f);
        };

        Vector3(D3DXVECTOR3 v)
            : x(v.x), y(v.y), z(v.z)
        {

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
        T x, y, z, w;

        Vector4()
            : x(), y(), z(), w()
        { };

        Vector4(T val)
            : x(val), y(val), z(val), w(val)
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

#ifdef D3DX_CONVERSION_FUNCS
        operator D3DXVECTOR4()
        {
            //return D3DXVECTOR4((float)x, (float)y, (float)z, (float)w);
            return *(D3DXVECTOR3*)this;
        };

        Vector4(D3DXVECTOR4 v)
            : x(v.x), y(v.y), z(v.z), w(v.w)
        {

        };
#endif
    };

    template<typename T>
    struct Matrix
    {
        union
        {
            Vector4<T> Vectors[4];
            T Cells[4][4];
            T Data[16];
        };

        Matrix()
        {
            memset(Data, 0, sizeof(T)*16);
        }

        Matrix(T value)
        {
            memset(Data, value, sizeof(T)*16);
        }

        Matrix(T * values)
        {
            memcpy(Data, values, sizeof(T)*16);
        }

        Matrix(Vector4<T> * vecs)
        {
            Vectors[0] = vecs[0];
            Vectors[1] = vecs[1];
            Vectors[2] = vecs[2];
            Vectors[3] = vecs[3];
        }

#ifdef D3DX_CONVERSION_FUNCS
        operator D3DXMATRIX()
        {
            return *(D3DXMATRIX*)this;
        };

        Matrix<float>(D3DXMATRIX v)
        {
            memcpy(mCells, v, sizeof(float)*16);
        };
#endif
    };

#    define INDEX2(o, n) o[n.x][n.y]
#    define INDEX3(o, n) o[n.x][n.y][n.z]
#    define INDEX4(o, n) o[n.x][n.y][n.z][n.w]

    typedef unsigned __int8  uint8;
    typedef __int8           int8;
    typedef unsigned __int16 uint16;
    typedef __int16          int16;
    typedef unsigned __int32 uint32;
    typedef __int32          int32;
    typedef unsigned __int64 uint64;
    typedef __int64          int64;

    typedef Vector2<float>   fvec2;
    typedef Vector3<float>   fvec3;
    typedef Vector4<float>   fvec4;

    typedef Vector2<uint32>  uvec2;
    typedef Vector3<uint32>  uvec3;
    typedef Vector4<uint32>  uvec4;

    typedef Vector2<int32>   ivec2;
    typedef Vector3<int32>   ivec3;
    typedef Vector4<int32>   ivec4;

    typedef Matrix<float>    fmat4x4;
    typedef Matrix<uint32>   umat4x4;
    typedef Matrix<int32>    imat4x4;

    /**
     * Standard vertex format. Contains position, normal, texture and color.
     * Not compatible with DirectX without a shader.
     */
    struct Vertex
    {
        fvec4 Position;
        fvec3 Normal;
        fvec3 Texture;
        fvec4 Color;

        Vertex()
            : Position(0.0f), Normal(0.0f), Texture(0.0f), Color(0.0f, 0.0f, 0.0f, 1.0f)
        { };

        Vertex
        (
            float x, float y, float z,
            float nx, float ny, float nz, 
            float tu, float tv, float tw, 
            float cr, float cg, float cb, float ca
        )
            : Position(x, y, z, 1.0f), Normal(nx, ny, nz), Texture(tu, tv, tw), Color(cr, cg, cb, ca)
        { };

        Vertex(fvec4 pos, fvec3 norm, fvec3 tex, fvec4 color)
            : Position(pos), Normal(norm), Texture(tex), Color(color)
        { };
    };
}
