#pragma once
#include <string>
#include <vector>

struct LongestCommonSubsequence
{
	std::string sentence1, sentence2;
	std::string lcs;
	int LCSLength;

	LongestCommonSubsequence(const std::string& one, const std::string& two) : sentence1(one), sentence2(two) { CreateLCS(); }

private:
	//같은 문자일 때 그 자리부터 뒷자리까지 기존 수+1을 해주고 아닐 경우는 기존의 숫자들 중 제일 큰 숫자로 가져가서 같은 문자였던 애들을 표시해주는거
	// 백트래킹으로 제일 마지막으로 같았던 부분을 추적해서 진행하는거라 공통부분이 여러개 있어도 문자열에서 뒷쪽에 있는 공통부분만 선택받dma
	//https://mojing.tistory.com/entry/Algorithm-Longest-Common-SubsequenceLCS-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-%EB%91%90-%EB%AC%B8%EC%9E%90%EC%97%B4-%EA%B0%84-%EC%B5%9C%EC%9E%A5-%EA%B3%B5%ED%86%B5-%EB%B6%80%EB%B6%84-%EC%88%98%EC%97%B4-%EC%B0%BE%EA%B8%B0
	void CreateLCS(void) { CreateLCS(sentence1, sentence2); }
	void CreateLCS(const std::string& sentence1, const std::string& sentence2)
	{
		int length1 = static_cast<int>(sentence1.length()), length2 = static_cast<int>(sentence2.length());
		std::vector<std::vector<int>> table(length1 + 1, std::vector<int>(length2 + 1, 0));
		int i, j;

		for (i = 1; i <= length1; i++)
		{
			for (j = 1; j <= length2; j++)
			{
				if (sentence1[i - 1] == sentence2[j - 1])
					table[i][j] = table[i - 1][j - 1] + 1;
				else
					table[i][j] = std::max(table[i - 1][j], table[i][j - 1]);
			}
		}

		i--;
		j--;

		LCSLength = table[i][j]; // lcs length

		while (i > 0 && j > 0)
		{
			if (sentence1[i - 1] == sentence2[j - 1])
			{
				lcs = sentence1[i - 1] + lcs;
				i--;
				j--;
			}
			else if (table[i - 1][j] >= table[i][j - 1])
				j--;
			else
				i--;
		}
	}


}typedef LCS;


struct LongestIncreasingSubsequence
{
	std::vector<int> arr;

	int Find(void)
	{
		int n = arr.size(), result = 0;
		std::vector<int> dp(n, 1);

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < i; ++j)
			{
				if (arr[j] < arr[i])
					dp[i] = std::max(dp[i], dp[j] + 1);
			}
		}

		for (int i = 0; i < n; ++i)
			result = std::max(result, dp[i]);

		return result;
	}

	int Find_Binary()
	{
		std::vector<int> lis;

		for (int x : arr)
		{
			auto it = lower_bound(lis.begin(), lis.end(), x);
			if (it == lis.end())
				lis.push_back(x);
			else
				*it = x; // 마지막 값보다 더 낮고 그 다음 값보다 높은 경우를 대비하는거임 [10, 20, 30, 20, 25, 27, 45] 이런 경우에서 30이 마지막 값이 되는데 그 이후에 25가 나옴, 30을 25로 갱신해주지 않으면 27이 들어올 수 없음
		}
	}


}typedef LIS;