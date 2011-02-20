#pragma once

#include "CommonDefs.hpp"

#include <stdlib.h>
#include "CommonTypes.hpp"
#include "Math.hpp"

#define COHERENTNOISE_SIZE 64

namespace Common
{
	namespace Noise
	{
		class COMMON_API CoherentNoise
		{
		public:
			CoherentNoise(int seed);

			float GetPoint(fvec3 position);
			float GetOctavePoint(fvec3 position, size_t octaves);

		private:
			float mValues[COHERENTNOISE_SIZE][COHERENTNOISE_SIZE][COHERENTNOISE_SIZE];
		};
	}
}
