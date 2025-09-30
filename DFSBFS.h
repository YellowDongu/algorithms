#pragma once
#include <stack> //dfs => list�� ��ü ����
#include <queue> //bfs => list�� ��ü ����
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

bool isValid(const std::vector<int>& selection)
{
	int sum = 0;
	for (int v : selection) sum += v;
	return sum % 2 == 0; // ��: ���õ� ���� ���� ¦���� ��츸 ��ȿ
}

// ���� ��� ��� �Լ�
int calculateResult(const std::vector<int>& selection)
{
	int sum = 0;
	for (int v : selection) sum += v;
	return sum;
}

int stackBacktrack(const std::vector<int>& candidates)
{
	struct State
	{
		int idx;
		std::vector<int> selection;
	};

	int bestResult = 0;
	std::stack<State> st;
	st.push({ 0, {} }); // �ʱ� ����: idx=0, ���õ� �ĺ� ����

	while (!st.empty())
	{
		State cur = st.top();
		st.pop();

		if (cur.idx == candidates.size())
		{
			// �ĺ��� �� Ȯ�������� ���� Ȯ�� �� ��� ����
			if (isValid(cur.selection))
			{
				bestResult = std::max(bestResult, calculateResult(cur.selection));
			}
			continue;
		}

		// 1. ���� �ĺ� ����
		std::vector<int> selectWith = cur.selection;
		selectWith.push_back(candidates[cur.idx]);
		st.push({ cur.idx + 1, selectWith });

		// 2. ���� �ĺ� �������� ����
		st.push({ cur.idx + 1, cur.selection });
	}

	return bestResult;
}


int backTracking(std::vector<int> info, std::vector<std::vector<int>>& graph)
{
	struct State
	{
		int index;
		int sheep;
		int wolves;
		std::vector<bool> candidates;
	};

	int answer = 0, size = info.size(), index, sheep, wolves;
	std::stack<State> stack;


	{
		std::vector<bool> initCandidates(size, false);
		for (auto child : graph[0])
			initCandidates[child] = true;

		stack.push({ 0, 0, 0, initCandidates });
	}

	while (!stack.empty())
	{
		State& backNode = stack.top();

		index = backNode.index;
		sheep = backNode.sheep;
		wolves = backNode.wolves;
		std::vector<bool> candidates = backNode.candidates;
		stack.pop();

		if (sheep <= wolves && 0 < sheep)
			continue;

		if (info[index])
			wolves++;
		else
			sheep++;

		answer = std::max(answer, sheep);

		for (auto child : graph[index])
			candidates[child] = true;

		for (int i = 0; i < size; ++i)
		{
			if (candidates[i])
			{
				candidates[i] = false;
				stack.push({ i, sheep, wolves, candidates });
				candidates[i] = true;
			}
		}
	}

	return answer;
}
