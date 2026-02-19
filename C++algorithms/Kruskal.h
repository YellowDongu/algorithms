#pragma once
#include <vector>
#include <algorithm>


struct Kruskal
{
	struct line // std::pair같은 애들로 대체 가능
	{
		int from, to;
		int cost;
	};

	struct UnionFind
	{
		UnionFind(void) = delete;
		~UnionFind(void) = default;
		UnionFind(int nodeSize) : size(nodeSize)
		{
			ranks.resize(size, 0);
			parents.resize(size);
			for (int i = 0; i < size; i++)
				parents[i] = i;
		}

		int size;
		std::vector<int> parents;
		std::vector<int> ranks;

		int FindRoot(int target, bool compressRoute = false)
		{
			int currentNode = target;
			while (parents[currentNode] != currentNode)
				currentNode = parents[currentNode];

			if (!compressRoute)
				return currentNode;

			while (target != currentNode)
			{
				int next = parents[target];
				parents[target] = currentNode;
				target = next;
			}
			return currentNode;
		}

		bool Unite(int x, int y)
		{
			x = FindRoot(x, true);
			y = FindRoot(y, true);
			if (x == y) return false; // 이미 같은 집단이라 연산 필요 없다고 알려주기 위해 bool값 리턴함

			if (ranks[x] < ranks[y])
				parents[x] = y;
			else
			{
				parents[y] = x;
				if (ranks[x] == ranks[y])
					ranks[y]++;
			}
			return true;
		}
	};

	Kruskal(void) = delete;
	~Kruskal(void) = default;
	Kruskal(int nodeCount, const std::vector<line>& lineList) : nodeSize(nodeCount), lines(lineList), unionFind(UnionFind(nodeCount))
	{
		std::sort(lineList.begin(), lineList.end(), [](const line& left, const line& right) { return left.cost < right.cost; });
		minimumRoutePayMent.resize(nodeSize, std::vector<int>());
		
		for (auto& line : lines) // 가장 짧은 간선부터 선택할 수 있으면 선택
		{
			if (unionFind.Unite(line.from, line.to)) // 이미 선택된 간선이 있는 노드들이면 스킵
			{
				mst.push_back(line);
				mstCost += line.cost;
			}
		}
	}

	int nodeSize;
	std::vector<line> lines;
	std::vector<std::vector<int>> minimumRoutePayMent;
	UnionFind unionFind;

	int mstCost{ 0 };
	std::vector<line> mst;
};
