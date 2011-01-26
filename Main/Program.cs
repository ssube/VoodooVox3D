using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using BoxEngine.RenderEngine;

namespace Main
{
    class Program
    {
        static private Renderer mRenderer;

        static void Main(string[] args)
        {
            mRenderer = new Renderer(640, 480);

            Vertex[] verts = new Vertex[]
            {
                new Vertex(250.0f,   0.0f,   0.0f,   0.0f, Color.Green),
                new Vertex(300.0f, 125.0f,   0.0f,   0.0f, Color.Blue),
                new Vertex(125.0f, 200.0f,   0.0f,   0.0f, Color.Red)
            };

            RenderObject tri = mRenderer.CreateObject();
            tri.SetGeometry(verts);

            mRenderer.StartLoop();
        }
    }
}
