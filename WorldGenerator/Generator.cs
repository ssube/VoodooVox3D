using System;
using System.Collections.Generic;
using System.Text;

namespace BoxEngine
{
	namespace WorldGenerator
	{
		public class Generator
		{
			private WorldGenerator.NativeGenerator mNative;

			public Generator()
			{
				mNative = new WorldGenerator.NativeGenerator();
				mNative.AddID(0);
				mNative.AddID(1);
				mNative.AddID(2);
			}

			public int[,] CreateGrid(int offsetx, int offsety, int sizex, int sizey)
			{
				int[,] values = new int[sizex, sizey];

				for (int px = 0; px < sizex; ++px)
				{
					for (int py = 0; py < sizey; ++py)
					{
						int fullx = offsetx + px;

						values[px, py] = mNative.GetPoint(offsetx + px, offsety + py, 0.0f);
					}
				}

				return values;
			}
		}
	}
}
