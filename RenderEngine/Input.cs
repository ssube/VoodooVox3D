using System;
using System.Collections.Generic;
using System.Windows.Forms;
using SlimDX;
using SlimDX.DirectInput;

namespace BoxEngine
{
	namespace RenderEngine
	{
		public class InputManager
		{
			Camera mCamera;
			DirectInput mInput;
			Mouse mMouse;
			Keyboard mKeyboard;
			Timer mTimer;

			public InputManager(IntPtr handle, Camera camera)
			{
				mCamera = camera;

				mTimer = new Timer();
				mTimer.Interval = 1000 / 50;
				mTimer.Tick += new EventHandler(mTimer_Tick);

				mInput = new DirectInput();

				CooperativeLevel coop = CooperativeLevel.Foreground | CooperativeLevel.Nonexclusive;

				mMouse = new Mouse(mInput);
				mMouse.SetCooperativeLevel(handle, coop);

				mKeyboard = new Keyboard(mInput);
				mKeyboard.SetCooperativeLevel(handle, coop);

				mTimer.Start();
			}

			void mTimer_Tick(object sender, EventArgs e)
			{
				ProcessMouseInput();
				ProcessKeyboardInput();
			}

			void ProcessMouseInput()
			{
				if (mMouse.Acquire().IsFailure) return;
				if (mMouse.Poll().IsFailure) return;

				MouseState state = mMouse.GetCurrentState();
				if (Result.Last.IsFailure) return;

				mCamera.Rotate(Vector3.UnitY, (state.X / 100.0f) * (float)(Math.PI / 4));
			}

			void ProcessKeyboardInput()
			{
				if (mKeyboard.Acquire().IsFailure) return;
				if (mKeyboard.Poll().IsFailure) return;

				KeyboardState state = mKeyboard.GetCurrentState();
				if (Result.Last.IsFailure) return;

				Vector3 translation = Vector3.Zero;
				if (state.IsPressed(Key.W))
				{
					translation.Z += 1.0f;
				}
				if (state.IsPressed(Key.S))
				{
					translation.Z -= 1.0f;
				}
				if (state.IsPressed(Key.A))
				{
					translation.X -= 1.0f;
				}
				if (state.IsPressed(Key.D))
				{
					translation.X += 1.0f;
				}

				mCamera.Translate(translation);
			}
		}
	}
} 
