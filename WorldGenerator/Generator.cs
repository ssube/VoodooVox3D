using System;
using System.Collections.Generic;
using System.Text;

namespace BoxEngine
{
	namespace WorldGenerator
	{
		public class Generator
		{
			private int mSeed;

			public Generator()
			{
				mSeed = System.DateTime.Now.Millisecond;
			}

			public int[,] CreateGrid(int offsetx, int offsety, int sizex, int sizey)
			{
				Random rng = new Random(offsetx * offsety);

				int[,] values = new int[sizex, sizey];

				for (int px = 0; px < sizex; ++px)
				{
					for (int py = 0; py < sizey; ++py)
					{
						values[px, py] = rng.Next();
					}
				}

				return values;
			}
		}
	}
}
