#pragma once
#include <stack> //dfs => vector나 list로 대체 가능
#include <queue> //bfs => vector나 list로 대체 가능
#include <vector>


class GraphSearch
{
public:
	static class Node
	{
	public:
		Node(void) {}
		~Node(void) {}

		std::vector<Node*> getNearNodes() const { return nearNodes; } // 전체 연결된 노드 반환
	private:
		std::vector<Node*> nearNodes; // 연결된 노드들, 벡터로 안묶고 따로 만들어도 된다.
		int value; // 노드가 가지고 있어야 하는 값 추가하면 됨
	};

	static void dfsFind(Node& startNode);
	static void bfsFind(Node& startNode);
private:
	static void process(Node& node) {} // 여기다 기능 구현하면 됨
};

void GraphSearch::dfsFind(Node& startNode)
{
	Node* currentNode = &startNode;
	std::stack<Node*> searchStack = {};
	searchStack.push(currentNode);

	while (!searchStack.empty())
	{
		for (int i = 0; i < currentNode->getNearNodes().size(); i++)
		{
			searchStack.push(currentNode->getNearNodes()[i]);
		}

		currentNode = searchStack.top();
		searchStack.pop();
		process(*currentNode);
	}
}

void GraphSearch::bfsFind(Node& startNode)
{
	Node* currentNode = &startNode;
	std::queue<Node*> searchQueue = {};

	while (!searchQueue.empty())
	{
		for (int i = 0; i < currentNode->getNearNodes().size(); i++)
		{
			searchQueue.push(currentNode->getNearNodes()[i]);
		}

		currentNode = searchQueue.front();
		searchQueue.pop();
		process(*currentNode);
	}

}



