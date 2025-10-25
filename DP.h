#pragma once
#include <vector>

int Solution(const std::vector<int>& list)
{
	int length = list[0], maxWeight = list[1];
	std::vector<std::vector<int>> table(length + 1, std::vector<int>(maxWeight + 1, 0));

	for (int i = 0; i < length; i++)
	{
		int weight = list[i * 2 + 2];
		int value = list[i * 2 + 3];

		for (int j = 0; j <= maxWeight; j++)
		{
			if (j < weight)
				table[i + 1][j] = table[i][j];
			else
				table[i + 1][j] = std::max(table[i][j], table[i][j - weight] + value);
		}
	}

	return table[length][maxWeight];
}
