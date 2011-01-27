using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Runtime.InteropServices;
using SlimDX;
using SlimDX.Direct3D9;

namespace BoxEngine
{
    namespace RenderEngine
    {
        public struct Vertex
        {
            public Vector4 Position;
            public int Color;

            public Vertex(float x, float y, float z, float w, Color color)
            {
                Position = new Vector4(x, y, z, w);
                Color = color.ToArgb();
            }
        }

        public class RenderObject
        {
            private VertexBuffer mVB;
			private Effect mShader;
            private Material mMaterial;
            private Matrix mPosition;
            private Device mDevice;
            private VertexDeclaration mVertDecl;
            private int mVertSize;
            private int mVBSize;
            private int mVertCount;

            public RenderObject(Device device)
            {
                mDevice = device;
				mPosition = Matrix.Identity;

                var elems = new[]
                {
                    new VertexElement(0,  0, DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.Position, 0),
                    new VertexElement(0, 16, DeclarationType.Color,  DeclarationMethod.Default, DeclarationUsage.Color, 0),
                    VertexElement.VertexDeclarationEnd
                };

                mVertDecl = new VertexDeclaration(mDevice, elems);
                mVertSize = Marshal.SizeOf(typeof(Vertex));
            }

            public void SetGeometry(Vertex[] data)
            {
                if (mVB != null)
                {
                    mVB.Dispose();
                }

                mVertCount = data.Length;
                mVBSize = mVertCount * mVertSize;

                mVB = new VertexBuffer(mDevice, mVBSize, Usage.WriteOnly, VertexFormat.None, Pool.Default);

                DataStream verts = mVB.Lock(0, mVBSize, LockFlags.Discard);

                foreach (Vertex v in data)
                {
                    verts.Write(v);
                }

                Result hr = mVB.Unlock();
                if (hr.IsFailure)
                {
                    throw new Exception("Error unlocking vb.");
                }
            }

            public void Render()
            {
				if (mMaterial != null)
				{
					mDevice.Material = mMaterial;
				}
				
				mDevice.SetTransform(TransformState.World, mPosition);

				DrawGeometry();
			}

			public void DrawGeometry()
			{
				mDevice.SetRenderState(RenderState.CullMode, Cull.None);

                Result hr = mDevice.BeginScene();

                if (hr.IsSuccess)
                {
                    mDevice.VertexDeclaration = mVertDecl;
                    hr = mDevice.SetStreamSource(0, mVB, 0, mVertSize);

                    if (hr.IsSuccess)
                    {
                        hr = mDevice.DrawPrimitives(PrimitiveType.TriangleStrip, 0, mVertCount);

                        if (hr.IsFailure)
                        {
                            Console.WriteLine("Failed drawing geometry.");
                        }
                    }
                    else
                    {
                        Console.WriteLine("Failed setting stream source.");
                    }

                    mDevice.EndScene();
                }
                else
                {
                    Console.WriteLine("Failed to begin scene.");
                }
            }
        }
    }
}