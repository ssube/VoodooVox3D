#pragma once

namespace Common
{
	/**
	 * Simple 2-component vector. When declared with Vector2<float>, this is compatible
	 * with D3DXVECTOR2.
	 */
	template<typename T>
	struct __declspec(dllexport) Vector2
	{
		T x, y;

#ifdef D3DX_CONVERSION_FUNCS
		operator D3DXVECTOR2()
		{
			return D3DXVECTOR2((float)x, (float)y);
		}

		operator D3DXVECTOR3()
		{
			return D3DXVECTOR3((float)x, (float)y, 0.0f);
		}

		operator D3DXVECTOR4()
		{
			return D3DXVECTOR4((float)x, (float)y, 0.0f, 1.0f);
		}

		Vector2(D3DXVECTOR2 v)
			: x(v.x), y(v.y)
		{

		}
#endif

		Vector2()
			: x(), y()
		{ };

		Vector2(T val)
			: x(val), y(val)
		{ };

		Vector2(T x, T y)
			: x(x), y(y)
		{ };

		Vector2<T> operator+(Vector2<T> other)
		{
			return Vector2<T>(this->x + other.x, this->y + other.y);
		}

		Vector2<T> operator-(Vector2<T> other)
		{
			return Vector2<T>(this->x - other.x, this->y - other.y);
		}

		Vector2<T> operator*(float value)
		{
			return Vector2<T>(this->x * value, this->y * value);
		}

		Vector2<T> operator/(float value)
		{
			return Vector2<T>(this->x / value, this->y / value);
		}
	};

	/**
	 * Simple 3-component vector. When declared with Vector3<float>, this is compatible
	 * with D3DXVECTOR3.
	 */
	template<typename T>
	struct __declspec(dllexport) Vector3
	{
		T x, y, z;

#ifdef D3DX_CONVERSION_FUNCS
		operator D3DXVECTOR3()
		{
			return D3DXVECTOR3((float)x, (float)y, (float)z);
		}

		operator D3DXVECTOR4()
		{
			return D3DXVECTOR4((float)x, (float)y, (float)z, 1.0f);
		}

		Vector3(D3DXVECTOR3 v)
			: x(v.x), y(v.y), z(v.z)
		{

		}
#endif

		Vector3()
			: x(), y(), z()
		{ };

		Vector3(T val)
			: x(val), y(val), z(val)
		{ };

		Vector3(T x, T y, T z)
			: x(x), y(y), z(z)
		{ };

		Vector3<T> operator+(Vector3<T> other)
		{
			return Vector3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
		}

		Vector3<T> operator-(Vector3<T> other)
		{
			return Vector3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
		}

		Vector3<T> operator*(float value)
		{
			return Vector3<T>(this->x * value, this->y * value, this->z * value);
		}

		Vector3<T> operator/(float value)
		{
			return Vector3<T>(this->x / value, this->y / value, this->z / value);
		}
	};

	/**
	 * Simple 4-component vector. When declared with Vector4<float>, this is compatible
	 * with D3DXVECTOR4.
	 */
	template<typename T>
	struct __declspec(dllexport) Vector4
	{
		T x, y, z, w;

#ifdef D3DX_CONVERSION_FUNCS
		operator D3DXVECTOR4()
		{
			return D3DXVECTOR4((float)x, (float)y, (float)z, (float)w);
		}

		Vector4(D3DXVECTOR4 v)
			: x(v.x), y(v.y), z(v.z), w(v.w)
		{

		}
#endif

		Vector4()
			: x(), y(), z(), w()
		{ };

		Vector4(T val)
			: x(val), y(val), z(val), w(val)
		{ };

		Vector4(T x, T y, T z, T w)
			: x(x), y(y), z(z), w(w)
		{ };

		Vector4<T> operator+(Vector4<T> other)
		{
			return Vector4<T>(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
		}

		Vector4<T> operator-(Vector4<T> other)
		{
			return Vector4<T>(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
		}

		Vector4<T> operator*(float value)
		{
			return Vector4<T>(this->x * value, this->y * value, this->z * value, this->w * value);
		}

		Vector4<T> operator/(float value)
		{
			return Vector4<T>(this->x / value, this->y / value, this->z / value, this->w / value);
		}
	};

#	define INDEX2(o, n) o[n.x][n.y]
#	define INDEX3(o, n) o[n.x][n.y][n.z]
#	define INDEX4(o, n) o[n.x][n.y][n.z][n.w]

	typedef Vector2<float>	fvec2;
	typedef Vector3<float>	fvec3;
	typedef Vector4<float>	fvec4;

	typedef Vector2<size_t> uvec2;
	typedef Vector3<size_t> uvec3;
	typedef Vector4<size_t> uvec4;

	typedef Vector2<int>	ivec2;
	typedef Vector3<int>	ivec3;
	typedef Vector4<int>	ivec4;


	/**
	 * Standard vertex format. Contains position, normal, texture and color (stored as an OpenGL
	 * compatible set of floats, with a helper method to get a DirectX DWORD).
	 */
	struct __declspec(dllexport) Vertex
	{
		Vector3<float> Position;
		Vector3<float> Normal;
		Vector3<float> Texture;
		Vector4<float> Color;

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
			: Position(x, y, z), Normal(nx, ny, nz), Texture(tu, tv, tw), Color(cr, cg, cb, ca)
		{ };

		unsigned long DXColor();
	};
}