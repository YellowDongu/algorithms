#pragma once
#include <string>
#include <unordered_set>
#include <vector>

struct Palindrome
{
	std::string printPalindromicSubstrings(const std::string& word)
	{
		int length = static_cast<int>(word.size());
		std::vector<std::vector<bool>> table(length + 1, std::vector<bool>(length + 1, false));
		std::unordered_set<std::string> set;
		std::string result;

		for (int i = 0; i < length; i++)
		{
			table[i][i] = true;
			set.insert(word.substr(i, 1));
		}

		for (int i = 0; i < length - 1; i++)
		{
			if (word[i] == word[i + 1])
			{
				table[i][i + 1] = true;
				set.insert(word.substr(i, 2));
			}

		}

		for (int k = 2; k < length; k++)
		{
			for (int i = 0; i < length - k; i++)
			{
				int j = i + k;
				if (word[i] == word[j] && table[i + 1][j - 1])
				{
					table[i][j] = true;
					set.insert(word.substr(i, k + 1));
				}

			}
		}

		for (auto& item : set)
			result += item;

		return result;
	}
};
