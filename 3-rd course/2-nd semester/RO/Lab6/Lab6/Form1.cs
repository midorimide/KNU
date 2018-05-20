using System;
using System.Drawing;
using System.Threading;
using System.Windows.Forms;

namespace Lab6
{
    public partial class Form1 : Form
    {
        Thread _gameThread;

        public Form1()
        {
            InitializeComponent();
        }

        private void UpdateScene(int[,] field)
        {   
            var bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            var g = Graphics.FromImage(bmp);
            g.Clear(Color.White);

            int n = field.GetLength(0);
            int m = field.GetLength(1);

            var widthResolution = (float)pictureBox1.Width / n;
            var heightResolution = (float) pictureBox1.Height / m;

            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < m; ++j)
                {
                    var rect = new RectangleF(i * widthResolution + 1, j * heightResolution + 1, widthResolution, heightResolution);
                    var c = field[i, j];
                    g.DrawRectangle(Pens.Black, i * widthResolution, j * heightResolution, widthResolution, heightResolution);
                    if (c != 0)
                        g.FillRectangle(new SolidBrush(Color.FromArgb((int)(255 * Math.Abs(Math.Sin(c))), (int)(255 * Math.Abs(Math.Cos(c))), (int)(255 * Math.Abs(Math.Sin(c) * Math.Cos(c))))), rect);
                }
            }

            pictureBox1.Image = bmp;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            pictureBox1.Size = Size;
            var game = new Game(100, 100, 4, UpdateScene);
            _gameThread = new Thread(new ThreadStart(game.Run));
            _gameThread.Start();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            _gameThread.Abort();
        }
    }
}
