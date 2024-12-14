#pragma once
#include <algorithm>
#include <vector>

// n�� �׸��� k�� ����
std::vector<std::vector<int>> binaryCase(int n, int k) //��Ʈ����ũ�� �̿���
{
    std::vector<std::vector<int>> result;
    int max = 1 << n;

    for (int i = 0; i < max; ++i)
    {
        std::vector<int> bitCase(n,0);
        int count = 0;

        for (int j = 0; j < n; ++j)
        {
            if (i & (1 << j))
            {
                bitCase[j] = 1;
                ++count;
            }
        }

        if (count != k) { continue; } // ��� ����� ���� ���̷��� �̰� ����� ��

        reverse(bitCase.begin(), bitCase.end());
        result.push_back(bitCase);
    }
    return result;
}


// n�� �׸��� k�� �ߺ�����
std::vector<std::vector<int>> numbericCase(int n, int k)
{
    std::vector<std::vector<int>> result;

    std::vector<int> combination(n, 0);
    while (true)
    {
        int i = n - 1;
        while (i >= 0 && combination[i] == k)
        {
            --i;
        }
        if (i < 0) { break; }

        for (int j = i + 1; j < n; ++j)
        {
            combination[j] = 0;
        }
        ++combination[i];


        int sum = 0;
        
        for (int j = 0; j < n; j++) // ��� ����� ���� ���̷��� ����κ� ����
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


//�� �Ʒ��� ä����Ƽ�� ������� ������


// �迭�� �׸��� �ߺ� ���� k�� �����Ѵٰ� ������ ����� bool �迭�� ��Ÿ�� 
int binaryCase()
{
    int n = 10; // �迭�� ũ��
    int k = 3;  // true�� ������ ����

    // n���� bool �迭�� �ʱ�ȭ (k���� true�� (n-k)���� false)
    std::vector<bool> combination(n, false);
    std::fill(combination.begin(), combination.begin() + k, true);

    // ��� ������ ���� ����
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
    //permutation :: �ٸ� n���� ���ҿ��� r���� �̾� �� �ٷ� ����� ����� ��
    return 0;
}



// 0~n���� ������ ���� ���տ��� k���� �ߺ� ����
void numbericCaseRecursive(int n, int k, int start, std::vector<int>& combination)
{
    // ������ �ϼ��� ��� ���
    if (k == 0)
    {
        for (int num : combination)
        {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
        return;
    }

    // �ߺ��� ����ϸ� ������ �ݺ�
    for (int i = start; i <= n; ++i)
    {
        combination.push_back(i);
        numbericCaseRecursive(n, k - 1, i, combination); // ���� ����
        combination.pop_back(); // ���� ������ ���
    }
}

int main_numbericCaseRecursive()
{
    int n = 3; // ������ �ִ밪
    int k = 2; // ������ ������ ����

    std::vector<int> combination;
    numbericCaseRecursive(n, k, 1, combination);

    return 0;
}


