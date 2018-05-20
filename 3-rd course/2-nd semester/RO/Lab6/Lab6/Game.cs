using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace Lab6
{
    class Game
    {
        public delegate void UpdateScene(int[,] field);

        int _width, _height, _civCount, _thrCount;
        int[,] _field, _newField;
        Random _rand;
        UpdateScene _updateScene;

        class PartData
        {
            int _top, _bottom, _width;

            public ManualResetEvent doneEvent;

            public int Top { get => _top; }
            public int Bottom { get => _bottom; }
            public int Width { get => _width; }

            public PartData(int top, int bottom, int width, ManualResetEvent mre)
            {
                _top = top;
                _bottom = bottom;
                _width = width;
                doneEvent = mre;
            }
        }

        public Game(int width, int heigth, int civCount, UpdateScene updateScene, int thrCount = 5)
        {
            _width = width;
            _height = heigth;
            _civCount = civCount + 1;
            _thrCount = thrCount;
            _updateScene = updateScene;
            _rand = new Random(DateTime.Now.Millisecond);
        }

        public void Run() {
            GenerateField();
            _newField = new int[_width, _height];

            int heightResolution = _height / _thrCount;

            var doneEvents = new ManualResetEvent[_thrCount];
            while (true)
            {
                _newField.Initialize();
                for (int i = 0; i < _thrCount; ++i)
                {
                    doneEvents[i] = new ManualResetEvent(false);
                    ThreadPool.QueueUserWorkItem(UpdatePart, new PartData(i * heightResolution, (i + 1) * heightResolution, _width, doneEvents[i]));
                }
                foreach (var doneEvent in doneEvents)
                    doneEvent.WaitOne();

                _field = (int[,])_newField.Clone();
                _updateScene(_field);
                Thread.Sleep(100);
            }
        }
        
        void UpdatePart(Object obj)
        {
            var partData = (PartData)obj;
            IList<int> points;
            IDictionary<int, int> neighbours;
            for (int i = partData.Top; i < partData.Bottom; ++i)
            {
                for (int j = 0; j < partData.Width; ++j)
                {
                    points = GetNearPoints(j, i);
                    int curPoint = _field[j, i];
                    neighbours = new Dictionary<int, int>();
                    points = GetNearPoints(j, i);
                    foreach (var point in points)
                        if (point != 0)
                            if (neighbours.ContainsKey(point))
                                neighbours[point]++;
                            else
                                neighbours.Add(point, 1);
                    if (curPoint != 0)
                        _newField[j, i] = neighbours.ContainsKey(curPoint) && (neighbours[curPoint] == 2 || neighbours[curPoint] == 3) ? curPoint : 0;
                    else
                    {
                        var candidates = neighbours.Where((KeyValuePair<int, int> p) => { return p.Value == 3 ? true : false; }).ToArray();
                        if (candidates.Length == 0)
                            _newField[j, i] = 0;
                        else
                            _newField[j, i] = candidates[_rand.Next(candidates.Length)].Key;
                    }
                }
            }
            partData.doneEvent.Set();
        }

        IList<int> GetNearPoints(int x, int y)
        {
            IList<int> points = new List<int>();
            if (x > 0)
            {
                if (y > 0)
                    points.Add(_field[x - 1, y - 1]);
                points.Add(_field[x - 1, y]);
                if (y < _height - 1)
                    points.Add(_field[x - 1, y + 1]);
            }
            if (y > 0)
                points.Add(_field[x, y - 1]);
            if (y < _height - 1)
                points.Add(_field[x, y + 1]);
            if (x < _width - 1)
            {
                if (y > 0)
                    points.Add(_field[x + 1, y - 1]);
                points.Add(_field[x + 1, y]);
                if (y < _height - 1)
                    points.Add(_field[x + 1, y + 1]);
            }
            return points;
        }

        void GenerateField()
        {
            _field = new int[_width, _height];
            _field.Initialize();

            int baseCount = _height * _width / 2;

            int x = 0, y = 0;

            for (int i = 0; i < baseCount; ++i)
            {
                while (_field[x, y] != 0)
                {
                    x = _rand.Next(_width);
                    y = _rand.Next(_height);
                }
                _field[x, y] = _rand.Next(1, _civCount);
            }
        }
    }
}
