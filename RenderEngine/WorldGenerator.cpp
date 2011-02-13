
#include "WorldGenerator.hpp"

Generator::Generator(int seed)
{
	// Fill the noise bin
	DWORD ticks = GetTickCount();
	srand(ticks);
	
	for ( int x = 0; x < WORLD_BLOCKS; ++x )
	{
		for ( int y = 0; y < WORLD_BLOCKS; ++y )
		{
			for ( int z = 0; z < WORLD_BLOCKS; ++z )
			{
				mValues[x][y][z] = rand() % 32768;
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

	cellx = cellx % WORLD_BLOCKS; nextx = nextx % WORLD_BLOCKS;
	celly = celly % WORLD_BLOCKS; nexty = nexty % WORLD_BLOCKS;
	cellz = cellz % WORLD_BLOCKS; nextz = nextz % WORLD_BLOCKS;

	float offx = x - cellx;
	float offy = y - celly;
	float offz = z - cellz;

	float x0y0z0 =	ToFloat(mValues[cellx][celly][cellz]);
	float x0y0z1 =	ToFloat(mValues[cellx][celly][nextz]);
	float x0y1z0 =	ToFloat(mValues[cellx][nexty][cellz]);
	float x0y1z1 =	ToFloat(mValues[cellx][nexty][nextz]);
	float x1y0z0 =	ToFloat(mValues[nextx][celly][cellz]);
	float x1y0z1 =	ToFloat(mValues[nextx][celly][nextz]);
	float x1y1z0 =	ToFloat(mValues[nextx][nexty][cellz]);
	float x1y1z1 =	ToFloat(mValues[nextx][nexty][nextz]);
	float x0y0 =	Interp(x0y0z0, x0y0z1, offz);
	float x0y1 =	Interp(x0y1z0, x0y1z1, offz);
	float x1y0 =	Interp(x1y0z0, x1y0z1, offz);
	float x1y1 =	Interp(x1y1z0, x1y1z1, offz);
	float x0 =		Interp(x0y0, x0y1, offy);
	float x1 =		Interp(x1y0, x1y1, offy);
	float final =	Interp(x0, x1, offx);

	return final;
}

float Generator::Interp(float a, float b, float x)
{
	float ft = x * 3.14159f;
	float f = ( 1 - cos(ft) ) * 0.5;
	return ( a * ( 1 - f ) ) + ( b * f );
}

float Generator::ToFloat(int i)
{
	return ( (float)i / 32768.0f );
}
