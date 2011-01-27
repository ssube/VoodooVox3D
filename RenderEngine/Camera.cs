using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using SlimDX;
using SlimDX.Direct3D9;
using SlimDX.Windows;

namespace BoxEngine
{
	namespace RenderEngine
	{
		public class Camera
		{
			Matrix mViewMatrix;

			public Matrix ViewMatrix
			{
				get
				{
					return mViewMatrix;
				}
				set
				{
					mViewMatrix = value;
				}
			}			

			public Camera(Vector3 position)
			{
				mViewMatrix = Matrix.LookAtLH(position, position + new Vector3(1, 0, 0), new Vector3(0, 1, 0));
			}

			public void Translate(Vector3 translation)
			{
				Matrix translationMatrix = Matrix.Translation(translation);
				mViewMatrix = Matrix.Multiply(mViewMatrix, translationMatrix);
			}

			public void Rotate(Quaternion rotation)
			{
				Matrix rotationMatrix = Matrix.RotationQuaternion(rotation);
				mViewMatrix = Matrix.Multiply(mViewMatrix, rotationMatrix);
			}
		}
	}
}
