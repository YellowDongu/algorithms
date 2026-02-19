#pragma once
#include <cmath>
#include <vector>
#include <list>
#include <memory>
#include <queue>
#include "Vector2.h"


struct BellmanFord
{
	std::vector<std::vector<std::pair<int, int>>> graph;

	void BaseMethod(int startIndex, std::vector<int>& result)
	{
		int length = static_cast<int>(graph.size());
		result.resize(length, std::numeric_limits<int>::max());
		result[startIndex] = 0;

		for (int i = 0; i < length - 1; ++i)
		{
			for (int j = 0; j < length; ++j)
			{
				if (result[j] == std::numeric_limits<int>::max())
					continue;

				for (auto& item : graph[j])
					result[item.first] = std::min(result[item.first], result[j] + item.second);
			}
		}
	}

	void SPFA(int startIndex, std::vector<int>& result)
	{
		int length = static_cast<int>(graph.size());
		std::list<int> queue{ startIndex };
		std::vector<bool> inQueue(length, false);

		result.resize(length, std::numeric_limits<int>::max());
		result[startIndex] = 0;

		while (!queue.empty())
		{
			int index = queue.front();
			queue.pop_front();

			inQueue[index] = false;

			for (auto& item : graph[index])
			{
				int nextCost = result[index] + item.second;
				if (result[item.first] <= nextCost)
					continue;

				result[item.first] = nextCost;

				if (!inQueue[item.first])
				{
					queue.push_back(item.first);
					inQueue[item.first] = true;
				}
			}
		}
	}

};





struct Dijkstra
{
	Dijkstra(void) : size(0) {}
	Dijkstra(int resize) : graph(resize), size(resize), costTable(resize) {}

	int Size(void) { return size; }
	const std::vector<std::vector<std::pair<int, int>>>& Graph(void) const { return graph; }
	const std::vector<std::vector<int>>& CostTable(void) const { return costTable; }

	int size{ 0 };
	std::vector<std::vector<std::pair<int, int>>> graph;
	std::vector<std::vector<int>> costTable;

	void PushLine(int from, int to, int cost)
	{
		int max = std::max(from, to);
		if (size <= max)
			graph.resize(max + 1);

		graph[from].push_back({ to, cost });
	}

	void CreateCostTable(int startNode, std::vector<int>& output)
	{
		output.resize(size);
		for (auto& item : output)
			item = std::numeric_limits<int>::max();
		output[startNode] = 0;

		std::priority_queue<std::pair<int, int>> queue;
		queue.push({ 0, startNode });
		//std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<int>> queue;

		while (!queue.empty())
		{
			int current = queue.top().second;
			int cost = queue.top().first;
			queue.pop();

			if (output[current] < cost)
				continue;

			for (auto& node : graph[current])
			{
				int nextCost = cost + node.second;
				if (nextCost >= output[node.first])
					continue;
				
				output[node.first] = nextCost;
				queue.push({nextCost, node.first});
			}
		}
	}

	void CalculateAll(void)
	{
		costTable.resize(size);
		for (int i = 0; i < size; i++)
			CreateCostTable(i, costTable[i]);
	}
};



// 각 좌표들을 객채로 생성해서 부표처럼 쓰고 있음
// 이 좌표가 현재 갈 수 있는지 같은걸 담고 있음
class NavNode
{
public:
	NavNode(void) = default;
	NavNode(Vector2Int pos, bool _passable = true) : pos(pos) , passable(_passable) {}
	~NavNode(void) = default;

	const Vector2Int& position(void) const { return pos; }
	const bool& canEnter(void) const { return passable; }

private:
	Vector2Int pos;
	bool passable;
};

class Grid2D // 전체 좌표 맵
{
public:
	Grid2D(void) : w(0), h(0), startPos{0,0} {}
	~Grid2D(void) = default;

	const int& width(void) const { return w; }
	const int& height(void) const { return h; }
	const Vector2Int& startPosition() const { return startPos; }
	void init(int _width, int _height, bool _defaultPassable = true)
	{
		w = _width;
		h = _height;
		if (!nodeList.empty()) nodeList.clear();

		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				Vector2Int nodePos = Vector2Int{ x,y } + startPos;
				nodeList.push_back(std::make_unique<NavNode>(nodePos, _defaultPassable));
			}
		}
	}

	NavNode* getNode(Vector2Int pos) const
	{
		Vector2Int relativePos = pos - startPos;
		if (relativePos.x <= 0 || relativePos.x > w || relativePos.y <= 0 || relativePos.y > h)
			return nullptr;
		int index = relativePos.y * w + relativePos.x;

		return nodeList[index].get();
	}

private:
	std::vector<std::unique_ptr<NavNode>> nodeList;

	Vector2Int startPos;
	int w, h;
};

// Vector2Int 사용 => 2d상의 좌표를 이용함, 상승과 하강은 높이에 따른 추가적인 값을 더해서 상승하는 길로 가는 코스트를 늘리는 형식으로 높이를 무시하지 못하게 하면 됨
// 올라갈때 느려지게 한다든지는 다른데서 구현하시구
// 대각선으로도 가게 해놨으니깐 참고하셈
class aStar
{
public:
	// 맵 정보를 주고 시작과 끝 좌표를 주면 알아서 연산해줌
	static std::list<NavNode*> findPath(const Grid2D& map, Vector2Int startPoint, Vector2Int arrivalPoint);

private:
	struct aStarInfo
	{
		NavNode* node;
		float gValue, fValue; // h : cost to end(heristic value), g : cost to start, f : total cost(g+h)
		aStarInfo(NavNode* newNode, float _gValue, float _fValue)
			: node(newNode), gValue(_gValue), fValue(_fValue) {}
	};
	//  주변의 노드들을 수집해서 아직 가지 않은 곳 중 갈 수 있는 곳만 가져다놓음
	static std::list<NavNode*> getNearNode(const Grid2D& map, const Vector2Int& currentPoint, std::list<NavNode*>& closedPath);
	// 수집된 주변 노드 중 목적지에 최대한 근접할 수 있는 노드 하나를 뽑음
	static NavNode* calculateNode(std::list<NavNode*>& closedPath, std::list<NavNode*>& nearNodes, const Vector2Int& arrivalPoint, aStarInfo& _PreviousNode);

	// TODO
	//static float calculateHuristicValue();
};





std::list<NavNode*> aStar::findPath(const Grid2D& map, Vector2Int startPoint, Vector2Int arrivalPoint)
{
	std::list<NavNode*> path;
	std::list<NavNode*> closedPath;

	NavNode* currentNode = map.getNode(startPoint);
	if (!currentNode)
		return path;
	NavNode* startNode = currentNode;
	aStarInfo previousNode = { currentNode, 0.0f, 0.0f };
	path.push_back(currentNode);
	closedPath.push_back(currentNode);


	while (path.back()->position() != arrivalPoint)
	{
		// Get near Nodes
		std::list<NavNode*> openPath = getNearNode(map, currentNode->position(), closedPath);
		if (openPath.empty()) // If cannot find path
		{
			path.clear();

			// Return to first node and initiate
			currentNode = startNode;
			previousNode = { currentNode, 0.0f, 0.0f };
			path.push_back(currentNode);

			// Restart at first
			std::list<NavNode*> tempPath = getNearNode(map, currentNode->position(), closedPath);
			if (tempPath.empty()) // No path found(Already find all route)
			{
				path.clear();
				return path;
			}
		}

		// Get which node has lowest fCost&gCost
		currentNode = calculateNode(closedPath, openPath, arrivalPoint, previousNode);
		path.push_front(currentNode);
		closedPath.push_back(currentNode);
	}
	return path;

	// 경로 없어도 백트래킹 안하고 결과만 뱉어버림
	//std::list<NavNode*> path;
	//std::list<NavNode*> closedPath;
	//
	//NavNode* currentNode = map.getNode(startPoint);
	//aStarInfo previousNode{ currentNode, 0.0f, 0.0f };
	//path.push_back(currentNode);
	//
	//while (path.back()->position() != arrivalPoint)
	//{
	//	// Get near Nodes
	//	std::list<NavNode*> openPath = getNearNode(map, currentNode->position(), closedPath);
	//	if (openPath.empty()) // If cannot find path
	//	{
	//		path.clear();
	//		return path;
	//	}
	//	// Get which node has lowest fCost&gCost
	//	currentNode = calculateNode(closedPath, openPath, arrivalPoint, previousNode);
	//	path.push_front(currentNode);
	//}
	//
	//return path;
}

std::list<NavNode*> aStar::getNearNode(const Grid2D& map, const Vector2Int& currentPoint, std::list<NavNode*>& closedPath) // Get near Nodes
{
	std::list<NavNode*> nearNodes;
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			if (x == 0 && y == 0) continue;
			//if (abs(x) + abs(y) == 2) continue; //=> 대각선 진행 막을려면 이걸 활성화

			Vector2Int targetPos(currentPoint.x + x, currentPoint.y + y);
			//if (targetPos.x <= 0 || targetPos.x > map.width() || targetPos.y <= 0 || targetPos.y > map.height()) continue; // => getNode에서 해주고 있음

			NavNode* node = map.getNode(targetPos);
			if (!node) continue;

			bool isClosedNode = false;
			for (const NavNode* closedNode : closedPath)
			{
				if (closedNode == node)
				{
					isClosedNode = true;
					break;
				}
			}
			if (isClosedNode) continue;

			if (!node->canEnter())
				closedPath.push_back(node);
			else
				nearNodes.push_back(node);
		}
	}
	return nearNodes;
}

NavNode* aStar::calculateNode(std::list<NavNode*>& closedPath, std::list<NavNode*>& openPath, const Vector2Int& arrivalPoint, aStarInfo& _PreviousNode)
{
	aStarInfo selected = { nullptr, FLT_MAX, FLT_MAX };

	for (NavNode* node : openPath)
	{
		float _hValue = Vector2Int::distance(node->position(), arrivalPoint);
		float _gValue = Vector2Int::distance(_PreviousNode.node->position(), node->position()) + _PreviousNode.gValue;

		//여기에 이전의fvalue < _hValue + _gValue면 continue;

		if (selected.fValue > _hValue + _gValue)
		{
			if (!selected.node)
				closedPath.push_back(selected.node);
			
			selected = { node, _gValue, _hValue + _gValue };
		}
		else if (selected.fValue == _hValue + _gValue && selected.gValue < _gValue)
		{
			if (!selected.node)
				closedPath.push_back(selected.node);

			selected = { node , _gValue,_hValue + _gValue };
		}
		else
		{
			closedPath.push_back(node);
		}
	}
	_PreviousNode = selected;
	return selected.node;
}


