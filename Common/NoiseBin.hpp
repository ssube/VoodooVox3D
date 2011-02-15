#pragma once

#include <vector>
#include "Math.hpp"

namespace Common
{
	namespace Noise
	{
		class __declspec(dllexport) NoiseBin
		{
		public:
			NoiseBin(size_t seed, size_t items);
			~NoiseBin(void);

			float Next();
			void Fill();

		private:
			size_t mSeed, mCount;
			std::vector<float> mBin;
		};
	}
}
