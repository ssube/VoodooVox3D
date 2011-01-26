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

			public Chunk(WorldGenerator.Generator gen)
			{
				int[,] grid = gen.CreateGrid(4, 4);

			}
        }
    }
}
