#pragma once
#include <stack> //dfs => vector�� list�� ��ü ����
#include <queue> //bfs => vector�� list�� ��ü ����
#include <vector>


class GraphSearch
{
public:
	static class Node
	{
	public:
		Node(void) {}
		~Node(void) {}

		std::vector<Node*> getNearNodes() const { return nearNodes; } // ��ü ����� ��� ��ȯ
	private:
		std::vector<Node*> nearNodes; // ����� ����, ���ͷ� �ȹ��� ���� ���� �ȴ�.
		int value; // ��尡 ������ �־�� �ϴ� �� �߰��ϸ� ��
	};

	static void dfsFind(Node& startNode);
	static void bfsFind(Node& startNode);
private:
	static void process(Node& node) {} // ����� ��� �����ϸ� ��
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



