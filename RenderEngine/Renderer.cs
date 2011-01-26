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
            private List<RenderObject> mObjects;

            public Renderer(int width, int height)
            {
                mObjects = new List<RenderObject>();

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
            }

            public RenderObject CreateObject()
            {
                RenderObject obj = new RenderObject(mDevice);

                mObjects.Add(obj);

                return obj;
            }

            public void StartLoop()
            {
                MessagePump.Run(mForm, RenderFrame);
            }

            long lastTicks;
            long frames;

            public void RenderFrame()
            {
                ++frames;
                if (DateTime.Now.Ticks > lastTicks)
                {
                    long nowTicks = DateTime.Now.Ticks;
                    double seconds = (double)(nowTicks - lastTicks) / 10000000.0;
                    Console.WriteLine("FPS: {0}", (double)frames / seconds);
                    lastTicks = nowTicks + 10000000;
                    frames = 0;
                }

                mDevice.Clear(ClearFlags.Target | ClearFlags.ZBuffer, Color.Orange, 1.0f, 0);

                foreach (RenderObject obj in mObjects)
                {
                    obj.Render();
                }

                mDevice.Present();
            }
        }
    }
}
