using System;
using System.Collections.Generic;
using System.Text;

namespace BoxEngine
{
    namespace ChunkEngine
    {   
        public class Chunk
        {
            private Block[,] mBlocks;

			public Chunk(BlockDictionary dict, WorldGenerator.Generator gen)
			{
				mBlocks = new Block[4, 4];

				int max = dict.GetTemplateCount();
				List<int> keys = dict.GetTemplateKeys();

				int[,] grid = gen.CreateGrid(4, 4);

				for (int px = 0; px < 4; ++px)
				{
					for (int py = 0; py < 4; ++py)
					{
						int position = grid[px, py] % max;
						BlockTemplate pt = dict.GetTemplate(keys[position]);
						mBlocks[px, py] = new Block(pt);
					}
				}
			}
		}
    }
}
