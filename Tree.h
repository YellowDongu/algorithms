#pragma once
#include <iostream>

#include <vector>
#include <list>
#include <stack>


struct BinaryTree
{
	std::vector<std::vector<int>> wholeTree;

	BinaryTree(int nodeNumber) : wholeTree(nodeNumber, std::vector<int>()) {}


	void PreOrder(int node, std::vector<int>& result)
	{
		result.push_back(node);
		for (auto& child : wholeTree[node])
			PreOrder(child, result);
	}


	void PostOrder(int node, std::vector<int>& result)
	{
		for (auto& child : wholeTree[node])
			PostOrder(child, result);
		result.push_back(node);
	}

	void InOrder(int node, std::vector<int>& result) // 이진 트리에서만 쓸 수 있음
	{
		if (node == -1 || wholeTree[node].size() > 2)
			return;

		// 왼쪽 자식 방문
		if (!wholeTree[node].empty())
			InOrder(wholeTree[node][0], result);

		// 현재 노드 처리
		result.push_back(node);

		// 오른쪽 자식 방문
		if (wholeTree[node].size() >= 2)
			InOrder(wholeTree[node][1], result);
	}

	void inOrder(int node, std::vector<int>& result)
	{
		if (wholeTree[node].empty())
		{
			result.push_back(node);
			return;
		}
		int half = static_cast<int>(wholeTree[node].size()) / 2;
		for (int i = 0; i < half; i++)
			inOrder(wholeTree[node][i], result);
		result.push_back(node);
		for (int i = half; i < static_cast<int>(wholeTree[node].size()); i++)
			inOrder(wholeTree[node][i], result);
	}

	inline void AddEdge(int parent, int child) { wholeTree[parent].push_back(child); }

	void Clear(void)
	{
		int size = static_cast<int>(wholeTree.size());
		wholeTree.clear();
		wholeTree.resize(size, std::vector<int>());
	}


	void PreOrderIterative(int root, std::vector<int>& result)
	{
		if (wholeTree.empty())
			return;

		std::vector<int> stack;
		stack.reserve(wholeTree.size());
		stack.push_back(root);

		while (!stack.empty())
		{
			int node = stack.back();
			stack.pop_back();
			result.push_back(node);

			for (int i = static_cast<int>(wholeTree[node].size()) - 1; i >= 0; --i)
				stack.push_back(wholeTree[node][i]);
		}
	}

	void PostOrderIterative(int root, std::vector<int>& result)
	{
		if (wholeTree.empty())
			return;

		std::vector<std::pair<int, int>> stack; // {node, nextChildIndex}
		stack.reserve(wholeTree.size());
		stack.push_back({ root, 0 });

		while (!stack.empty())
		{
			int& node = stack.back().first;
			int& index = stack.back().second;

			if (index < static_cast<int>(wholeTree[node].size()))
			{
				int child = wholeTree[node][index];
				index++;
				stack.push_back({ child, 0 });
			}
			else
			{
				result.push_back(node);
				stack.pop_back();
			}
		}
	}

	void InOrderIterative(int root, std::vector<int>& result)
	{
		if (wholeTree.empty())
			return;

		std::vector<int> stack;
		stack.reserve(wholeTree.size());
		int current = root;

		while (!stack.empty() || current != -1)
		{
			// 왼쪽 자식으로 끝까지 이동
			while (current != -1)
			{
				stack.push_back(current);
				if (!wholeTree[current].empty())
					current = wholeTree[current][0]; // 왼쪽 자식
				else
					current = -1;
			}

			// 스택의 top 처리
			current = stack.back();
			stack.pop_back();
			result.push_back(current);

			// 오른쪽 자식 이동
			if (wholeTree[current].size() >= 2)
				current = wholeTree[current][1]; // 오른쪽 자식
			else
				current = -1;
		}
	}



};

void PreOrderRestore(std::vector<std::vector<int>>& tree, const std::vector<int>& preorder, const std::vector<int>& inorder)
{
	int length = static_cast<int>(preorder.size());
	tree.resize(length + 1); // 1-based
	if (length == 0)
		return;

	std::vector<int> stack{ preorder[0] };
	stack.reserve(length);
	int inorderIndex = 0;

	for (int i = 1; i < length; ++i)
	{
		int current = preorder[i];
		int parent = -1;

		// 스택 상단이 아직 중위에서 처리되지 않았으면 왼쪽 자식
		if (stack.back() != inorder[inorderIndex])
		{
			parent = stack.back();
			tree[parent].push_back(current); // 왼쪽 자식
			stack.push_back(current);
		}
		else
		{
			// 중위 순서에 따라 오른쪽 자식 탐색
			while (!stack.empty() && stack.back() == inorder[inorderIndex])
			{
				parent = stack.back();
				stack.pop_back();
				inorderIndex++;
			}
			tree[parent].push_back(current); // 오른쪽 자식
			stack.push_back(current);
		}
	}

}

void PostOrderRestore(std::vector<std::vector<int>>& tree, const std::vector<int>& inorder, const std::vector<int>& postorder)
{
	int length = static_cast<int>(postorder.size());
	tree.reserve(length + 1);
	if (length == 0)
		return;

	// 중위 순회에서 인덱스를 빠르게 찾기 위한 맵
	std::vector<int> inorderIndex(length + 1);
	for (int i = 0; i < length; ++i)
		inorderIndex[inorder[i]] = i;

	std::vector<int> stack{ postorder[length - 1] };
	stack.reserve(length);
	int inorderPos = length - 1;

	for (int i = length - 2; i >= 0; --i)
	{
		int current = postorder[i];
		int parent = -1;

		// 스택의 top이 아직 중위 순회에서 처리되지 않은 노드라면 → 오른쪽 자식
		if (inorderIndex[current] > inorderIndex[stack.back()])
		{
			parent = stack.back();
			tree[parent].push_back(current); // 오른쪽 자식
			stack.push_back(current);
		}
		else
		{
			// 중위 순서에 따라 왼쪽 자식을 찾아감
			while (!stack.empty() && inorderIndex[current] < inorderIndex[stack.back()])
			{
				parent = stack.back();
				stack.pop_back();
			}
			tree[parent].push_back(current); // 왼쪽 자식
			stack.push_back(current);
		}
	}
}

#include <functional>
void PostOrderRestoreRecursive(std::vector<std::vector<int>>& tree, const std::vector<int>& inorder, const std::vector<int>& postorder)
{
	int length = static_cast<int>(postorder.size());
	std::vector<int> inorderIndex(length + 1);

	// 재귀함수 정의
	std::function<void(int, int, int, int)> build = [&](int inOrderLeft, int inOrderRight, int postOrderLeft, int postOrderRight)
		{
			if (inOrderLeft > inOrderRight || postOrderLeft > postOrderRight)
				return;

			int rootValue = postorder[postOrderRight];
			int rootIndex = inorderIndex[rootValue];
			int leftSize = rootIndex - inOrderLeft;

			if (leftSize > 0)
				tree[rootValue].push_back(postorder[postOrderLeft + leftSize - 1]/*왼쪽 서브트리 루트*/);

			if (rootIndex < inOrderRight)
				tree[rootValue].push_back(postorder[postOrderRight - 1]/*오른쪽 서브트리 루트*/);

			build(inOrderLeft, rootIndex - 1, postOrderLeft, postOrderLeft + leftSize - 1); // 왼쪽 서브트리
			build(rootIndex + 1, inOrderRight, postOrderLeft + leftSize, postOrderRight - 1); // 오른쪽 서브트리
		};

	tree.resize(length + 1);// 1-based
	if (length == 0)
		return;

	for (int i = 0; i < length; ++i)
		inorderIndex[inorder[i]] = i;

	build(0, length - 1, 0, length - 1);
}
 
//세그먼트 트리
// 구간의 합을 미리 구해놓는거
// 원본 배열이 {1, 2, 1, 4, 5}일때의 예시임
// node 1 [1-5] =13 : █ █ █ █ █
// node 2 [1-3] = 4 : █ █ █ □ □
// node 3 [4-5] = 9 : □ □ □ █ █
// node 4 [1-2] = 3 : █ █ □ □ □
// node 5 [3-3] = 1 : □ □ █ □ □
// node 6 [4-4] = 4 : □ □ □ █ □
// node 7 [5-5] = 5 : □ □ □ □ █
// node 8 [1-1] = 1 : █ □ □ □ □
// node 9 [2-2] = 2 : □ █ □ □ □
//각 구간을 왼쪽부터 쪼개서 우선적으로 배열에 할당하는거고 할당이 끝났으면 상향식으로 계속 더해가면서 넣는거
// ㄴ이걸 재귀로 실행하는거
//이걸 구간합을 구할때 역순으로 타고 올라가서 구하는거
// 2~5를 예를 들면
// 2~3과 4~5로 쪼개고 (2~3의 인덱스는 1 * 2, 4~5의 인덱스는 1 * 2 + 1)
// 4~5는 그대로 충족되니깐 그걸 그대로 사용하고
// 2~3은 1~3에 충족 안되니 한번 더 쪼개서 2~2와 3~3, 인덱스는 2(1 * 2) * 2, 2 * 2 + 1
// 2~2와 3~3은 그대로 충족되니깐 그걸 그대로 사용해서 총 구간이 3개가 나와 3개의 값을 더해야 구간 합이 나와
// (2~2 =>) 2 + (3~3 =>) 1 + (4~5 =>) 9 = 12

class SegmentTree // 1 base index
{
public:
	SegmentTree(void) = default;
	SegmentTree(const std::vector<int>& list) : array(list), size(static_cast<int>(list.size())), tree(list.size() * 4)
	{
		array.insert(array.begin(), 0);
		Build(array, 1, 1, size);
	}
	SegmentTree(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end, int nodeLength) : array(begin, end), size(nodeLength), tree(nodeLength * 4)
	{
		array.insert(array.begin(), 0);
		Build(array, 1, 1, size);
	}

	void CreateTree(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end)
	{
		array = std::vector<int>(begin, end);
		size = array.size();
		tree.resize(size * 4);
		Build(array, 1, 0, size - 1);
	}
	void CreateTree(const std::vector<int>& list)
	{
		array = list;
		size = array.size();
		tree.resize(size * 4);
		Build(array, 1, 0, size - 1);
	}
	void Update(int index, int value)
	{
		array[index] = value;
		Update(1, 1, size, index, value);
	}

	int Search(int left, int right) { return Search(1, 1, size, left, right); }

	static void Sample(void)
	{
		std::vector<int> origin = { 1, 3, 5, 7, 9, 11 };
		SegmentTree sampleTree(origin);

		std::cout << sampleTree.Search(2, 4) << "\n"; // 3+5+7 = 15
		sampleTree.Update(2, 10); // arr[1] = 10
		std::cout << sampleTree[2] << "\n"; // arr[1] => 10
		std::cout << sampleTree.Search(2, 4) << "\n"; // 10+5+7 = 22
	}

	int operator[] (int index) { return array[index + 1]; }

private:
	void Build(const std::vector<int>& arr, int node, int start, int end)
	{
		if (start == end)
		{
			tree[node] = arr[start];
			return;
		}

		int middle = (start + end) / 2;
		Build(arr, node * 2, start, middle);
		Build(arr, node * 2 + 1, middle + 1, end);
		tree[node] = tree[node * 2] + tree[node * 2 + 1];
	}

	void Update(int node, int start, int end, int index, int value)
	{
		if (start == end)
			tree[node] = value;
		else
		{
			int middle = (start + end) / 2;
			if (index <= middle)
				Update(node * 2, start, middle, index, value);
			else
				Update(node * 2 + 1, middle + 1, end, index, value);

			tree[node] = tree[node * 2] + tree[node * 2 + 1];
		}
	}

	int Search(int node, int start, int end, int left, int right)
	{
		if (right < start || end < left)
			return 0; // outside
		if (left <= start && end <= right)
			return tree[node]; // inside

		int middle = (start + end) / 2;
		return Search(node * 2, start, middle, left, right) + Search(node * 2 + 1, middle + 1, end, left, right);
	}

	std::vector<int> array;
	std::vector<int> tree;
	int size;
};