using System;
using System.ComponentModel;
using System.Windows.Forms;
using Microsoft.DirectX.DirectSound;
using System.Threading;
using System.IO;

namespace AirGap
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();
        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            //TODO: nach AudioSource verschieben
            try
            {
                WaveFormat wf = new WaveFormat();
                wf.FormatTag = WaveFormatTag.Pcm;
                wf.SamplesPerSecond = 44100;
                wf.BitsPerSample = 16;
                wf.Channels = 1;

                wf.BlockAlign = (short)(wf.Channels * wf.BitsPerSample / 8);
                wf.AverageBytesPerSecond = wf.SamplesPerSecond * wf.BlockAlign;

                CaptureBufferDescription captureBufferDescription = new CaptureBufferDescription();
                captureBufferDescription.BufferBytes = 512 * 2 * 2;
                captureBufferDescription.Format = wf;

                CaptureDevicesCollection cdc = new CaptureDevicesCollection();

                CaptureBuffer applicationBuffer =
                    new CaptureBuffer(captureBufferDescription, new Capture(cdc[0].DriverGuid));

                MemoryStream ms = new MemoryStream();

                var notify = new Notify(applicationBuffer);
                AutoResetEvent are = new AutoResetEvent(false);

                var bpn = new BufferPositionNotify();
                bpn.Offset = applicationBuffer.Caps.BufferBytes / 2 - 1;
                bpn.EventNotifyHandle = are.SafeWaitHandle.DangerousGetHandle();

                var bpn2 = new BufferPositionNotify();
                bpn2.Offset = applicationBuffer.Caps.BufferBytes - 1;
                bpn2.EventNotifyHandle = are.SafeWaitHandle.DangerousGetHandle();

                notify.SetNotificationPositions(new[] { bpn, bpn2 });

                applicationBuffer.Start(true);

                int i = 0;
                int count = 0;
                while (true)
                {
                    are.WaitOne(Timeout.Infinite, false);

                    Array a = applicationBuffer.Read(count, typeof(byte), LockFlag.None, 512 * 2);
                    count = (count + 512 * 2) % 1024 * 2;

                    backgroundWorker1.ReportProgress(0, i++);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            var i = (int)e.UserState;

            textBox1.Text = i.ToString();
        }
    }
}
