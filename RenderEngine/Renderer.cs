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
        public class Renderer
        {
            private RenderForm mForm;

            private Direct3D mObject;
            private Device mDevice;

			private Matrix mProj;

            private List<RenderObject> mObjects;
			private Camera mCamera;

			private SlimDX.DirectInput.DirectInput mInput;
			private SlimDX.DirectInput.Mouse mMouse;
			private SlimDX.DirectInput.Keyboard mKeyboard;
			Timer mTimer;

            public Renderer(int width, int height)
            {
                mObjects = new List<RenderObject>();

				mCamera = new Camera(new Vector3(-3.0f, 1.0f, 1.5f), 0.0f, 0.0f);

                mForm = new RenderForm("BoxGame Render Engine");
                mForm.Height = height;
                mForm.Width = width;

                mObject = new Direct3D();

                bool canMakeDevice = mObject.CheckDeviceType(
                    mObject.Adapters.DefaultAdapter.Adapter, DeviceType.Hardware,
                    Format.X8R8G8B8, Format.X8R8G8B8, true);

                if (!canMakeDevice)
                {
                    throw new Exception("Cannot create DirectX device.");
                }

                PresentParameters pp = new PresentParameters();
                pp.BackBufferCount = 1;
                pp.BackBufferHeight = 480;
                pp.BackBufferWidth = 640;
                pp.EnableAutoDepthStencil = true;
                pp.PresentFlags = PresentFlags.None;
                pp.SwapEffect = SwapEffect.Flip;
                pp.Windowed = true;
                pp.Multisample = MultisampleType.None;

                mDevice = new Device(mObject, mObject.Adapters.DefaultAdapter.Adapter, DeviceType.Hardware,
                    mForm.Handle, CreateFlags.HardwareVertexProcessing, pp);

                mForm.Show();

				float aspect = width / (float)height;
				mProj = Matrix.PerspectiveFovLH((float)Math.PI / 4, aspect, 1.0f, 1000.0f);
				mDevice.SetTransform(TransformState.Projection, mProj);

				mTimer = new Timer();
				mTimer.Interval = 1000 / 50;
				mTimer.Tick += new EventHandler(mTimer_Tick);

				mInput = new SlimDX.DirectInput.DirectInput();

				SlimDX.DirectInput.CooperativeLevel coop = SlimDX.DirectInput.CooperativeLevel.Foreground | SlimDX.DirectInput.CooperativeLevel.Nonexclusive;

				mMouse = new SlimDX.DirectInput.Mouse(mInput);
				mMouse.SetCooperativeLevel(mForm.Handle, coop);

				mKeyboard = new SlimDX.DirectInput.Keyboard(mInput);
				mKeyboard.SetCooperativeLevel(mForm.Handle, coop);

				mTimer.Start();
            }

			~Renderer()
			{
				mKeyboard.Unacquire();
				mKeyboard.Dispose();

				mMouse.Unacquire();
				mMouse.Dispose();
			}

            public RenderObject CreateObject()
            {
                RenderObject obj = new RenderObject(mDevice);

                mObjects.Add(obj);

                return obj;
            }

            public void StartLoop()
            {
				matrix = Matrix.Identity;

                MessagePump.Run(mForm, RenderFrame);
            }

            long lastTicks;
			long nowTicks;
            long frames;
			Matrix matrix;

            public void RenderFrame()
            {
                ++frames;

				nowTicks = DateTime.Now.Ticks;

                if (nowTicks > lastTicks)
                {
                    double seconds = (double)(nowTicks - lastTicks) / 10000000.0;
                    Console.WriteLine("FPS: {0}", (double)frames / seconds);
                    lastTicks = nowTicks + 10000000;
                    frames = 0;
                }

                mDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, Color.Orange, 1.0f, 0);

				mDevice.SetTransform(TransformState.View, mCamera.GetViewMatrix());

                foreach (RenderObject obj in mObjects)
                {
                    obj.Render();
                }

                mDevice.Present();
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

				SlimDX.DirectInput.MouseState state = mMouse.GetCurrentState();
				if (Result.Last.IsFailure) return;

				mCamera.Rotate(state.X / 10.0f, state.Y / 10.0f);
			}

			void ProcessKeyboardInput()
			{
				if (mKeyboard.Acquire().IsFailure) return;
				if (mKeyboard.Poll().IsFailure) return;

				SlimDX.DirectInput.KeyboardState state = mKeyboard.GetCurrentState();
				if (Result.Last.IsFailure) return;

				if (state.IsPressed(SlimDX.DirectInput.Key.Escape))
				{
					mForm.Close();
				}

				Vector3 translation = Vector3.Zero;
				if (state.IsPressed(SlimDX.DirectInput.Key.W))
				{
					translation.Z += 1.0f;
				}
				if (state.IsPressed(SlimDX.DirectInput.Key.S))
				{
					translation.Z -= 1.0f;
				}
				if (state.IsPressed(SlimDX.DirectInput.Key.A))
				{
					translation.X -= 1.0f;
				}
				if (state.IsPressed(SlimDX.DirectInput.Key.D))
				{
					translation.X += 1.0f;
				}

				// Handle gravity here

				mCamera.Translate(translation);
			}
        }
    }
}
