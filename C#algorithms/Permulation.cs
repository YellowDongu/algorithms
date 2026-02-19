using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    class Permulation
    {
        public Permulation() { }

        static IEnumerable<List<T>> GetPermutations<T>(List<T> items)
        {
            int n = items.Count;
            int[] c = new int[n];
            yield return new List<T>(items);

            int i = 0;
            while (i < n)
            {
                if (c[i] < i)
                {
                    if (i % 2 == 0)
                    {
                        (items[0], items[i]) = (items[i], items[0]);
                    }
                    else
                    {
                        (items[c[i]], items[i]) = (items[i], items[c[i]]);
                    }

                    yield return new List<T>(items);
                    c[i] += 1;
                    i = 0;
                }
                else
                {
                    c[i] = 0;
                    i++;
                }
            }
        }


        static bool NextPermutation<T>(List<T> arr) where T : IComparable<T>
        {
            int i = arr.Count - 2;
            while (i >= 0 && arr[i].CompareTo(arr[i + 1]) >= 0)
                i--;

            if (i < 0)
                return false;

            int j = arr.Count - 1;

            while (arr[j].CompareTo(arr[i]) <= 0)
                j--;

            (arr[i], arr[j]) = (arr[j], arr[i]);

            arr.Reverse(i + 1, arr.Count - (i + 1));

            return true;
        }


    }
}
