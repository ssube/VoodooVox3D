
#include "Generator.hpp"

#define NOISESIZE 16
#define INDEX(x, y, z) ((x*NOISESIZE*NOISESIZE)+(y*NOISESIZE)+z)

Generator::Generator(int seed)
{
	mValues = new int[NOISESIZE * NOISESIZE * NOISESIZE];

	// Fill the noise bin
	srand(seed);
	
	for ( int x = 0; x < 16; ++x )
	{
		for ( int y = 0; y < 16; ++y )
		{
			for ( int z = 0; z < 16; ++z )
			{
				mValues[INDEX(x, y, z)] = abs(rand()) % 0xFFFF;
			}
		}
	}
}

Generator::~Generator()
{
	delete[] mValues;
}

float Generator::GetPoint(float x, float y, float z)
{
	int cellx = floor(x); int nextx = cellx + 1;
	int celly = floor(y); int nexty = celly + 1;
	int cellz = floor(z); int nextz = cellz + 1;

	cellx = cellx % NOISESIZE; nextx = nextx % NOISESIZE;
	celly = celly % NOISESIZE; nexty = nexty % NOISESIZE;
	cellz = cellz % NOISESIZE; nextz = nextz % NOISESIZE;

	float offx = x - cellx;
	float offy = y - celly;
	float offz = z - cellz;

	size_t offset0 = INDEX(x, y, z);
	size_t offsetX = offset0 + ( NOISESIZE * NOISESIZE );
	size_t offsetY = offset0 + NOISESIZE;
	size_t offsetZ = offset0 + 1;

	float tx = Interp(ToFloat(mValues[offset0]), ToFloat(mValues[offsetX]), offx);
	float ty = Interp(ToFloat(mValues[offset0]), ToFloat(mValues[offsetY]), offy);
	float tz = Interp(ToFloat(mValues[offset0]), ToFloat(mValues[offsetZ]), offz);

	return ( tx + ty + tz ) / 3.0f;
}

float Generator::Interp(float a, float b, float x)
{
	float ft = x * 3.14159f;
	float f = ( 1 - cos(ft) ) * 0.5;
	return ( a * ( 1 - f ) ) + ( b * f );
}

float Generator::ToFloat(int i)
{
	return ( (float)i / (float)0xFFFF );
}
