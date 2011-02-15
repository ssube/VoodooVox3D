
#include "CommonTypes.hpp"

namespace Common
{
	unsigned long Vertex::DXColor()
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
