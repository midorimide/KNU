using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Lab1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            switch (e.Button)
            {
                case MouseButtons.Left:
                    LeftButtonClicked(e.Location);
                    break;
                case MouseButtons.Right:
                    RightButtonClicked(e.Location);
                    break;
                default:
                    break;
            }
        }

        private bool PointIn(Point point)
        {
            if (points.Count < 3)
                return false;

            if (Rotate(points[0], points[1], point) < 0 || Rotate(points[0], points[points.Count - 1], point) > 0)
                return false;

            int l = 1, r = points.Count - 1;
            while (r - l > 1)
            {
                int m = (l + r) / 2;
                if (Rotate(points[0], points[m], point) < 0)
                    r = m;
                else
                    l = m;
            }

            return Rotate(points[l], points[r], point) > 0;
        }

        private int Rotate(Point A, Point B, Point C)
        {
            return (B.X - A.X) * (C.Y - B.Y) - (B.Y - A.Y) * (C.X - B.X);
        }

        private float Distance(Point A, Point B, Point C)
        {
            return (float)(Math.Abs((B.Y - A.Y) * C.X - (B.X - A.X) * C.Y + B.X * A.Y - B.Y * A.X) / Math.Sqrt(Math.Pow(B.Y - A.Y, 2) + Math.Pow(B.X - A.X, 2)));
        }

        private Point FindMaxPoint(Point A, Point B, List<Point> points)
        {
            Point MaxPoint = new Point();
            float MaxDistance = 0;

            foreach (Point point in points)
            {
                float Temp = Distance(A, B, point);
                if (Temp > MaxDistance)
                {
                    MaxDistance = Temp;
                    MaxPoint = point;
                }
            }

            return MaxPoint;
        }

        private List<Point> QuickHull(Point A, Point B, List<Point> points)
        {
            Point C = FindMaxPoint(A, B, points);
            points.Remove(C);

            List<Point> List1 = new List<Point>();
            List<Point> List2 = new List<Point>();
            List<Point> Result = new List<Point>();

            foreach (Point point in points)
                if (Rotate(A, C, point) < 0)
                    List1.Add(point);
                else if (Rotate(C, B, point) < 0)
                    List2.Add(point);

            if (List1.Count != 0)
                Result = QuickHull(A, C, List1);
            Result.Add(C);
            if (List2.Count != 0)
                Result.AddRange(QuickHull(C, B, List2));

            return Result;
        }

        private void UpdatePoints()
        {
            if (points.Count < 4)
                return;

            Point A = points[0], B = points[0];
            List<Point> List1 = new List<Point>();
            List<Point> List2 = new List<Point>();
            List<Point> Result = new List<Point>();

            foreach (Point point in points)
                if (point.X < A.X || (point.X == A.X && point.Y > A.Y))
                    A = point;
                else if (point.X > B.X)
                    B = point;

            points.Remove(A);
            points.Remove(B);

            foreach (Point point in points)
                if (Rotate(A, B, point) < 0)
                    List1.Add(point);
                else
                    List2.Add(point);

            Result.Add(A);
            if (List1.Count > 0)
                Result.AddRange(QuickHull(A, B, List1));
            Result.Add(B);
            if (List2.Count > 0)
                Result.AddRange(QuickHull(B, A, List2));

            points = Result;
        }

        private void DrawScene()
        {
            if (points.Count < 2)
                return;

            Bitmap bmp = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Graphics g = Graphics.FromImage(bmp);
            g.Clear(Color.White);
            g.DrawLines(Pens.Black, points.ToArray());
            g.DrawLine(Pens.Black, points[points.Count - 1], points[0]);
            pictureBox1.Image = bmp;
        }

        private void LeftButtonClicked(Point point)
        {
            points.Add(point);
            UpdatePoints();
            DrawScene();
        }

        private void RightButtonClicked(Point point)
        {
            MessageBox.Show(PointIn(point) ? "Всередині" : "Ззовні");
        }

        private void Form1_ResizeEnd(object sender, EventArgs e)
        {
            pictureBox1.Size = Size;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            pictureBox1.Size = Size;
        }

        private List<Point> points = new List<Point>();
    }
}
