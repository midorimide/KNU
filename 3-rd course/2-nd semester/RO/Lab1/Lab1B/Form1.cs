using System;
using System.Threading;
using System.Windows.Forms;

namespace Lab1B
{
    public partial class Form1 : Form
    {
        bool blocked = false;
        Thread thr;
        delegate void ChangeValue(int value);

        public Form1()
        {
            InitializeComponent();
        }        

        private void ChangeTrackBarValue(int value)
        {
            if (trackBar1.InvokeRequired)
            {
                try
                {
                    Invoke(new ChangeValue(ChangeTrackBarValue), value);
                }
                catch (Exception)
                {
                    Thread.CurrentThread.Abort();
                }
                Thread.Sleep(1);
            }
            else
                trackBar1.Value = Math.Min(trackBar1.Maximum, Math.Max(trackBar1.Minimum, trackBar1.Value + value));
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (blocked)
            {
                MessageBox.Show("Blocked by other thread!!!");
                return;
            }

            button4.Enabled = false;

            blocked = true;
            thr = new Thread(() => { while (true) ChangeTrackBarValue(-1); });
            thr.Priority = ThreadPriority.Lowest;
            thr.Start();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (blocked)
            {
                MessageBox.Show("Blocked by other thread!!!");
                return;
            }

            button2.Enabled = false;

            blocked = true;
            thr = new Thread(() => { while (true) ChangeTrackBarValue(1); });
            thr.Priority = ThreadPriority.Highest;
            thr.Start();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            thr.Abort();
            button4.Enabled = true;
            blocked = false;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            thr.Abort();
            button2.Enabled = true;
            blocked = false;
        }
    }
}
