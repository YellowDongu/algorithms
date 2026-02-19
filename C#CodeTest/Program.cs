using System;
using System.Numerics;

namespace algorithms
{
    internal static class IO
    {
        static IO()
        {
            reader = new StreamReader(Console.OpenStandardInput());
            writer = new StreamWriter(Console.OpenStandardOutput()) { AutoFlush = false };

            Console.SetIn(reader);
            Console.SetOut(writer);
        }

        public static T? InputSingle<T>() { return (T?)Convert.ChangeType(reader.ReadLine(), typeof(T)); }
        public static List<T> Input<T>(bool readLine = false)
        {
            List<T> output = new List<T>();
            string? line;
            if (readLine)
            {
                while ((line = reader.ReadLine()) != null)
                    output.AddRange(line.Split('\n').Where(x => string.IsNullOrEmpty(x) == false).Select(word => (T)Convert.ChangeType(word, typeof(T))));
                //output.Add((T)Convert.ChangeType(line, typeof(T)));
            }
            else
                while ((line = reader.ReadLine()) != null)
                    output.AddRange(line.Split(' ').Where(x => string.IsNullOrEmpty(x) == false).Select(word => (T)Convert.ChangeType(word, typeof(T))));

            return output;
        }

        public static List<T> Input<T>(int length, bool readLine = false)
        {
            List<T> output = new List<T>(length);
            string? line;
            if (readLine)
            {
                while (length > 0 && (line = reader.ReadLine()) != null)
                {
                    length--;
                    output.AddRange(line.Split('\n').Where(x => string.IsNullOrEmpty(x) == false).Select(word => (T)Convert.ChangeType(word, typeof(T))));
                }
                //output.Add((T)Convert.ChangeType(line, typeof(T)));
            }
            else
                while (length > 0 && (line = reader.ReadLine()) != null)
                {
                    length--;
                    output.AddRange(line.Split(' ').Where(x => string.IsNullOrEmpty(x) == false).Select(word => (T)Convert.ChangeType(word, typeof(T))));
                }

            return output;
        }

        public static void Output<T>(T output) { Console.Write(output?.ToString()); Console.Out.Flush(); }
        public static void Output<T>(List<T> output, bool oneLine = false)
        {
            string endl = "\n";
            if (oneLine) endl = " ";
            foreach (var item in output)
            {
                Console.Write(item?.ToString());
                Console.Write(endl);
            }

            Console.Out.Flush();
        }
        public static void Output<T>(List<List<T>> output)
        {
            foreach (var itemList in output)
            {
                foreach (var item in itemList)
                {
                    Console.Write(item?.ToString());
                    Console.Write(" ");
                }
                Console.Write("\n");
            }

            Console.Out.Flush();
        }

        public static string ShowDecimalValue(double value, int decimalPlace = 14) { return value.ToString("F" + decimalPlace.ToString()); }
        public static string ShowDecimalValue(float value, int decimalPlace = 7) { return value.ToString("F" + decimalPlace.ToString()); }
        //public static void CustomOutput<T>(List<T> output) { Console.SetOut(writer); logic; Console.Out.Flush(); }

        private static readonly StreamReader reader;
        private static readonly StreamWriter writer;

    }

    public class Program
    {
        public static int Main()
        {
            IO.Output<int>(Solution(IO.Input<int>()));
            return 0;
        }

        static int Solution(List<int> list)
        {
            int length = list[0], lineLength = list[1], result = 0, max = 0;
            List<List<(int, int)>> graph = new List<List<(int, int)>>(length + 1);
            bool[] visiTable = new bool[length + 1];
            PriorityQueue<int, int> priorityQueue = new PriorityQueue<int, int>(length);

            for (int i = 0; i <= length; i++)
                graph.Add(new List<(int, int)>());

            for (int i = 0; i < lineLength; i++)
            {
                int index = i * 3 + 2;
                graph[list[index]].Add((list[index + 1], list[index + 2]));
                graph[list[index + 1]].Add((list[index], list[index + 2]));
            }

            priorityQueue.Enqueue(1, 0);

            while (priorityQueue.Count > 0)
            {
                priorityQueue.TryDequeue(out int index, out int weight);

                if (visiTable[index])
                    continue;

                visiTable[index] = true;
                result += weight;
                max = Math.Max(max, weight);

                foreach (var item in graph[index])
                {
                    if (visiTable[item.Item1])
                        continue;

                    priorityQueue.Enqueue(item.Item1, item.Item2);
                }
            }

            return result - max;
        }
    }
}

/*
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Runtime.CompilerServices;
using System.Runtime.ExceptionServices;
using System.Text;
🥵
*/
namespace algorithms
{
    public class SolvedSolution
    {
        static int Solution(List<int> list)
        {
            int length = list[0], result = 0;
            return result;
        }

        static int Solution(List<string> list)
        {
            int length = int.Parse(list[0]), result = 0;
            return result;
        }

        //static List<int> Solution(List<int> list) { return new List<int>(); }
        //static List<string> Solution(List<string> list) { return new List<string>(); }
        /*
        static List<int> Solution(int caseLength)
        {
            List<int> result = new List<int>(caseLength);
            while (caseLength-- > 0)
            {
                List<int> boardInfo = IO.Input<int>(1);
                int height = boardInfo[0], width = boardInfo[1], stack = 0;
                List<string> list = IO.Input<string>(height);
                string keyInfo = IO.InputSingle<string>() ?? "";

                List<List<char>> board = new List<List<char>>(height);
                Dictionary<char, List<(int, int)>> door = new Dictionary<char, List<(int, int)>>(26);
                Queue<(int, int)> queue = new Queue<(int, int)>(height * width);
                HashSet<char> keyList = new HashSet<char>(26);
                bool[,] visiTable = new bool[width, height];

                var UnLock = (char key) =>
                {
                    if (keyList.Contains(key))
                        return;
                    if (door.TryGetValue(key, out List<(int, int)>? list) && list != null)
                    {
                        foreach (var item in list)
                            queue.Enqueue(item);
                        list.Clear();
                    }
                    keyList.Add(key);
                };

                var Visit = (int x, int y) =>
                {
                    if (x < 0 || x >= width || y < 0 || y >= height)
                        return;

                    if (board[y][x] == '*' || visiTable[x, y])
                        return;

                    visiTable[x, y] = true;

                    if (char.IsUpper(board[y][x]) && !keyList.Contains(board[y][x]))
                    {
                        if (door.TryGetValue(board[y][x], out List<(int, int)>? list))
                            list.Add((x, y));
                        else
                            door.Add(board[y][x], new List<(int, int)>() { (x, y) });
                        return;
                    }

                    queue.Enqueue((x, y));
                    if (char.IsLower(board[y][x]))
                        UnLock(char.ToUpper(board[y][x]));
                    else if (board[y][x] == '$')
                        stack++;
                };

                foreach (var item in keyInfo)
                    keyList.Add(char.ToUpper(item));

                for (int i = 0; i < height; i++)
                    board.Add(list[i].ToList());

                for (int i = 0; i < height; i++)
                {
                    Visit(0, i);
                    Visit(width - 1, i);
                }
                for (int j = 0; j < width; j++)
                {
                    Visit(j, 0);
                    Visit(j, height - 1);
                }

                while (queue.Count > 0)
                {
                    (int x, int y) = queue.Dequeue();

                    Visit(x + 1, y);
                    Visit(x - 1, y);
                    Visit(x, y + 1);
                    Visit(x, y - 1);
                }

                result.Add(stack);
            }

            return result;
        }

        static List<int> Solution(List<int> list)
        {
            int length = list[0], root = list[1], queryLength = list[2];
            List<List<int>> tree = new List<List<int>>(length + 1);
            List<int> result = new List<int>(queryLength);
            int[] table = new int[length + 1];

            var DFS = null as Func<int, int>;
            DFS = (int index) =>
            {
                int sum = 1;
                foreach (var item in tree[index])
                {
                    if (table[item] != 0)
                        continue;

                    table[item] = -1;
                    sum += DFS(item);
                }

                table[index] = sum;
                return sum;
            };
            for (int i = 0; i <= length; i++)
                tree.Add(new List<int>());

            length--;
            for (int i = 0; i < length; i++)
            {
                int index = i * 2 + 3;
                tree[list[index]].Add(list[index + 1]);
                tree[list[index + 1]].Add(list[index]);
            }

            table[root] = -1;
            DFS(root);

            int baseIndex = length * 2 + 3;
            for (int i = 0; i < queryLength; i++)
                result.Add(table[list[baseIndex + i]]);

            return result;
        }

        static List<int> Solution(List<int> list)
        {
            int length = list[0];
            Queue<int> queue = new Queue<int>(length);
            List<List<int>> tree = new List<List<int>>(length + 1);
            List<int> result = new List<int>();

            for (int i = 0; i <= length; i++)
            {
                tree.Add(new List<int>());
                result.Add(0);
            }

            length--;
            for (int i = 0; i < length; i++)
            {
                int from = list[i * 2 + 1];
                int to = list[i * 2 + 2];

                tree[from].Add(to);
                tree[to].Add(from);
            }

            result[1] = 1;
            queue.Enqueue(1);

            while (queue.Count > 0)
            {
                int index = queue.Dequeue();

                foreach (var item in tree[index])
                {
                    if (result[item] != 0)
                        continue;

                    result[item] = index;
                    queue.Enqueue(item);
                }
            }

            result.RemoveRange(0, 2);
            return result;
        }

        static List<List<int>> Solution(List<int> list)
        {
            int length = list[0], targetLength = list[1];
            List<int> sorted = list.GetRange(2, length);
            List<List<int>> result = new List<List<int>>();
            Queue<(int, int, List<int>)> queue = new Queue<(int, int, List<int>)>(targetLength * targetLength);

            sorted.Sort();
            queue.Enqueue((0, 0, new List<int>()));

            while (queue.Count > 0)
            {
                (int depth, int bit, List<int> stack) = queue.Dequeue();

                if (depth >= targetLength)
                {
                    result.Add(stack);
                    continue;
                }

                depth++;
                int previous = int.MaxValue;

                for (int i = 0; i < length; i++)
                {
                    if ((bit & (1 << i)) != 0)
                        continue;

                    if (sorted[i] == previous)
                        continue;

                    previous = sorted[i];
                    List<int> newList = new List<int>(targetLength);
                    newList.AddRange(stack);
                    newList.Add(sorted[i]);
                    queue.Enqueue((depth, bit | (1 << i), newList));
                }
            }

            return result;
        }
        static List<List<int>> Solution(List<int> list)
        {
            int length = list[0], targetLength = list[1];
            List<int> sorted = list.GetRange(2, length);
            List<List<int>> result = new List<List<int>>();

            var Recursive = null as Action<int, int, List<int>>;

            Recursive = (int depth, int bit, List<int> list) =>
            {
                if (depth >= targetLength)
                {
                    result.Add(list);
                    return;
                }

                depth++;
                int previous = int.MaxValue;

                for (int i = 0; i < length; i++)
                {
                    if ((bit & (1 << i)) != 0)
                        continue;

                    if (sorted[i] == previous)
                        continue;

                    previous = sorted[i];
                    List<int> newList = new List<int>(targetLength);
                    newList.AddRange(list);
                    newList.Add(sorted[i]);
                    Recursive(depth, bit | (1 << i), newList);
                }
            };

            sorted.Sort();
            Recursive(0, 0, new List<int>());

            return result;
        }

        static List<string> Solution(int length)
        {
            int triangleHeight = length / 3, charLength = triangleHeight * 5 + triangleHeight;
            List<string> result = new List<string>(length);
            List<List<char>> board = new List<List<char>>(length);

            var Triangle = (int x, int y) =>
            {
                board[y + 2][x + 2] = '*';
                board[y + 1][x + 1] = '*';
                board[y + 1][x + 3] = '*';
                for (int i = 0; i < 5; i++)
                    board[y][x + i] = '*';
            };

            var Recursive = null as Action<int, int, int>;
            Recursive = (int x, int y, int triangleFloor) =>
            {
                if (triangleFloor <= 1)
                {
                    Triangle(x, y);
                    return;
                }
                triangleFloor /= 2;
                int halfX = triangleFloor * 5 + triangleFloor;
                int halfY = triangleFloor * 3;

                Recursive(x, y, triangleFloor);
                Recursive(x + halfX, y, triangleFloor);
                Recursive(x + halfX / 2, halfY + y, triangleFloor);

            };

            for (int i = 0; i < length; i++)
            {
                board.Add(new List<char>(charLength));
                for (int j = 0; j < charLength; j++)
                    board[i].Add(' ');
            }

            Recursive(0, 0, triangleHeight);

            for (int i = 0; i < length; i++)
                result.Add(new string(board[length - (i + 1)].ToArray()));

            return result;
        }

        static List<string> Solution(int length)
        {
            int triangleHeight = length / 3, charLength = triangleHeight * 5 + triangleHeight;
            List<string> result = new List<string>(length);
            List<List<char>> board = new List<List<char>>(length);

            var Triangle = (int x, int y) =>
            {
                board[y + 2][x + 2] = '*';
                board[y + 1][x + 1] = '*';
                board[y + 1][x + 3] = '*';
                for (int i = 0; i < 5; i++)
                    board[y][x + i] = '*';
            };

            var Recursive = null as Action<int, int, int>;
            Recursive = (int x, int y, int triangleFloor) =>
            {
                if (triangleFloor <= 1)
                {
                    Triangle(x, y);
                    return;
                }
                triangleFloor /= 2;
                int halfX = triangleFloor * 5 + triangleFloor;
                int halfY = triangleFloor * 3;

                Recursive(x, y, triangleFloor);
                Recursive(x + halfX, y, triangleFloor);
                Recursive(x + halfX / 2, halfY + y, triangleFloor);
            };

            for (int i = 0; i < length; i++)
            {
                board.Add(new List<char>(charLength));
                for (int j = 0; j < charLength; j++)
                    board[i].Add(' ');
            }

            Recursive(0, 0, triangleHeight);

            for (int i = 0; i < length; i++)
                result.Add(new string(board[length - (i + 1)].ToArray()));

            return result;
        }

        static List<int> Solution(List<int> list)
        {
            int length = list[0], caseLength = list[1];
            int[] sum = new int[length + 1];
            List<int> result = new List<int>(caseLength);

            for (int i = 0; i < length; i++)
                sum[i + 1] = sum[i] + list[2 + i];

            for (int i = 0; i < caseLength; i++)
            {
                int index = i * 2 + 2 + length;
                result.Add(sum[list[index + 1]] - sum[list[index] - 1]);
            }

            return result;
        }

        static List<int> Solution(List<int> list)
        {
            int length = list[0], caseLength = list[1];
            int targetLeft = 0, targetRight = 0;
            int[] sum = new int[length * 4 + 1];
            var CreateSegmentTree = null as Func<int, int, int, int>;
            var Find = null as Func<int, int, int, int>;
            List<int> result = new List<int>(caseLength);

            CreateSegmentTree = (int left, int right, int index) =>
            {
                if(left == right)
                {
                    sum[index] = list[left + 1];
                    return sum[index];
                }

                int middle = (left + right) / 2;
                sum[index] = CreateSegmentTree(left, middle, index * 2) + CreateSegmentTree(middle + 1, right, index * 2 + 1);
                return sum[index];
            };

            Find = (int left, int right, int index) =>
            {
                if (right < targetLeft || left > targetRight)
                    return 0;
                if (targetLeft <= left && targetRight >= right)
                    return sum[index];

                int middle = (left + right) / 2;
                return Find(left, middle, index * 2) + Find(middle + 1, right, index * 2 + 1);
            };

            CreateSegmentTree(1, length, 1);

            for (int i = 0; i < caseLength; i++)
            {
                int index = i * 2 + 2 + length;
                targetLeft = list[index];
                targetRight = list[index + 1];
                if(targetLeft > targetRight)
                {
                    int temp = targetLeft;
                    targetLeft = targetRight;
                    targetRight = temp;
                }

                result.Add(Find(1, length, 1));
            }

            return result;
        }
        static List<int> Solution(List<int> list)
        {
            int size = list[0];
            List<int> result = new List<int>(2) { 0, 0 };
            var Recursive = null as Func<int, int, int, int, int>;

            var CheckArea = (int minX, int minY, int maxX, int maxY) =>
            {
                int isBlue = list[minX + minY * size + 1];
                for (int y = minY; y <= maxY; y++)
                {
                    for (int x = minX; x <= maxX; x++)
                    {
                        if (isBlue != list[x + y * size + 1])
                            return Recursive(minX, minY, maxX, maxY);
                    }
                }

                if (isBlue == 0)
                    result[0]++;

                return isBlue;
            };

            Recursive = (int minX, int minY, int maxX, int maxY) =>
            {
                int blue = 0;
                int middleX = (minX + maxX) / 2;
                int middleY = (minY + maxY) / 2;

                blue += CheckArea(minX, minY, middleX, middleY);
                blue += CheckArea(minX, middleY + 1, middleX, maxY);
                blue += CheckArea(middleX + 1, minY, maxX, middleY);
                blue += CheckArea(middleX + 1, middleY + 1, maxX, maxY);

                return blue;
            };

            result[1] = CheckArea(0, 0, size - 1, size - 1);
            return result;
        }

        static int Solution(List<int> list)
        {
            int nodeLength = list[0], lineLength = list[1], step = 0;
            Queue<int> queue = new Queue<int>(nodeLength);
            List<int>[] graph = new List<int>[nodeLength + 1];
            bool[] visitable = new bool[nodeLength + 1];

            for (int i = 0; i <= nodeLength; i++)
                graph[i] = new List<int>();

            for (int i = 0; i < lineLength; i++)
            {
                int index = (i + 1) * 2;
                graph[list[index]].Add(list[index + 1]);
                graph[list[index + 1]].Add(list[index]);
            }

            for (int i = 1; i <= nodeLength; i++)
            {
                if (visitable[i])
                    continue;

                queue.Enqueue(i);
                visitable[i] = true;

                while (queue.Count > 0)
                {
                    int index = queue.Dequeue();
                    foreach (int nextIndex in graph[index])
                    {
                        if (visitable[nextIndex])
                            continue;
                        visitable[nextIndex] = true;
                        queue.Enqueue(nextIndex);
                    }
                }

                step++;
            }

            return step;
        }

        static List<string> Solution(List<int> list) // for performance test between SortedSet and PriorityQueue
        {
            int nodeLength = list[0], lineLength = list[1], start = list[2];
            PriorityQueue<int, int> priorityQueue = new PriorityQueue<int, int>(nodeLength);
            List<(int, int)>[] graph = new List<(int, int)>[nodeLength + 1];
            int[] distance = new int[nodeLength + 1];

            Array.Fill(distance, int.MaxValue);
            for (int i = 0; i <= nodeLength; i++)
                graph[i] = new List<(int, int)>();

            for (int i = 0; i < lineLength; i++)
            {
                int index = (i + 1) * 3;
                graph[list[index]].Add((list[index + 1], list[index + 2]));
            }

            priorityQueue.Enqueue(start, 0);
            distance[start] = 0;

            while (priorityQueue.Count > 0)
            {
                priorityQueue.TryDequeue(out int index, out int value);

                foreach ((int nextIndex, int cost) in graph[index])
                {
                    int total = value + cost;
                    if (total >= distance[nextIndex])
                        continue;
                    distance[nextIndex] = total;
                    priorityQueue.Enqueue(nextIndex, total);
                }
            }

            List<string> result = new List<string>(nodeLength);
            for (int i = 1; i <= nodeLength; i++)
                result.Add(((distance[i] == int.MaxValue) ? "INF" : distance[i].ToString()));
            return result;
        }

        static List<string> Solution(List<int> list) // for test at under dotnet 7
        {
            var Compare = Comparer<(int, int)>.Create(((int x, int y) left, (int x, int y) right) => { if (left.x == right.x) return left.y - right.y; return left.x - right.x; });
            SortedSet<(int, int)> priorityQueue = new SortedSet<(int, int)>(Compare);
            int nodeLength = list[0], lineLength = list[1], start = list[2];
            List<(int, int)>[] graph = new List<(int, int)>[nodeLength + 1];
            int[] distance = new int[nodeLength + 1];

            Array.Fill(distance, int.MaxValue);
            for (int i = 0; i <= nodeLength; i++)
                graph[i] = new List<(int, int)>();

            for (int i = 0; i < lineLength; i++)
            {
                int index = (i + 1) * 3;
                graph[list[index]].Add((list[index + 1], list[index + 2]));
            }

            priorityQueue.Add((0, start));
            distance[start] = 0;

            while (priorityQueue.Count > 0)
            {
                (int value, int index) = priorityQueue.Min;
                priorityQueue.Remove(priorityQueue.Min);

                foreach ((int nextIndex, int cost) in graph[index])
                {
                    int total = value + cost;
                    if (total >= distance[nextIndex])
                        continue;
                    distance[nextIndex] = total;
                    priorityQueue.Add((total, nextIndex));
                }
            }

            List<string> result = new List<string>(nodeLength);
            for (int i = 1; i <= nodeLength; i++)
                result.Add(((distance[i] == int.MaxValue) ? "INF" : distance[i].ToString()));
            return result;
        }

        static List<List<int>> Solution(List<int> list)
        {
            List<List<int>> result = new List<List<int>>();
            int length = list[0], maxLength = list[1];
            List<int> sorted = new List<int>(length);

            var Recursive = null as Action<int, int, List<int>>;
            Recursive = (int depth, int bit, List<int> resultList) =>
            {
                if(depth >= maxLength)
                {
                    List<int> newList = new List<int>(maxLength);
                    newList.AddRange(resultList);
                    result.Add(newList);
                    return;
                }

                depth++;
                int previous = int.MaxValue;
                for (int i = 0; i < length; i++)
                {
                    if ((bit & (1 << i)) != 0 || sorted[i] == previous)
                        continue;

                    previous = sorted[i];
                    resultList.Add(sorted[i]);
                    Recursive(depth, bit | (1 << i), resultList);
                    resultList.RemoveAt(resultList.Count - 1);
                }
            };

            for (int i = 0; i < length; i++)
                sorted.Add(list[2 + i]);

            sorted.Sort();

            Recursive(0, 0, new List<int>(maxLength));

            return result;
        }

        static List<List<int>> Solution(List<int> list)
        {
            List<List<int>> result = new List<List<int>>();
            int length = list[0], maxLength = list[1];
            List<int> sorted = new List<int>(length);

            var Recursive = null as Action<int, int, List<int>>;
            Recursive = (int depth, int bit, List<int> resultList) =>
            {
                if(depth >= maxLength)
                {
                    List<int> newList = new List<int>(maxLength);
                    newList.AddRange(resultList);
                    result.Add(newList);
                    return;
                }

                depth++;
                int previous = int.MaxValue;
                for (int i = 0; i < length; i++)
                {
                    if ((bit & (1 << i)) != 0 || sorted[i] == previous)
                        continue;

                    previous = sorted[i];
                    resultList.Add(sorted[i]);
                    Recursive(depth, bit | (1 << i), resultList);
                    resultList.RemoveAt(resultList.Count - 1);
                }
            };

            for (int i = 0; i < length; i++)
                sorted.Add(list[2 + i]);

            sorted.Sort();

            Recursive(0, 0, new List<int>(maxLength));

            return result;
        }

        static int Solution(List<int> list)
        {
            var LowerBound = (int target, List<int> array) =>
            {
                int left = 0, right = array.Count;

                while (left < right)
                {
                    int middle = (right + left) / 2;

                    if (target <= array[middle])
                        right = middle;
                    else
                        left = middle + 1;
                }
                return left;
            };


            int length = list[0];
            List<int> table = new List<int>(length);

            for (int i = 0; i < length; i++)
            {
                int number = list[i + 1];
                int index = LowerBound(number, table);
                if (index >= table.Count)
                    table.Add(number);
                else
                    table[index] = number;
            }

            return table.Count;
        }

        static long Solution(List<long> list)
        {
            long result = 0;
            int length = (int)list[0];
            List<long> sum = new List<long>(length * length);

            var UpperBound = (long target, List<long> list) =>
                {
                    int left = 0, right = list.Count;
                    while (left < right)
                    {
                        int middle = (left + right) / 2;
                        if (target < list[middle])
                            right = middle;
                        else
                            left = middle + 1;
                    }
                    return left;
                };
            var LowerBound = (long target, List<long> list, out int index) =>
            {
                int left = 0, right = list.Count;
                while (left < right)
                {
                    int middle = (left + right) / 2;
                    if (target <= list[middle])
                        right = middle;
                    else
                        left = middle + 1;
                }
                index = left;
                return (index < 0 || index >= list.Count) ? false : target == list[index];
            };

            for (int i = 0; i < length; i++)
            {
                int index = 3 + i * 4;
                for (int j = 0; j < length; j++)
                    sum.Add(list[index] + list[4 + j * 4]);
            }

            sum.Sort();

            for (int i = 0; i < length; i++)
            {
                int indexA = 1 + i * 4;
                for (int j = 0; j < length; j++)
                {
                    long item = list[indexA] + list[2 + j * 4];

                    if (!LowerBound(item * -1, sum, out int index))
                        continue;
                    result += (long)(UpperBound(item * -1, sum) - index);
                }
            }

            return result;
        }

        static List<int> Solution(int caseLength)
        {
            List<int> result = new List<int>(caseLength);
            Queue<int> queue = new Queue<int>();

            for (int i = 0; i < caseLength; i++)
            {
                int counter = 0;
                int length = IO.InputSingle<int>();
                List<int> list = IO.Input<int>(1);
                List<int> inDegree = Enumerable.Repeat(0, length + 1).ToList();
                
                for (int j = 0; j < length; j++)
                    inDegree[list[j]]++;
                for (int j = 1; j <= length; j++)
                    if(inDegree[j] == 0)
                        queue.Enqueue(j);

                while (queue.Count > 0)
                {
                    int index = queue.Dequeue();
                    int next = list[index - 1];
                    counter++;
                    inDegree[next]--;

                    if (inDegree[next] == 0)
                        queue.Enqueue(next);
                }
                result.Add(counter);
            }
            return result;
        }

        static int Solution(List<int> list)
        {
            int nodeLength = list[1], limit = list[0], max = 0, result = int.MaxValue;
            List<(int, int)> cost = new List<(int, int)>(nodeLength);
            List<int> table;  // table[customerCount] = minCost

            for (int i = 0; i < nodeLength; i++)
            {
                cost.Add((list[i * 2 + 2], list[i * 2 + 3]));
                max = Math.Max(cost.Last().Item2, max);
            }

            max += limit;
            table = Enumerable.Repeat(int.MaxValue, max + 1).ToList(); //lazy initialization
            table[0] = 0;

            for (int i = 0; i < nodeLength; i++)
            {
                for (int j = cost[i].Item2; j <= max; j++)
                {
                    int previous = table[j - cost[i].Item2];
                    if (previous == int.MaxValue)
                        continue;

                    table[j] = Math.Min(table[j], previous + cost[i].Item1);
                }
            }

            for (int i = limit; i <= max; i++) // get result
                result = Math.Min(table[i], result);

            return result;
        }

        static List<int> Solution(List<int> list)
        {
            int nodeLength = list[0], caseLength = list[1];
            List<List<int>> graph = new List<List<int>>(nodeLength + 1);
            List<int> inDegree = new List<int>(nodeLength + 1), result = new List<int>(nodeLength);
            Queue<int> queue = new Queue<int>(nodeLength + 1);
            int pointer = 2;

            for (int i = 0; i <= nodeLength; i++)
            {
                graph.Add(new List<int>());
                inDegree.Add(0);
            }

            for (int i = 0; i < caseLength; i++)
            {
                int length = list[pointer++] - 1;

                for (int j = 0; j < length; j++)
                {
                    int front = list[pointer++];

                    graph[front].Add(list[pointer]);
                    inDegree[list[pointer]]++;
                }

                pointer++;
            }

            for (int i = 1; i <= nodeLength; i++)
                if (inDegree[i] == 0)
                    queue.Enqueue(i);

            while (queue.Count > 0)
            {
                int index = queue.Dequeue();

                result.Add(index);
                foreach (int item in graph[index])
                {
                    inDegree[item]--;
                    if (inDegree[item] != 0)
                        continue;

                    queue.Enqueue(item);
                }
            }

            if(result.Count != nodeLength)
                return new List<int>{ 0 };

            return result;
        }

        static int Solution(List<string> list)
        {
            int height = int.Parse(list[0]), width = int.Parse(list[1]);
            int maxHeight = height - 7, maxWidth = width - 7;
            int result = int.MaxValue, stack = 0, reverseStack = 0;
            Queue<(int, int, bool, bool)> queue = new Queue<(int, int, bool, bool)>(64);
            int i = 0, j = 0, endI = 0, endJ = 0;
            (bool, bool)[,] board = new (bool, bool)[height, width];

            var ReverseTravel = (int x, int y, bool black) =>
            {
                if (board[y, x].Item2)
                    return;

                board[y, x].Item2 = true;
                bool isBlack = list[y + 2][x] == 'B';
                if (black != isBlack)
                    reverseStack++;

                queue.Enqueue((x, y, black, true));
            };

            var Travel = (int x, int y, bool black, bool reverse) =>
            {
                if (x < j || y < i || x >= endJ || y >= endI)
                    return;

                if (reverse) { ReverseTravel(x, y, black); return; }

                if (board[y, x].Item1)
                    return;

                board[y, x].Item1 = true;
                bool isBlack = list[y + 2][x] == 'B';
                if (black != isBlack)
                    stack++;

                queue.Enqueue((x, y, black, false));
            };



            for (i = 0; i < maxHeight; i++)
            {
                for (j = 0; j < maxWidth; j++)
                {
                    endI = i + 8;
                    endJ = j + 8;

                    bool isBlack = list[i + 2][j] == 'B';
                    queue.Enqueue((j, i, isBlack, false));
                    queue.Enqueue((j, i, !isBlack, true));
                    stack = 0;
                    reverseStack = 1;

                    while (queue.Count > 0)
                    {
                        (int x, int y, bool black, bool reverse) = queue.Dequeue();

                        Travel(x + 1, y, !black, reverse);
                        Travel(x, y + 1, !black, reverse);
                    }
                    result = Math.Min(result, stack);
                    result = Math.Min(result, reverseStack);

                    board = new (bool, bool)[height, width];

                }
            }


            return result;
        }

        static string Solution(List<int> list)
        {
            int length = list[0], step = list[1];
            Queue<int> queue = new Queue<int>(length);
            string result = "<";
            
            for (int i = 0; i < length; i++)
                queue.Enqueue(i + 1);

            while (queue.Count > 1)
            {
                for (int i = 1; i < step; i++)
                    queue.Enqueue(queue.Dequeue());


                result += queue.Dequeue().ToString() + ", ";
            }
            result += queue.First().ToString() + ">";

            return result;
        }

        static List<string> Solution(List<int> list)
        {
            int max = list[0], length = list[1];
            int maxBit = 1 << max;
            List<string> result = new List<string>();

            string storage;
            for (int i = 0; i < maxBit; i++)
            {
                int activeBit = 0;
                storage = "";
                for (int j = 0; j < max; j++)
                {
                    if ((i & (1 << j)) == 0)
                        continue;

                    activeBit++;
                    storage += (j + 1).ToString() + " ";
                }

                if (activeBit != length)
                    continue;

                if (storage.Length != 0)
                    result.Add(storage.TrimEnd());
            }
            result.Sort();
            return result;
        }

        static int Solution(List<int> list)
        {
            int nodeLength = list[0], lineLength = list[1], destination = list[2], result = 0;
            List<int> departure = Enumerable.Repeat(int.MaxValue, nodeLength + 1).ToList(), arrival = departure.ToList();
            List<List<(int, int)>> graph = Enumerable.Repeat(0, nodeLength + 1).Select(index => new List<(int, int)>()).ToList();
            List<List<(int, int)>> graphReversed = Enumerable.Repeat(0, nodeLength + 1).Select(index => new List<(int, int)>()).ToList();
            PriorityQueue<int, int> queue = new PriorityQueue<int, int>();

            var Function = (List<List<(int, int)>> targetGraph, List<int> resultList, int start) =>
            {
                queue.Enqueue(start, 0);
                resultList[start] = 0;

                while (queue.TryDequeue(out int index, out int cost))
                {
                    if (cost > resultList[index])
                        continue;

                    foreach ((int nextIndex, int nextCost) in targetGraph[index])
                    {
                        int total = nextCost + cost;
                        if (total >= resultList[nextIndex])
                            continue;
                        resultList[nextIndex] = total;
                        queue.Enqueue(nextIndex, total);
                    }
                }
            };

            for (int i = 0; i < lineLength; i++)
            {
                int start = list[i * 3 + 3], end = list[i * 3 + 4], cost = list[i * 3 + 5];
                graph[start].Add((end, cost));
                graphReversed[end].Add((start, cost));
            }

            Function(graph, departure, destination);
            Function(graphReversed, arrival, destination);

            for (int i = 0; i < nodeLength; i++)
                result = Math.Max(result, departure[i + 1] + arrival[i + 1]);

            return result;
        }

        static int Solution(List<int> list)
        {
            int length = list[0], max = 0, result = 0;
            List<(int, int)> timeTable = Enumerable.Repeat((0, 0), length).ToList();

            for (int i = 0; i < length; i++)
            {
                int start = list[i * 2 + 1];
                int end = list[i * 2 + 2];

                if(start > end)
                {
                    start = end;
                    end = list[i * 2 + 1];
                }
                timeTable[i] = (start, end);

            }

            timeTable.Sort(((int, int) left, (int, int) right) => left.Item2 != right.Item2 ? left.Item2.CompareTo(right.Item2) : left.Item1.CompareTo(right.Item1));

            foreach ((int start, int end) in timeTable)
            {
                if(max <= start)
                {
                    max = end;
                    result++;
                }
            }

            return result;
        }

        static int Solution(List<int> list)
        {
            int width = list[0], height = list[1], result = 0;
            Queue<(int, int)> queue = new Queue<(int, int)>();
            List<List<int>> time = Enumerable.Repeat(0, height).Select(index => Enumerable.Repeat(int.MaxValue, width).ToList()).ToList();

            Action<int, int, int> Travel = (int i, int j, int next) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
                if (time[i][j] <= next)
                    return;

                time[i][j] = next;
                queue.Enqueue((i,j));
            };

            for (int i = 0; i < height; i++)
            {
                int pointer = 2 + width * i;
                for (int j = 0; j < width; j++)
                {
                    if (list[pointer + j] != 0)
                    {
                        time[i][j] = 0;
                        if (list[pointer + j] == 1)
                            queue.Enqueue((i, j));
                    }
                }
            }

            while (queue.Count > 0)
            {
                (int i, int j) = queue.Dequeue();
                int next = time[i][j] + 1;

                Travel(i - 1, j, next);
                Travel(i + 1, j, next);
                Travel(i, j - 1, next);
                Travel(i, j + 1, next);
            }

            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (time[i][j] == int.MaxValue)
                        return -1;
                    result = Math.Max(result, time[i][j]);
                }
            }


            return result;
        }

        static int Solution(List<int> list)
        {
            int length = list[0], target = list[1], result = 0;
            int left = 0, right = list.Max(), middle;

            while (left <= right)
            {
                middle = (left + right) / 2;
                long sum = 0;
                for (int i = 0; i < length; i++)
                {
                    int height = list[2 + i];
                    if (height > middle)
                        sum += Convert.ToInt64(height - middle);
                }

                if (sum >= target)
                {
                    left = middle + 1;
                    result = middle;
                }
                else
                    right = middle - 1;
            }

            return result;
        }

        static int Solution(List<int> list)
        {
            int length = list[0], target = list[1];
            Func<int, int, int>? Recursive = null;

            Recursive = (int index, int stack) => 
            {
                int result = 0;
                stack += list[index + 2];

                if (stack == target)
                    result++;

                for (int i = index + 1; i < length; i++)
                    result += Recursive(i, stack);

                return result;
            };


            int result = 0;

            for (int i = 0; i < length; i++)
                result += Recursive(i, 0);

            return result;
        }

        static List<string> Solution(List<int> list)
        {
            int nodeLength = list[0], length = list[1];
            int start = list[list.Count - 2], end = list[list.Count - 1];
            List<List<(int, int)>> graph = Enumerable.Repeat(0, nodeLength + 1).Select(innerList => new List<(int, int)>()).ToList();
            List<int> costResult = Enumerable.Repeat(int.MaxValue, nodeLength + 1).ToList(), parents = costResult.ToList(), trace = new List<int>();
            List<string> result = new List<string>();
            PriorityQueue<int, int> queue = new PriorityQueue<int, int>();

            for (int i = 0; i < length; i++)
            {
                int index = list[i * 3 + 2];
                int nextIndex = list[i * 3 + 3];
                int cost = list[i * 3 + 4];

                graph[index].Add((nextIndex, cost));
            }

            costResult[start] = 0;
            queue.Enqueue(start, 0);
            while (queue.Count > 0)
            {
                int index, cost;
                if (!queue.TryDequeue(out index, out cost))
                    break;

                if (cost > costResult[index])
                    continue;

                foreach ((int nextIndex, int nextCost) in graph[index])
                {
                    int total = cost + nextCost;
                    if (total >= costResult[nextIndex])
                        continue;

                    parents[nextIndex] = index;
                    costResult[nextIndex] = total;
                    queue.Enqueue(nextIndex, total);
                }
            }

            int current = end;
            trace.Add(end);
            while (current != start) 
            {
                current = parents[current];
                trace.Add(current);
            }
            trace.Reverse();

            result.Add(costResult[end].ToString());
            result.Add(trace.Count.ToString());
            result.Add(string.Join(" ", trace));

            return result;
        }

        static int Solution(List<int> list)
        {
            int nodeLength = list[0], length = list[1];
            List<List<int>> graph = Enumerable.Repeat(0, nodeLength + 1).Select(innerList => new List<int>()).ToList();
            List<bool> origin = Enumerable.Repeat(false, nodeLength + 1).ToList();
            Queue<(int, int)> queue = new Queue<(int, int)>();
            SortedSet<(int, int)> resultList = new SortedSet<(int, int)>();

            for (int i = 0; i < length; i++)
            {
                int index = list[i * 2 + 2];
                int nextIndex = list[i * 2 + 3];

                graph[index].Add(nextIndex);
                graph[nextIndex].Add(index);
            }

            for (int i = 1; i <= nodeLength; i++)
            {
                int sum = 0;
                List<bool> visitable = origin.ToList();
                queue.Enqueue((i, 0));
                visitable[i] = true;

                while (queue.Count > 0)
                {
                    (int index, int depth) = queue.Dequeue();
                    depth++;

                    foreach (int node in graph[index])
                    {
                        if (visitable[node])
                            continue;

                        visitable[node] = true;
                        sum += depth;
                        queue.Enqueue((node, depth));
                    }
                }
                resultList.Add((sum, i));
            }

            return resultList.Min().Item2;
        }

        static int Solution(List<int> list)
        {
            int nodeLength = list[0], length = list[1], result = 0;
            List<List<(int, int)>> graph = Enumerable.Repeat(0, nodeLength + 1).Select(innerList => new List<(int, int)>()).ToList();
            List<bool> visitable = Enumerable.Repeat(false, nodeLength + 1).ToList();
            PriorityQueue<int, int> queue = new PriorityQueue<int, int>(nodeLength);

            queue.Enqueue(1, 0);
            for (int i = 0; i < length; i++)
            {
                int index = list[i * 3 + 2];
                int nextIndex = list[i * 3 + 3];
                int cost = list[i * 3 + 4];

                graph[index].Add((nextIndex, cost));
                graph[nextIndex].Add((index, cost));
            }

            while (queue.Count > 0)
            {
                int index, cost;
                if (!queue.TryDequeue(out index, out cost))
                    break;

                if (visitable[index])
                    continue;

                visitable[index] = true;
                result += cost;

                foreach ((int nextIndex, int nextCost) in graph[index])
                {
                    if (visitable[nextIndex])
                        continue;

                    queue.Enqueue(nextIndex, nextCost);
                }
            }
            return result;
        }

        static int Solution(List<int> list)
        {
            int first = list[0], second = list[1], last = list[2];

            int group = Math.Min(first / 2, second);

            int left = first + second - group * 3;
            if (left >= last)
                return group;

            int need = (int)Math.Ceiling(Convert.ToDouble(last - left) / 3.0d) + 1;
            if (group - need < 0)
                return 0;

            return group - need;
        }

        static int Solution(List<int> list)
        {
            int size = list[0];
            List<(int, int)> movement = new List<(int, int)> { (-2, -1), (-2, 1), (0, -2), (0, 2), (2, -1), (2, 1) };
            List<List<int>> board = Enumerable.Repeat(0, size).Select(innerList => Enumerable.Repeat(int.MaxValue, size).ToList()).ToList();
            Queue<(int, int)> queue = new Queue<(int, int)>(size * size);
            queue.Enqueue((list[1], list[2]));
            board[list[1]][list[2]] = 0;

            while (queue.Count > 0)
            {
                (int i, int j) = queue.Dequeue();
                int cost = board[i][j] + 1;

                for (int k = 0; k < movement.Count; k++)
                {
                    int nextI = i + movement[k].Item1;
                    int nextJ = j + movement[k].Item2;

                    if (nextI < 0 || nextJ < 0 || nextI >= size || nextJ >= size)
                        continue;

                    if (board[nextI][nextJ] <= cost)
                        continue;

                    board[nextI][nextJ] = cost;
                    queue.Enqueue((nextI, nextJ));
                }
            }

            if (board[list[3]][list[4]] == int.MaxValue)
                return -1;

            return board[list[3]][list[4]];
        }

        static int Solution(List<string> list)
        {
            int height = int.Parse(list[0]), width = int.Parse(list[1]), result = 0;
            int depth = int.Parse(list[2]), floor = int.Parse(list[3]);
            List<List<int>> board = Enumerable.Repeat(0, height).Select(innerlist => Enumerable.Repeat(1, width).ToList()).ToList();
            List<List<int>> boardOrigin = Enumerable.Repeat(0, height).Select(innerlist => Enumerable.Repeat(1, width).ToList()).ToList();
            Queue<(int, int, int)> queue = new Queue<(int, int, int)>();
        
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (list[i + 6][j] == '.')
                        continue;
        
                    if (list[i + 6][j] == '*')
                    {
                        board[i][j] = 0;
                        boardOrigin[i][j] = 0;
                        queue.Enqueue((i, j, depth - 1));
                    }
                    else
                        boardOrigin[i][j] = board[i][j] = floor + 1;
                }
            }
        
            Action<int, int, int> Search = (int i, int j, int depth) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
        
                if (board[i][j] == 0 || board[i][j] != boardOrigin[i][j])
                    return;
        
        
                board[i][j]--;
                queue.Enqueue((i, j, depth));
        
            };
            IO.Output<int>(board);
        
            while (queue.Count > 0)
            {
                (int i, int j, int currentDepth) = queue.Dequeue();
        
                if (currentDepth <= 0)
                    continue;
        
                if (board[i][j] > 0)
                {
                    board[i][j]--;
                    queue.Enqueue((i, j, currentDepth - 1));
                    continue;
                }
        
                Search(i - 1, j, currentDepth - 1);
                Search(i + 1, j, currentDepth - 1);
                Search(i, j - 1, currentDepth - 1);
                Search(i, j + 1, currentDepth - 1);
            }
        
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < depth; j++)
                {
                    if (board[i][j] > 0)
                        result++;
                }
            }
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], endIndex = list[1], result;
            List<(int, int)> reverse = new List<(int, int)>(length);
            List<(int, int)> innerList = new List<(int, int)>(length);
        
            for (int i = 0; i < length; i++)
            {
                int from = list[2 + i * 2];
                int to = list[3 + i * 2];
        
                if (from <= to)
                    continue;
        
                reverse.Add((from, to));
            }
        
            reverse.Sort((left, right) => right.Item1.CompareTo(left.Item1));
            (int max, int min) = reverse[0];
        
            for (int i = 1; i < reverse.Count; i++)
            {
                if (min > reverse[i].Item1)
                {
                    innerList.Add((max, min));
                    max = reverse[i].Item1;
                    min = reverse[i].Item2;
                }
                else
                {
                    max = Math.Max(max, reverse[i].Item1);
                    min = Math.Min(min, reverse[i].Item2);
                }
            }
        
            innerList.Add((max, min));
            result = endIndex;
            for (int i = 0; i < innerList.Count; i++)
                result += (innerList[i].Item1 - innerList[i].Item2) * 2;
        
            return result;
        }*/
        /*
        static string Solution(List<int> list)
        {
            int length = list[0], minResult = int.MaxValue, maxResult = 0;
        
            List<List<int>> minBoard = Enumerable.Repeat(0, length + 1).Select(innerList => Enumerable.Repeat(int.MaxValue, 3).ToList()).ToList();
            List<List<int>> maxBoard = Enumerable.Repeat(0, length + 1).Select(innerList => Enumerable.Repeat(0, 3).ToList()).ToList();
        
            for (int i = 0; i < 3; i++)
            {
                minBoard[0][i] = 0;
                maxBoard[0][i] = 0;
            }
        
            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int k = -1; k < 2; k++)
                    {
                        int index = k + j;
                        if (index < 0 || index > 2)
                            continue;
        
                        int cost = list[i * 3 + index + 1];
        
                        minBoard[i + 1][index] = Math.Min(minBoard[i + 1][index], minBoard[i][j] + cost);
                        maxBoard[i + 1][index] = Math.Max(maxBoard[i + 1][index], maxBoard[i][j] + cost);
                    }
                }
            }
        
            for (int i = 0; i < 3; i++)
            {
                minResult = Math.Min(minResult, minBoard[length][i]);
                maxResult = Math.Max(maxResult, maxBoard[length][i]);
            }
        
            IO.Output<int>(minBoard);
            IO.Output<int>(maxBoard);
        
            return maxResult.ToString() + " " + minResult.ToString();
        }
        
        struct UnionFind
        {
            public UnionFind() { }
            public Dictionary<string, int> nameSet = new Dictionary<string, int>();
            public List<int> parents = new List<int>();
            public List<int> setCount = new List<int>();
        
            public int GetIndex(string name)
            {
                int outValue;
                if (!nameSet.TryGetValue(name, out outValue))
                {
                    outValue = parents.Count;
                    nameSet.Add(name, parents.Count);
                    parents.Add(outValue);
                    setCount.Add(1);
                }
        
                return outValue;
            }
        
            public int FindParent(int index)
            {
                if (parents[index] == index)
                    return index;
        
                parents[index] = FindParent(parents[index]);
                return parents[index];
            }
        
            public void Unite(int indexA, int indexB)
            {
                indexA = FindParent(indexA);
                indexB = FindParent(indexB);
        
                parents[indexB] = indexA;
                setCount[indexA] += setCount[indexB];
            }
        
            public int GetCount(int index) { return setCount[FindParent(index)]; }
            public void Clear() { nameSet.Clear(); parents.Clear(); setCount.Clear(); }
        }
        
        static List<int> Solution(List<string> list)
        {
            int caseLength = int.Parse(list[0]), pointer = 1;
            UnionFind unionFind = new UnionFind();
            List<int> result = new List<int>();
        
            for (int step = 0; step < caseLength; step++)
            {
                int lineLength = int.Parse(list[pointer]);
                pointer++;
                for (int i = 0; i < lineLength; i++)
                {
                    int indexA = unionFind.GetIndex(list[pointer + i * 2]);
                    int indexB = unionFind.GetIndex(list[pointer + i * 2 + 1]);
        
                    if(indexA == indexB || unionFind.FindParent(indexA) == unionFind.FindParent(indexB))
                    {
                        result.Add(unionFind.GetCount(indexA));
                        continue;
                    }
        
                    unionFind.Unite(indexA, indexB);
                    result.Add(unionFind.GetCount(indexA));
                }
        
                unionFind.Clear();
                pointer += lineLength * 2;
            }
        
            return result;
        }
        
        static List<string> Solution(List<string> list)
        {
            int height = int.Parse(list[0]), width = int.Parse(list[1]);
        
            List<List<int>> board = Enumerable.Repeat(0, height).Select(innerList => Enumerable.Repeat(-1, width).ToList()).ToList();
            List<string> result = Enumerable.Repeat("", height).ToList();
            Queue<(int, int)> queue = new Queue<(int, int)>();
            List<int> emptySpace = new List<int>();
        
            Action<int, int, int> Search = (int i, int j, int index) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
        
                if (list[i + 2][j] != '0' || board[i][j] != -1)
                    return;
        
                board[i][j] = index;
                queue.Enqueue((i, j));
            };
            Action<int, int> Get = (int i, int j) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width || board[i][j] == -1)
                    return;
        
                foreach ((int index, int count) in queue)
                {
                    if (index == board[i][j])
                        return;
                }
        
                queue.Enqueue((board[i][j], emptySpace[board[i][j]]));
            };
        
        
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (list[i + 2][j] == '1' || board[i][j] != -1)
                        continue;
        
                    queue.Enqueue((i, j));
                    board[i][j] = emptySpace.Count;
                    int empty = 0;
                    while (queue.Count != 0)
                    {
                        (int x, int y) = queue.Dequeue();
                        empty++;
        
                        Search(x - 1, y, emptySpace.Count);
                        Search(x + 1, y, emptySpace.Count);
                        Search(x, y - 1, emptySpace.Count);
                        Search(x, y + 1, emptySpace.Count);
                    }
        
                    emptySpace.Add(empty);
                }
            }
        
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    if (list[i + 2][j] != '1')
                    {
                        result[i] += "0";
                        continue;
                    }
        
                    int empty = 1;
                    Get(i - 1, j);
                    Get(i + 1, j);
                    Get(i, j - 1);
                    Get(i, j + 1);
        
                    foreach ((int index, int count) in queue)
                        empty += count;
        
                    queue.Clear();
        
                    result[i] += (empty % 10).ToString();
                }
            }
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], result = 0;
            PriorityQueue<int, int> queue = new PriorityQueue<int, int>();
        
            for (int i = 0; i < length; i++)
                queue.Enqueue(list[1 + i], list[1 + i]);
        
            while (queue.Count > 1)
            {
                int value = queue.Dequeue();
                value += queue.Dequeue();
        
                result += value;
        
                queue.Enqueue(value, value);
            }
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0];
        
            List<int> table = new List<int>();
        
            for (int i = 0; i < length; i++)
            {
                int number = list[i + 1];
                int left = 0, right = table.Count, middle;
        
                while (left < right)
                {
                    middle = (left + right) / 2;
        
                    if (table[middle] > number)
                        left = middle + 1;
                    else
                        right = middle;
                }
        
                if (left == table.Count)
                    table.Add(number);
                else
                    table[left] = number;
            }
        
            return length - table.Count;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0];
            List<int> tableA = Enumerable.Repeat(0, length + 1).ToList();
            List<int> tableB = Enumerable.Repeat(0, length + 1).ToList();
            List<int> lisTable = new List<int>();
        
            for (int i = 0; i < length; i++)
                tableA[list[i + 1]] = i;
        
            for (int i = 0; i < length; i++)
                tableB[i] = tableA[list[i + 1 + length]];
        
            foreach (var item in tableB)
            {
                int left = 0, right = lisTable.Count, middle;
                while (left < right)
                {
                    middle = (left + right) / 2;
        
                    if (lisTable[middle] < item)
                        left = middle + 1;
                    else
                        right = middle;
                }
        
                if (left == lisTable.Count)
                    lisTable.Add(item);
                else
                    lisTable[left] = item;
        
            }
        
            return lisTable.Count;
        }
        
        static int Solution(List<int> list)
        {
            List<int> table = new List<int>(list.Count);
        
            for (int i = 0; i < list[0]; i++)
            {
                int number = list[i + 1];
                int left = 0, right = table.Count, middle;
        
                while (left < right)
                {
                    middle = (right + left) / 2;
        
                    if (table[middle] < number)
                        left = middle + 1;
                    else
                        right = middle;
                }
        
                if (left == table.Count)
                    table.Add(number);
                else
                    table[left] = number;
            }
        
            return table.Count;
        }
        
        static int Solution(List<string> list)
        {
            int lengthA = list[0].Length, lengthB = list[1].Length, lengthC = list[2].Length;
            List<List<List<int>>> table = Enumerable.Repeat(0, lengthA  + 1).Select(innerListA => Enumerable.Repeat(0, lengthB + 1).Select(innerListB => Enumerable.Repeat(0, lengthC + 1).ToList()).ToList()).ToList();
        
            for (int i = 1; i <= lengthA; i++)
            {
                for (int j = 1; j <= lengthB; j++)
                {
                    for (int k = 1; k <= lengthC; k++)
                    {
                        if (list[0][i - 1] == list[1][j - 1] && list[1][j - 1] == list[2][k - 1])
                            table[i][j][k] = Math.Max(table[i][j][k], table[i - 1][j - 1][k - 1] + 1);
                        else
                        {
                            table[i][j][k] = Math.Max(table[i][j][k], table[i - 1][j][k]);
                            table[i][j][k] = Math.Max(table[i][j][k], table[i][j - 1][k]);
                            table[i][j][k] = Math.Max(table[i][j][k], table[i][j][k - 1]);
                            table[i][j][k] = Math.Max(table[i][j][k], table[i - 1][j - 1][k]);
                            table[i][j][k] = Math.Max(table[i][j][k], table[i - 1][j][k - 1]);
                            table[i][j][k] = Math.Max(table[i][j][k], table[i][j - 1][k - 1]);
                        }
                    }
                }
            }
        
            return table[lengthA][lengthB][lengthC];
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], result = 0;
            List<int> table = Enumerable.Repeat(1, length).ToList();
        
            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (list[i + 1] > list[j + 1])
                        table[i] = Math.Max(table[i], table[j] + 1); 
                }
            }
        
            for (int i = 0; i < length; i++)
                result = Math.Max(result, table[i]);
        
            return result;
        }
        
        static int Solution(List<string> list)
        {
            int height = int.Parse(list[0]), width = int.Parse(list[1]);
            List<List<int>> board = Enumerable.Repeat(0, height).Select(innerList => Enumerable.Repeat(int.MaxValue, width).ToList()).ToList();
            Queue<(int, int, bool)> queue = new Queue<(int, int, bool)>();
            Action<int, int, int, bool> Visit = (int i, int j, int cost, bool breakable) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
        
                if (board[i][j] <= cost)
                    return;
        
                if (list[i + 2][j] == '1')
                {
                    if (breakable)
                        breakable = !breakable;
                    else
                        return;
                }
        
                board[i][j] = cost;
        
                queue.Enqueue((i, j, breakable));
            };
        
            queue.Enqueue((0,0,true));
            board[0][0] = 1;
        
            while (queue.Count != 0)
            {
                var (i, j, breakable) = queue.Dequeue();
                int cost = board[i][j] + 1;
        
                Visit(i - 1, j, cost, breakable);
                Visit(i + 1, j, cost, breakable);
                Visit(i, j - 1, cost, breakable);
                Visit(i, j + 1, cost, breakable);
            }
            IO.Output(board);
        
            if (board[height - 1][width - 1] == int.MaxValue)
                return -1;
        
            return board[height - 1][width - 1];
        }
        
        static int Solution(List<int> list)
        {
            int size = list[2], length = list[3], result = 0;
        
            for (int i = 0; i < length; i++)
            {
                int x = list[i * 2 + 4];
        
                for (int j = 0; j < length; j++)
                {
                    int y = list[j * 2 + 5];
                    int count = 0;
                    for (int k = 0; k < length; k++)
                    {
                        int xOther = list[k * 2 + 4];
                        int yOther = list[k * 2 + 5];
        
                        if (x <= xOther && xOther <= x + size && y <= yOther && yOther <= y + size)
                            count++;
                    }
                    result = Math.Max(result, count);
                }
            }
        
            return length - result;
        }
        
        static int Solution(List<int> list)
        {
            int ladder = list[0], snake = list[1], length = ladder + snake;
            List<int> route = Enumerable.Repeat(-1, 101).ToList();
            List<int> table = Enumerable.Repeat(int.MaxValue, 101).ToList();
            Queue<int> queue = new Queue<int>();
        
            for (int i = 0; i < length; i++)
                route[list[i * 2 + 2]] = list[i * 2 + 3];
        
            table[1] = 0;
            queue.Enqueue(1);
            while (queue.Count != 0)
            {
                int index = queue.Dequeue();
                int count = table[index];
        
                if (index == 100)
                    break;
        
                if(route[index] != -1)
                {
                    if (table[route[index]] <= count)
                        continue;
        
                    table[route[index]] = count;
                    queue.Enqueue(route[index]);
                }
                else
                {
                    count++;
                    for (int i = 1; i < 7; i++)
                    {
                        int nextIndex = index + i;
                        if (nextIndex > 100)
                            break;
        
                        if (table[nextIndex] <= count)
                            continue;
        
                        table[nextIndex] = count;
                        queue.Enqueue(nextIndex);
                    }
                }
        
            }
        
            return table[100];
        }
        
        static int Solution(List<int> list)
        {
            int height = list[0], width = list[1], result = 0;
            List<int> leftTable = Enumerable.Repeat(0, width).ToList(), rightTable = leftTable.ToList();
        
            leftTable[0] = list[2];
            for (int i = 1; i < width; i++)
                leftTable[i] = Math.Max(leftTable[i - 1], list[i + 2]);
        
            rightTable[width - 1] = list.Last();
            for (int i = width - 2; i >= 0; i--)
                rightTable[i] = Math.Max(rightTable[i + 1], list[i + 2]);
        
            for (int i = 0; i < width; i++)
            {
                int value = Math.Min(leftTable[i], rightTable[i]);
        
                if (value > list[i + 2])
                    result += value - list[i + 2];
            }
        
            return result;
        }
        
        static List<List<int>> Solution(List<int> list)
        {
            int height = list[0], width = list[1];
            List<List<int>> board = Enumerable.Repeat(0, height).Select(innerList => Enumerable.Repeat(int.MaxValue, width).ToList()).ToList();
            Queue<(int, int)> queue = new Queue<(int, int)>();
            Action<int, int, int> Go = (int i, int j, int count) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
        
                if (board[i][j] <= count)
                    return;
        
                board[i][j] = count;
                queue.Enqueue((i,j));
            };
        
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    int index = (i * width) + j + 2;
                    if (list[index] == 1)
                        continue;
        
                    board[i][j] = 0;
        
                    if (list[index] == 2)
                        queue.Enqueue((i, j));
                }
            }
        
            while (queue.Count != 0)
            {
                (int x, int y) = queue.Dequeue();
                int cost = board[x][y] + 1;
        
                Go(x - 1, y, cost);
                Go(x + 1, y, cost);
                Go(x, y - 1, cost);
                Go(x, y + 1, cost);
            }
        
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    if (board[i][j] == int.MaxValue)
                        board[i][j] = -1;
        
            return board;
        }
        
        static List<string> Solution(List<int> list)
        {
            int length = list[0], limitNumber = 0;
            List<int> table = Enumerable.Repeat(0, 10).ToList();
            List<string> result = new List<string>();
        
            Action<int>? Search = null;
            Search = (int number) =>
            {
                if (number >= limitNumber)
                {
                    int intStack = 0, operatorStack = 1;
                    string stacked = "";
        
                    for (int i = 1; i <= limitNumber; i++)
                    {
                        if(table[i] == 0)
                        {
                            stacked += i.ToString();
                            continue;
                        }
        
                        intStack += int.Parse(stacked) * operatorStack;
                        operatorStack = table[i];
                        stacked = i.ToString();
                    }
        
                    intStack += int.Parse(stacked) * operatorStack;
        
        
                    if (intStack == 0)
                    {
                        stacked = "1";
        
                        for (int i = 2; i <= limitNumber; i++)
                        {
                            switch (table[i])
                            {
                                case -1:
                                    stacked += "-" + i.ToString();
                                    break;
                                case 0:
                                    stacked += " " + i.ToString();
                                    break;
                                case 1:
                                    stacked += "+" + i.ToString();
                                    break;
                                default:
                                    break;
                            }
                        }
        
                        result.Add(stacked);
                    }
        
                    return;
                }
        
                table[number + 1] = 0;
                Search(number + 1);
        
                table[number + 1] = 1;
                Search(number + 1);
        
                table[number + 1] = -1;
                Search(number + 1);
        
            };
        
            for (int step = 0; step < length; step++)
            {
                limitNumber = list[step + 1];
        
                Search(1);
                result.Add("");
            }
        
            return result;
        }
        
        static int Solution(List<string> list)
        {
            int height = int.Parse(list[0]), width = int.Parse(list[1]);
            List<List<int>> board = Enumerable.Repeat(0, height).Select(list => Enumerable.Repeat(int.MaxValue, width).ToList()).ToList();
            Queue<(int, int)> queue = new Queue<(int, int)>();
        
            Action<int, int, int> Search = (int i, int j, int cost) =>
            {
                if (i < 0 || j < 0 || i >= height || j >= width)
                    return;
        
                if (list[i + 2][j] == '0' || board[i][j] <= cost)
                    return;
        
                board[i][j] = cost;
                queue.Enqueue((i, j));
            };
        
            board[0][0] = 1;
            queue.Enqueue((0, 0));
        
            while (queue.Count > 0)
            {
                (int x, int y) position = queue.Dequeue();
        
                int cost = board[position.x][position.y] + 1;
                Search(position.x - 1, position.y, cost);
                Search(position.x + 1, position.y, cost);
                Search(position.x, position.y- 1, cost);
                Search(position.x, position.y+ 1, cost);
            }
        
            return board[height - 1][width - 1];
        }
        
        static List<string> Solution(List<string> list)
        {
            int length = int.Parse(list[0]);
            List<string> result = new List<string>(length);
            HashSet<char> selected = new HashSet<char>();
            char part;
            Func<int, string, string> SetHighlight = (index, item) => { return item.Substring(0, index) + "[" + item[index] + "]" + item.Substring(index + 1, item.Length - (index + 1)); };
        
            for (int i = 0; i < length; i++)
            {
                string target = list[i + 1];
                bool done = false;
                List<string> token = target.Split(' ', StringSplitOptions.RemoveEmptyEntries).ToList();
        
                for (int j = 0; j < token.Count; j++)
                {
                    if (token[j].Length == 0 || token[j].First() == ' ')
                        continue;
        
                    part = char.ToUpper(token[j].First());
                    if (selected.Contains(part))
                        continue;
        
                    done = true;
                    selected.Add(part);
                    token[j] = SetHighlight(0, token[j]);
                    break;
                }
        
                if(done)
                    result.Add(string.Join(" ", token));
                else
                {
                    for (int j = 0; j < target.Length; j++)
                    {
                        if (target[j] == ' ')
                            continue;
        
                        part = char.ToUpper(target[j]);
                        if (selected.Contains(part))
                            continue;
        
                        selected.Add(part);
                        target = SetHighlight(j, target);
                        break;
                    }
        
                    result.Add(target);
                }
            }
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0];
        
            List<int> table = Enumerable.Repeat(0, length + 1).ToList(); 
            for (int i = 0; i < length; i++)
            {
                int duration = list[i * 2 + 1];
                int reward = list[i * 2 + 2];
        
                table[i + 1] = Math.Max(table[i + 1], table[i]);
                if (i + duration <= length)
                    table[i + duration] = Math.Max(table[i + duration], table[i] + reward);
            }
        
        
            return table[length];
        }
        
        static List<int> Solution(List<int> list)
        {
            int nodeLength = list[0], length = list[1 + nodeLength];
        
            List<int> result = new List<int>();
            HashSet<int> hashSet = list.GetRange(1, nodeLength).ToHashSet();
        
            for (int i = 0; i < length; i++)
                result.Add((hashSet.Contains(list[2 + nodeLength + i])) ? 1 : 0);
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], lineLength = list[1];
            List<List<int>> graph = Enumerable.Range(0, length).Select(list => new List<int>()).ToList();
            List<bool> visitable = Enumerable.Repeat(false, length).ToList();
            List<Tuple<int, int>> stack = new List<Tuple<int, int>>(length);
        
            Func<int, int, int> DFS = null;
            DFS = (int index, int depth) =>
            {
                visitable[index] = true;
        
                if (depth >= 5)
                    return 1;
        
                foreach (var item in graph[index])
                {
                    if (visitable[item])
                        continue;
        
                    if (DFS(item, depth + 1) == 1)
                        return 1;
                }
        
                visitable[index] = false;
                return 0;
            };
        
            for (int i = 0; i < lineLength; i++)
            {
                graph[list[(i + 1) * 2]].Add(list[(i + 1) * 2 + 1]);
                graph[list[(i + 1) * 2 + 1]].Add(list[(i + 1) * 2]);
            }
        
            for (int i = 0; i < length; i++)
            {
                if (DFS(i, 1) == 1)
                    return 1;
            }
        
            return 0;
        }
        
        static List<int> Solution(List<string> list)
        {
            int length = Convert.ToInt32(list[0]);
            List<int> result = new List<int>();
            List<List<bool>> board = Enumerable.Range(0, length).Select(list => Enumerable.Repeat(false, length).ToList()).ToList();
            List<Tuple<int, int>> stack = new List<Tuple<int, int>>();
        
            var Input = (int x, int y) =>
            {
                if (x < 0 || x >= length || y < 0 || y >= length)
                    return;
        
                if (board[x][y] || list[x + 1][y] != '1')
                    return;
        
                board[x][y] = true;
                stack.Add(Tuple.Create(x, y));
            };
        
            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < length; j++)
                {
                    if (list[i + 1][j] != '1' || board[i][j])
                        continue;
        
                    int count = 0;
                    stack.Add(Tuple.Create(i, j));
                    board[i][j] = true;
                    while (stack.Count != 0)
                    {
                        Tuple<int, int> position = stack.Last();
                        stack.RemoveAt(stack.Count - 1);
        
                        count++;
        
                        Input(position.Item1 + 1, position.Item2);
                        Input(position.Item1 - 1, position.Item2);
                        Input(position.Item1, position.Item2 + 1);
                        Input(position.Item1, position.Item2 - 1);
                    }
        
                    result.Add(count);
                }
            }
        
            result.Sort();
            result.Insert(0, result.Count);
        
            return result;
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], maxWeight = list[1];
            List<List<int>> table = Enumerable.Range(0, length + 1).Select(innerList => Enumerable.Repeat(0, maxWeight + 1).ToList()).ToList();
        
            for (int i = 0; i < length; i++)
            {
                int weight = list[i * 2 + 2];
                int value = list[i * 2 + 3];
        
                for (int j = 0; j <= maxWeight; j++)
                {
                    if (j < weight)
                        table[i + 1][j] = table[i][j];
                    else
                        table[i + 1][j] = Math.Max(table[i][j], table[i][j - weight] + value);
                }
            }
        
            return table[length][maxWeight];
        }
        
        static int Solution(List<int> list)
        {
            int length = list[0], result = 0;
            List<int> table = Enumerable.Repeat(1, length).ToList();
        
            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (list[i + 1] <= list[j + 1])
                        continue;
        
                    table[i] = Math.Max(table[i], table[j] + 1);
                }
            }
        
            for (int i = 0; i < length; i++)
                result = Math.Max(result, table[i]);
        
            return result;
        }
        */
    }
}