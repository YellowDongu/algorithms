#pragma once
#include <stack> //dfs => list로 대체 가능
#include <queue> //bfs => list로 대체 가능
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

bool isValid(const std::vector<int>& selection)
{
	int sum = 0;
	for (int v : selection) sum += v;
	return sum % 2 == 0; // 예: 선택된 수의 합이 짝수인 경우만 유효
}

// 예시 결과 계산 함수
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
	st.push({ 0, {} }); // 초기 상태: idx=0, 선택된 후보 없음

	while (!st.empty())
	{
		State cur = st.top();
		st.pop();

		if (cur.idx == candidates.size())
		{
			// 후보를 다 확인했으면 조건 확인 후 결과 갱신
			if (isValid(cur.selection))
			{
				bestResult = std::max(bestResult, calculateResult(cur.selection));
			}
			continue;
		}

		// 1. 현재 후보 선택
		std::vector<int> selectWith = cur.selection;
		selectWith.push_back(candidates[cur.idx]);
		st.push({ cur.idx + 1, selectWith });

		// 2. 현재 후보 선택하지 않음
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
