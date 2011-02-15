#pragma once

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

		float Luminance(Vector3<float> x)
		{
			return ( x.x * 0.27f + x.y * 0.67f + x.z * 0.06f );
		}
	}
}
