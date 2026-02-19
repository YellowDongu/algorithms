#pragma once

#include <vector>

struct FloydWarshall
{
	enum class Condition
	{
		Lose = -1,
		Win = 1,
		Unknown = 0
	};
	struct InsertGuide
	{
		int from, to;
		Condition condition;
	};
	FloydWarshall(void) = delete;
	FloydWarshall(int nodeCount) : table(nodeCount, std::vector<Condition>(nodeCount, Condition::Unknown)) { }

	std::vector<std::vector<Condition>> table;

	void CompleteTable(void)
	{
		int length = static_cast<int>(table.size());

		for (int k = 0; k < length; k++) // 중간 대상
		{
			for (int i = 0; i < length; i++) // 출발 대상
			{
				if (k == i)
					continue;

				for (int j = 0; j < length; j++) // 도착 대상
				{
					if (i == j || j == k)
						continue;

					if (table[i][k] == Condition::Win && table[k][j] == Condition::Win) // 출발 > 중간 > 도착
					{
						table[j][i] = Condition::Win;
						table[i][j] = Condition::Lose;
					}
					else if (table[i][k] == Condition::Lose && table[k][j] == Condition::Lose)
					{
						table[i][j] = Condition::Lose;
						table[j][i] = Condition::Win;
					}
				}
			}
		}
	}

	inline void CreateLink(int from, Condition condition, int to)
	{
		table[from][to] = condition;
		table[to][from] = static_cast<Condition>(static_cast<int>(condition) * -1);
	}
	
	inline void CreateLink(const std::vector<InsertGuide>& list)
	{
		for (auto& item : list)
			CreateLink(item.from, item.condition, item.to);
	}


};