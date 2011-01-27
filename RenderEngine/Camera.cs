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
			/*Matrix mViewMatrix;

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
			}*/

			Vector3 mPosition;
			Vector3 mUp, mRight, mForward;

			float yaw, pitch, row;

			public Camera(Vector3 position)
			{
				mPosition = position;
				mUp = Vector3.UnitY;
				mRight = Vector3.UnitX;
				mForward = Vector3.UnitZ;
			}

			public void Translate(Vector3 translation)
			{
				//mPosition += Vector3.Multiply(translation, new Vector3((float)Math.Sin(yaw), 1.0f, (float)Math.Cos(yaw)));
			}

			public void Translate(float x, float y, float z)
			{
				Matrix translationMatrix = Matrix.Translation(x, y, z);
				mViewMatrix += translationMatrix;
			}

			public void Rotate(Quaternion rotation)
			{
				Matrix rotationMatrix = Matrix.RotationQuaternion(rotation);
				mViewMatrix += rotationMatrix;
			}

			public void Rotate(float x, float y, float z, float w)
			{
				Rotate(new Quaternion(x, y, z, w));
			}

			public void Rotate(Vector3 axis, float amount)
			{
				Matrix rotationMatrix = Matrix.RotationAxis(axis, amount);
				mViewMatrix += rotationMatrix;
			}
		}
	}
}
