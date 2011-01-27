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

			public Generator()
			{
				mInternal = new BasicNoiseGenerator(System.DateTime.Now.Millisecond, 255);
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
