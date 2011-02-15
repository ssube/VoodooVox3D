#pragma once

#include <stdlib.h>
#include "CommonTypes.hpp"
#include "Math.hpp"

#define COHERENTNOISE_SIZE 64

namespace Common
{
	namespace Noise
	{
		class __declspec(dllexport) CoherentNoise
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
