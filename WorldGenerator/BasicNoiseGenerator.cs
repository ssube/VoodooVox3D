using System;
using System.Collections.Generic;
using System.Text;

namespace BoxEngine
{
	namespace WorldGenerator
	{
		private class BasicNoiseGenerator
		{
			int mPrimarySeed;
			int mMax;
			Random mRand;

			public BasicNoiseGenerator(int seed, int max)
			{
				mPrimarySeed = seed;
				mMax = max;
			}

			public int GetPoint(int x, int y)
			{
				mRand = new Random(mPrimarySeed * x);
				int py = 0;
				while (py < y)
				{
					mRand.Next(mMax);
					++py;
				}
				return mRand.Next();
			}
		}
	}
}
