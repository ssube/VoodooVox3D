using namespace System;
using namespace System::Collections::Generic;

#include <noise/noise.h>
#include <noise/module/perlin.h>

#pragma comment(lib, "libnoise.lib")

namespace BoxEngine
{
	namespace WorldGenerator
	{
		public ref class Generator
		{
		public:
			Generator()
			{
				mPerlin = new noise::module::Perlin();	
				//mPerlin->SetOctaveCount(4);
				//mPerlin->SetFrequency(1.0f);
				//mPerlin->SetPersistence(0.5f);
			}

			~Generator()
			{
				delete mPerlin;
			}

			double GetPoint(double x, double y, double z)
			{
				return mPerlin->GetValue(x, y, z);
			}

			private:
				noise::module::Perlin * mPerlin;
		};
	}
}
