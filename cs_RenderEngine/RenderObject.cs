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
			public Vector3 Normal;
			public Vector3 TexCoord;
            public int Color;

			public Color SysColor
			{
				set
				{
					Color = value.ToArgb();
				}
			}

            public Vertex(Vector4 pos, Vector3 norm, Vector3 tex, Color color)
            {
                Position = pos;
				Normal = norm;
				TexCoord = tex;
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
					new VertexElement(0, 16, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.Normal, 0),
					new VertexElement(0, 28, DeclarationType.Float3, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),
                    new VertexElement(0, 40, DeclarationType.Color,  DeclarationMethod.Default, DeclarationUsage.Color, 0),
                    VertexElement.VertexDeclarationEnd
                };

                mVertDecl = new VertexDeclaration(mDevice, elems);
                mVertSize = Marshal.SizeOf(typeof(Vertex));
            }

			public void SetGeometry(List<Vertex> data)
			{
				if (mVB != null)
				{
					mVB.Dispose();
				}

				mVertCount = data.Count;
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
					Console.WriteLine("Error unlocking vb.");
				}
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
                    Console.WriteLine("Error unlocking vb.");
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
				mDevice.SetRenderState(RenderState.CullMode, Cull.Counterclockwise);
				mDevice.SetRenderState(RenderState.ColorVertex, true);
				mDevice.SetRenderState(RenderState.Lighting, false);

                Result hr = mDevice.BeginScene();

                if (hr.IsSuccess)
                {
                    mDevice.VertexDeclaration = mVertDecl;

                    hr = mDevice.SetStreamSource(0, mVB, 0, mVertSize);

                    if (hr.IsSuccess)
                    {
                        hr = mDevice.DrawPrimitives(PrimitiveType.TriangleList, 0, mVertCount);

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