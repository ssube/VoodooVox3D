#include "NoiseBin.hpp"

using namespace Common::Math;

namespace Common
{
    namespace Noise
    {
        NoiseBin::NoiseBin(size_t seed, size_t items)
            : mSeed(seed), mCount(items)
        {
            Fill();
        }

        NoiseBin::~NoiseBin(void)
        {
        }

        float NoiseBin::Next()
        {
            std::vector<float>::iterator current = mBin.begin();
            if ( current == mBin.end() )
            {
                // Empty, refill
                Fill();

                current = mBin.begin();
            }

            float item = *current;
            mBin.erase(current);
            return item;
        }

        void NoiseBin::Fill()
        {
            srand(mSeed);
            for ( size_t i = 0; i < mCount; ++i )
            {
                float val = Math::Bounce(rand(), 1024) / 1024.0f;
                mBin.push_back(val);
            }
        }
    }
}
