using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using BoxEngine.RenderEngine;
using BoxEngine.ChunkEngine;
using BoxEngine.WorldGenerator;

namespace Main
{
    class Program
    {
        static private Renderer mRenderer;

        static void Main(string[] args)
        {
			BlockDictionary dict = BlockDictionary.FromFile("blocks.dict");
			Generator gen = new Generator();
			Chunk mChunk = new Chunk(dict, gen);

            mRenderer = new Renderer(640, 480);

            Vertex[] verts = new Vertex[]
            {
                new Vertex(320.0f, 080.0f, 0.5f, 1.0f, Color.Green),
                new Vertex(520.0f, 400.0f, 0.5f, 1.0f, Color.Blue ),
                new Vertex(120.0f, 400.0f, 0.5f, 1.0f, Color.Red  )
            };

            RenderObject tri = mRenderer.CreateObject();
            tri.SetGeometry(verts);

            mRenderer.StartLoop();
        }
    }
}
