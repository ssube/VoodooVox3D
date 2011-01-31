using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using BoxEngine.RenderEngine;
using BoxEngine.ChunkEngine;

namespace Main
{
    class Program
    {
        static private Renderer mRenderer;

        static void Main(string[] args)
        {
			BlockDictionary dict = BlockDictionary.FromFile("blocks.dict");
			Chunk mChunk = new Chunk(dict);
			mChunk.GenerateGeometry();

            mRenderer = new Renderer(640, 480);

            RenderObject tri = mRenderer.CreateObject();
            tri.SetGeometry(mChunk.GetGeometry());

            mRenderer.StartLoop();
        }
    }
}
