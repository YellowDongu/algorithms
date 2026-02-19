using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace algorithms
{
    internal class NumberOfCases
    {


        static void Main_createBoolCombi()
        {
            int n = 4; // 총 비트 수
            int k = 2; // 선택할 비트 수

            foreach (var combination in createBoolCombi(n, k))
            {
                Console.WriteLine(string.Join(", ", combination));
            }
        }

        static IEnumerable<int[]> createBoolCombi(int n, int k)
        {
            int max = 1 << n; // 2^n => 비트 자리수에 따른 최대값 도출
            for (int i = 0; i < max; i++)
            {
                int count = 0;
                int num = i;
                while (num > 0)
                {
                    count += (num & 1);
                    num >>= 1;
                }
                // 비트가 k개만큼 1인지 확인
                if (count == k)
                {
                    int[] result = new int[n];
                    for (int j = 0; j < n; j++)
                    {
                        result[n - 1 - j] = (i & (1 << j)) != 0 ? 1 : 0; // 1<<j로 해당 비트 자리까지 0을 채우고 숫자와 and연산해서 해당 자리에 1비트가 있는지 확인
                    }
                    yield return result; // yield가 있어서 메서드의 코드 전체를 실행시키고나서 값뭉탱이를 리턴함 => foreach로 빼서 쓰면 됨
                }
            }
        }
    }
}
