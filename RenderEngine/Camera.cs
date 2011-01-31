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
		public class MathHelper
		{
			public static float ToRadian(float input)
			{
				return (input / 360.0f) * (2.0f * (float)Math.PI);
			}
		}

		public class Camera
		{
			private Vector3 mPosition;
			private float mYaw, mPitch;

			public Camera(Vector3 position, float yaw, float pitch)
			{
				mPosition = position;
				mYaw = yaw;
				mPitch = pitch;
			}

			public void Translate(Vector3 translation)
			{
				Matrix rotationMatrix = Matrix.RotationYawPitchRoll(MathHelper.ToRadian(mYaw), MathHelper.ToRadian(mPitch), MathHelper.ToRadian(0.0f));

				mPosition += Vector3.TransformCoordinate(translation, rotationMatrix);
			}

			public void Rotate(float yaw, float pitch)
			{
				mYaw = ( mYaw + yaw ) % 360;
				mPitch = Math.Max(Math.Min(mPitch + pitch, 75.0f), -75.0f);
			}

			public Matrix GetViewMatrix()
			{
				Matrix rotationMatrix = Matrix.RotationYawPitchRoll(MathHelper.ToRadian(mYaw), MathHelper.ToRadian(mPitch), MathHelper.ToRadian(0.0f));

				Vector3 forward = Vector3.TransformCoordinate(Vector3.UnitZ, rotationMatrix);
				Vector3 target = mPosition + forward;

				return Matrix.LookAtLH(mPosition, target, Vector3.UnitY);
			}
		}
	}
}
