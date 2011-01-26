using System;
using System.Collections.Generic;
using System.Text;

namespace BoxEngine
{
	namespace WorldGenerator
	{
		public class Generator
		{
			private BasicNoiseGenerator mInternal;
			private int mMax;

			public Generator(int max)
			{
				mMax = max;
				mInternal = new BasicNoiseGenerator(System.DateTime.Now.Millisecond, max);
			}

			public int[,] CreateGrid(int x, int y)
			{
				int[,] values = new int[x,y];

				for (int px = 0; px < x; ++px)
				{
					for (int py = 0; py < y; ++py)
					{
						values[px, py] = mInternal.GetPoint(x, y);
					}
				}

				return values;
			}
		}
	}
}
