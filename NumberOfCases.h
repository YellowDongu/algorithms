#pragma once
#include <algorithm>
#include <vector>

// n개 항목을 k번 선택
std::vector<std::vector<int>> binaryCase(int size, int k) //비트마스크를 이용함
{
    std::vector<std::vector<int>> result;
    int max = 1 << size;

    for (int i = 0; i < max; ++i)
    {
        std::vector<int> bitCase(size,0);
        int count = 0;

        for (int j = 0; j < size; ++j)
        {
            if (i & (1 << j))
            {
                bitCase[j] = 1;
                ++count;
            }
        }

        if (count != k) { continue; } // 모든 경우의 수를 보이려면 이거 지우면 됨

        reverse(bitCase.begin(), bitCase.end());
        result.push_back(bitCase);
    }
    return result;
}


// n개 항목을 k번 중복선택
std::vector<std::vector<int>> numbericCase(int size, int k)
{
    std::vector<std::vector<int>> result;

    std::vector<int> combination(size, 0);
    while (true)
    {
        int i = size - 1;
        while (i >= 0 && combination[i] == k)
        {
            --i;
        }
        if (i < 0) { break; }

        for (int j = i + 1; j < size; ++j)
        {
            combination[j] = 0;
        }
        ++combination[i];


        int sum = 0;
        
        for (int j = 0; j < size; j++) // 모든 경우의 수를 보이려면 여기부분 수정
        {
            sum += combination[j];
        }
        if (sum == k)
        {
            result.push_back(combination);
        }
    }
    return result;
}


//이 아래는 채찍피티가 만들어준 참고용


// 배열의 항목을 중복 없이 k개 선택한다고 했을때 결과를 bool 배열로 나타냄 
int binaryCase()
{
    int size = 10; // 배열의 크기
    int k = 3;  // true로 설정할 개수

    // n개의 bool 배열을 초기화 (k개의 true와 (n-k)개의 false)
    std::vector<bool> combination(size, false);
    std::fill(combination.begin(), combination.begin() + k, true);

    // 모든 가능한 조합 생성
    do
    {
        for (bool val : combination)
        {
            std::cout << val << ' ';
        }
        std::cout << std::endl;
    }
    while (std::next_permutation(combination.begin(), combination.end()));
    // <algorithm> std::next_permutation
    //permutation :: 다른 n개의 원소에서 r개를 뽑아 한 줄로 세우는 경우의 수
    return 0;
}



// 0~n으로 구성된 숫자 조합에서 k개를 중복 선출
void numbericCaseRecursive(int size, int k, int start, std::vector<int>& combination)
{
    // 조합이 완성된 경우 출력
    if (k == 0)
    {
        for (int num : combination)
        {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
        return;
    }

    // 중복을 허용하며 선택을 반복
    for (int i = start; i <= size; ++i)
    {
        combination.push_back(i);
        numbericCaseRecursive(size, k - 1, i, combination); // 다음 선택
        combination.pop_back(); // 현재 선택을 취소
    }
}

int main_numbericCaseRecursive()
{
    int size = 3; // 숫자의 최대값
    int k = 2; // 선택할 숫자의 개수

    std::vector<int> combination;
    numbericCaseRecursive(size, k, 1, combination);

    return 0;
}



