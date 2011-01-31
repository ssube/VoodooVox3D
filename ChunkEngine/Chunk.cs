using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using BoxEngine.RenderEngine;
using SlimDX;

namespace BoxEngine
{
	namespace ChunkEngine
	{
		public class Chunk
		{
			private WorldGenerator.Generator mGen;
			private Block[, ,] mBlocks;

			// System memory geometry shadow buffer
			private List<Vertex> mVerts;

			public Chunk(BlockDictionary dict)
			{
				mVerts = new List<Vertex>();
				mGen = new BoxEngine.WorldGenerator.Generator(13);
				mBlocks = new Block[4, 4, 4];

				//Random rnd = new Random();

				int max = dict.GetTemplateCount();
				List<int> keys = dict.GetTemplateKeys();

				for (int px = 0; px < 4; ++px)
				{
					for (int py = 0; py < 4; ++py)
					{
						for (int pz = 0; pz < 4; ++pz)
						{
							//double rawvalue = mGen.GetPoint((float)px, (float)py, 10.0f);
							float raw = mGen.GetPoint(px * 4.0f, py * 4.0f, pz * 4.0f); //rnd.Next() % max;
							int id = (int)( raw * max );

							BlockTemplate pt = dict.GetTemplate(keys[id]);

							mBlocks[px, py, pz] = new Block(pt);
						}
					}
				}
			}

			public List<Vertex> GetGeometry()
			{
				return mVerts;
			}

			public void GenerateGeometry()
			{
				mVerts.Clear();

				for (int px = 0; px < 4; ++px)
				{
					for (int py = 0; py < 4; ++py)
					{
						for (int pz = 0; pz < 4; ++pz)
						{
							ProcessPoint(px, py, pz);
						}
					}
				}
			}

			private void AddVertex(float x, float y, float z, float nx, float ny, float nz, float tu, float tv, float tw, Color color)
			{
				mVerts.Add(new Vertex(new Vector4(x, y, z, 1.0f), new Vector3(nx, ny, nz), new Vector3(tu, tv, tw), color));
			}

			/// <summary>
			/// Generates the necessary geometry for a given point in the
			/// chunk volume.
			/// </summary>
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <param name="z"></param>
			/// <returns></returns>
			private void ProcessPoint(int x, int y, int z)
			{
				// Flag names and chunk sizes
				int minSize = 0, maxSize = 3;
				String attrTex = "fTexture", attrOcc = "bOcclude";

				// Build the flags
				float texture = 0.0f;
				Boolean[, ,] b = new Boolean[3, 3, 3];

				try
				{
					Block block = mBlocks[x, y, z];
					b[1, 1, 1] = block.GetAttribute<Boolean>(attrOcc);
					texture = block.GetAttribute<float>(attrTex);
				}
				catch
				{
					b[1, 1, 1] = false;
				}

				try
				{
					Block block = mBlocks[x - 1, y, z];
					b[0, 1, 1] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[0, 1, 1] = false;
				}

				try
				{
					Block block = mBlocks[x + 1, y, z];
					b[2, 1, 1] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[2, 1, 1] = false;
				}

				try
				{
					Block block = mBlocks[x, y - 1, z];
					b[1, 0, 1] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[1, 0, 1] = false;
				}

				try
				{
					Block block = mBlocks[x, y + 1, z];
					b[1, 2, 1] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[1, 2, 1] = false;
				}

				try
				{
					Block block = mBlocks[x, y, z - 1];
					b[1, 1, 0] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[1, 1, 0] = false;
				}

				try
				{
					Block block = mBlocks[x, y, z + 1];
					b[1, 1, 2] = block.GetAttribute<Boolean>(attrOcc);
				}
				catch
				{
					b[1, 1, 2] = false;
				}

				// Calculate the offset and positions
				float BOXENGINE_BLOCK_UNITS = 10.0f;
				float offset = (BOXENGINE_BLOCK_UNITS / 2);
				float px = (float)x * BOXENGINE_BLOCK_UNITS;
				float py = (float)y * BOXENGINE_BLOCK_UNITS;
				float pz = (float)z * BOXENGINE_BLOCK_UNITS;

				// Now, build geometry. We'll test each of the 6 surfaces and create a quad
				// if one should exist. The important thing here is to make sure the quad
				// points the right way, that is, toward the empty block.

				// Test the left:
				if (b[1, 1, 1] && !b[0, 1, 1])
				{
					// On the -X, facing -X, with b[1, 1]'s texture
					AddVertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, Color.Blue);
					AddVertex(px - offset, py - offset, pz + offset, -1, 0, 0, 0, 0, texture, Color.Orange);
					AddVertex(px - offset, py + offset, pz + offset, -1, 0, 0, 0, 0, texture, Color.Blue);

					AddVertex(px - offset, py + offset, pz + offset, -1, 0, 0, 0, 0, texture, Color.Blue);
					AddVertex(px - offset, py + offset, pz - offset, -1, 0, 0, 0, 0, texture, Color.Red);
					AddVertex(px - offset, py - offset, pz - offset, -1, 0, 0, 0, 0, texture, Color.Blue);
				}

				// Test the right:
				if (b[1, 1, 1] && !b[2, 1, 1])
				{
					// On the +X, facing -X
					AddVertex(px + offset, py + offset, pz + offset, +1, 0, 0, 0, 0, texture, Color.Orange);
					AddVertex(px + offset, py - offset, pz + offset, +1, 0, 0, 0, 0, texture, Color.Green);
					AddVertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, Color.Orange);

					AddVertex(px + offset, py - offset, pz - offset, +1, 0, 0, 0, 0, texture, Color.Orange);
					AddVertex(px + offset, py + offset, pz - offset, +1, 0, 0, 0, 0, texture, Color.Green);
					AddVertex(px + offset, py + offset, pz + offset, +1, 0, 0, 0, 0, texture, Color.Orange);
				}

				// Test the bottom:
				if (b[1, 1, 1] && !b[1, 0, 1])
				{
					// On the -Y, facing -Y
					AddVertex(px + offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, Color.Orange);
					AddVertex(px + offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, Color.Green);
					AddVertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, Color.Orange);

					AddVertex(px - offset, py - offset, pz + offset, 0, -1, 0, 0, 0, texture, Color.Orange);
					AddVertex(px - offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, Color.Blue);
					AddVertex(px + offset, py - offset, pz - offset, 0, -1, 0, 0, 0, texture, Color.Orange);
				}

				// Test the top:
				if (b[1, 1, 1] && !b[1, 2, 1])
				{
					// On the +Y, facing -Y
					AddVertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, Color.Blue);
					AddVertex(px + offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, Color.Orange);
					AddVertex(px + offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, Color.Green);

					AddVertex(px + offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, Color.Green);
					AddVertex(px - offset, py + offset, pz - offset, 0, +1, 0, 0, 0, texture, Color.Red);
					AddVertex(px - offset, py + offset, pz + offset, 0, +1, 0, 0, 0, texture, Color.Blue);
				}

				// Test the near side:
				if (b[1, 1, 1] && !b[1, 1, 0])
				{
					// On the -Z, facing -Z
					AddVertex(px + offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Green);
					AddVertex(px + offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Orange);
					AddVertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Blue);

					AddVertex(px - offset, py - offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Blue);
					AddVertex(px - offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Red);
					AddVertex(px + offset, py + offset, pz - offset, 0, 0, -1, 0, 0, texture, Color.Green);
				}

				// Test the far side:
				if (b[1, 1, 1] && !b[1, 1, 2])
				{
					// On the +Z, facing -Z
					AddVertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Orange);
					AddVertex(px + offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Green);
					AddVertex(px + offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Orange);

					AddVertex(px + offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Orange);
					AddVertex(px - offset, py + offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Blue);
					AddVertex(px - offset, py - offset, pz + offset, 0, 0, +1, 0, 0, texture, Color.Orange);
				}
			}
		}
	}
}