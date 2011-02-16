
#include "CoherentNoise.hpp"

namespace Common
{
	namespace Noise
	{
		CoherentNoise::CoherentNoise(int seed)
		{
			// Fill the noise bin
			srand(seed);
			
			for ( int x = 0; x < COHERENTNOISE_SIZE; ++x )
			{
				for ( int y = 0; y < COHERENTNOISE_SIZE; ++y )
				{
					for ( int z = 0; z < COHERENTNOISE_SIZE; ++z )
					{
						mValues[x][y][z] = rand() / 32767.0f;
					}
				}
			}
		}

		float CoherentNoise::GetPoint(fvec3 position)
		{
			int cellx = (int)floor(position.x); int nextx = cellx + 1;
			int celly = (int)floor(position.y); int nexty = celly + 1;
			int cellz = (int)floor(position.z); int nextz = cellz + 1;

			cellx = cellx % COHERENTNOISE_SIZE; nextx = nextx % COHERENTNOISE_SIZE;
			celly = celly % COHERENTNOISE_SIZE; nexty = nexty % COHERENTNOISE_SIZE;
			cellz = cellz % COHERENTNOISE_SIZE; nextz = nextz % COHERENTNOISE_SIZE;

			float offx = position.x - cellx;
			float offy = position.y - celly;
			float offz = position.z - cellz;

			float x0y0z0 =	mValues[cellx][celly][cellz];
			float x0y0z1 =	mValues[cellx][celly][nextz];
			float x0y1z0 =	mValues[cellx][nexty][cellz];
			float x0y1z1 =	mValues[cellx][nexty][nextz];
			float x1y0z0 =	mValues[nextx][celly][cellz];
			float x1y0z1 =	mValues[nextx][celly][nextz];
			float x1y1z0 =	mValues[nextx][nexty][cellz];
			float x1y1z1 =	mValues[nextx][nexty][nextz];
			float x0y0 =	Math::CosInterp(x0y0z0, x0y0z1, offz);
			float x0y1 =	Math::CosInterp(x0y1z0, x0y1z1, offz);
			float x1y0 =	Math::CosInterp(x1y0z0, x1y0z1, offz);
			float x1y1 =	Math::CosInterp(x1y1z0, x1y1z1, offz);
			float x0 =		Math::CosInterp(x0y0, x0y1, offy);
			float x1 =		Math::CosInterp(x1y0, x1y1, offy);
			float final =	Math::CosInterp(x0, x1, offx);

			return final;
		}

		float CoherentNoise::GetOctavePoint(fvec3 position, size_t octaves)
		{
			float sum = 0.0f;

			for ( size_t octave = 0; octave < octaves; ++octave )
			{
				float factor = pow(2.0f, (float)octave + 1.0f);
				sum += this->GetPoint(position * factor) / factor;
			}

			return sum;
		}
	}
}
