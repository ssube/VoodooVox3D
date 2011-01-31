using namespace System;
using namespace System::Collections::Generic;

//#include <noise/noise.h>
//#include <noise/module/perlin.h>

//#pragma comment(lib, "libnoise.lib")

namespace BoxEngine
{
	namespace WorldGenerator
	{
		public ref class Generator
		{
		public:
			Generator(int seed)
			{
				mValues = gcnew cli::array<int, 3>(16, 16, 16);

				// Fill the noise bin
				Random ^ rnd = gcnew System::Random(seed);

				for ( int x = 0; x < 16; ++x )
				{
					for ( int y = 0; y < 16; ++y )
					{
						for ( int z = 0; z < 16; ++z )
						{
							mValues[x, y, z] = rnd->Next();
						}
					}
				}
			}

			~Generator()
			{
			}

			float GetPoint(float x, float y, float z)
			{
				int cellx = (int)Math::Floor(x); int nextx = cellx + 1;
				int celly = (int)Math::Floor(y); int nexty = celly + 1;
				int cellz = (int)Math::Floor(z); int nextz = cellz + 1;

				cellx = cellx % 16; nextx = nextx % 16;
				celly = celly % 16; nexty = nexty % 16;
				cellz = cellz % 16; nextz = nextz % 16;

				float offx = x - cellx;
				float offy = y - celly;
				float offz = z - cellz;

				float tx = Interp(ToFloat(mValues[cellx, celly, cellz]), ToFloat(mValues[cellx+1, celly, cellz]), offx);
				float ty = Interp(ToFloat(mValues[cellx, celly, cellz]), ToFloat(mValues[cellx, celly+1, cellz]), offy);
				float tz = Interp(ToFloat(mValues[cellx, celly, cellz]), ToFloat(mValues[cellx, celly, cellz+1]), offz);

				return ( tx + ty + tz ) / 3.0f;
			}

			private:
				//noise::module::Perlin * mPerlin;
				cli::array<int, 3> ^ mValues;

				float Interp(float a, float b, float x)
				{
					float ft = x * Math::PI;
					float f = ( 1 - Math::Cos(ft)) * 0.5;
					return ( a * ( 1-f) ) + ( b * f );
				}

				float ToFloat(int i)
				{
					return ( (float)i / (float)0x7FFFFFFF );
				}
		};
	}
}
