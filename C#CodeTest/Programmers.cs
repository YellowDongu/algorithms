using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    public class Solution
    {
        struct queueNode { public int index, weight; }
        public int solution(int[,] land, int height)
        {
            int boardWidth = land.GetLength(0), boardHeight = land.GetLength(1);
            int[,] visitable = new int[boardWidth, boardHeight];
            int[,] movement = new int[4, 2] { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
            List<List<int>> graph = new List<List<int>>();
            Queue<(int, int)> queue = new Queue<(int, int)>();

            int step = 1;
            for (int i = 0; i < boardHeight; i++)
            {
                for (int j = 0; j < boardWidth; j++)
                {
                    if (visitable[j, i] != 0)
                        continue;

                    visitable[j, i] = step;
                    queue.Enqueue((j, i));
                    List<int> newList = new List<int>(graph.Count) { int.MaxValue };
                    foreach (var item in graph)
                    {
                        item.Add(int.MaxValue);
                        newList.Add(int.MaxValue);
                    }
                    graph.Add(newList);

                    while (queue.Count > 0)
                    {
                        (int currentX, int currentY) = queue.Dequeue();
                        int currentHeight = land[currentX, currentY];
                        for (int k = 0; k < 4; k++)
                        {
                            int x = currentX + movement[k, 0], y = currentY + movement[k, 1];

                            if (x < 0 || y < 0 || x >= boardWidth || y >= boardHeight)
                                continue;

                            int difference = Math.Abs(land[x, y] - currentHeight);
                            if (visitable[x, y] == 0)
                            {
                                if (difference > height)
                                    continue;
                                visitable[x, y] = step;
                                queue.Enqueue((x, y));
                            }
                            else if (visitable[x, y] != step)
                            {
                                if (graph[step - 1][visitable[x, y] - 1] < difference)
                                    continue;

                                graph[step - 1][visitable[x, y] - 1] = graph[visitable[x, y] - 1][step - 1] = difference;
                                continue;
                            }
                        }
                    }
                    step++;
                }

            }

            int answer = 0;
            var Compare = Comparer<queueNode>.Create((queueNode left, queueNode right) => { if (left.weight == right.weight) return left.index - right.index; return left.weight - right.weight; });
            SortedSet<queueNode> priorityQueue = new SortedSet<queueNode>(Compare);
            List<bool> mstVisitable = new List<bool>(graph.Count) { false };

            for (int i = 1; i < graph.Count; i++)
                mstVisitable.Add(false);

            priorityQueue.Add(new queueNode() { index = 0, weight = 0 });

            while (priorityQueue.Count > 0)
            {
                queueNode node = priorityQueue.Min;
                priorityQueue.Remove(node);

                if (mstVisitable[node.index])
                    continue;

                mstVisitable[node.index] = true;
                answer += node.weight;

                for (int i = 0; i < graph.Count; i++)
                {
                    if (mstVisitable[i] || graph[node.index][i] == int.MaxValue)
                        continue;

                    priorityQueue.Add(new queueNode() { index = i, weight = graph[node.index][i] });
                }
            }
            return answer;
        }
    }




    internal class Programmers
    {
        public static int solution(int n)
        {
            int answer = 0;

            while (n >= 1)
            {
                answer += (n % 2);
                n = n / 2;
            }

            return answer;
        }
        static public int solution(int[] priorities, int location)
        {
            int answer = 1;
            ValueTuple<int, bool> currentValue;
            LinkedList<ValueTuple<int, bool>> queue = new LinkedList<ValueTuple<int, bool>>();
            SortedDictionary<int, int> priority = new SortedDictionary<int, int>();

            for (int i = 0; i < priorities.Length; i++)
            {
                if (i == location)
                    queue.AddLast((priorities[i], true));
                else
                    queue.AddLast((priorities[i], false));

                if (priority.TryGetValue(priorities[i], out int storage))
                    priority[priorities[i]] = storage + 1;
                else
                    priority[priorities[i]] = 1;
            }

            while (queue.First != null)
            {
                currentValue = queue.First.Value;
                if (currentValue.Item1 < priority.Keys.Max())
                {
                    queue.AddLast(currentValue);
                    queue.RemoveFirst();
                    continue;
                }

                if (queue.First.Value.Item2)
                    break;

                int count = priority[priority.Keys.Max()] - 1;
                answer++;
                if (count == 0)
                    priority.Remove(priority.Keys.Max());
                else
                    priority[priority.Keys.Max()] = count;

                queue.RemoveFirst();
            }


            return answer;
        }

        static public int[] solution(int[] prices)
        {
            int[] answer = Enumerable.Repeat(0, prices.Length).ToArray();
            Queue<ValueTuple<int, int>> queue = new Queue<ValueTuple<int, int>>();
            int length;
            ValueTuple<int, int> currentValue;
            for (int i = 0; i < prices.Length; i++)
            {
                length = queue.Count;
                for (int j = 0; j < length; j++)
                {
                    currentValue = queue.Dequeue();
                    currentValue.Item2++;

                    if (prices[currentValue.Item1] > prices[i])
                        answer[currentValue.Item1] = currentValue.Item2;
                    else
                        queue.Enqueue(currentValue);
                }

                queue.Enqueue(new ValueTuple<int, int>(i, 0));
            }

            foreach (var item in queue)
                answer[item.Item1] = item.Item2;

            return answer;
        }


        static public int solution1(int[] numbers)
        {
            Array.Sort(numbers);
            return numbers[numbers.Count() - 1] * numbers[numbers.Count() - 2];
        }


        public int[] solution1(int[] arr, int divisor)
        {
            List<int> answer = new List<int>();

            foreach (var item in arr)
            {
                if (item % divisor == 0)
                    answer.Add(item);
            }
            if (answer.Count == 0)
                answer.Add(-1);
            else
                answer.Sort();
            return answer.ToArray();
        }

        public int solution1(int n)
        {
            return n.ToString().Select(x => x - '0').Sum(); // 0N의 각 자릿수의 합을 구해줌, 왤케 간단함ㅋㅋ
            //return Enumerable.Range(1, n).ToList().FirstOrDefault(x => n % x == 1); // 0~n 사이에 x에 대해 n % x == 1이 되는 것을 찾는 구문, 왤케 간단함ㅋㅋ
        }


        public string solution(string phone_number)
        {
            int position = phone_number.Length - 5;
            return new string(phone_number.Select((character, index) => (index > position) ? character : '*').ToArray());
        }

        public int[] solution(long n)
        {
            return n.ToString().Select(x => x - '0').Reverse().ToArray();
        }
        public int solution(int[] a, int[] b)
        {
            int result = 0, length = a.Length;
            for (int i = 0; i < length; i++)
                result += a[i] * b[i];
            return result;
        }

        public int[] solution(int[] array, int[,] commands)
        {
            int length = array.Length;
            List<int> answer = new List<int>(length);
            length = commands.GetLength(0);
            for (int i = 0; i < length; i++)
            {
                int front = commands[i, 0] - 1, back = commands[i, 1] - 1;
                List<int> splited = new ArraySegment<int>(array, front, back - front + 1).ToList();
                splited.Sort();

                answer.Add(splited[commands[i, 2] - 1]);
            }

            return answer.ToArray();
        }

        public string solution2(int[] numbers)
        {
            List<string> numberConverted = numbers.Select(x => x.ToString()).ToList();

            numberConverted.Sort((string left, string right) => string.Compare(left + right, right + left) * -1);
            string answer = string.Join(null, numberConverted);
            if (answer[0] == '0')
                return "0";
            return answer;
        }

        /* 이거 가져다 붙일거
using System.Numerics;
using System.Linq;
using System.Collections.Generic;
        */
    }
}
