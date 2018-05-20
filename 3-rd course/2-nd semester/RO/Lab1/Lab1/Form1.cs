using System;
using System.Threading;
using System.Windows.Forms;

namespace Lab1
{
    public partial class Form1 : Form
    {
        Thread thr1, thr2;
        delegate void ChangeValue(int value);

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            thr1 = new Thread(() => { while (true) ChangeTrackBarValue(-1); });
            thr1.Name = "thr1";
            thr1.Start();

            thr2 = new Thread(() => { while (true) ChangeTrackBarValue(1); });
            thr2.Name = "thr2";
            thr2.Start();

            trackBar2.Enabled = true;
            trackBar3.Enabled = true;
            button1.Enabled = false;
        }

        private void TrackBarValueChanged(object sender, EventArgs e)
        {
            var trackBar = sender as TrackBar;
            var thr = trackBar.Name == trackBar2.Name ? thr1 : thr2;
            switch (trackBar.Value)
            {
                case 0:
                    thr.Priority = ThreadPriority.Lowest;
                    break;
                case 1:
                    thr.Priority = ThreadPriority.BelowNormal;
                    break;
                case 2:
                    thr.Priority = ThreadPriority.Normal;
                    break;
                case 3:
                    thr.Priority = ThreadPriority.AboveNormal;
                    break;
                case 4:
                    thr.Priority = ThreadPriority.Highest;
                    break;
            }
            Console.WriteLine("Thread " + thr.Name + " changed priority to " + thr.Priority);
        }

        private void ChangeTrackBarValue(int value)
        {
            if (trackBar1.InvokeRequired)
            {
                try
                {
                    Invoke(new ChangeValue(ChangeTrackBarValue), value);
                } catch (Exception) {
                    Thread.CurrentThread.Abort();
                }
                Thread.Sleep(1);
            }
            else
                trackBar1.Value = Math.Min(trackBar1.Maximum, Math.Max(trackBar1.Minimum, trackBar1.Value + value));
        }
    }
}
