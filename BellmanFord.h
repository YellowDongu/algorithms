#pragma once
#include <vector>


class BellmanFord
{
public:
	struct Line { int startIndex, endIndex, cost; };
public:
	BellmanFord(void) = delete;
	BellmanFord(std::vector<Line>& lines, int nodeLength) : lineList(lines), nodeCount(nodeLength) {}
	~BellmanFord(void) = default;

	bool operator() (int startIndex, std::vector<int>& output)
	{
		if (startIndex > nodeCount)
			return false;

		int repeatCount = nodeCount - 1;
		output.clear();
		output.resize(nodeCount + 1, std::numeric_limits<int>::max());
		output[startIndex] = 0;

		for (int i = 0; i < repeatCount; i++)
		{
			bool trigger = true;

			for (auto& line : lineList)
			{
				if (output[line.startIndex] == std::numeric_limits<int>::max())
					continue;

				int distance = output[line.startIndex] + line.cost;
				if (output[line.endIndex] > distance)
				{
					output[line.endIndex] = distance;
					trigger = false;
				}
			}

			if (trigger)
				break;
		}


		for (auto& line : lineList)
		{
			if (output[line.startIndex] == std::numeric_limits<int>::max())
				continue;

			int distance = output[line.startIndex] + line.cost;
			if (output[line.endIndex] > distance)
				return false;
		}


	}

private:
	std::vector<Line>& lineList;
	int nodeCount;
};
