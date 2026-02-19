// SimpleCodeTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include "pch.h"
#include <string>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>
#include <stack>
#include <functional>
#include <memory>
#include <unordered_map>
#include <set>

using namespace std;

//#include <stack>
//
//std::list<int> container;
//bool on = true;
//void init();
//
//std::mutex listLock;
//
//std::thread creatingThread([&]()
//	{
//		std::random_device rd;  // 시드로 사용할 랜덤 장치
//		std::mt19937 gen(rd()); // Mersenne Twister 엔진 사용
//		// 생성할 숫자의 범위 설정 (1부터 100 사이의 난수)
//		std::uniform_int_distribution<int> dist(1, 100);
//
//		while (on)
//		{
//			if (listLock.try_lock())
//			{
//				container.push_back(dist(gen));
//				listLock.unlock();
//			}
//			else continue;
//		}
//	});
//
//int main()
//{
//    std::cout << "Hello World!\n";
//	init();
//	creatingThread.join();
//}
//
//void init()
//{
//	for (int i = 0; i < 2; i++)
//	{
//		std::thread newThread([&]()
//			{
//				while (on)
//				{
//					if (container.empty()) continue;
//					else
//					{
//						if (!listLock.try_lock()) continue;
//						int out = container.front();
//						container.pop_front();
//						listLock.unlock();
//						std::cout << out << std::endl;
//					}
//				}
//
//			});
//	}
//
//}

//struct Node
//{
//	int index{0};
//	Node* leftChild{nullptr};
//	Node* rightChild{nullptr};
//
//	Node(void) = default;
//	~Node(void) { if (leftChild != nullptr) delete leftChild; if (rightChild != nullptr) delete rightChild; }
//};

//struct Node
//{
//	int index{ 0 };
//	std::vector<std::shared_ptr<Node>> neighbors;
//	
//	Node(void) = default;
//	~Node(void) = default;
//
//	static std::shared_ptr<Node> Create(int idx = -1)
//	{
//		std::shared_ptr<Node> newNode = std::make_shared<Node>();
//		newNode->index = idx;
//		return newNode;
//	}
//};
//
//
//void preorder(const std::shared_ptr<Node>& root, std::vector<int>& output)
//{
//	std::list<std::shared_ptr<Node>> stack;
//	stack.push_back(root);
//	std::shared_ptr<Node> currentNode;
//	while (!stack.empty())
//	{
//		currentNode = stack.back();
//		stack.pop_back();
//		output.push_back(currentNode->index);
//
//		for (auto& node : currentNode->neighbors)
//		{
//			stack.push_back(node);
//		}
//	}
//}
//void preorder(const std::shared_ptr<Node>& root, std::vector<std::shared_ptr<Node>>& output)
//{
//	std::list<std::shared_ptr<Node>> stack;
//	std::map<int, bool> nodeMap;
//	std::map<int, bool>::iterator nodeMapIterator;
//	std::shared_ptr<Node> currentNode;
//
//	nodeMap[root->index] = true;
//	stack.push_back(root);
//
//	while (!stack.empty())
//	{
//		currentNode = stack.back();
//		stack.pop_back();
//		output.push_back(currentNode);
//
//		for (auto& node : currentNode->neighbors)
//		{
//			nodeMapIterator = nodeMap.find(node->index);
//			if (nodeMapIterator != nodeMap.end())
//				continue;
//			nodeMap[node->index] = true;
//			stack.push_back(node);
//		}
//	}
//}
//
//void postorder(const std::shared_ptr<Node>& root, std::vector<std::shared_ptr<Node>>& output)
//{
//	std::list<std::shared_ptr<Node>> stackOne, stackTwo;
//	std::shared_ptr<Node> currentNode = nullptr;
//	stackOne.push_back(root);
//
//	while (!stackOne.empty())
//	{
//		currentNode = stackOne.back(); stackOne.pop_back();
//		stackTwo.push_back(currentNode);
//
//		for (auto& node : currentNode->neighbors)
//		{
//			stackOne.push_back(node);
//		}
//	}
//
//	while (!stackTwo.empty())
//	{
//		output.push_back(stackTwo.back());
//		stackTwo.pop_back();
//	}
//}
//
//void postorder(const std::shared_ptr<Node>& root, std::vector<int>& output)
//{
//	std::list<std::shared_ptr<Node>> stackOne, stackTwo;
//	std::shared_ptr<Node> currentNode = nullptr;
//	stackOne.push_back(root);
//
//	while (!stackOne.empty())
//	{
//		currentNode = stackOne.back(); stackOne.pop_back();
//		stackTwo.push_back(currentNode);
//
//		for (auto& node : currentNode->neighbors)
//		{
//			stackOne.push_back(node);
//		}
//	}
//
//	while (!stackTwo.empty())
//	{
//		output.push_back(stackTwo.back()->index);
//		stackTwo.pop_back();
//	}
//}

//void preorder(Node* root, std::vector<int>& output)
//{
//	std::list<Node*> stack;
//	stack.push_back(root);
//
//	while (!stack.empty())
//	{
//		Node* node = stack.back();
//		stack.pop_back();
//		output.push_back(node->index);
//
//		if (node->rightChild != nullptr) stack.push_back(node->rightChild);
//		if (node->leftChild != nullptr) stack.push_back(node->leftChild);
//	}
//}
//
//void postorder(Node* root, std::vector<int>& output)
//{
//	std::list<Node*> stackOne, stackTwo;
//	Node* node = nullptr;
//	stackOne.push_back(root);
//
//	while (!stackOne.empty())
//	{
//		Node* node = stackOne.back(); stackOne.pop_back();
//		stackTwo.push_back(node);
//
//		if (node->rightChild != nullptr) stackOne.push_back(node->rightChild);
//		if (node->leftChild != nullptr) stackOne.push_back(node->leftChild);
//	}
//
//	while (!stackTwo.empty())
//	{
//		output.push_back(stackTwo.back()->index);
//		stackTwo.pop_back();
//	}
//}

//Node* buildTree(const std::vector<std::vector<int>>& nodeinfo, std::vector<int>& order)
//{
//	if (order.empty()) return nullptr;
//
//	Node* root = new Node{ order[0] };
//
//	std::stack<std::pair<Node*, std::vector<int>>> nodeStack;
//	std::vector<int> rest(order.begin() + 1, order.end());
//	nodeStack.push({ root, rest });
//	Node* parent;
//	std::vector<int> leftNodes, rightNodes;
//
//	while (!nodeStack.empty())
//	{
//		leftNodes.clear();
//		rightNodes.clear();
//
//		parent = nodeStack.top().first;
//		for (int idx : nodeStack.top().second)
//		{
//			if (nodeinfo[idx][0] < nodeinfo[parent->index][0])
//				leftNodes.push_back(idx);
//			else
//				rightNodes.push_back(idx);
//		}
//		nodeStack.pop();
//
//		if (!leftNodes.empty())
//		{
//			Node* leftChild = new Node();
//			leftChild->index = leftNodes[0];
//			parent->leftChild = leftChild;
//			nodeStack.push({ leftChild, std::vector<int>(leftNodes.begin() + 1, leftNodes.end()) });
//		}
//
//		if (!rightNodes.empty())
//		{
//			Node* rightChild = new Node();
//			rightChild->index = rightNodes[0];
//			parent->rightChild = rightChild;
//			nodeStack.push({ rightChild, std::vector<int>(rightNodes.begin() + 1, rightNodes.end()) });
//		}
//	}
//
//	return root;
//}
//
//vector<vector<int>> solution(vector<vector<int>> nodeinfo)
//{
//	std::vector<std::vector<int>> answer(2, std::vector<int>());
//	size_t length = nodeinfo.size();
//
//	std::vector<int> order(length);
//
//	for (size_t i = 0; i < length; i++)
//		order[i] = i;
//
//	// y sort(Descend)
//	std::sort(order.begin(), order.end(), [&](int one, int other) { if (nodeinfo[one][1] != nodeinfo[other][1]) return nodeinfo[one][1] > nodeinfo[other][1]; return nodeinfo[one][0] < nodeinfo[other][0]; });
//
//	Node* rootNode = buildTree(nodeinfo, order);
//
//	preorder(rootNode, answer[0]);
//	postorder(rootNode, answer[1]);
//	delete rootNode;
//	return answer;
//}










vector<int> solution(vector<string> park, vector<string> routes)
{
	int length = static_cast<int>(routes.size()), sizeX = static_cast<int>(park.size()), sizeY = static_cast<int>(park[0].size()), virtualPosition, scalar;
	std::vector<std::vector<bool>> parkData(sizeX, std::vector<bool>(sizeY, true));
	std::pair<int, int> currentPosition;


	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{
			if(park[x][y] == 'X')
				parkData[x][y] = false;
			else if (park[x][y] == 'S')
				currentPosition = { x, y };
		}
	}

	for (int i = 0; i < length; i++)
	{
		scalar = std::stoi(routes[i].substr(2, 1));
		
		switch (routes[i][0])
		{
		case 'N':
			if (currentPosition.first - scalar < 0)
				continue;

			for (virtualPosition = 1; virtualPosition <= scalar; virtualPosition++)
			{
				if (!parkData[currentPosition.first - virtualPosition][currentPosition.second])
					break;
			}

			if (virtualPosition > scalar)
				currentPosition.first -= scalar;
			break;
		case 'S':
			if (currentPosition.first + scalar >= sizeX)
				continue;

			for (virtualPosition = 1; virtualPosition <= scalar; virtualPosition++)
			{
				if (!parkData[currentPosition.first + virtualPosition][currentPosition.second])
					break;
			}

			if (virtualPosition > scalar)
				currentPosition.first += scalar;
			break;
		case 'E':
			if (currentPosition.second + scalar >= sizeY)
				continue;

			for (virtualPosition = 1; virtualPosition <= scalar; virtualPosition++)
			{
				if (!parkData[currentPosition.first][currentPosition.second + virtualPosition])
					break;
			}

			if (virtualPosition > scalar)
				currentPosition.second += scalar;
			break;
		case 'W':
			if (currentPosition.second - scalar < 0)
				continue;

			for (virtualPosition = 1; virtualPosition <= scalar; virtualPosition++)
			{
				if (!parkData[currentPosition.first][currentPosition.second - virtualPosition])
					break;
			}

			if (virtualPosition > scalar)
				currentPosition.second -= scalar;
			break;
		default:
			break;
		}
	}


	return { currentPosition.first, currentPosition.second };
}


int solution(vector<int> mats, vector<vector<string>> park)
{
	int sizeX = static_cast<int>(park.size()), sizeY = static_cast<int>(park[0].size());
	int currentSize, areaSize, currentX, currentY;

	std::vector<std::vector<int>> emptySpace(sizeX, std::vector<int>(sizeY, 0));

	for (int x = 0; x < sizeX; x++) // 0,0부터 x,y까지 비어있는 영역 갯수 합산 => 0,0에서부터 시작해 이전 좌표의 값을 합산해 누적 합을 구하는거
	{
		for (int y = 0; y < sizeY; y++)
		{
			if (park[x][y][0] == '-')
				emptySpace[x][y] = 1;

			if (x > 0) emptySpace[x][y] += emptySpace[x - 1][y];
			if (y > 0) emptySpace[x][y] += emptySpace[x][y - 1];
			if (x > 0 && y > 0) emptySpace[x][y] -= emptySpace[x - 1][y - 1]; // 중복 영역 제거-> 0,0에서 x-1,y-1의 카운트가 2번 더해진 상황임
		}
	}

	std::sort(mats.begin(), mats.end(), std::greater<int>()); // 큰 순서대로 정렬

	for (auto& size : mats)
	{
		areaSize = size * size;
		for (int x = size; x < sizeX; x++) // 0,0부터 x,y까지 비어있는 영역 갯수 합산
		{
			for (int y = size; y < sizeY; y++)
			{
				currentX = x - size + 1;
				currentY = y - size + 1;

				currentSize = emptySpace[x][y];
				if (currentX > 0)
				currentSize -= emptySpace[currentX - 1][y];
				if (currentY > 0)
				currentSize -= emptySpace[x][currentY - 1];
				if (currentX > 0 && currentY > 0)
				currentSize += emptySpace[currentX - 1][currentY - 1];

				if (areaSize == currentSize)
					return size;
			}
		}

	}

	return -1;
}

int solution(vector<int> bandage, int health, vector<vector<int>> attacks)
{
	int timeLength = attacks.back()[0] + 1, usingTime = -1, attackIndex = 0, maxHealth = health;

	for (int time = 0; time < timeLength; time++)
	{
		if (time == attacks[attackIndex][0])
		{
			health -= attacks[attackIndex][1];
			if (health <= 0) return -1;

			usingTime = 0;
			attackIndex++;
			continue;
		}

		usingTime++;
		health += bandage[1];

		if (usingTime == bandage[0])
		{
			health += bandage[2];
			usingTime = 0;
		}

		if (health > maxHealth)
			health = maxHealth;
	}

	return health;
}



string solution(string video_len, string pos, string op_start, string op_end, vector<string> commands)
{
	std::string result;
	std::map<std::string, bool> commandMap = { {"next", true}, {"prev", false} };

	int videoLength = std::stoi(video_len.substr(0, 2)) * 60 + std::stoi(video_len.substr(3, 2));
	int position = std::stoi(pos.substr(0, 2)) * 60 + std::stoi(pos.substr(3, 2));
	int openingTime = std::stoi(op_start.substr(0, 2)) * 60 + std::stoi(op_start.substr(3, 2));
	int openingEndTime = std::stoi(op_end.substr(0, 2)) * 60 + std::stoi(op_end.substr(3, 2));

	for (auto& command : commands)
	{
		if (openingTime < position && position < openingEndTime)
			position = openingEndTime;

		if (commandMap.find(command)->second) // forward
		{
			position += 10;

			if (position > videoLength)
				position = videoLength;
		}
		else // backward
		{
			position -= 10;

			if (position < 0)
				position = 0;
		}
	}

	if (openingTime < position && position < openingEndTime)
		position = openingEndTime;

	int intBuffer = position / 60;
	if (intBuffer > 10)
		result = std::to_string(intBuffer);
	else
		result = "0" + std::to_string(intBuffer);
	intBuffer = position % 60;
	if (intBuffer > 10)
		result += ":" + std::to_string(intBuffer);
	else
		result += ":0" + std::to_string(intBuffer);

	return result;
}





int Distance(const std::pair<int, int>& one, const std::pair<int, int>& other){	return std::abs(one.first - other.first) + std::abs(one.second - other.second); }

string solution(vector<int> numbers, string hand)
{
	size_t length = numbers.size();
	std::string answer(length, 'R');
	std::map<int, std::pair<int, int>> keyPad = {  {0, {1,0}}, {1, {0,3}}, {2, {1,3}}, {3, {2,3}}, {4, {0,2}}, {5, {1,2}}, {6, {2,2}}, {7, {0,1}}, {8, {1,1}}, {9, {2,1}} };
	bool leftHand = (hand == "left");

	std::pair<int, int> currentPositionLeft = { 0, 0 };
	std::pair<int, int> currentPositionRight = { 2, 0 };
	std::pair<int, int> targetPosition;
	int leftDistance, rightDistance;
	for (size_t i = 0; i < length; i++)
	{
		targetPosition = keyPad[numbers[i]];
		
		switch (targetPosition.first)
		{
		case 0:
			currentPositionLeft = targetPosition;
			answer[i] = 'L';
			break;
		case 2:
			currentPositionRight = targetPosition;
			break;
		default:
			leftDistance = Distance(currentPositionLeft, targetPosition);
			rightDistance = Distance(currentPositionRight, targetPosition);
			if (leftDistance == rightDistance)
			{
				if (leftHand)
				{
					currentPositionLeft = targetPosition;
					answer[i] = 'L';
				}
				else
					currentPositionRight = targetPosition;
			}
			else if (leftDistance < rightDistance)
			{
				currentPositionLeft = targetPosition;
				answer[i] = 'L';
			}
			else
				currentPositionRight = targetPosition;
			break;
		}

	}

	return answer;
}

int solution(int size, vector<int> lost, vector<int> reserve)
{
	std::vector<int> students(size, 1);
	int answer = 0;

	for (auto& number : lost)
		students[number - 1]--;
	for (auto& number : reserve)
		students[number - 1]++;


	for (size_t i = 0; i < students.size(); i++)
	{
		if (students[i] != 0)
			answer++;
		else if (i > 0 && students[i - 1] == 2)
		{
			students[i - 1]--;
			students[i]++;
			answer++;
		}
		else if (i < size - 1 && students[i + 1] == 2)
		{
			students[i + 1]--;
			students[i]++;
			answer++;
		}
	}

	return answer;
}

struct Node
{
	int parent{-1};
	int id{-1};
	int people{-1};
	vector<int> children;
};


pair<int, int> DepthSearch(int cur, int parent, int max_allowed, vector<Node>& nodes)
{

	int sum = nodes[cur].people, groups = 0;

	for (int child : nodes[cur].children)
	{
		if (child == parent) continue;
		pair<int, int> result = DepthSearch(child, cur, max_allowed, nodes);
		groups += result.second;

		if (result.first + sum > max_allowed)
			groups += 1;
		else
			sum += result.first;
	}

	return { sum, groups };
}

int solution(int k, vector<int> num, vector<vector<int>> links)
{
	std::vector<Node> nodes;
	int left = 0, right = 0, group, length = static_cast<int>(num.size()), root = 0;

	nodes.resize(length);
	for (int i = 0; i < length; i++)
	{
		nodes[i].id = i;
		nodes[i].people = num[i];
		nodes[i].children.clear();

		if (links[i][0] != -1)
		{
			nodes[i].children.push_back(links[i][0]);
			nodes[links[i][0]].parent = i;
		}

		if (links[i][1] != -1)
		{
			nodes[i].children.push_back(links[i][1]);
			nodes[links[i][1]].parent = i;
		}

		right += num[i];
		left = std::max(left, num[i]);
	}

	for (int i = 0; i < length; i++)
		if (nodes[i].parent == -1) root = i;

	int answer = right;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		group = DepthSearch(root, -1, mid, nodes).second;

		if (group + 1 <= k)
		{
			answer = mid;
			right = mid - 1;
		}
		else
			left = mid + 1;
	}

	return answer;
}

int solution(int size, int k, vector<int> num, vector<vector<int>> links)
{
	//int n; // 시험장 수
	//int k; // 그룹 수
	vector<Node> tree;
	int left = 0, right = 0, group;

	tree.resize(size);
	for (int i = 0; i < size; i++)
	{
		tree[i].id = i;
		tree[i].people = num[i];
		tree[i].children.clear();

		tree[links[i][0]].children.push_back(links[i][1]);
		tree[links[i][1]].children.push_back(links[i][0]);
		
		right += num[i];
		left = std::max(left, num[i]);
	}

	int answer = right;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		group = DepthSearch(0, -1, mid, tree).second;

		if (group + 1 <= k)
		{
			answer = mid;
			right = mid - 1;
		}
		else
			left = mid + 1;
	}

	return answer;
}


vector<pair<int, int>> hanoiPattern(int size, int from, int to, int aux, std::map<int, std::vector<std::pair<int, int>>>& cache)
{
	if (cache.count(size)) return cache[size];  // 캐싱된 패턴 반환

	vector<pair<int, int>> moves;
	if (size == 1)
	{
		moves.push_back({ from, to });
	}
	else
	{
		auto left = hanoiPattern(size - 1, from, aux, to, cache);
		moves.insert(moves.end(), left.begin(), left.end());
		moves.push_back({ from, to });
		auto right = hanoiPattern(size - 1, aux, to, from, cache);
		moves.insert(moves.end(), right.begin(), right.end());
	}
	cache[size] = moves;
	return moves;
}
void hanoi(int size, std::vector<std::vector<int>>& returnList)
{
	struct state
	{
		int count;
		int from;
		int to;
		int auxiliary;
		int state;
	};
	int from = 1, to = 3, aux = 2;
	std::list<state> stack;


	stack.push_back({ size, from, to, aux, 0 });
	state currentState;
	while (!stack.empty())
	{
		currentState = stack.back();
		stack.pop_back();

		if (currentState.count == 1)
			returnList.push_back({ currentState.from, currentState.to });
		else if (currentState.state == 0)
		{
			currentState.state = 1;
			stack.push_back(currentState);
			stack.push_back({ currentState.count - 1, currentState.from, currentState.auxiliary, currentState.to, 0 });
		}
		else if (currentState.state == 1)
		{
			returnList.push_back({ currentState.from, currentState.to });
			currentState.state = 2;
			stack.push_back(currentState);
		}
		else
			stack.push_back({ currentState.count - 1, currentState.auxiliary, currentState.to, currentState.from, 0 });

	}



}

vector<vector<int>> solution(int size)
{
	std::vector<std::vector<int>> answer;
	hanoi(size, answer);
	return answer;
}


//void preorderSubTree(const std::shared_ptr<Node>& root, const std::shared_ptr<Node>& parent, size_t nodeNumber, std::vector<std::shared_ptr<Node>>& output)
//{
//	std::list<std::shared_ptr<Node>> stack;
//	std::vector<bool> nodeMap(nodeNumber + 1, false);
//	std::shared_ptr<Node> currentNode;
//
//	nodeMap[root->index] = true;
//	nodeMap[parent->index] = true;
//	stack.push_back(root);
//
//	while (!stack.empty())
//	{
//		currentNode = stack.back();
//		stack.pop_back();
//		output.push_back(currentNode);
//
//		for (auto& node : currentNode->neighbors)
//		{
//			if (nodeMap[node->index])
//				continue;
//
//			nodeMap[node->index] = true;
//			stack.push_back(node);
//		}
//	}
//}
//
//int getSubNodeDifference(std::shared_ptr<Node>& root, size_t nodeCount)
//{
//	int difference;
//	std::vector<int> returnList;
//	std::vector<std::shared_ptr<Node>> output;
//	int returnValue = std::numeric_limits<int>::max();
//	for (auto& node : root->neighbors)
//	{
//		output.clear();
//		preorderSubTree(node, root, nodeCount, output);
//		returnList.push_back(static_cast<int>(output.size()/*본인 포함 갯수임*/));
//	}
//	int length = static_cast<int>(returnList.size());
//	for (int j = 0; j < length; j++)
//	{
//		difference = 1;
//		for (int i = 0; i < length; i++)
//		{
//			if(j == i)
//				continue;
//			difference += returnList[i];
//		}
//
//		returnValue = std::min(returnValue, std::abs(difference - returnList[j]));
//	}
//
//	return returnValue;
//}
//
//int solution(int n, vector<vector<int>> wires)
//{
//	int answer = std::numeric_limits<int>::max();
//	std::vector<std::shared_ptr<Node>> nodes(n);
//	for (int i = 0; i < n; i++)
//	{
//		nodes[i] = Node::Create(i + 1);
//	}
//
//	for (auto& connection : wires)
//	{
//		nodes[connection[0] - 1]->neighbors.push_back(nodes[connection[1] - 1]);
//		nodes[connection[1] - 1]->neighbors.push_back(nodes[connection[0] - 1]);
//	}
//
//	for (int i = 0; i < n; i++)
//	{
//		answer = std::min(answer, getSubNodeDifference(nodes[i], n));
//	}
//
//	return answer;
//}

int dijkstra(int departure, int arrivalA, int arrivalB, int nodeIndex, std::vector<std::map<int, int>>& graph)
{
	auto condition = [](std::pair<int, int>& a, std::pair<int, int>& b)->bool { return a.second > b.second; };
	std::vector<int> distance(graph.size(), std::numeric_limits<int>::max());
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(condition)> queue(condition);
	int currentIndex, currentIndexDistance, calculatedDistance;

	queue.push({ nodeIndex, 0 });
	distance[nodeIndex] = 0;

	while (!queue.empty())
	{
		currentIndex = queue.top().first;
		currentIndexDistance = queue.top().second;
		queue.pop();
		if (currentIndexDistance > distance[currentIndex])
			continue;

		for (auto& road : graph[currentIndex])
		{
			calculatedDistance = currentIndexDistance + road.second;
			if (calculatedDistance < distance[road.first])
			{
				distance[road.first] = calculatedDistance;
				queue.push({ road.first, calculatedDistance });
			}
		}

	}

	return distance[departure] + distance[arrivalA] + distance[arrivalB];
}

void dijkstra(int nodeIndex, const std::vector<std::vector<std::pair<int, int>>>& graph, std::vector<int>& distance)
{
	auto condition = [](std::pair<int, int>& a, std::pair<int, int>& b)->bool { return a.second > b.second; };
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(condition)> queue(condition);
	int currentIndex, currentIndexDistance, calculatedDistance;

	queue.push({ nodeIndex, 0 });
	distance[nodeIndex] = 0;

	while (!queue.empty())
	{
		currentIndex = queue.top().first;
		currentIndexDistance = queue.top().second;
		queue.pop();
		if (currentIndexDistance > distance[currentIndex])
			continue;

		for (auto& road : graph[currentIndex])
		{
			calculatedDistance = currentIndexDistance + road.second;
			if (calculatedDistance < distance[road.first])
			{
				distance[road.first] = calculatedDistance;
				queue.push({ road.first, calculatedDistance });
			}
		}

	}

}

int solution(int size, int s, int a, int b, vector<vector<int>> fares)
{
	s--; a--; b--;
	int answer = std::numeric_limits<int>::max(), result;
	std::vector<std::vector<std::pair<int, int>>> graph(size, std::vector<std::pair<int, int>>());

	std::vector<int> departureResult(size, std::numeric_limits<int>::max());
	std::vector<int> arrivalAResult(size, std::numeric_limits<int>::max());
	std::vector<int> arrivalBResult(size, std::numeric_limits<int>::max());

	for (auto& road : fares)
	{
		graph[road[0] - 1].push_back({road[1] - 1, road[2]});
		graph[road[1] - 1].push_back({road[0] - 1, road[2]});
	}
	dijkstra(s, graph, departureResult);
	dijkstra(a, graph, arrivalAResult);
	dijkstra(b, graph, arrivalBResult);
	for (size_t i = 0; i < size; i++)
	{
		result = departureResult[i] + arrivalAResult[i] + arrivalBResult[i];
		if (result < answer)
			answer = result;
	}

	return answer;
}

std::vector<int> solution(std::vector<std::string> id_list, std::vector<std::string> report, int k)
{
	int x = 0, y, reportCount, length = static_cast<int>(id_list.size());
	std::vector<int> answer(length);
	std::map<std::string, int> indexMap;
	std::map<std::string, int>::iterator indexIterator;
	std::vector<std::vector<bool>> reportStatus(length, std::vector<bool>(length, false));
	istringstream stream;
	string buffer;

	// Create Index
	for (auto& id : id_list)
	{
		indexMap[id] = x;
		x++;
	}
	x = 0;

	// Collect Reports
	for (auto& history : report)
	{
		// Reporter
		stream.clear();
		stream.str(history);
		std::getline(stream, buffer, ' ');

		indexIterator = indexMap.find(buffer);
		if (indexIterator == indexMap.end())
			continue;
		y = indexIterator->second;

		// Target
		std::getline(stream, buffer, ' ');

		indexIterator = indexMap.find(buffer);
		if (indexIterator == indexMap.end())
			continue;
		x = indexIterator->second;

		reportStatus[x][y] = true;
	}

	// Process Reports
	for (int x = 0; x < length; x++)
	{
		// Count Reports
		reportCount = 0;
		for (int y = 0; y < length; y++)
		{
			if (reportStatus[x][y])
			{
				reportCount++;
				if (reportCount >= k)
					break;
			}
		}

		// Mail to Reporters
		if (reportCount >= k)
		{
			for (int y = 0; y < length; y++)
			{
				if (reportStatus[x][y])
					answer[y]++;
			}
		}
	}

	return answer;
}


//int solution(std::vector<std::string> friends, std::vector<std::string> gifts)
//{
//	int answer = 0;
//	int giftTotalValue = 0, x, y, length = static_cast<int>(friends.size());
//	std::vector<std::vector<int>> giftCount(length, std::vector<int>(length, 0));
//	std::vector<int> giftValue(length, 0);
//	std::map<std::string, int> index;
//	std::map<std::string, int>::iterator indexIterator;
//	for (auto& name : friends)
//	{
//		index[name] = giftTotalValue;
//		giftTotalValue++;
//	}
//	giftTotalValue = 0;
//
//	istringstream stream;
//	string buffer;
//
//	for (auto& history : gifts)
//	{
//		stream.clear();
//		stream.str(history);
//		std::getline(stream, buffer, ' ');
//		indexIterator = index.find(buffer);
//		if (indexIterator == index.end())
//			continue;
//		x = indexIterator->second;
//
//		std::getline(stream, buffer, ' ');
//		indexIterator = index.find(buffer);
//		if (indexIterator == index.end())
//			continue;
//		y = indexIterator->second;
//
//		giftCount[x][y]++;
//		giftValue[x]++;
//		giftValue[y]--;
//	}
//
//	for (x = 0; x < length; x++)
//	{
//		for (y = 0; y < length; y++)
//		{
//			if (x == y)
//				continue;
//			if (giftCount[x][y] > giftCount[y][x])
//			{
//				giftTotalValue++;
//			}
//			else if (giftCount[x][y] == giftCount[y][x])
//			{
//				if (giftValue[x] > giftValue[y])
//					giftTotalValue++;
//			}
//		}
//
//		if (giftTotalValue > answer)
//			answer = giftTotalValue;
//
//		giftTotalValue = 0;
//	}
//
//
//	return answer;
//}

long long solution(int a, int b, vector<int> g, vector<int> s, vector<int> w, vector<int> t)
{
	long long answer = -1;
	long long leftTime = 0, rightTime = static_cast<long long>(4e14), middleTime = 0;

	auto canDeliver = [&](long long timeLimit) -> bool
		{
			long long totalGold = 0, totalSilver = 0;
			long long totalGoldSecond = 0, totalSilverSecond = 0;
			long long trips, carryAmount, totalLoad, load;
			

			for (size_t i = 0; i < g.size(); ++i)
			{
				trips = timeLimit / (2 * t[i]);
				trips += (timeLimit % (2 * t[i]) >= t[i]) ? 1 : 0;

				carryAmount = w[i] * trips;
				totalLoad = carryAmount;

				if (totalGold < a)
				{
					load = ((totalLoad < g[i]) ? totalLoad : g[i]);
					load = (load < a) ? load : a;
					totalGold += load;
					totalLoad -= load;
					load = ((totalLoad < s[i]) ? totalLoad : s[i]);
					load = (load < b) ? load : b;
					totalSilver += load;
				}
				else
				{
					load = ((totalLoad < s[i]) ? totalLoad : s[i]);
					load = (load < b) ? load : b;
					totalSilver += load;
				}

				if (totalSilverSecond < b)
				{
					totalLoad = carryAmount;
					load = (totalLoad < s[i]) ? totalLoad : s[i];
					load = (load < b) ? load : b;
					totalSilverSecond += load;
					totalLoad -= load;
					load = ((totalLoad < g[i]) ? totalLoad : g[i]);
					load = (load < a) ? load : a;
					totalGoldSecond += load;
				}
				else
				{
					load = ((totalLoad < g[i]) ? totalLoad : g[i]);
					load = (load < a) ? load : a;
					totalGoldSecond += load;
				}

			}

			return (totalGold >= a && totalSilver >= b) || (totalGoldSecond >= a && totalSilverSecond >= b);
		};
	
	while (leftTime <= rightTime)
	{
		middleTime = leftTime + (rightTime - leftTime) / 2;

		if (canDeliver(middleTime))
		{
			answer = middleTime;
			rightTime = middleTime - 1;
		}
		else
		{
			leftTime = middleTime + 1;
		}
	}

	return answer;
}



int Timer(const std::wstring& stringLines)
{
	int result = 0;
	std::wstringstream stream(stringLines);

	std::list<std::wstring> lines;
	std::wstring buffer;
	while (std::getline(stream, buffer, L'\n'))
	{
		if (buffer.size() == 0)
			continue;

		if (buffer[0] == '#' || buffer[0] == '@' || buffer[0] == '/')
			continue;

		lines.push_back(buffer);
	}

	for (auto& oneLine : lines)
	{
		if (oneLine.size() > 40) // 긴문장이면
			result += 10; //7초 + 3초
		else
			result += 7; // 5초 + 2초
	}

	return result;
}

std::wstring GetTime(void)
{
	std::wstring stringLines = L"### 맵\n@ 티르코네일 돌아다니기\n총 6개의 맵을 제작하고, 맵에 필요한 배경 객체들까지 구현했습니다.\nRPG의 마을 역할과 일반 사냥터를 겸하고 있는 티르코네일입니다.\n@ 시드스넷타 돌아다니기\n퀘스트 플롯을 위해 제작된 시드스넷타입니다.\n@ 여신의 신전 진행하기\n컷신을 위해 제작된 모리안 여신의 신전입니다.\n@ 모리안 던전 진행하기\n스토리 플롯과 전투에 사용하는 첫 던전입니다.\n@ 글라스기브넨 던전 진행하기\n마지막을 장식하는 두번째 던전입니다.\n### 배경 객체 인스턴싱 및 컬링\n@ 티르코네일 풍경 보기\n맵의 배경 객체들은 대부분 모델 인스턴싱을 통해 렌더링합니다.\n인스턴스들의 월드행렬들은 맵툴에서 제작한 바이너리 파일에서 불러옵니다.\n이 인스턴스들은 렌더되기 전 컴퓨트 셰이더를 통해 두가지 방식으로 컬링을 실시합니다.\n첫번째로 깊이비교 컬링 방식입니다.\n인스턴스에는 맵툴에서 AABB 박스를 미리 설정합니다.또한 깊이버퍼 텍스쳐를 깊이기록용 렌더타깃에서 지속적으로 가져옵니다.\nAABB 박스의 정점을 깊이버퍼 텍스쳐와 비교하기 위해 UV 좌표로 변환시켜 깊이비교를 합니다.\n@맵툴에서 나무 인스턴스를 하나 생성해서 카메라를 땡기기\n다만 영상처럼 AABB 정점이 전부 카메라 밖에 나가있지만 물체는 보여야되는 상황이 생깁니다.\n이를 위해 AABB 정점의 UV 좌표 영역 내 UV 좌표 중 몇 곳을 선정해서 추가로 깊이 비교를 해 한군데라도 통과하면 해당 인스턴스를 그리도록 합니다.\n@ 티르코네일에서 먼 곳을 바라볼 수 있는 장면이기만 하면 됨, 아무데서나 따와도 되고 돌아다니는 걸 찍어도 됨\n두번째로는 단순 거리비교 컬링입니다.\n인스턴스의 갯수가 매우 많지만 메쉬 자체는 단순한 형태일 때를 위한 컬링입니다.\n카메라 위치와 인스턴스의 위치로 거리로 계산해 비교해 컬링을 실시합니다.\n### 디더링\n@ 티르코네일 나무 아래에 있기\n카메라를 가리는 나무와 나뭇잎에 디더링을 적용해 시야에 방해되지 않도록 처리했습니다.\n픽셀 셰이더에서 투영 좌표의 w값과 플레이어용 카메라의 거리값을 이용해 디더링 세기를 결정합니다.\nHLSL의 exp 함수로 지수 함수 형태로 디더링 세기가 높아집니다.\n### 네비메쉬\n@ 네비메쉬 디버깅 렌더 키기\n@ 티르코네일에서 계속 돌아다니기만 하면 됨\n가게 할 지역에 메쉬를 깔아 캐릭터가 그 위에서만 움직일 수 있도록 하는 시스템입니다.\n캐릭터는 네비메쉬의 삼각형 중 하나인 셀을 타고 있게 하는 컴포넌트를 가집니다.\n셀의 한 점 기준 다른 점 방향 벡터와 캐릭터 좌표 방향 벡터를 내적으로 비교해 캐릭터가 셀을 벗어났는지 체크합니다.\n셀을 벗어났다면 해당 방향의 인접 삼각형에서 위의 검사를 반복합니다.\n또한 캐릭터의 y좌표값을 평면 방정식으로 보정합니다.\n@ 어딘가 가서 비비고 있기\n만약 벗어난 방향에 삼각형이 없으면 처음 삼각형의 빗변 벡터를 타게 합니다.\n@ 카메라를 벽에다가 비비기\n카메라도 같은 네비메쉬를 타게 해서 지형을 뚫고 보지 않게 했습니다.\n@ 카메라를 바닥에다가 비비기\n또한 카메라도 y값 보정을 통해 네비메쉬 아랫방향으로 내려가지 않게 합니다.\n//지형변경\n@ 글라스기브넨 떄려패서 지형파괴만 나올 수 있기만 하면 됨\n네비메쉬를 관리자와 사용자들로 나뉘어 공통된 셀의 정보를 공유하도록 했습니다.\n지형 파괴 시 파괴된 곳으로 가지 못하게끔 관리자 측에서 네비메쉬를 교체합니다.\n네비메쉬를 교체하는 시기에 관리자들은 사용자들에게 갱신이 필요하다는 신호를 줍니다.\n사용자들은 이에 따라 셀 정보를 조회할 때 이 신호에 의해 자신이 타고 있는 셀을 갱신합니다.\n@ 네비메쉬 디버깅 렌더 끄기\n### 조명\n@ 모리안 던전 파랑 불 바라보고 있기\n점 광원 계산은 램버트 공식을 기본으로 약간의 개조를 통해 구현했습니다.\n광원을 두 단계로 나뉘어 하이라이트 부분과 나머지 부분으로 나눠 대조되게 했습니다.\n@ 글라스기브넨 던전 적색 불 바라보고 있기\n램버트 공식에서 나온 빛 세기 값과 설정값을 비교해 구역을 나누고, 서로 다른 색과 밝기로 나눴습니다.\n설정된 값과 가까운 쪽에서는 하이라이트 부분과 나머지 부분을 보간했습니다.\n@ 티르코네일 밤 풍경 보여주기\n하이라이트 크기를 조절할 수 있게 해 조명의 밝기를 자연스럽도록 구현했습니다.\n### 게임 오브젝트 - 기믹\n@모리안던전 기믹 맞추기\n기믹은 두개가 있습니다.\n그림맞추기 기믹은 그림을 f키로 전환시키면서 자리에 맞게 맞추는 기믹입니다.\n통과 판정 시 큰 진동 후 문이 열립니다.\n@글라스기브넨 던전 기믹 한번 틀리고 전투 후 맞추기\n또 다른 기믹은 찍기를 하여 정답인 기둥에 상호작용하는 기믹입니다.\n정답이 아닌 기둥에 상호작용하면 적이 소환되고 해당 기둥은 비활성화됩니다.\n정답인 기둥에 상호작용하면 전체 기둥이 비활성화되고 문이 열립니다.\n### 게임 오브젝트 - 문게이트\n@문게이트 컷신 보여주고 문게이트 타는 것 보여주기\n같은 개체 간 텔레포트를 담당하는 문게이트입니다.\n문게이트와 상호작용 시 선택된 문게이트에 순간이동됩니다.\n문게이트 선택은 사전에 엮어둔 문게이트로 가는 것이 기본값입니다.\n### 게임 오브젝트 - 트리거\n@트리코네일에서 시드스넷타 가는 거 보여주기\n트리거는 레벨 전환, 컷신, 던전 스폰 트리거가 구현되어 있습니다.\n레벨 전환 시 쓰는 트리거들은 지역 진입형과 키 입력형 두개의 타입으로 만들었습니다.\n@모리안 던전 진입 보여주고 화면 전환으로 기믹방 지나고 기믹방 이후 헤메is디거 스폰 보여주기\n던전에 사용되는 지역 진입형 몬스터 스폰 트리거입니다.\n@모리안 던전 컷신 지역 들어가서 컷신 재생 보여주기\n컷신 트리거들은 지역 진입형과 특정 조건형 두개의 타입이 있습니다.\n@모리안 때려패고 난 뒤의 컷씬 나오는거 보여주기\n컷신 트리거 중 특정 조건형은 보스의 체력 비율에 반응해서 발동합니다.\n컷신 트리거 중 일부는 컷신 중 시간에 따라서 발동하는 이펙트나 페이드아웃등의 제어도 겸하여 제작되었습니다.\n### 바람\n@트리코네일 어딘가 가만히 서서 빨리감기\n버텍스 셰이더에서 바람에 흔들리는 나무 효과를 구현했습니다.\nCPU에서 바람을 행렬로 만들고, 버텍스 셰이더에서 모델 정점에 행렬을 가중치와 같이 곱해서 구현했습니다.\n가중치 값은 지수함수로 높이가 높을수록 바람의 영향을 더 받는 효과처럼 커지게끔 설정했습니다.\n또한 멀티스레딩으로 나무마다 따로 랜덤 계산을 해서 서로 다른 흔들림을 하도록 구현했습니다.\n### 자동이동\n@학교에서 목장으로 자동이동 통해서 가는거 보여줌(퀘스트중이어야됨) → 이거 아니더라도 자동이동이기만 하면 됨\nA* 알고리즘과 네비메쉬를 활용해 자동이동을 구현했습니다.\n가까운 도로까지 이동한 뒤 도로를 따라 목적지 인근까지 가고, 목적지와 가장 가까운 도로에서 이탈해 도착하는 구조입니다.\n네비메쉬의 셀과 도로의 웨이포인트를 노드로 이용해 A* 알고리즘으로 찾은 길을 리스트에 저장합니다.\n스레드 세개로 나뉘어 각자 길을 찾은 뒤 하나로 합치고, 플레이어는 이 결과에서 하나씩 꺼내서 이동합니다.\n### 맵 툴\n@객체 배치 후 위치 조정 → 어케하는지는 녹화 시에 알려줌\n유니티의 UI와 UX를 유사하게 설계하려 한 다용도 배치 툴입니다.\n미리 제작된 객체를 배치 및 조정 할 수 있습니다.\n@인스턴스 객체 배치 후 위치 조정 → 어케하는지는 녹화 시에 알려줌\nInstancingBaker창으로 모델 인스턴스를 동적으로 생성 및 배치, 조정할 수 있습니다.\n@이펙트 객체 배치 후 위치 조정 → 어케하는지는 녹화 시에 알려줌\nEffectPlacement창으로 이펙트들을 동적으로 생성 및 배치, 조정할 수 있습니다.\n@네비메쉬 제작하는거 보여준 후 웨이포인트 제작하는걸 보여줌 → 어케하는지는 녹화 시에 알려줌\nNavMesh창으로 네비메쉬를 제작하고 자동이동에 쓰는 길을 배치할 수 있습니다.\n### 컷신 툴\n@Camera 창 다루는거 보여주기 → 어케하는지는 녹화 시에 알려줌\n컷신은 Camera 창에서 실시간으로 재생하면서 제작할 수 있습니다.\nAnimation Curve로 움직임을 보간시킬 수 있고, 다른 옵션들로 컷 전환 및 카메라 앵글 고정을 할 수 있습니다.\n@애니메이션 다루는거 보여주기 → 어케하는지는 녹화 시에 알려줌\n컷신 내에서 보여줄 애니메이션 세트의 타이밍을 정해서 컷신 중 재생시킬 수 있습니다.\n@컷신들 보여주기->아무거나 진짜 상관없어서 만약 찍을때 뭘 해야될지 모르겠다면 문의 바람\n이렇게 만든 컷신은 JSON으로 저장 후 컷신 카메라가 불러와 컷신을 재생시킵니다.\n";
	int second = Timer(stringLines);

	int minute = second / 60;
	second = second % 60;
	
	return std::wstring(to_wstring(minute / 10) + to_wstring(minute % 10) + L':' + to_wstring(second / 10) + to_wstring(second % 10));
}



//L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

vector<int> solution(string today, vector<string> terms, vector<string> privacies)
{
	struct dayType
	{
		int year, month, day;
		dayType(void) = default;
		dayType(const std::string& time) { SetDayType(time); }
		void SetDayType(const std::string& time)
		{
			year = std::stoi(time.substr(0, 4));
			month = std::stoi(time.substr(5, 2));
			day = std::stoi(time.substr(8, 2));
		}
	};

	vector<int> answer;

	std::map<char, int> termMap;

	for (auto& term : terms)
		termMap[term[0]] = std::stoi(term.substr(2));

	dayType currentDay(today);
	dayType privacyDay;

	for (size_t i = 0; i < privacies.size(); i++)
	{
		privacyDay.SetDayType(privacies[i].substr(0, 10));
		privacyDay.month += termMap.find(privacies[i][11])->second;
		while (privacyDay.month > 12)
		{
			privacyDay.month -= 12;
			privacyDay.year += 1;
		}

		if (currentDay.year == privacyDay.year)
		{
			if (currentDay.month == privacyDay.month)
			{
				if (currentDay.day >= privacyDay.day)
					answer.push_back(static_cast<int>(i) + 1);
			}
			else if (currentDay.month > privacyDay.month)
				answer.push_back(static_cast<int>(i) + 1);
		}
		else if (currentDay.year > privacyDay.year)
			answer.push_back(static_cast<int>(i) + 1);
	}

	return answer;
}

int solution(vector<int> schedules, vector<vector<int>> timelogs, int startday)
{
	size_t length = timelogs.size();
	vector<bool> failed(length, false);
	int answer = static_cast<int>(length);

	for (auto& time : schedules)
	{
		time += 10;
		if (time % 100 >= 60)
		{
			time += 100;
			time -= 60;
		}
	}

	for (int day = 0; day < 7; day++)
	{
		if (startday > 5)
		{
			startday++;
			if (startday > 7) startday -= 7;
			continue;
		}

		for (size_t i = 0; i < length; i++)
		{
			if (failed[i])continue;
			if (schedules[i] < timelogs[i][day])
				failed[i] = true;

		}

		startday++;
	}

	for (auto log : failed)
		if (log) answer--;

	return answer;
}

vector<string> solution1(vector<string> players, vector<string> callings)
{
	vector<string>::iterator playerIterator;
	std::unordered_map<std::string, vector<string>::iterator> stringIndex;
	std::unordered_map<std::string, vector<string>::iterator>::iterator stringIndexIterator;

	for (auto iterator = players.begin(); iterator != players.end(); iterator++)
		stringIndex[*iterator] = iterator;

	vector<string> answer;
	std::swap(answer, players);

	for (auto& call : callings)
	{
		stringIndexIterator = stringIndex.find(call);
		playerIterator = stringIndexIterator->second;

		if (playerIterator != answer.begin())
		{
			stringIndex.find(*(playerIterator - 1))->second = playerIterator;
			stringIndexIterator->second = playerIterator - 1;
			std::iter_swap(playerIterator, playerIterator - 1);
		}
	}


	return answer;
}

string solution(string new_id)
{
	std::string answer;
	bool flag = false;

	for (auto& character : new_id)
	{
		if (isalnum(character))
		{
			answer.push_back(tolower(character));
			flag = false;
			continue;
		}

		if (character == '-' || character == '_')
		{
			answer.push_back(character);
			flag = false;
			continue;
		}

		if (character == '.')
		{
			if (flag)
				continue;

			answer.push_back(character);
			flag = true;
		}
	}

	while (answer.front() == '.')
		answer = answer.substr(1);

	while (answer.back() == '.')
		answer.pop_back();

	if(answer.empty())
		return "aaa";

	if (answer.size() >= 16)
	{
		answer = answer.substr(0, 15);
		while (answer.back() == '.')
			answer.pop_back();
	}

	while (answer.size() < 3)
		answer.push_back(answer.back());

	return answer;
}

int solution(vector<vector<int>> board, vector<int> moves)
{
	size_t length = board[0].size();
	std::vector<std::list<int>> boardStack(length);
	std::list<int> bucket;
	int slot, answer = 0;

	for (int i = static_cast<int>(board.size()) - 1; i >= 0; i--)
	{
		for (size_t j = 0; j < length; j++)
		{
			if (board[i][j] == 0)
				continue;

			boardStack[j].push_back(board[i][j]);
		}

	}

	for (auto& move : moves)
	{
		move--;
		if (boardStack[move].empty())
			continue;

		slot = boardStack[move].back();
		boardStack[move].pop_back();

		if (!bucket.empty() && bucket.back() == slot)
		{
			bucket.pop_back();
			answer++;
		}
		else
			bucket.push_back(slot);
	}
	
	return answer * 2;
}

template<typename Output, typename... Args>
std::list<std::function<Output(Args...)>> MakeRecursionStack(void){	return std::list<std::function<Output(Args...)>>();}

int fib(int size)
{
	std::unordered_map<int, int> dp;
	auto stack = MakeRecursionStack<void>();

	std::function<void(int)> fibona = [&](int cur)
		{
			if (dp.count(cur)) return;

			if (cur == 0 || cur == 1)
			{
				dp[cur] = cur;
			}
			else if (dp.count(cur - 1) && dp.count(cur - 2))
			{
				dp[cur] = dp[cur - 1] + dp[cur - 2];
			}
			else
			{
				if (!dp.count(cur - 1))
					stack.push_back([cur, &stack, &fibona] { fibona(cur - 1); }); // 재귀를 다른 방식으로 -> 스택에 함수 넣어서 범용적으로
				if (!dp.count(cur - 2))
					stack.push_back([cur, &stack, &fibona] { fibona(cur - 2); }); // 재귀를 다른 방식으로

				stack.push_back([cur, &dp] { dp[cur] = dp[cur - 1] + dp[cur - 2]; }); // 재귀를 다른 방식으로
			}
		};

	// 초기 호출
	stack.push_back([size, &fibona] { fibona(size); });

	while (!stack.empty())
	{
		auto cur = stack.back();
		stack.pop_back();
		cur(); // 함수 실행
	}

	return dp[size];
}


int solution(vector<vector<int>> triangle)
{
	int preHeight, length;

	for (int y = static_cast<int>(triangle.size()) - 2; y >= 0; y--)
	{
		preHeight = y + 1;
		length = static_cast<int>(triangle[y].size());
		for (int x = 0; x < length; x++)
		{
			if (triangle[preHeight][x] >= triangle[preHeight][x + 1])
				triangle[y][x] += triangle[preHeight][x];
			else
				triangle[y][x] += triangle[preHeight][x + 1];
		}
	}

	return triangle[0][0];
}



string solution(vector<string> survey, vector<int> choices)
{
	size_t length = survey.size();
	string answer(4, 'Z');
	std::vector<int> typeScore(4, 0);
	std::map<char, std::pair<int, int>> typeIndex = { {'R', {0, -1}}, {'T', {0, 1}}, {'C', {1, -1}}, {'F', {1, 1}}, {'J', {2, -1}}, {'M', {2, 1}}, {'A', {3, -1}}, {'N', {3, 1}} };
	std::map<char, std::pair<int, int>>::iterator typeIndexIterator;

	for (size_t index = 0; index < length; index++)
	{
		if (choices[index] == 4)
			continue;

		typeIndexIterator = typeIndex.find((choices[index] > 4) ? survey[index][1] : survey[index][0]);
		typeScore[typeIndexIterator->second.first] += std::abs(choices[index] - 4) * typeIndexIterator->second.second;
	}

	answer[0] = (typeScore[0] <= 0) ? 'R' : 'T';
	answer[1] = (typeScore[1] <= 0) ? 'C' : 'F';
	answer[2] = (typeScore[2] <= 0) ? 'J' : 'M';
	answer[3] = (typeScore[3] <= 0) ? 'A' : 'N';

	return answer;
}


int solution(int size, vector<vector<int>> computers)
{
	int answer = 0, currentNode;
	std::list<int> stack;
	std::vector<bool> visited(size, false);
	std::map<int, std::vector<int>> graph;

	for (int i = 0; i < size; i++)
	{
		graph[i] = {};
		for (int j = 0; j < size; j++)
		{
			if (i == j)
				continue;

			if(computers[i][j] == 1)
				graph[i].push_back(j);
		}
	}

	for (int i = 0; i < size; i++)
	{
		if (visited[i])
			continue;

		visited[i] = true;
		stack.push_back(i);

		while (!stack.empty())
		{
			currentNode = stack.back();
			stack.pop_back();
			for (auto& linkedNode : graph[currentNode])
			{
				if (visited[linkedNode])
					continue;

				visited[linkedNode] = true;
				stack.push_back(linkedNode);
			}
		}
		answer++;
	}

	return answer;
}

vector<int> solution1(vector<string> operations)
{
	std::multiset<int> set;
	std::multiset<int>::iterator setIterator;

	for (auto& order : operations)
	{

		switch (order[0])
		{
		case 'I':
			set.emplace(std::stoi(order.substr(2)));
			break;
		case 'D':
		{
			if (set.empty())
				break;

			switch (order[2])
			{
			case '-':
				set.erase(set.begin());
				break;
			default:
				set.erase(std::prev(set.end()));
				break;
			}
		}
		break;
		default:
			break;
		}
	}

	size_t length = set.size();

	if(length == 0)
		return { 0, 0 };
	if(length == 1)
		return { *set.begin(), *set.begin() };

	return { *std::prev(set.end()), *set.begin() };
}

long long solution(int size, vector<int> works) // 정렬이 필요하다 하면 자기가 직접 정렬하거나 std::sort를 쓰는 것보다 set이나 priority_queue를 쓰는게 더 빠른 경우가 있을 수 있으니 주의
{
	std::multiset<int> set;
	std::multiset<int>::iterator setIterator;
	long long answer = 0;
	int length = static_cast<int>(works.size()), storage;

	for (int i = 0; i < length; i++)
		set.emplace(works[i]);

	for (int i = 0; i < size; i++)
	{
		setIterator = std::prev(set.end());
		if (*setIterator <= 0)
			break;
		storage = *setIterator;
		storage--;
		set.erase(setIterator);
		set.emplace(storage);
	}

	for (auto& amount : set)
		answer += static_cast<long long>(amount) * static_cast<long long>(amount);

	return answer;
}

bool Match(const std::string& one, const std::string& other, size_t wordLength)
{

	bool noConnection = false, mismatch = false;
	for (size_t k = 0; k < wordLength; k++)
	{
		if (one[k] == other[k])
			continue;

		if (mismatch)
		{
			noConnection = true;
			break;
		}
		else
			mismatch = true;
	}

	if (noConnection)
		return false;

	return true;
}

int solution(string begin, string target, vector<string> words)
{
	size_t wordLength = begin.size(); int length = static_cast<int>(words.size());
	std::map<int, std::vector<int>> wordMap;
	std::vector<bool> visited(length, false);
	wordMap[-1] = std::vector<int>(); // begin
	bool notFind = true;

	if (Match(target, begin, wordLength))
		return 1;

	for (int i = 0; i < length; i++)
	{
		wordMap[static_cast<int>(i)] = std::vector<int>();
		for (int j = i + 1; j < length; j++)
		{
			if (Match(words[i], words[j], wordLength))
			{
				wordMap[i].push_back(j);
				wordMap[j].push_back(i);
			}
		}

		if (Match(words[i], begin, wordLength))
			wordMap.find(-1)->second.push_back(i);

		if (Match(words[i], target, wordLength))
		{
			wordMap.find(i)->second.push_back(-2);
			if (words[i] == target)
				notFind = false;
		}
	}
	if(notFind)
		return 0;

	std::list<std::pair<int, int>> queue;
	int depth = std::numeric_limits<int>::max(), index, currentDepth;

	for (auto& word : wordMap.find(-1)->second)
	{
		visited[word] = true;
		queue.push_back({ word, 1 });
	}

	while (!queue.empty())
	{
		index = queue.front().first;
		currentDepth = queue.front().second;
		queue.pop_front();

		if (index == -2)
		{
			depth = std::min(depth, currentDepth);
			continue;
		}

		for (auto& connectedIndex : wordMap.find(index)->second)
		{
			if (connectedIndex != -2)
			{
				if (visited[connectedIndex])
					continue;
				else
					visited[connectedIndex] = true;
			}
			queue.push_back({ connectedIndex, currentDepth + 1 });
		}
	}

	return depth;
}

int solution2(int size, vector<vector<int>> edge)
{
	std::map<int, std::vector<int>> graph;
	std::map<int, std::list<int>> leaves;

	std::vector<int> visited(size, -1);
	visited[0] = 0;
	std::list<int> queue{ 0 };
	int currentNode, currentDepth;
	bool leaf = false;

	for (auto& line : edge)
	{
		graph[line[0] - 1].push_back(line[1] - 1);
		graph[line[1] - 1].push_back(line[0] - 1);
	}

	while (!queue.empty())
	{
		currentNode = queue.front();
		queue.pop_front();
		currentDepth = visited[currentNode];
		currentDepth++;
		leaf = true;

		for (auto& connected : graph.find(currentNode)->second)
		{
			if (visited[connected] != -1)
				continue;

			visited[connected] = currentDepth;
			queue.push_back(connected);
			leaf = false;
		}

		if (leaf)
			leaves[visited[currentNode]].push_back(currentNode);
	}

	return static_cast<int>(std::prev(leaves.end())->second.size());
}

string solution(int size, int t, int m, vector<string> timetable)
{
	string answer;

	std::vector<std::vector<int>> bus(size);
	std::multiset<int> busTimeSet;

	for (auto& time : timetable)
		busTimeSet.insert(std::stoi(time.substr(0, 2)) * 100 + std::stoi(time.substr(3, 2)));
	int currentTime, store;

	for (int i = 0; i < size; i++)
	{
		store = i * t;
		currentTime = 900 + (store / 60) * 100 + store % 60;
		for (auto iterator = busTimeSet.begin(); iterator != busTimeSet.end(); )
		{
			if (currentTime < *iterator)
				break;

			if (bus[i].size() >= m)
				break;

			bus[i].push_back(*iterator);

			iterator = busTimeSet.erase(iterator);
			//iterator++;
		}
	}
	store = size - 1;
	auto iterator = std::prev(bus.end());

	if (std::prev(bus.end())->size() < m)
	{
		store *= t;
		int hour = 9 + (store / 60);
		int minute = store % 60;

		answer = to_string(hour / 10) + to_string(hour % 10) + ":" + to_string(minute / 10) + to_string(minute % 10);
	}
	else
	{
		std::sort(iterator->begin(), iterator->end());
		int hour = (*std::prev(iterator->end())) / 100;
		int minute = (*std::prev(iterator->end())) % 100;
		if (minute == 0)
		{
			hour--;
			answer = to_string(hour / 10) + to_string(hour % 10) + ":59";
		}
		else
		{
			minute--;
			answer = to_string(hour / 10) + to_string(hour % 10) + ":" + to_string(minute / 10) + to_string(minute % 10);
		}
	}


	return answer;
}


void Process(int leftMoney, int coinIndex, std::vector<int>& money, int& output)
{
	if (coinIndex == 0)
	{
		if (leftMoney % money[coinIndex] == 0)
			output++;
		return;

	}

	int count = leftMoney / money[coinIndex];

	for (int i = 0; i <= count; i++)
	{
		Process(leftMoney - money[coinIndex] * i, coinIndex - 1, money, output);
	}

}


int solution1(int size, vector<int> money)
{
	std::vector<int> array(size + 1, 0);
	array[0] = 1;
	for (auto& coin : money)
	{ // i가 결과값
		for (int i = coin; i <= size; i++) // 0에서부터 coin을 더할 수 있는 경우의 수를 이렇게 표현함, 예를 들어 j + coin가 n이 초과되지 않으면 무조건 성립하는 경우의 수이니 그걸  j + coin의 경우의 숫자 카운트에 +1 해주는거
		{
			array[i] += array[i - coin];
		}
	}

	return array[size];
}


int solution1(int size, vector<vector<int>> results)
{
	std::vector<std::vector<int>> resultTable(size, std::vector<int>(size, 0)); // 1: 이김, -1: 짐, 0: 모름 => 0이 자기자신 제외 없으면 됨

	for (auto& result : results) // 관계 적립
	{
		resultTable[result[0] - 1][result[1] - 1] = 1;
		resultTable[result[1] - 1][result[0] - 1] = -1;
	}

	for (int k = 0; k < size; k++) // 중간 대상
	{
		for (int j = 0; j < size; j++) // 도착 대상
		{
			for (int i = 0; i < size; i++) // 출발 대상
			{
				if (resultTable[i][k] == 1 && resultTable[k][j] == 1) // i가 k를 이기고 k가 j를 이기면 i는 j를 이긴다
				{
					resultTable[i][j] = 1;
					resultTable[j][i] = -1;
				}
			}

		}
	}
	bool unknown;
	int answer = 0;

	for (int i = 0; i < size; i++) // 검사
	{
		unknown = false;
		for (int j = 0; j < size; j++)
		{
			if (i == j)
				continue;

			if (resultTable[i][j] == 0)
			{
				unknown = true;
				break;
			}
		}
		if(!unknown)
			answer++;
	}

	return answer;
}

int solution2(vector<vector<int>> routes)
{
	int answer = 0, length = static_cast<int>(routes.size()), point;

	std::sort(routes.begin(), routes.end(), [](const auto& left, const auto& right) { return left[1] < right[1]; });

	for (int i = 0; i < length; )
	{
		point = routes[i][1];
		i++;

		while (i != length)
		{
			if (routes[i][0] > point)
				break;

			i++;
		}
		answer++;
	}

	return answer;
}


vector<int> solution(int size, int s)
{
	if (size > s)
		return { -1 };

	vector<int> answer(size, s / size);
	int left = s % size;

	while (left != 0)
	{
		for (int i = size - 1; i >= 0; i--)
		{
			answer[i]++;
			left--;
			if (left == 0)
				break;
		}
	}

	return answer;
}
int solution1(vector<int> cookie)
{
	int leftSum, rightSum, left, right, result = 0;
	int length = static_cast<int>(cookie.size()), rightLimit = length - 1;

	for (int i = 1; i < length; i++)
	{
		left = i - 1; right = i;
		rightSum = cookie[right];
		leftSum = cookie[left];

		while (left >= 0 || right <= rightLimit)
		{
			if (leftSum == rightSum)
				result = std::max(leftSum, result);
			if (leftSum > rightSum)
			{
				if (right >= rightLimit)
					break;

				right++;
				rightSum += cookie[right];
			}
			else
			{
				if (left <= 0)
					break;

				left--;
				leftSum += cookie[left];
			}
		}
	}

	return result;
}
//int solution(vector<string> words)
//{
//	struct TriNode
//	{
//		std::map<char, TriNode> child;
//		int branchCount = 0;
//		bool isEndwordExsist = false;
//	};
//
//	std::map<char, TriNode> triMapRoot;
//	TriNode* currentNode;
//
//	for (auto& word : words)
//	{
//		if(triMapRoot.find(word[0]) == triMapRoot.end())
//			triMapRoot.emplace(word[0], TriNode{});
//
//		currentNode = &triMapRoot[word[0]];
//
//		for (auto& character : word)
//		{
//			if (currentNode->child.find(character) == currentNode->child.end())
//				currentNode->child.emplace(character, TriNode{});
//
//			currentNode = &currentNode->child[character];
//		}
//
//		currentNode->isEndwordExsist = true;
//	}
//
//
//
//	std::list<std::pair<std::pair<TriNode*, int>, TriNode*>> stack;
//
//
//	for (auto& node : triMapRoot)
//		stack.push_back({ {&node.second, 1}, nullptr });
//
//	TriNode* backTrace;
//	while (!stack.empty())
//	{
//		currentNode = stack.front().first.first;
//		backTrace = stack.front().second;
//		stack.pop_front();
//
//		if (currentNode->isEndwordExsist)
//			backTrace = currentNode;
//
//		if (currentNode->child.empty())
//		{
//			bool duplicated = false;
//			for (auto& endNode : leaf)
//			{
//				duplicated |= (endNode == backTrace);
//			}
//			if (!duplicated)
//				leaf.push_back(backTrace);
//		}
//		else
//		{
//			for (auto& child : currentNode->child)
//				stack.push_back({ &child.second, backTrace });
//		}
//	}
//
//
//
//
//	int answer = 0;
//	return answer;
//}

int solution(int m, int size, vector<vector<int>> puddles)
{
	vector<vector<int>> map(size, vector<int>(m, 0));
	for (auto& puddle :puddles) // 사전 세팅
		map[puddle[1] - 1][puddle[0] - 1] = -1;

	map[0][0] = 1; // 시작점 세팅

	for (int y = 0; y < m; y++)
	{
		for (int x = 0; x < size; x++)
		{
			if (map[x][y] == -1) // 웅덩이 스킵
				continue;

			if (x > 0 && map[x - 1][y] != -1) // 이전 좌표들의 값을 합산(그림 기준 좌측과 상단)
				map[x][y] += map[x - 1][y];
			if (y > 0 && map[x][y - 1] != -1)
				map[x][y] += map[x][y - 1];

			map[x][y] = map[x][y] % 1000000007;
		}
	}

	return map[size - 1][m - 1];
}

int solution(std::vector<int> info, std::vector<std::vector<int>> edges)
{
	struct State
	{
		int index;
		int sheep;
		int wolves;
		std::vector<bool> candidates;
	};

	int answer = 0, size = static_cast<int>(info.size()), index, sheep, wolves;
	std::vector<std::vector<int>> graph(size);
	std::stack<State> stack;

	for (auto& line : edges)
		graph[line[0]].push_back(line[1]);

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

//int solution(vector<int> info, vector<vector<int>> edges)
//{
//	int answer = 0;
//
//	const int size = info.size();
//
//	std::unordered_map<int, std::vector<int>> graph;
//	for (const auto& ele : edges)
//	{
//		graph[ele[0]].push_back(ele[1]);
//	}
//
//	std::vector<bool> candidates(size, false);
//	std::function<void(int, int, int)> bfs = [&](int num, int sheep, int wolves)
//		{
//			std::cout << "num: " << num << ", sheep: " << sheep << ", wolves: " << wolves << std::endl;
//			if (0 < sheep && sheep <= wolves)
//			{
//				std::cout << "return " << "num: " << num << ", sheep: " << sheep << ", wolves: " << wolves << std::endl;
//				return;
//			}
//
//			if (info[num])
//			{
//				wolves += 1;
//			}
//			else
//			{
//				sheep += 1;
//			}
//
//			answer = std::max(answer, sheep);
//
//			for (const auto& ele : graph[num])
//			{
//				candidates[ele] = true;
//			}
//
//			for (int i = 0; i < size; ++i)
//			{
//				if (candidates[i])
//				{
//					candidates[i] = false;
//					bfs(i, sheep, wolves);
//					candidates[i] = true;
//				}
//			}
//
//			for (const auto& ele : graph[num])
//			{
//				candidates[ele] = false;
//			}
//			std::cout << "end " << "num: " << num << ", sheep: " << sheep << ", wolves: " << wolves << std::endl;
//		};
//
//	bfs(0, 0, 0);
//
//	return answer;
//}

int solution(int temperature, int t1, int t2, int a, int b, vector<int> onboard)
{
	const int compensation = 10;
	t1 += compensation;
	t2 += compensation;
	temperature += compensation;

	int timeLength = static_cast<int>(onboard.size()), maxTemperature = 41 + compensation, stay, upper, lower;
	std::vector<std::vector<int>> table(timeLength, std::vector<int>(maxTemperature, std::numeric_limits<int>::max()));

	table[0][temperature] = 0;
	if (temperature != 0)
		table[0][temperature - 1] = a;
	if (temperature != maxTemperature - 1)
		table[0][temperature + 1] = a;


	for (int i = 0; i < timeLength - 1; i++)
	{
		for (int t = 0; t < maxTemperature - 1; t++)
		{
			if (table[i][t] == std::numeric_limits<int>::max())
				continue;

			if (onboard[i] != 0 && !(t1 <= t && t <= t2))
			{
				table[i][t] = std::numeric_limits<int>::max();
				continue;
			}

			// 전력 비용 계산
			stay = (t == temperature) ? 0 : b;
			upper = (t > temperature) ? 0 : a;
			lower = (t < temperature) ? 0 : a;

			//유지
			table[i + 1][t] = std::min(table[i + 1][t], table[i][t] + stay);

			//낮춤
			if (t != 0)
				table[i + 1][t - 1] = std::min(table[i + 1][t - 1], table[i][t] + upper);

			//올림
			if (t != maxTemperature - 1)
				table[i + 1][t + 1] = std::min(table[i + 1][t + 1], table[i][t] + lower);

		}

	}

	int answer = std::numeric_limits<int>::max();
	for (int t = 0; t < maxTemperature; t++)
	{
		if (onboard.back() != 0 && !(t1 <= t && t <= t2))
			continue;
		answer = std::min(answer, table[timeLength - 1][t]);
	}

	return answer;
}



string solution2(vector<string> participant, vector<string> completion)
{
	std::multiset<std::string> set;
	std::multiset<std::string>::iterator setIterator;

	for (auto& name : participant)
		set.insert(name);

	for (auto& name : completion)
	{
		setIterator = set.find(name);
		if (setIterator != set.end())
			set.erase(setIterator);
	}

	return *set.begin();
}


int solution(vector<int> nums)
{
	int maxCount = static_cast<int>(nums.size()) / 2;
	std::map<int, int> pokemonDictionary;
	std::map<int, int>::iterator iterator;

	for (auto& number : nums)
	{
		iterator = pokemonDictionary.find(number);
		if(iterator != pokemonDictionary.end())
		iterator->second += 1;
		else
			pokemonDictionary[number] = 1;
	}

	if (maxCount > static_cast<int>(pokemonDictionary.size()))
		return static_cast<int>(pokemonDictionary.size());
	else
		return maxCount;
}

bool solution(vector<string> phone_book)
{
	std::sort(phone_book.begin(), phone_book.end());
	int length = static_cast<int>(phone_book.size());
	int size, j;
	length--;
	for (int i = 0; i < length; i++)
	{
		size = std::min(static_cast<int>(phone_book[i].size()), static_cast<int>(phone_book[i + 1].size()));
		for (j = 0; j < size; j++)
		{
			if (phone_book[i][j] != phone_book[i + 1][j])
				break;

		}
		if (j == size)
			return false;
	}

	return true;
}


int solution(vector<vector<string>> clothes)
{
	std::map<std::string, int> clothDictionary;
	std::map<std::string, int>::iterator iterator;
	int answer = 1;

	for (auto& cloth : clothes)
	{
		iterator = clothDictionary.find(cloth[1]);
		if(iterator == clothDictionary.end())
			clothDictionary[cloth[1]] = 1;
		else
			iterator->second++;
	}

	for (auto& cloth : clothDictionary)
		answer *= (cloth.second + 1);

	answer--;
	return answer;
}

vector<int> solution2(vector<string> genres, vector<int> plays)
{
	int length = static_cast<int>(genres.size());
	vector<int> answer;
	std::map<std::string, std::pair<int, std::vector<std::pair<int, int>>>> genreMap;
	std::map<std::string, std::pair<int, std::vector<std::pair<int, int>>>>::iterator genreMapIterator;
	std::vector<std::map<std::string, std::pair<int, std::vector<std::pair<int, int>>>>::iterator> salesMap;
	std::vector<std::pair<int, int>>::iterator salesIterator;
	for (int i = 0; i < length; i++)
	{
		genreMapIterator = genreMap.find(genres[i]);
		if (genreMapIterator == genreMap.end())
		{
			genreMap[genres[i]] = { 0, std::vector<std::pair<int, int>>() };
			genreMapIterator = genreMap.find(genres[i]);
			salesMap.push_back(genreMapIterator);
		}
		genreMapIterator->second.first += plays[i];
		genreMapIterator->second.second.push_back({ plays[i], i });
	}

	for (auto& item : genreMap)
		std::sort(item.second.second.begin(), item.second.second.end(), [](const auto& left, const auto& right) { if (left.first == right.first) return left.second < right.second; return left.first > right.first; });

	std::sort(salesMap.begin(), salesMap.end(), [](const auto& left, const auto& right) { return left->second.first > right->second.first; });

	length = static_cast<int>(salesMap.size());
	for (int i = 0; i < length; i++)
	{
		salesIterator = salesMap[i]->second.second.begin();

		answer.push_back(salesIterator->second);
		salesIterator++;
		if (salesIterator != salesMap[i]->second.second.end())
			answer.push_back(salesIterator->second);
	}

	return answer;
}

vector<int> solution2(vector<int> arr)
{
	std::vector<int> answer;
	answer.reserve(arr.size());

	for (auto& number : arr)
	{
		if(!answer.empty() && answer.back() != number)
			answer.push_back(number);
	}

	return answer;
}

vector<int> solution1(vector<int> progresses, vector<int> speeds)
{
	int i = 0, leftPrograss = 100 - progresses[i], length = static_cast<int>(progresses.size());
	int work = leftPrograss / speeds[i] + ((leftPrograss % speeds[i] > 0) ? 1 : 0);
	int count = 1, workAmount = work;
	vector<int> answer;
	i++;

	for (; i < length; i++)
	{
		leftPrograss = 100 - progresses[i];
		work = leftPrograss / speeds[i] + ((leftPrograss % speeds[i] > 0) ? 1 : 0);

		if (workAmount < work)
		{
			answer.push_back(count);
			workAmount = work;
			count = 1;
		}
		else
			count++;
	}
	answer.push_back(count);

	return answer;
}

int solution2(int N, int number)
{
	std::vector<std::set<int>> table(9, std::set<int>());
	int storage;
	for (int i = 1; i < 9; i++)
	{
		storage = 0;
		for (int j = 0; j < i; j++)
			storage = storage * 10 + N;

		table[i].insert(storage);

		for (int j = 1; j < i; j++)
		{
			for (auto& item : table[j])
			{
				for (auto& nextItem : table[i - j])
				{
					table[i].insert(item + nextItem);
					table[i].insert(item - nextItem);
					table[i].insert(item * nextItem);
					if(nextItem != 0)
						table[i].insert(item / nextItem);
				}

			}
		}

		if (table[i].find(number) != table[i].end())
			return i;
	}

	return -1;
}

vector<vector<int>> solution4(vector<vector<int>> nodeinfo)
{
	auto preorder = [](const std::vector<std::vector<int>>& tree, int root, std::vector<int>& output) -> void
		{
			std::list<int> stack{ root };
			int currentNode;

			while (!stack.empty())
			{
				currentNode = stack.back();
				stack.pop_back();
				output.push_back(currentNode + 1);


				for (auto iterator = tree[currentNode].rbegin(); iterator != tree[currentNode].rend(); iterator++)
					stack.push_back(*iterator);
			}
		};
	auto postorder = [](const std::vector<std::vector<int>>& tree, int root, std::vector<int>& output) -> void
		{
			std::list<int> stackOne{ root }, stackTwo;
			int currentNode;

			while (!stackOne.empty())
			{
				currentNode = stackOne.back();
				stackOne.pop_back();
				stackTwo.push_back(currentNode);

				for (auto& node : tree[currentNode])
				{
					stackOne.push_back(node);
				}
			}

			while (!stackTwo.empty())
			{
				output.push_back(stackTwo.back() + 1);
				stackTwo.pop_back();
			}
		};
	/*트리 구조 제작, x가 */
	int length = static_cast<int>(nodeinfo.size());
	std::list<int> nodes(length, 0);
	std::vector<std::vector<int>> tree(length);
	vector<vector<int>> answer(2, vector<int>());
	int index = 0, root;

	for (auto iterator = nodes.begin(); iterator != nodes.end(); iterator++)
	{
		*iterator = index;
		index++;
	}

	nodes.sort([&](const auto& left, const auto& right) { return nodeinfo[left][1] < nodeinfo[right][1]; });
	root = nodes.back();
	nodes.pop_back();
	std::list<std::pair<int, std::list<int>>> queue{ {root, nodes} };

	while (!queue.empty())
	{
		std::pair<int, std::list<int>> current = queue.back();
		queue.pop_back();
		nodes.clear();
		int seperator = std::numeric_limits<int>::max();
		std::list<int> innerList;
		for (int i = 0; i < 2; i++)
		{
			if (current.second.empty())
				break;

			index = current.second.back();
			current.second.pop_back();

			if (i == 0)
			{
				nodes.push_back(index);
				continue;
			}
			if (nodeinfo[nodes.back()][1] != nodeinfo[index][1])
			{
				current.second.push_back(index);
				break;
			}

			seperator = nodeinfo[current.first][0];
			if (nodeinfo[nodes.back()][0] > nodeinfo[index][0])
				nodes.push_back(index);
			else
				nodes.push_front(index);
		}

		while (!nodes.empty())
		{
			innerList.clear();
			index = nodes.back();
			nodes.pop_back();
			tree[current.first].push_back(index);

			for (auto iterator = current.second.begin(); iterator != current.second.end(); )
			{
				if (nodeinfo[*iterator][0] < seperator)
				{
					innerList.push_back(*iterator);
					iterator = current.second.erase(iterator);
				}
				else
					iterator++;
			}

			queue.push_back({ index, innerList });
			seperator = std::numeric_limits<int>::max();
		}
	}
	
	preorder(tree, root, answer[0]);
	postorder(tree, root, answer[1]);


	return answer;
}


bool solution1(string s)
{
	int balance = 0;

	for (auto& character : s)
	{
		if(character == 'y' || character == 'Y')
			balance++;
		else if(character == 'p' || character == 'P')
			balance--;
	}

	return balance == 0;
}

int solution(vector<int> numbers, int target)
{
	int answer = 0;
	int size = 1 << numbers.size(); // 이건 모든 경우의 수인 2^number.size()를 말함, 문제는 한 넘버당 2가지 경우의 수(+, -)밖에 없으니 가능
	// 비트마스킹 방법
	for (int i = 1; i < size; i++) // i는 어디 자리에 1을 놓아서 -를 만들고 +를 만들지 선택하는거, ex) i = 2이고 numbers.size() = 3일때 i = 010 => 두번째만 -을 하는거
	{
		int temp = 0;
		for (int j = 0; j < numbers.size(); j++) // j는 이진수 기준 해당 자릿수를 나타냄
		{
			if (i & (1 << j)) // i의 j번째 자리수가 1인가 0인가, i와 2^j의 and연산으로 함
			{
				temp -= numbers[j];
			}
			else temp += numbers[j];
		}
		if (temp == target) answer++;
	}
	return answer;
	// 일반 방법
	int /*answer = 0,*/ length = static_cast<int>(numbers.size());
	std::list<std::pair<int, int>> stack{ { 0, numbers.front() }, { 0, numbers.front() * -1 } };
	std::pair<int, int> currentNode;
	while (!stack.empty())
	{
		currentNode = stack.back();
		stack.pop_back();
		currentNode.first++;
		if (currentNode.first == length)
		{
			std::cout << to_string(currentNode.second) << std::endl;
			if (currentNode.second == target)
				answer++;
			continue;
		}

		stack.push_back({ currentNode.first, currentNode.second + numbers[currentNode.first]});
		stack.push_back({ currentNode.first, currentNode.second - numbers[currentNode.first]});
	}

	return answer;
}

int solution(vector<vector<int>> rectangle, int characterX, int characterY, int itemX, int itemY)
{
	const int maxArea = 51, compensation = 2;
	int simArea = maxArea * compensation;
	int answer = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> route(simArea, std::vector<int>(simArea, 0)); // 사각형 외각선을 1, 안쪽은 -1
	int minX, maxX, minY, maxY;
	characterX *= compensation;
	characterY *= compensation;
	itemX *= compensation;
	itemY *= compensation;

	for (auto& single : rectangle)
	{
		minX = single[0] * compensation;
		maxX = single[2] * compensation;
		minY = single[1] * compensation;
		maxY = single[3] * compensation;

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				if (route[x][y] == -1)
					continue;

				if (x == minX || x == maxX || y == minY || y == maxY)
					route[x][y] = 1;
				else
					route[x][y] = -1;
			}
		}
	}

	std::list<std::pair<std::pair<int, int>, int>> queue{ {{ characterX, characterY }, 0} };
	route[characterX][characterY] = 2;

	int currentX, currentY;
	while (!queue.empty())
	{
		std::pair<std::pair<int, int>, int> currentNode = queue.front();
		queue.pop_front();

		route[currentNode.first.first][currentNode.first.second] = 2;

		if (currentNode.first.first == itemX && currentNode.first.second == itemY)
		{
			answer = std::min(answer, currentNode.second);
			continue;
		}

		currentNode.second++;

		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				if (std::abs(x + y) != 1)
					continue;

				currentX = x + currentNode.first.first;
				currentY = y + currentNode.first.second;
				if (!((0 <= currentX && currentX < simArea) && (0 <= currentY && currentY < simArea)) || route[currentX][currentY] != 1)
					continue;

				queue.push_back({ { currentX, currentY }, currentNode.second });
			}
		}

	}

	return answer / 2 + ((answer % 2 == 1) ? 1 : 0);
}



int solution3(vector<vector<int>> matrix_sizes)  // 문제가 상당히 쓰레기임, 필수적인 설명이 빠져있음, matrix_sizes 순서 그대로 곱한다는 전제로 함
{
	// https://school.programmers.co.kr/learn/courses/30/lessons/12942
	// https://breath-in317.tistory.com/entry/%EC%97%B0%EC%87%84-%ED%96%89%EB%A0%AC-%EA%B3%B1%EC%85%88Matrix-Chain-Multiplication-%EC%95%8C%EA%B3%A0%EB%A6%AC%EC%A6%98-DP
	// 연쇄 행렬 곱셈
	// 점화식 : M(i,j) = (i≤k<j) min​(M(i, k) + M(k+1, j) + ri​ * ck​ * cj​)​
	// min​((같은 행과 같은 열에 있는 애들끼리 가능한 조합을 골라 결과를 더함) + (고른 조합들의 중간에 있는 행렬들의 곱 결과 값)​)​
	// 이건 그냥 수식이다. 근의 공식 쓰듯이 하면 될거 같은데 이거 외워서 쓸 일이 많을까?

	int length = static_cast<int>(matrix_sizes.size()), i, j, k;
	std::vector<std::vector<int>> table(length, std::vector<int>(length, std::numeric_limits<int>::max()));
	std::vector<int> matrixMap(length + 1); // 편의상 생성, 모든 수를 한번에 처리하고 싶어서 함

	for (i = 0; i < length; i++)
	{
		matrixMap[i] = matrix_sizes[i][0];
		table[i][i] = 0;
	}

	matrixMap[length] = matrix_sizes[length - 1][1];

	for (int l = 2; l <= length; l++) // 행렬 곱을 어디까지 진행했나(l이 2이면 2번째까지의 행렬곱셈중임)
	{
		for (i = 0; i <= length - l; i++)
		{
			j = l + (i - 1); // 가능한 조합 생성
			for (k = i; k < j; k++)
				table[i][j] = std::min(table[i][j], table[i][k] + table[k + 1][j] + matrixMap[i] * matrixMap[k + 1] * matrixMap[j + 1]);
		}
	}

	return table.front().back();
}

int solution2(string name)
{
	int answer = 0, length = static_cast<int>(name.size());
	int range = static_cast<int>('Z' - 'A') + 1;
	int min = length - 1, reverseIndex, reverse;

	for (int i = 0; i < length; i++)
	{
		answer += std::min((name[i] - 'A'), range - (name[i] - 'A'));

		reverseIndex = i + 1;

		while (name[reverseIndex] == 'A')
			reverseIndex++;

		reverse = length - reverseIndex;

		min = std::min(min, std::min(i * 2 + reverse, reverse * 2 + i));
	}
	answer += min;

	return answer;
}



int solution(int h1, int m1, int s1, int h2, int m2, int s2)
{// https://school.programmers.co.kr/learn/courses/30/lessons/250135
	int answer = 0;
	int hour = h2 - h1;

	if (hour == 0)
	{
		int minute = m2 - m1;

		if (m1 < h1 * 5 && h1 * 5 < m2)
			minute -= 1;

		return minute;
	}

	int preMinute = 60 - m1 - ((m1 < s1) ? 1 : 0);

	if (m1 <= ((h1 >= 12) ? (h1 - 12) * 5 : h1 * 5))
		preMinute -= 1;

	int aftMinute = m2 + ((m2 <= s2) ? 1 : 0);

	if (m2 >= ((h2 >= 12) ? (h2 - 12) * 5 : h2 * 5))
		aftMinute -= 1;

	int plusMinute = (hour - 1) * 59 + (hour - 1) * 60;

	answer = plusMinute + preMinute + aftMinute;

	return answer;
}


vector<int> solution5(vector<vector<int>> edges)
{
	std::map<int, std::vector<int>> graph;
	std::map<int, std::pair<int, int>> inOut;
	std::vector<int> answer(4, 0);

	for (auto& edge : edges)
	{
		graph[edge[0]].push_back(edge[1]);
		inOut[edge[0]].second++;
		inOut[edge[1]].first++;
	}

	for (auto& status : inOut)
	{
		if (status.second.first == 0 && status.second.second >= 1)
		{
			answer[0] = status.first;
			break;
		}
	}

	std::vector<bool> visited(inOut.size() + 1, false);
	visited[answer[0]] = true;

	for (auto& node : graph.find(answer[0])->second)
	{
		std::list<int> stack{ node };
		visited[node] = true;

		while (!stack.empty())
		{
			int currentNode = stack.back();
			stack.pop_back();
			auto iterator = inOut.find(currentNode);

			if (iterator->second.second >= 2)
			{
				stack.push_back(node);
				answer[3]++;
				break;
			}

			if (iterator->second.second == 0 && iterator->second.first > 0)
			{
				stack.push_back(node);
				answer[2]++;
				break;
			}

			for (auto& other : graph[currentNode])
			{
				if (visited[other])
					continue;

				visited[other] = true;
				stack.push_back(other);
			}
		}

		if(stack.empty())
			answer[1]++;
		//std::cout << answer[0] << answer[1] << answer[2] << answer[3] << std::endl;

	}

	return answer;
}

std::vector<int> solution(int A, int B, int C)
{
	std::vector<int> answer(4);

	answer[0] = ((A + B) % C);
	answer[1] = ((A % C) + (B % C)) % C;
	answer[2] = (A * B) % C;
	answer[3] = ((A % C) * (B % C)) % C;

	return answer;
}
/*
int solution2(vector<int> people, int limit)
{
	int answer = 0, length = static_cast<int>(people.size());
	std::sort(people.begin(), people.end());
	int left = 0, right = length - 1;

	while (left <= right)
	{
		right--;
		if (people[right] + people[left] <= limit)
			left++;
		answer++;
	}

	return answer;
}*/
int solution2(vector<int> people, int limit)
{
	int answer = 0, length = static_cast<int>(people.size());
	std::sort(people.begin(), people.end());
	int left = 0, right = length - 1;

	while (left <= right)
	{
		cout << left << ":" << right << "	";
		cout << people[right] + people[left];
		if (people[right] + people[left] <= limit)
		{
			left++;
			cout << "	left+";
		}
		right--;
		answer++;
		cout << endl;
	}

	return answer;
}

long long solution(int cap, int size, vector<int> deliveries, vector<int> pickups)
{
	long long answer = 0;
	int deliver = 0, pickup = 0;

	for (int i = size - 1; i >= 0; i--)
	{
		int count = 0;
		deliver += deliveries[i];
		pickup += pickups[i];

		while (deliver >= 0 && pickup >= 0)
		{
			deliver -= cap;
			pickup -= cap;
			count++;
		}

		answer += (i + 1) * 2 * count;
	}
	return answer;
}

//int main(void)
//{
//	Output(solution(4, 5, { 1, 0, 3, 1, 2 }, {0, 3, 0, 4, 0 }));
//	int alarm = 0;
//}


/*
#include <string>
#include <list>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>
#include <stack>
#include <functional>
#include <memory>
#include <unordered_map>
#include <set>
*/

/*못푼거*/


int solution(int size, vector<int> stations, int w)
{
	int answer = 0, index = 0, areaMin = stations[index] - w - 1, areaMax = stations[index] + w - 1, area = 0, range = w * 2 + 1, length = static_cast<int>(stations.size());

	for (int i = 0; i < size; i++)
	{
		if (i <= areaMin)
		{
			if (areaMin == i)
			{
				areaMax = stations.front() + w - 1;
				answer += static_cast<int>(std::ceil(static_cast<float>(area) / static_cast<float>(range)));
				area = 0;
			}
			else
				area++;
		}
		else if (i == areaMax)
		{
			index++;
			if (index == length)
				areaMin = areaMax = std::numeric_limits<int>::max();
			else
			{
				areaMin = stations[index] - w - 1;
				areaMax = stations[index] + w - 1;
			}
		}
	}

	answer += static_cast<int>(std::ceil(static_cast<float>(area) / static_cast<float>(range)));
	return answer;
}


int solution1(vector<vector<int>> land, int height)
{
	int length = static_cast<int>(land.size()), answer = 0, index = 0;
	std::vector<std::vector<int>> blockInfomation(length, std::vector<int>(length, -1));
	std::vector<std::map<int, int>> difference;
	std::vector<std::pair<int, int>> stack;
	std::vector<std::pair<bool, int>> visited;

	auto Input = [&](int x, int y, int currentHeight) -> void
		{
			if (x < 0 || x >= length || y < 0 || y >= length)
				return;

			if (blockInfomation[x][y] != -1)
				return;

			if (std::abs(land[x][y] - currentHeight) <= height)
			{
				blockInfomation[x][y] = index;
				stack.push_back({ x, y });
			}
		};

	auto Check = [&](int x, int y, int currentHeight, int currentGroup, std::map<int, int>& link) -> void
		{
			if (x < 0 || x >= length || y < 0 || y >= length)
				return;

			int group = blockInfomation[x][y];
			if (currentGroup == group)
				return;

			std::map<int, int>::iterator iterator = link.find(group);

			if (iterator == link.end())
				link[group] = std::abs(land[x][y] - currentHeight);
			else
				iterator->second = std::min(iterator->second, std::abs(land[x][y] - currentHeight));
		};

	std::function<void(int, int)> DFS = [&](int current, int cost) -> void
		{
			if (visited[current].second <= cost)
				return;

			visited[current].second = cost;
			for (auto& item : difference[current])
			{
				if (visited[item.first].first)
					continue;

				visited[item.first].first = true;
				DFS(item.first, cost + item.second);
				visited[item.first].first = false;
			}
		};

	for (int x = 0; x < length; x++)
	{
		for (int y = 0; y < length; y++)
		{
			if (blockInfomation[x][y] != -1)
				continue;

			difference.push_back({});

			blockInfomation[x][y] = index;
			stack.push_back({ x, y });

			while (!stack.empty())
			{
				std::pair<int, int> position = stack.back();
				stack.pop_back();

				Input(position.first - 1, position.second, land[position.first][position.second]);
				Input(position.first + 1, position.second, land[position.first][position.second]);
				Input(position.first, position.second - 1, land[position.first][position.second]);
				Input(position.first, position.second + 1, land[position.first][position.second]);
			}

			index++;
		}
	}

	visited.resize(difference.size(), { false, std::numeric_limits<int>::max() });
	for (int x = 0; x < length; x++)
	{
		for (int y = 0; y < length; y++)
		{
			index = blockInfomation[x][y];

			Check(x - 1, y, land[x][y], index, difference[index]);
			Check(x + 1, y, land[x][y], index, difference[index]);
			Check(x, y - 1, land[x][y], index, difference[index]);
			Check(x, y + 1, land[x][y], index, difference[index]);
		}
	}

	DFS(0, 0);

	for (auto& item : visited)
		answer = std::max(answer, item.second);

	return answer;
}





// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.