#pragma once
#include <cmath>
#include <vector>
#include <list>
#include <memory>
#include "Vector2.h"


// �� ��ǥ���� ��ä�� �����ؼ� ��ǥó�� ���� ����
// �� ��ǥ�� ���� �� �� �ִ��� ������ ��� ����
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

class Grid2D // ��ü ��ǥ ��
{
public:
	Grid2D(void) : w(0), h(0), startPos{0,0} {}
	~Grid2D(void) = default;

	void init(int _width, int _height, bool _defaultPassable = true);
	const int& width(void) const { return w; }
	const int& height(void) const { return h; }
	const Vector2Int& startPosition() const { return startPos; }
	NavNode* getNode(Vector2Int) const;

private:
	std::vector<std::unique_ptr<NavNode>> nodeList;

	Vector2Int startPos;
	int w, h;
};

NavNode* Grid2D::getNode(Vector2Int pos) const
{
	Vector2Int relativePos = pos - startPos;
	if (relativePos.x <= 0 || relativePos.x > w || relativePos.y <= 0 || relativePos.y > h)
		return nullptr;
	int index = relativePos.y * w + relativePos.x;

	return nodeList[index].get();
}

 void Grid2D::init(int _width, int _height, bool _defaultPassable)
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


// Vector2Int ��� => 2d���� ��ǥ�� �̿���, ��°� �ϰ��� ���̿� ���� �߰����� ���� ���ؼ� ����ϴ� ��� ���� �ڽ�Ʈ�� �ø��� �������� ���̸� �������� ���ϰ� �ϸ� ��
// �ö󰥶� �������� �Ѵٵ����� �ٸ����� �����Ͻñ�
// �밢�����ε� ���� �س����ϱ� �����ϼ�
class aStar
{
public:
	// �� ������ �ְ� ���۰� �� ��ǥ�� �ָ� �˾Ƽ� ��������
	static std::list<NavNode*> findPath(const Grid2D& map, Vector2Int startPoint, Vector2Int arrivalPoint);

private:
	struct aStarInfo
	{
		NavNode* node;
		float gValue, fValue; // h : cost to end(heristic value), g : cost to start, f : total cost(g+h)
		aStarInfo(NavNode* newNode, float _gValue, float _fValue)
			: node(newNode), gValue(_gValue), fValue(_fValue) {}
	};
	//  �ֺ��� ������ �����ؼ� ���� ���� ���� �� �� �� �� �ִ� ���� �����ٳ���
	static std::list<NavNode*> getNearNode(const Grid2D& map, const Vector2Int& currentPoint, std::list<NavNode*>& closedPath);
	// ������ �ֺ� ��� �� �������� �ִ��� ������ �� �ִ� ��� �ϳ��� ����
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

	// ��� ��� ��Ʈ��ŷ ���ϰ� ����� ������
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
			//if (abs(x) + abs(y) == 2) continue; //=> �밢�� ���� �������� �̰� Ȱ��ȭ

			Vector2Int targetPos(currentPoint.x + x, currentPoint.y + y);
			//if (targetPos.x <= 0 || targetPos.x > map.width() || targetPos.y <= 0 || targetPos.y > map.height()) continue; // => getNode���� ���ְ� ����

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

		//���⿡ ������fvalue < _hValue + _gValue�� continue;

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


