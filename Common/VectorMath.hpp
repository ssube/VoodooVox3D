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

		unsigned long DXColor(fvec4 Color)
		{
			unsigned long color = 0;
			unsigned char r = (size_t)(Color.x * 255.0f);
			unsigned char g = (size_t)(Color.y * 255.0f);
			unsigned char b = (size_t)(Color.z * 255.0f);
			unsigned char a = (size_t)(Color.w * 255.0f);
			color = ( a << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
			return color;
		}
	}
}
