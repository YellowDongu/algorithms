#pragma once
#include "Vector2.h"
#include <vector>
#include <list>
#include <memory>

class Node2D
{
public:
	Node2D() : worldPos(Vector2::zero()), gridPos(Vector2Int::zero()), nodeSize(0) {}
	Node2D(int x, int y, float _nodeSize) : worldPos(x* _nodeSize + (_nodeSize / 2), y* _nodeSize + (_nodeSize / 2)), gridPos(x, y), nodeSize(_nodeSize) {}

	Vector2 position() const { return worldPos; }
	Vector2Int coordination() const { return gridPos; }

private:
	Vector2 worldPos;
	Vector2Int gridPos;

	float nodeSize;
};


class QuadNode
{
public:
	QuadNode() : children{}, nodes{}, depth(0) {}


	std::vector<std::unique_ptr<QuadNode>> children;
	std::vector<Node2D*> nodes;
	int depth;
};


class QuadTree
{
public:
	QuadTree() : root(nullptr) {}
	~QuadTree() {}


	void createAsc(const std::vector<Node2D*>& nodeMap, int mapWidth, int mapheight);
	void createDec(const std::vector<Node2D*>& nodes, int mapWidth, int mapheight);
	//void createQuadTreeAsc(GridMap map);

private:
	std::unique_ptr<QuadNode> root;
};


// 상향식 쿼드트리 만들기
 void QuadTree::createAsc(const std::vector<Node2D*>& nodeMap, int mapWidth, int mapheight)
{
	int currentWidth = mapWidth / 2;
	int currentHeight = mapheight / 2;
	int currentDepth = 0;

	std::vector<std::unique_ptr<QuadNode>> QuadMap;
	for (int y = 0; y < currentHeight; y++)
	{
		for (int x = 0; x < currentWidth; x++)
		{
			QuadNode* node = new QuadNode();
			node->nodes.push_back(nodeMap[((y * 2) * mapWidth) + (x * 2)]);
			if ((x != currentWidth - 1 || mapWidth % 2 == 0) && currentWidth != 1)
			{
				node->nodes.push_back(nodeMap[((y * 2) * mapWidth) + (x * 2 + 1)]);
			}
			if ((y != currentHeight - 1 || mapheight % 2 == 0) && currentHeight != 1)
			{
				node->nodes.push_back(nodeMap[((y * 2 + 1) * mapWidth) + (x * 2)]);
				if ((x != currentWidth - 1 || mapWidth % 2 == 0) && currentWidth != 1)
				{
					node->nodes.push_back(nodeMap[((y * 2 + 1) * mapWidth) + (x * 2 + 1)]);
				}
			}
			node->depth = currentDepth;
			QuadMap.push_back(std::make_unique<QuadNode>(node));
		}
	}

	int preWidth = currentWidth;
	int preHeight = currentHeight;
	currentDepth++;

	while (currentWidth != 1 && currentHeight != 1)
	{
		std::vector< std::unique_ptr<QuadNode>> tempQuadMap;
		currentWidth = preWidth;
		currentHeight = preHeight;
		if (preWidth != 1) { currentWidth /= 2; }
		if (preHeight != 1) { currentHeight /= 2; }

		for (int y = 0; y < currentHeight; y++)
		{
			for (int x = 0; x < currentWidth; x++)
			{
				QuadNode* node = new QuadNode();

				QuadNode* childnode1 = QuadMap[((y * 2) * preWidth) + (x * 2)].get();
				node->children.push_back(std::make_unique<QuadNode>(childnode1));
				for (int i = 0; i < childnode1->children.size(); i++)
				{
					node->children.push_back(childnode1->children[i]);
				}

				if ((x != currentWidth - 1 || preWidth % 2 == 0) && currentWidth != 1)
				{
					QuadNode* childnode2 = QuadMap[((y * 2) * preWidth) + (x * 2 + 1)].get();
					node->children.push_back(std::make_unique<QuadNode>(childnode2));
					for (int i = 0; i < childnode2->children.size(); i++)
					{
						node->children.push_back(childnode2->children[i]);
					}
				}
				if ((y != currentHeight - 1 || preWidth % 2 == 0) && currentHeight != 1)
				{
					QuadNode* childnode3 = QuadMap[((y * 2 + 1) * preWidth) + (x * 2)].get();
					node->children.push_back(std::make_unique<QuadNode>(childnode3));
					for (int i = 0; i < childnode3->children.size(); i++)
					{
						node->children.push_back(childnode3->children[i]);
					}

					if ((x != currentWidth - 1 || preWidth % 2 == 0) && currentWidth != 1)
					{
						QuadNode* childnode4 = QuadMap[((y * 2 + 1) * preWidth) + (x * 2 + 1)].get();
						node->children.push_back(std::make_unique<QuadNode>(childnode4));
						for (int i = 0; i < childnode4->children.size(); i++)
						{
							node->children.push_back(childnode4->children[i]);
						}
					}
				}
				node->depth = currentDepth;
				tempQuadMap.push_back(std::make_unique<QuadNode>(node));
			}
		}

		preWidth = currentWidth;
		preHeight = currentHeight;
		currentDepth++;
		QuadMap.swap(tempQuadMap);
	}
	root = std::move(QuadMap[0]);

	currentDepth--;
	std::list<QuadNode*> queue = { root.get() };
	while (!queue.empty())
	{
		QuadNode* node = queue.front();
		if (!node->children.empty())
		{
			for (int i = 0; i < node->children.size(); i++)
			{
				queue.push_back(node->children[i].get());
			}
		}
		node->depth -= currentDepth;
		queue.pop_front();
	}
}

// 하향식 쿼드트리 만들기
void QuadTree::createDec(const std::vector<Node2D*>& nodes, int mapWidth, int mapheight)
{

	root = std::unique_ptr<QuadNode>(new QuadNode());
	root->depth = 0;
	std::list<QuadNode*> nodeQueue{ root.get() };

	while (!nodeQueue.empty())
	{
		QuadNode* currentNode = nodeQueue.front();
		int halfX = mapWidth;
		int halfY = mapheight;
		for (int i = 0; i < currentNode->depth + 1; i++)
		{
			halfX /= 2;
			halfY /= 2;
		}

		if (currentNode->children.size() <= 4)
		{
			nodeQueue.pop_front();
			continue;
		}

		// 자식 노드 만들기
		std::vector<Node2D*> childNodes[4];

		for (Node2D* gridNode : nodes)
		{
			if (gridNode->coordination().x < halfX && gridNode->coordination().y < halfY)
				childNodes[0].push_back(gridNode);
			else if (gridNode->coordination().x >= halfX && gridNode->coordination().y < halfY)
				childNodes[1].push_back(gridNode);
			else if (gridNode->coordination().x < halfX && gridNode->coordination().y >= halfY)
				childNodes[2].push_back(gridNode);
			else
				childNodes[3].push_back(gridNode);
		}

		for (int i = 0; i < 4; ++i)
		{
			if (!childNodes[i].empty())
			{
				std::unique_ptr<QuadNode> child = std::unique_ptr<QuadNode>(new QuadNode());
				for (Node2D* gridNode : childNodes[i])
				{
					child->nodes.push_back(gridNode);
				}
				child->depth = currentNode->depth + 1;
				currentNode->children.push_back(child);
				nodeQueue.push_back(child.get());
			}
		}
		nodeQueue.pop_front();
	}
}

