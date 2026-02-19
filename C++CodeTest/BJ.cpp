#include <vector>
#include <list>
#include <algorithm>
#include <set>
#include <map>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <cmath>
#include <iomanip>
#include <functional>
#include <array>

std::string ToString(double value, int precision = 15) { std::ostringstream oss; oss << std::fixed << std::setprecision(precision) << value; return oss.str(); }
std::string ToString(float value, int precision = 7) { std::ostringstream oss; oss << std::fixed << std::setprecision(precision) << value; return oss.str(); }

void IOInitialize(void)
{
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		std::ios::sync_with_stdio(false);
		std::cin.tie(nullptr);
		std::cout.tie(nullptr);
	}
}

template<typename Type>
std::vector<Type> Input(void)
{
	IOInitialize();
	std::vector<Type> output;
	Type input;

	while (std::cin >> input)
		output.push_back(input);

	return output;
}

template<typename Type>
Type InputSingle(void)
{
	IOInitialize();
	Type input;
	std::cin >> input;

	return input;
}

template<typename Type>
void Output(const Type& item) { std::cout << item << std::endl; }
template<typename Type>
void Output(const std::vector<Type>& list)
{
	for (auto& item : list)
		std::cout << item << "\n";
}

template<typename Type>
void Output(const std::vector<std::vector<Type>>& list)
{
	for (auto& itemList : list)
	{
		size_t length = itemList.size(), end = length - 1;
		for (size_t i = 0; i < length; i++)
		{
			if (i != end)
				std::cout << itemList[i] << " ";
			else
				std::cout << itemList[i];
		}
		std::cout << "\n";
	}
	//std::cout.flush();
}

int Solution(const std::vector<std::string>& list);
int main(void)
{
	Output(Solution(Input<std::string>()));

	//std::cout << ToString(Solution(Input<double>()), 15) << "\n";
	//std::cout << ToString(Solution(Input<float>()), 7) << "\n";
	return 0;
}

int Solution(const std::vector<std::string>& list)
{
	int height = std::stoi(list[0]), width = std::stoi(list[1]), step = 1;
	std::vector<std::vector<int>> visiTable(height, std::vector<int>(width, 0));
	std::unordered_map<char, std::pair<int, int>> movement = { { 'U', {-1, 0} }, { 'D', { 1, 0} }, { 'L', { 0,-1} }, { 'R', { 0, 1} } };
	std::queue<std::pair<int, int>> queue;

	std::function<int(int, int, int)> Recursive;
	Recursive = [&](int i, int j, int currentStep) -> int
		{
			visiTable[i][j] = currentStep;
			std::pair<int, int> move = movement.find(list[i + 2][j])->second;
			
			int nextI = i + move.first;
			int nextJ = j + move.second;

			if (visiTable[nextI][nextJ] == 0)
				visiTable[i][j] = Recursive(nextI, nextJ, currentStep);
			else
				visiTable[i][j] = visiTable[nextI][nextJ];

			return visiTable[i][j];
		};

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (visiTable[i][j] != 0)
				continue;

			if (Recursive(i, j, step) == step)
				step++;
		}
	}

	return step - 1;
}

/* storage
*/
long long Solution1(const std::vector<int>& list)
{
	static const long long ModularNumber = 1000000007ll;
	int length = list[0], select = list[1];
	std::vector<long long> factorial(length + 1, -1ll);
	std::function<long long(long long, long long)> ModularInverse = [&](long long one, long long other) -> long long
		{
			long long result = 1;
			while (other != 0)
			{
				if (other & 1)
					result = (result * one) % ModularNumber;
				one = (one * one) % ModularNumber;
				other >>= 1;
			}
			return result;
		};


	std::function<long long(int)> Factorial = [&](int i) -> long long
		{
			if (factorial[i] != -1ll)
				return factorial[i];
			if (i == 0 || i == 1)
				return 1ll;
			factorial[i] = (static_cast<long long>(i) * Factorial(i - 1)) % ModularNumber;
			return factorial[i];
		};

	long long denom = (Factorial(select) * Factorial(length - select)) % ModularNumber;
	return (Factorial(length) * ModularInverse(denom, ModularNumber - 2)) % ModularNumber;
}






















// solved
//#include <bits/stdc++.h>
/*

int Solution(const std::vector<std::string>& list)
{
	int height = std::stoi(list[0]), width = std::stoi(list[1]), step = 1;
	std::vector<std::vector<int>> visiTable(height, std::vector<int>(width, 0));
	std::unordered_map<char, std::pair<int, int>> movement = { { 'U', {-1, 0} }, { 'D', { 1, 0} }, { 'L', { 0,-1} }, { 'R', { 0, 1} } };
	std::queue<std::pair<int, int>> queue;

	std::function<int(int, int, int)> Recursive;
	Recursive = [&](int i, int j, int currentStep) -> int
		{
			visiTable[i][j] = currentStep;
			std::pair<int, int> move = movement.find(list[i + 2][j])->second;
			
			int nextI = i + move.first;
			int nextJ = j + move.second;

			if (visiTable[nextI][nextJ] == 0)
				visiTable[i][j] = Recursive(nextI, nextJ, currentStep);
			else
				visiTable[i][j] = visiTable[nextI][nextJ];

			return visiTable[i][j];
		};

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (visiTable[i][j] != 0)
				continue;

			if (Recursive(i, j, step) == step)
				step++;
		}
	}

	return step - 1;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int length = list[0];
	std::vector<std::vector<int>> graph(length + 1);
	std::vector<int> inDegree(length + 1, 0), result;
	std::queue<int> queue;

	result.reserve(length);
	for (int i = 0; i < list[1]; i++)
	{
		int index = i * 2 + 2;
		graph[list[index]].push_back(list[index + 1]);
		inDegree[list[index + 1]]++;
	}

	for (int i = 0; i < length; i++)
	{
		int index = i + 1;
		if (inDegree[index] == 0)
			queue.push(index);
	}

	while (!queue.empty())
	{
		int index = queue.front();
		queue.pop();

		result.push_back(index);
		for (auto& item : graph[index])
		{
			inDegree[item]--;
			if (inDegree[item] == 0)
				queue.push(item);
		}
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], departure = *(list.rbegin() + 1);
	std::vector<std::vector<std::pair<int, int>>> graph(nodeLength + 1);
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> queue;
	std::vector<int> cosTable(nodeLength + 1, std::numeric_limits<int>::max());

	for (int i = 0; i < list[1]; i++)
	{
		int index = i * 3 + 2;
		graph[list[index]].push_back({ list[index + 1], list[index + 2] });
	}

	queue.push({0, departure });

	while (!queue.empty())
	{
		int index = queue.top().second;
		int weight = queue.top().first;
		queue.pop();

		if (cosTable[index] < weight)
			continue;

		for (auto item : graph[index])
		{
			int nextWeight = weight + item.second;
			if (cosTable[item.first] <= nextWeight)
				continue;

			cosTable[item.first] = nextWeight;
			queue.push({ nextWeight, item.first });
		}
	}

	return cosTable[*list.rbegin()];
}

std::vector<std::string> Solution(int length)
{
	int triangleHeight = length / 3;
	std::vector<std::string> result(length, std::string(triangleHeight * 5 + triangleHeight, ' '));

	auto Triangle = [&](int x, int y) -> void
		{
			result[y + 2][x + 2] = '*';
			result[y + 1][x + 1] = '*';
			result[y + 1][x + 3] = '*';
			for (int i = 0; i < 5; i++)
				result[y][x + i] = '*';
		};

	std::function<void(int, int, int)> Recursive;
	Recursive = [&](int x, int y, int triangleFloor) -> void
		{
			if (triangleFloor <= 1)
			{
				Triangle(x, y);
				return;
			}
			triangleFloor /= 2;
			int halfX = triangleFloor * 5 + triangleFloor;
			int halfY = triangleFloor * 3;

			Recursive(x, y, triangleFloor);
			Recursive(x + halfX, y, triangleFloor);
			Recursive(x + halfX / 2, halfY + y, triangleFloor);
		};

	Recursive(0, 0, triangleHeight);
	std::reverse(result.begin(), result.end());

	return result;
}
int Solution(const std::vector<int>& list)
{
	int length = list[0];
	std::vector<std::vector<int>> table(length + 1, std::vector<int>(3, 0));

	for (int i = 0; i < length; i++)
	{
		int index = i * 3 + 1;
		table[i + 1][0] = std::min(table[i][1], table[i][2]) + list[index];
		table[i + 1][1] = std::min(table[i][0], table[i][2]) + list[index + 1];
		table[i + 1][2] = std::min(table[i][0], table[i][1]) + list[index + 2];
	}

	return std::min(table[length][0], std::min(table[length][1], table[length][2]));
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int length = static_cast<int>(list.size()), index = 0;
	std::vector<int> result;
	std::function<void(int)> Recursive;
	Recursive = [&](int right) -> void
	{
			if (index >= length)
				return;
			int current = list[index];
			if (right < current)
				return;

			index++;
			Recursive(current);
			Recursive(right);
			result.push_back(current);
	};

	result.reserve(length);
	Recursive(std::numeric_limits<int>::max());

	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int length = list[0], caseLength = list[1];
	std::vector<int> result, stack, visitable(length + 1, -1);
	std::vector<std::vector<int>> graph(length + 1);

	result.reserve(length);
	stack.reserve(length);
	for (int i = 0; i < list[1]; i++)
		graph[list[i * 2 + 2]].push_back(list[i * 2 + 3]);

	for (int i = 1; i <= length; i++)
	{
		if (visitable[i] != -1)
			continue;

		stack.push_back(i);

		while (!stack.empty())
		{
			int index = stack.back();

			if (visitable[i] == 1)
			{
				result.push_back(index);
				stack.pop_back();
				continue;
			}

			visitable[index] = 1;

			for (auto& item : graph[index])
			{
				if (visitable[item] != -1)
					continue;

				visitable[item] = 0;
				stack.push_back(item);
			}
		}
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], number = list[1], result = std::numeric_limits<int>::max();
	const int* array = &list[2];
	int left = 0, right = 0;
	long long sum = 0;

	while (right < length)
	{
		sum += static_cast<long long>(array[right]);

		while (sum >= number)
		{
			result = std::min(result, right - left + 1);

			sum -= static_cast<long long>(array[left]);
			left++;
		}
		
		right++;
	}

	return (result == std::numeric_limits<int>::max()) ? 0 : result;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list)
{
	int lengthA = list[0], lengthB = list[lengthA + 1];
	int trackerA = 0, trackerB = 0, iteratorA, iteratorB;
	const int* listA = &list[1], *listB = &list[lengthA + 2];
	std::set<int, std::greater<int>> commons;
	std::vector<std::vector<int>> output(2);

	output[0] = {0};

	for (int i = 0; i < lengthA; i++)
		for (int j = 0; j < lengthB; j++)
			if (listA[i] == listB[j])
				commons.insert(listA[i]);

	while (true)
	{
		bool flag = true;

		for (auto& item : commons)
		{
			iteratorA = trackerA;
			iteratorB = trackerB;

			for (; iteratorA < lengthA; iteratorA++)
				if (item == listA[iteratorA])
					break;

			if (iteratorA == lengthA)
				continue;

			for (; iteratorB < lengthB; iteratorB++)
				if (item == listB[iteratorB])
					break;

			if (iteratorB == lengthB)
				continue;

			output[1].push_back(item);
			flag = false;
			break;
		}

		if (flag)
			break;

		trackerA = iteratorA + 1;
		trackerB = iteratorB + 1;
	}

	output[0][0] = static_cast<int>(output[1].size());

	return output;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], result = 0;
	std::vector<std::vector<int>> board(length, std::vector<int>(length, 0));

	std::function<void(int, int, bool)> Fill = [&](int x, int y, bool placing) -> void
		{
			int nextX, nextY, delta = (placing) ? 1 : -1;
			for (int i = 0; i < length; i++)
			{
				board[i][x] += delta;
				board[y][i] += delta;
			}
			board[y][x] -= delta;

			for (nextX = x - 1, nextY = y - 1; nextX >= 0 && nextY >= 0; nextX--, nextY--)
				board[nextY][nextX] += delta;
			for (nextX = x + 1, nextY = y - 1; nextX < length && nextY >= 0; nextX++, nextY--)
				board[nextY][nextX] += delta;
			for (nextX = x - 1, nextY = y + 1; nextX >= 0 && nextY < length; nextX--, nextY++)
				board[nextY][nextX] += delta;
			for (nextX = x + 1, nextY = y + 1; nextX < length && nextY < length; nextX++, nextY++)
				board[nextY][nextX] += delta;
		};

	std::function<void(int)> Placing = [&](int y) -> void
		{
			if (y >= length)
			{
				result++;
				return;
			}

			for (int i = 0; i < length; i++)
			{
				if (board[y][i] > 0)
					continue;
				Fill(i, y, true);
				Placing(y + 1);
				Fill(i, y, false);
			}
		};

	Placing(0);
	return result;
}

int Solution(const std::vector<std::string>& list)
{
	struct Block { int x, y; bool breaked; };
	int height = std::stoi(list[0]), width = std::stoi(list[1]);
	std::vector<std::vector<std::pair<int, int>>> boardResult(height, std::vector<std::pair<int, int>>(width, { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() }));
	std::vector<std::vector<bool>> board(height, std::vector<bool>(width, false));
	std::queue<Block> queue;

	auto Travel = [&](int x, int y, int cost, bool breaked) -> void
		{
			if (x < 0 || y < 0 || x >= width || y >= height)
				return;

			int* resultCost = (breaked) ? &boardResult[y][x].second : &boardResult[y][x].first;
			if (board[y][x])
			{
				if (breaked)
					return;
				breaked = true;
				resultCost = &boardResult[y][x].second;
			}

			if (*resultCost <= cost)
				return;

			*resultCost = cost;
			queue.push({ x, y, breaked });
		};

	queue.push({ 0, 0, false });
	boardResult[0][0] = { 1,1 };
	for (int i = 0; i < height; i++)
	{
		const std::string& line = list[i + 2];
		for (int j = 0; j < width; j++)
			board[i][j] = line[j] == '1';
	}

	while (!queue.empty())
	{
		Block current = queue.front();
		queue.pop();
		int cost = ((current.breaked) ? boardResult[current.y][current.x].second : boardResult[current.y][current.x].first) + 1;

		Travel(current.x - 1, current.y, cost, current.breaked);
		Travel(current.x + 1, current.y, cost, current.breaked);
		Travel(current.x, current.y - 1, cost, current.breaked);
		Travel(current.x, current.y + 1, cost, current.breaked);
	}

	int result = std::min(boardResult[height - 1][width - 1].first, boardResult[height - 1][width - 1].second);
	return (result == std::numeric_limits<int>::max()) ? -1 : result;
}

int Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], lineLength = list[1], destination = list[2], result = 0;
	std::vector<int> departure(nodeLength + 1, std::numeric_limits<int>::max()), arrival(nodeLength + 1, std::numeric_limits<int>::max());
	std::vector<std::vector<std::pair<int, int>>> graph(nodeLength + 1);
	std::vector<std::vector<std::pair<int, int>>> graphReversed(nodeLength + 1);
	std::priority_queue<std::pair<int, int>> queue;

	auto Function = [&](int start, std::vector<int>& result, const std::vector<std::vector<std::pair<int, int>>>& graph) -> void
		{
			result[start] = 0;
			queue.push({ 0, start });
			while (!queue.empty())
			{
				int index = queue.top().second;
				int cost = queue.top().first;
				queue.pop();

				if (result[index] < cost)
					continue;

				for (auto& child : graph[index])
				{
					int nextCost = result[index] + child.second;
					if (nextCost >= result[child.first])
						continue;
					result[child.first] = nextCost;
					queue.push({ nextCost, child.first });
				}
			}
		};


	for (int i = 0; i < lineLength; i++)
	{
		int start = list[i * 3 + 3], end = list[i * 3 + 4], cost = list[i * 3 + 5];

		graph[start].push_back({ end, cost });
		graphReversed[end].push_back({ start, cost });
	}

	Function(destination, departure, graphReversed);
	Function(destination, arrival, graph);

	for (int i = 0; i < nodeLength; i++)
		result = std::max(result, departure[i + 1] + arrival[i + 1]);

	return result;
}

std::vector<std::string> Solution(const std::vector<int>& list)
{
	struct BellmanFord
	{
	public:
		BellmanFord(void) = default;
		~BellmanFord(void) = default;
		bool operator() (std::vector<int>::const_iterator& start)
		{
			struct Line { int start, end, cost; };
			int plusLineCount, minusLineCount, nodeCount = *(start++), loop = nodeCount - 1;
			std::vector<Line> lines;
			std::vector<int> costTable(nodeCount + 1, 0);

			plusLineCount = *(start++);
			minusLineCount = *(start++);

			lines.reserve(plusLineCount + minusLineCount);
			for (int i = 0; i < plusLineCount; i++)
			{
				int startIndex = *(start++), end = *(start++), cost = *(start++);
				lines.push_back({ startIndex, end, cost });
				lines.push_back({ end, startIndex, cost });
			}
			for (int i = 0; i < minusLineCount; i++)
			{
				int startIndex = *(start++), end = *(start++), cost = *(start++) * -1;
				lines.push_back({ startIndex, end, cost });
			}

			for (int i = 0; i < loop; i++)
			{
				bool trigger = true;
				for (auto& line : lines)
				{
					int distance = costTable[line.start] + line.cost;
					if (costTable[line.end] > distance)
					{
						costTable[line.end] = distance;
						trigger = false;
					}
				}

				if (trigger)
					break;
			}

			for (auto& line : lines)
				if (costTable[line.end] > costTable[line.start] + line.cost)
					return true;

			return false;
		}
	};

	std::vector<int>::const_iterator listIterator = list.begin() + 1;
	BellmanFord Function;
	std::vector<std::string> result(list[0]);
	for (int caseLength = 0; caseLength < list[0]; caseLength++)
		result[caseLength] = (Function(listIterator)) ? "YES" : "NO";

	return result;
}

std::vector<std::string> Solution(const std::vector<int>& list)
{
	struct BellmanFord
	{
	public:
		BellmanFord(void) = default;
		~BellmanFord(void) = default;
		bool operator() (std::vector<int>::const_iterator& start)
		{
			struct Line { int start, end, cost; };
			int plusLineCount, minusLineCount, nodeCount = *(start++), loop = nodeCount - 1;
			std::vector<Line> lines;
			std::vector<int> costTable(nodeCount + 1, 0);

			plusLineCount = *(start++);
			minusLineCount = *(start++);

			lines.reserve(plusLineCount + minusLineCount);
			for (int i = 0; i < plusLineCount; i++)
			{
				int startIndex = *(start++), end = *(start++), cost = *(start++);
				lines.push_back({ startIndex, end, cost });
				lines.push_back({ end, startIndex, cost });
			}
			for (int i = 0; i < minusLineCount; i++)
			{
				int startIndex = *(start++), end = *(start++), cost = *(start++) * -1;
				lines.push_back({ startIndex, end, cost });
			}

			for (int i = 0; i < loop; i++)
			{
				bool trigger = true;
				for (auto& line : lines)
				{
					int distance = costTable[line.start] + line.cost;
					if (costTable[line.end] > distance)
					{
						costTable[line.end] = distance;
						trigger = false;
					}
				}

				if (trigger)
					break;
			}

			for (auto& line : lines)
				if (costTable[line.end] > costTable[line.start] + line.cost)
					return true;

			return false;
		}
	};

	std::vector<int>::const_iterator listIterator = list.begin() + 1;
	BellmanFord Function;
	std::vector<std::string> result(list[0]);
	for (int caseLength = 0; caseLength < list[0]; caseLength++)
		result[caseLength] = (Function(listIterator)) ? "YES" : "NO";

	return result;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], result = 0, storage = 0;
	std::vector<std::vector<std::pair<int, int>>> tree(length + 1);
	std::vector<int>::const_iterator listIterator = list.begin() + 1;
	std::vector<int> stack, lengthStack(length + 1, -1);

	stack.reserve(length);
	for (int i = 0; i < length; i++)
	{
		int index = *(listIterator++);
		while (*listIterator != -1)
		{
			int childIndex = *(listIterator++);
			tree[index].push_back({ childIndex, *(listIterator++) });
		}
		listIterator++;
	}

	stack.push_back(1);
	lengthStack[1] = 0;

	while (!stack.empty())
	{
		int index = stack.back();
		stack.pop_back();

		int stackedLength = lengthStack[index];

		for (auto& child : tree[index])
		{
			if (lengthStack[child.first] != -1)
				continue;

			lengthStack[child.first] = stackedLength + child.second;
			stack.push_back(child.first);

			if (lengthStack[child.first] > storage)
			{
				storage = lengthStack[child.first];
				result = child.first;
			}
		}
	}

	for (int i = 0; i < length; i++)
		lengthStack[i + 1] = -1;


	stack.push_back(result);
	lengthStack[result] = 0;
	result = 0;

	while (!stack.empty())
	{
		int index = stack.back();
		stack.pop_back();

		for (auto& child : tree[index])
		{
			if (lengthStack[child.first] != -1)
				continue;

			lengthStack[child.first] = lengthStack[index] + child.second;
			stack.push_back(child.first);
			result = std::max(result, lengthStack[child.first]);
		}
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int width = list[0], height = list[1], result = 0;
	std::vector<const int*> original(height);
	std::vector<std::vector<int>> board(height, std::vector<int>(width)), time(height, std::vector<int>(width, std::numeric_limits<int>::max()));
	std::queue<std::pair<int, int>> queue;

	std::function<void(int, int, int)> Travel = [&](int i, int j, int next) -> void
		{
			if (i < 0 || j < 0 || i >= height || j >= width)
				return;
			if (time[i][j] <= next)
				return;
			time[i][j] = next;
			queue.push({ i, j });
		};


	for (int i = 0; i < height; i++)
	{
		original[i] = &list[2 + width * i];
		for (int j = 0; j < width; j++)
		{
			board[i][j] = original[i][j];
			switch (board[i][j])
			{
			case 1:
				queue.push({ i, j });
				time[i][j] = 0;
				break;
			case -1:
				time[i][j] = -1;
				break;
			default:
				break;
			}
		}
	}

	while (!queue.empty())
	{
		int i = queue.front().first;
		int j = queue.front().second;
		queue.pop();

		int next = time[i][j] + 1;

		Travel(i - 1, j, next);
		Travel(i + 1, j, next);
		Travel(i, j - 1, next);
		Travel(i, j + 1, next);
	}


	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (time[i][j] == std::numeric_limits<int>::max())
				return -1;
			result = std::max(result, time[i][j]);
		}
	}

	return result;
}

void Solution(void)
{
	struct Set
	{
	private:
		std::set<int> storage;
		std::set<int> all{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	public:
		bool Exsist(int number) {
			auto iterator = storage.find(number);
			return storage.find(number) != storage.end();
		}
		void Add(int number) { if (Exsist(number)) return; storage.insert(number); }
		void Remove(int number) { auto iterator = storage.find(number); if(iterator != storage.end()) storage.erase(iterator); }
		void Toggle(int number) { auto iterator = storage.find(number); if (iterator != storage.end()) storage.erase(iterator); else storage.insert(number); }
		void All(void) { storage = all; }
		void Empty(void) { storage.clear(); }
	};

	int length = InputSingle<int>();
	Set set;

	std::map<std::string, std::function<void(void)>> functions{
		{ "add", [&](void) -> void { set.Add(InputSingle<int>()); } },
		{ "remove", [&](void) -> void { set.Remove(InputSingle<int>()); } },
		{ "check", [&](void) -> void { std::cout << (set.Exsist(InputSingle<int>()) ? "1\n" : "0\n"); }},
		{ "toggle", [&](void) -> void { set.Toggle(InputSingle<int>()); } },
		{ "all", [&](void) -> void { set.All(); } },
		{ "empty", [&](void) -> void { set.Empty(); } }
	};

	for (int i = 0; i < length; i++)
		functions.find(InputSingle<std::string>())->second();
}

int Solution(const std::vector<int>& list)
{
	int halfX = (std::pow(2, list[0]) - 1) / 2, halfY = halfX, output = 0;

	for (int i = list[0]; i > 0; i--)
	{
		int halfLength = 1 << i - 1;
		int nodeCount = halfLength * halfLength;
		bool x = list[2] <= halfX;
		bool y = list[1] <= halfY;

		halfX = halfX + halfLength / ((x) ? -2 : 2);
		halfY = halfY + halfLength / ((y) ? -2 : 2);

		if (!x)
		{
			if (y)
				output += nodeCount;
			else
				output += nodeCount * 3;
		}
		else if(!y)
			output += nodeCount * 2;
	}

	return output;
}

int Solution(const std::vector<int>& list)
{
	int start = list[0], end = list[1], max = std::max(start, end) * 2;

	std::vector<int> table(max + 1, std::numeric_limits<int>::max());
	std::queue<int> queue;
	std::function<void(int, int)> Insert = [&](int index, int cost) -> void
		{
			if (index < 0 || index > max) return;
			if (table[index] <= cost) return;
			table[index] = cost;
			queue.push(index);
		};

	table[start] = 0;
	queue.push(start);
	while (!queue.empty())
	{
		int index = queue.front();
		queue.pop();
		int cost = table[index] + 1;

		Insert(index * 2, table[index]);
		Insert(index - 1, cost);
		Insert(index + 1, cost);
	}
	return table[end];
}

int Solution(const std::vector<int>& list)
{
	int minimum = list[1], length = list[0];
	std::vector<int> table(minimum + 1, std::numeric_limits<int>::max());
	const int* weightList = &list[2],* costList = &list[2 + length];

	table[0] = 0;
	for (int i = 0; i < length; i++)
	{
		for (int j = minimum; j >= 0; j--)
		{
			if (table[j] == std::numeric_limits<int>::max())
				continue;

			int nextWeight = std::min(j + weightList[i], minimum);
			table[nextWeight] = std::min(table[nextWeight], table[j] + costList[i]);
		}
	}

	return table[minimum];
}

int Solution(const std::vector<int>& list)
{
	int minimum = list[0], length = list[1], minValue = std::numeric_limits<int>::max();
	unsigned long long maxBit = 1ull << (length + 1);
	std::vector<std::pair<int, int>> innerList(length);
	for (int i = 0; i < length; i++)
		innerList[i] = { list[i * 2 + 2],list[i * 2 + 3] };

	for (unsigned long long i = 1ull; i < maxBit; i++)
	{
		int stack = 0, nodeStack = 0, nodeCost = 0;
		for (int j = 0; j < length; j++)
		{
			if (i & (1ull << j))
			{
				nodeCost += innerList[j].first;
				stack += innerList[j].second;
				nodeStack++;

				if (stack > length)
					break;
			}
		}

		if (nodeStack > length)
			continue;

		if (stack >= minimum)
			minValue = std::min(minValue, stack);
	}

	return minValue;
}

std::string Solution(const std::vector<int>& list)
{
	std::vector<int> innerList(list.begin() + 1, list.end());
	int left = 0, right = static_cast<int>(innerList.size()) - 1;
	int minimum = std::numeric_limits<int>::max(), minLeft = 0, minRight = right;

	std::sort(innerList.begin(), innerList.end());

	while (left < right)
	{
		int middle = std::abs(innerList[left] + innerList[right]);
		if (middle < minimum)
		{
			minimum = middle;
			minLeft = left;
			minRight = right;
		}


		if (std::abs(innerList[left]) < std::abs(innerList[right]))
			right--;
		else
			left++;
	}

	if (innerList[minLeft] > innerList[minRight])
		std::swap(minLeft, minRight);

	return std::to_string(innerList[minLeft]) + " " + std::to_string(innerList[minRight]);
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], count = list[1], result = 0;
	std::vector<int> priority, secondary;
	priority.reserve(length), secondary.reserve(length);

	for (int i = 0; i < length; i++)
	{
		int value = list[i + 2];
		if (value < 10) continue;
		if (value == 10)
			result++;
		else if (value % 10 == 0)
			priority.push_back(value);
		else
			secondary.push_back(value);
	}
	std::sort(priority.begin(), priority.end(), std::less<int>());
	std::sort(secondary.begin(), secondary.end(), std::less<int>());

	for (auto& item : priority)
	{
		int slice = item / 10 - 1;
		if (count < slice)
		{
			result += count;
			count = 0;
			break;
		}
		else
		{
			result += slice + 1;
			count -= slice;
		}
	}

	for (auto& item : secondary)
	{
		int slice = item / 10;

		if (count < slice)
		{
			result += count;
			count = 0;
			break;
		}
		else
		{
			result += slice;
			count -= slice;
		}
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], total = 0, result = 0;
	std::vector<bool> table;

	std::function<void(int, int)> DFS = [&](int depth, int stack) -> void
		{
			if (depth == length)
			{
				if(stack <= total && stack > 0)
					table[stack] = true;
				return;
			}

			DFS(depth + 1, stack - list[1 + depth]);
			DFS(depth + 1, stack);
			DFS(depth + 1, stack + list[1 + depth]);
		};

	for (int i = 0; i < length; i++)
		total += list[i + 1];

	table.resize(total + 1, false);

	DFS(0, 0);

	for (int i = 1; i <= total; i++)
		if (!table[i])
			result++;

	return result;
}

std::vector<std::string> Solution(const std::vector<std::string>& list)
{
	int nodeLength = std::stoi(list[0]);
	std::vector<std::string> result(3);
	std::vector<std::pair<int, int>> tree(nodeLength, {-1, -1});

	std::function<void(int, std::string&)> preOrder = [&](int index, std::string& result) -> void
	{
		int left = tree[index].first, right = tree[index].second;

		result.push_back(static_cast<char>(index) + 'A');
		if (left >= 0)
			preOrder(left, result);
		if (right >= 0)
			preOrder(right, result);
	};

	std::function<void(int, std::string&)> inOrder = [&](int index, std::string& result) -> void
		{
			int left = tree[index].first, right = tree[index].second;

			if (left >= 0)
				inOrder(left, result);
			result.push_back(static_cast<char>(index) + 'A');
			if (right >= 0)
				inOrder(right, result);
		};

	std::function<void(int, std::string&)> postOrder = [&](int index, std::string& result) -> void
		{
			int left = tree[index].first, right = tree[index].second;

			if (left >= 0)
				postOrder(left, result);
			if (right >= 0)
				postOrder(right, result);
			result.push_back(static_cast<char>(index) + 'A');
		};

	for (int i = 0; i < nodeLength; i++)
	{
		int rootIndex = static_cast<int>(list[1 + i * 3][0] - 'A');
		int leftChildIndex = static_cast<int>(list[1 + i * 3 + 1][0] - 'A');
		int rightChildIndex = static_cast<int>(list[1 + i * 3 + 2][0] - 'A');

		tree[rootIndex] = { leftChildIndex, rightChildIndex };
	}

	result[0].reserve(nodeLength);
	preOrder(0, result[0]);

	result[1].reserve(nodeLength);
	inOrder(0, result[1]);

	result[2].reserve(nodeLength);
	postOrder(0, result[2]);

	return result;
}

int Solution(const std::vector<int>& list)
{
	constexpr int maxVolume = 1001;
	int nodeNumber = list[0];
	int max = list[2];
	const int* differenceList = &list[3];
	std::vector<std::vector<bool>> table(nodeNumber + 1, std::vector<bool>(maxVolume, false));

	std::function<void(int, int, int)> Calculate = [&](int target, int value, int index)-> void
		{
			int storage = target + value;
			if (storage > max || storage < 0)
				return;

			table[index][storage] = true;
		};

	table[0][list[1]] = true;

	for (int i = 0; i < nodeNumber; i++)
	{
		for (int j = 0; j <= max; j++)
		{
			if (!table[i][j])
				continue;

			Calculate(j, differenceList[i], i + 1);
			Calculate(j, differenceList[i] * -1, i + 1);
		}
	}

	for (int j = max; j >= 0; j--)
	{
		if (table[nodeNumber][j])
			return j;
	}

	return -1;
}

std::vector<std::string> Solution(const std::vector<std::string>& list)
{
	int height = std::stoi(list[0]), width = std::stoi(list[1]);

	std::vector<std::vector<int>> board(height, std::vector<int>(width, -1));
	std::vector<std::string> result(height, std::string(width, '0'));
	std::vector<std::pair<int, int>> stack;
	std::vector<int> emptySpace;

	auto Search = [&](int x, int y, int index)
		{
			if (x < 0 || x >= height || y < 0 || y >= width)
				return;

			if (board[x][y] != -1 || list[x + 2][y] == '1')
				return;

			board[x][y] = index;
			stack.push_back({ x,y });
		};

	auto Find = [&](int x, int y)
		{
			if (x < 0 || x >= height || y < 0 || y >= width || board[x][y] == -1)
				return;

			for (auto item : stack)
			{
				if (board[x][y] == item.first)
					return;
			}

			stack.push_back({ board[x][y], emptySpace[board[x][y]] });
		};

	emptySpace.reserve(height * width);
	stack.reserve(height * width);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (list[i + 2][j] == '1' || board[i][j] != -1)
				continue;

			int index = static_cast<int>(emptySpace.size()), empty = 0;
			stack.push_back({i, j});
			board[i][j] = index;

			while (!stack.empty())
			{
				int x = stack.back().first;
				int y = stack.back().second;
				stack.pop_back();
				empty++;

				Search(x - 1, y, index);
				Search(x + 1, y, index);
				Search(x, y - 1, index);
				Search(x, y + 1, index);
			}
			emptySpace.push_back(empty);
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (list[i + 2][j] != '1')
				continue;

			int empty = 1;

			Find(i - 1, j);
			Find(i + 1, j);
			Find(i, j - 1);
			Find(i, j + 1);

			for (auto& item : stack)
				empty += item.second;

			result[i][j] = std::to_string(empty).back();
			stack.clear();
		}
	}

	return result;
}

int Solution(const std::vector<std::string>& list)
{
	bool multiply = false, storage = false;
	int stack = 0, checker = -1;

	for (int i = 0; i < 12; i++)
	{
		if (list[0][i] != '*')
			stack += (list[0][i] - '0') * ((multiply) ? 3 : 1);
		else
			storage = multiply;
		multiply = !multiply;
	}

	if (list[0].back() != '*')
		checker = list[0].back() - '0';
	else
		return (10 - (stack % 10)) % 10;

	for (int i = 0; i < 10; i++)
	{
		int total = stack + i * ((storage) ? 3 : 1);
		total = (10 - (total % 10)) % 10;

		if(checker == total)
			return i;
	}

	return 0;
}

long long Solution(const std::vector<long long>& list)
{
	long long length = list[0], result = 0;
	const long long* innerList = &list[1];

	std::vector<long long> lisStack;
	std::vector<long long>::iterator stackIterator;
	lisStack.reserve(length);

	for (long long i = 0; i < length; i++)
	{
		stackIterator = std::lower_bound(lisStack.begin(), lisStack.end(), innerList[i]);

		if (stackIterator == lisStack.end())
			lisStack.push_back(innerList[i]);
		else
			*stackIterator = innerList[i];
	}


	return static_cast<long long>(lisStack.size());
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], result = 0;
	const int* innerList = &list[1];

	std::vector<int> table(length, 1);

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (innerList[i] > innerList[j])
				table[i] = std::max(table[i], table[j] + 1);
		}
	}

	for (int i = 0; i < length; i++)
		result = std::max(result, table[i]);

	return result;
}

long long Solution(const std::vector<long long>& list)
{
	long long length = list[0], targetLength = list[1];
	long long middle, left = 1, right = 0;

	for (long long i = 0; i < length; i++)
		right = std::max(right, list[2 + i]);

	while (left <= right)
	{
		middle = (left + right) / 2;
		long long returnValue = 0;

		for (long long i = 0; i < length; i++)
			returnValue += list[2 + i] / middle;

		if (returnValue < targetLength)
			right = middle - 1;
		else
		{
			left = middle + 1;
			returnValue = middle;
		}
	}

	return right;
}

int Solution(const std::vector<int>& list)
{
	int start = list[0], end = list[1];
	int endIndex = std::max(start, end) * 2;
	std::vector<int> table(endIndex + 1, std::numeric_limits<int>::max());
	std::list<int> queue{ start };
	table[start] = 0;

	while (!queue.empty())
	{
		int index = queue.front();
		queue.pop_front();

		if (index == end)
			break;

		int step = table[index] + 1;
		if (index != 0 && table[index - 1] > step)
		{
			table[index - 1] = step;
			queue.push_back(index - 1);
		}
		if (index < endIndex && table[index + 1] > step)
		{
			table[index + 1] = step;
			queue.push_back(index + 1);
		}
		if (index * 2 <= endIndex && table[index * 2] > step)
		{
			table[index * 2] = step;
			queue.push_back(index * 2);
		}
	}
	return table[end];
}

std::vector<int> Solution(const std::vector<int>& list)
{
	std::priority_queue<int> queue;
	std::vector<int> result;
	for (int i = 0; i < list[0]; i++)
	{
		if (list[i + 1] == 0)
		{
			if(queue.empty())
				result.push_back(0);
			else
			{
				result.push_back(queue.top());
				queue.pop();
			}
		}
		else
			queue.push(list[i + 1]);
	}
	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	std::list<int> queue{1};
	std::vector<std::pair<int, std::vector<int>>> tree(list[0] + 1, { -1, {} });
	std::vector<std::pair<int, int>> treeInfomation(list[0] + 1, {-1, -1});
	std::vector<int> result;

	int pointer = list[0] + list[0] - 1, nodeA, nodeB;
	for (int i = 1; i < list[0]; i++)
	{
		nodeA = list[(i - 1) * 2 + 1];
		nodeB = list[(i - 1) * 2 + 2];
		tree[nodeA].second.push_back(nodeB);
		tree[nodeB].second.push_back(nodeA);
	}

	result.reserve(list[pointer]);
	treeInfomation[1].second = 0;
	treeInfomation[1].first = 1;

	while (!queue.empty())
	{
		int index = queue.front();
		queue.pop_front();

		int depth = treeInfomation[index].second + 1;

		for (auto item : tree[index].second)
		{
			if (treeInfomation[item].second != -1)
				continue;

			treeInfomation[item].first = index;
			treeInfomation[item].second = depth;
			queue.push_back(item);
		}
	}

	for (int i = 0; i < list[pointer]; i++)
	{
		nodeA = list[pointer + i * 2 + 1];
		nodeB = list[pointer + i * 2 + 2];
		
		int depthA = treeInfomation[nodeA].second;
		int depthB = treeInfomation[nodeB].second;

		while (nodeA != nodeB)
		{
			if (depthA > depthB)
			{
				nodeA = treeInfomation[nodeA].first;
				depthA = treeInfomation[nodeA].second;
			}
			else
			{
				nodeB = treeInfomation[nodeB].first;
				depthB = treeInfomation[nodeB].second;
			}
		}

		result.push_back(nodeA);
	}

	return result;
}

long long Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], range = list[3];
	long long lowCost = static_cast<long long>(list[4]), highCost = static_cast<long long>(list[5]);
	std::vector<std::pair<int, std::vector<int>>> graph(nodeLength + 1, { 0, {} });
	std::vector<long long> cosTable(nodeLength + 1, std::numeric_limits<long long>::max());
	std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>, std::greater<std::pair<long long, int>>> queue;
	std::queue<std::pair<int, int>> bfsQueue;

	int storage = 6 + list[2];
	for (int i = 0; i < list[1]; i++) // line
{
	int point1 = list[storage + i * 2];
	int point2 = list[storage + i * 2 + 1];

	graph[point1].second.push_back(point2);
	graph[point2].second.push_back(point1);
}

for (int i = 0; i < list[2]; i++)//dangerCity
{
	graph[list[6 + i]].first = 2;
	bfsQueue.push({ list[6 + i],0 });
}

while (!bfsQueue.empty())
{
	int index = bfsQueue.front().first;
	int depth = bfsQueue.front().second;
	bfsQueue.pop();

	if (graph[index].first == 0)
		graph[index].first = 1;
	depth++;

	if (depth > range)
		continue;

	for (auto& item : graph[index].second)
	{
		if (graph[item].first != 0)
			continue;

		bfsQueue.push({ item, depth });
	}
}


graph[0].first = graph[nodeLength].first = 0;
cosTable[1] = 0ll;
queue.push({ 0ll,1 });

while (!queue.empty())
{
	long long cost = queue.top().first;
	int index = queue.top().second;
	queue.pop();

	if (cost > cosTable[index])
		continue;

	for (auto& item : graph[index].second)
	{
		if (graph[item].first == 2)
			continue;

		long long nextCost = cost + ((graph[item].first == 0) ? lowCost : highCost);

		if (nextCost >= cosTable[item])
			continue;

		cosTable[item] = nextCost;
		queue.push({ nextCost, item });
	}
}

return cosTable[nodeLength] - lowCost;
}

std::string Solution(const std::vector<std::string>& list)
{
	std::string output, stack;
	long long length = static_cast<long long>(list[1].size()), middleResult = 0;
	char lastOperator = 'A';
	stack.reserve(length);

	for (int i = 0; i < length; i++)
	{
		if ('0' <= list[1][i] && '9' >= list[1][i])
			stack += list[1][i];
		else
		{
			switch (lastOperator)
			{
			case 'S':
				middleResult -= std::stoll(stack);
				break;
			case 'M':
				middleResult *= std::stoll(stack);
				break;
			case 'U':
				middleResult /= std::stoll(stack);
				break;
			case 'P':
				middleResult += std::stoll(stack);
				break;
			case 'C':
				output += std::to_string(middleResult) + " ";
				if (!stack.empty())
					middleResult = std::stoll(stack);
				break;
			default:
				middleResult = std::stoll(stack);
				break;
			}

			stack.clear();
			lastOperator = list[1][i];
		}
	}

	if (lastOperator == 'C')
		output += std::to_string(middleResult);

	if (output.empty())
		output = "NO OUTPUT";

	return output;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], result = 0;
	const int* targetList = &list[1];
	std::vector<std::vector<int>> tree(length);
	std::vector<bool> visitable(length, false);
	std::function<void(int, int)> DFS = [&](int index, int color) -> void
		{
			visitable[index] = true;

			if (targetList[index] != color)
			{
				color = targetList[index];
				result++;
			}

			for (auto& item : tree[index])
			{
				if (visitable[item])
					continue;
				DFS(item, color);
			}
		};

	for (auto iterator = list.begin() + length + 1; iterator != list.end(); iterator += 2)
	{
		tree[(*iterator) - 1].push_back((*(iterator + 1)) - 1);
		tree[(*(iterator + 1)) - 1].push_back((*iterator) - 1);
	}

	DFS(0, 0);

	return result;
}

int Solution(const std::vector<int>& list)
{
	struct Vector2
	{
		int x, y;
		Vector2 operator - (const Vector2& other) const { return { x - other.x, y - other.y }; }
		static long long Cross(const Vector2& one, const Vector2& other) { return static_cast<long long>(one.x) * static_cast<long long>(other.y) - static_cast<long long>(one.y) * static_cast<long long>(other.x); }
		static long long CounterClockwise(const Vector2& point, const Vector2& lineStart, const Vector2& lineEnd) { return Cross(lineStart - point, lineEnd - point); }

		static bool Intersect(const Vector2& oneStart, const Vector2& oneEnd, const Vector2& otherStart, const Vector2& otherEnd)
		{
			long long value1 = CounterClockwise(oneStart, oneEnd, otherStart);
			long long value2 = CounterClockwise(oneStart, oneEnd, otherEnd);
			long long value3 = CounterClockwise(otherStart, otherEnd, oneStart);
			long long value4 = CounterClockwise(otherStart, otherEnd, oneEnd);

			if (((value1 > 0 && value2 < 0) || (value1 < 0 && value2 > 0)) && ((value3 > 0 && value4 < 0) || (value3 < 0 && value4 > 0)))
				return true;

			if (value1 == 0 || value2 == 0 || value3 == 0 || value4 == 0)
				return true;

			return false;
		}
	};

	return Vector2::Intersect({ list[0], list[1] }, { list[2], list[3] }, { list[4], list[5] }, { list[6], list[7] }) ? 1 : 0;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], result = 0;
	std::vector<int> table(length, 1);

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (list[1 + i] <= list[1 + j])
				continue;
			table[i] = std::max(table[i], table[j] + 1);
		}
	}

	for (int i = 0; i < length; i++)	
		result = std::max(result, table[i]);

	return result;
}


int Solution(const std::vector<std::string>& list)
{
	int width = std::stoi(list[1]), height = std::stoi(list[0]), result = 0;
	unsigned int bitLength = 1 << width * height;
	std::string container;
	std::vector<std::vector<bool>> visitable(height, std::vector<bool>(width));
	std::vector<std::pair<int, int>> stack;
	container.reserve(std::max(width, height));
	for (unsigned int bit = 0; bit < bitLength; bit++)
	{
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				visitable[i][j] = false;

		int sum = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (visitable[i][j])
					continue;

				stack.push_back({i, j});
				container.clear();
				while (!stack.empty())
				{
					std::pair<int, int> position = stack.back();
					stack.pop_back();

					bool vertical = bit & (1 << (position.first * width + position.second));
					container += list[position.first + 2][position.second];
					visitable[position.first][position.second] = true;
					if(vertical)
					{
						if (position.first + 1 == height || visitable[position.first + 1][position.second])
							continue;

						bool next = bit & (1 << ((position.first + 1) * width + position.second));

						if (vertical != next)
							continue;
						stack.push_back({ position.first + 1, position.second });
					}
					else
					{
						if (position.second + 1 == width || visitable[position.first][position.second + 1])
							continue;

						bool next = bit & (1 << (position.first * width + (position.second + 1)));

						if (vertical != next)
							continue;

						stack.push_back({ position.first, position.second + 1 });
					}
				}
				sum += std::stoi(container);
			}
		}
		result = std::max(result, sum);
	}

	return result;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], lineLength = list[1], root = list[2];
	std::vector<std::set<int>> graph(nodeLength + 1);
	std::vector<std::vector<int>> result(2);
	std::list<int> queue{root};
	std::vector<int> stack{root};
	std::vector<bool> visited(nodeLength + 1, false);

	for (int i = 0; i < lineLength; i++)
	{
		graph[list[i * 2 + 3]].insert(list[i * 2 + 4]);
		graph[list[i * 2 + 4]].insert(list[i * 2 + 3]);
	}

	while (!stack.empty())
	{
		int index = stack.back();
		stack.pop_back();

		if (visited[index])
			continue;

		visited[index] = true;
		result[0].push_back(index);

		for (auto iterator = graph[index].rbegin(); iterator != graph[index].rend(); iterator++)
			stack.push_back(*iterator);
	}

	for (int i = 0; i <= nodeLength; i++)
		visited[i] = false;

	while (!queue.empty())
	{
		int index = queue.front();
		queue.pop_front();

		if (visited[index])
			continue;

		visited[index] = true;
		result[1].push_back(index);

		for (auto& item : graph[index])
			queue.push_back(item);
	}

	return result;
}

long long Solution(const std::vector<int>& list)
{
	int length = list[0];
	std::vector<std::vector<long long>> table(length, std::vector<long long>(length, 0ll));
	std::vector<int> matrix(length + 1);

	matrix[0] = list[1];
	for (int i = 0; i < length; i++)
		matrix[i + 1] = list[i * 2 + 2];
	
	for (int size = 2; size <= length; size++)
	{
		int last = length - (size - 1);
		for (int left = 0; left < last; left++)
		{
			int right = left + size - 1;
			table[left][right] = std::numeric_limits<long long>::max();

			for (int middle = left; middle < right; middle++)
				table[left][right] = std::min(table[left][right], table[left][middle] + table[middle + 1][right] + static_cast<long long>(matrix[left] * matrix[middle + 1] * matrix[right + 1]));
		}
	}

	return table[0][length - 1];
}

int Solution(const std::vector<std::string>& list)
{
	int length = static_cast<int>(list[0].size()), result = 0, stackNumber = 0;
	std::string stack;
	stack.reserve(length);
	bool minusStack = false;

	for (int i = 0; i < length; i++)
	{
		switch (list[0][i])
		{
		case '+':
		{
			stackNumber += std::stoi(stack);
			stack.clear();

			break;
		}
		case '-':
		{
			stackNumber += std::stoi(stack);
			stack.clear();

			if (minusStack)
				stackNumber *= -1;

			result += stackNumber;
			stackNumber = 0;
			minusStack = true;
			break;
		}
		default:
			stack.push_back(list[0][i]);
			break;
		}

	}
	stackNumber += std::stoi(stack);
	if (minusStack)
		stackNumber *= -1;
	result += stackNumber;

	return result;
}

int Solution(const std::vector<int>& list)
{
	int targetNumber = list[0], pickCount = list[1];
	std::vector<std::vector<int>> table(pickCount + 1, std::vector<int>(targetNumber + 1, 0));

	for (int i = 0; i <= pickCount; i++)
		table[i][0] = 1;

	for (int i = 1; i <= pickCount; i++)
		for (int j = 1; j <= targetNumber; j++)
			table[i][j] = (table[i - 1][j] + table[i][j - 1]) % 1000000000;

	return table[pickCount][targetNumber];
}

int Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], lineLength = list[1], mstValue = 0;

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> queue;
	std::vector<std::vector<std::pair<int, int>>> graph(nodeLength + 1);
	std::vector<bool> visitable(nodeLength + 1, false);

	for (int i = 0; i < lineLength; i++)
	{
		int nodeA = list[i * 3 + 2];
		int nodeB = list[i * 3 + 3];
		int weight = list[i * 3 + 4];
		graph[nodeA].push_back({ nodeB, weight });
		graph[nodeB].push_back({ nodeA , weight });
	}

	queue.push({ 0, 1 });

	while (!queue.empty())
	{
		int index = queue.top().second;
		int weight = queue.top().first;
		queue.pop();

		if (visitable[index])
			continue;

		visitable[index] = true;
		mstValue += weight;

		for (auto& item : graph[index])
		{
			if (visitable[item.first])
				continue;

			queue.push({ item.second, item.first });
		}
	}

	return mstValue;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], min = std::numeric_limits<int>::max();
	std::vector<std::vector<int>> table(length + 1, std::vector<int>(3));

	for (int j = 0; j < 3; j++)
	{
		for (int i = 2; i <= length; i++)
		{
			table[i][0] = std::numeric_limits<int>::max();
			table[i][1] = std::numeric_limits<int>::max();
			table[i][2] = std::numeric_limits<int>::max();
		}

		table[1][j] = list[1 + j];
		table[1][(j + 1) % 3] = 10000;
		table[1][(j + 2) % 3] = 10000;

		for (int i = 1; i < length; i++)
		{
			int cost0 = list[i * 3 + 1];
			int cost1 = list[i * 3 + 2];
			int cost2 = list[i * 3 + 3];

			table[i + 1][0] = std::min(table[i + 1][0], std::min(table[i][1] + cost0, table[i][2] + cost0));
			table[i + 1][1] = std::min(table[i + 1][1], std::min(table[i][0] + cost1, table[i][2] + cost1));
			table[i + 1][2] = std::min(table[i + 1][2], std::min(table[i][1] + cost2, table[i][0] + cost2));
		}

		min = std::min(min, table[length][(j + 1) % 3]);
		min = std::min(min, table[length][(j + 2) % 3]);
	}

	return min;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0];
	const int* dolaList = &list[1];

	if (static_cast<int>(list.size()) != length + 1)
		return 0;

	int startIndex = 0;
	int minimum = std::numeric_limits<int>::max();
	std::vector<int> replaced;

	for (int i = 0; i < length; i++)
	{
		if (dolaList[i] < minimum)
		{
			startIndex = i;
			minimum = dolaList[i];
		}
	}

	if (minimum < length)
		for (int i = 0; i < length; i++)
		{
			int dolaIndex = dolaList[(i + startIndex) % length];
			if (dolaIndex > length)
				replaced.push_back(dolaIndex);
			else if (dolaIndex - minimum != i)
				return 0;
		}
	else
		for (int i = 0; i < length; i++)
			replaced.push_back(dolaList[i]);


	if (replaced.size() == 0)
		return 1;

	std::sort(replaced.begin(), replaced.end());

	length = replaced.size() - 1;

	for (size_t i = 0; i < length; i++)
	{
		if (replaced[i] == replaced[i + 1])
			return 0;
	}

	return 1;
}

int Solution(const std::vector<std::string>& list)
{
	int height = std::stoi(list[0]), width = std::stoi(list[1]), result = 0;
	int heightIndex = height - 1, widthIndex = width - 1;

	if (height < 3 || width < 3)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (list[i + 2][j] != list[i + 2 + height][j])
					return -1;
			}
		}

		return 0;
	}

	std::vector<std::vector<bool>> board(height, std::vector<bool>(width)), target(height, std::vector<bool>(width));

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			board[i][j] = (list[i + 2][j] == '1');
			target[i][j] = (list[i + 2 + height][j] == '1');
		}
	}

	for (int i = 0; i < heightIndex - 1; i++)
	{
		for (int j = 0; j < widthIndex - 1; j++)
		{
			if (board[i][j] == target[i][j])
				continue;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					int iIndex = i + x + 1;
					int jIndex = j + y + 1;

					board[iIndex][jIndex] = !board[iIndex][jIndex];

				}
			}
			result++;
		}

		if (board[i][widthIndex] != target[i][widthIndex])
			return -1;
		if (board[i][widthIndex - 1] != target[i][widthIndex - 1])
			return -1;
	}

	for (int j = 0; j < width; j++)
	{
		if (board[heightIndex][j] != target[heightIndex][j])
			return -1;
		if (board[heightIndex - 1][j] != target[heightIndex - 1][j])
			return -1;
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0],target = list[1];
	int result = 0;
	std::vector<int> coinTable(target + 1, std::numeric_limits<int>::max());
	coinTable[0] = 0;

	for (int i = length - 1; i >= 0; i--)
	{
		int coin = list[2 + i];
		for (int j = coin; j <= target; j++)
		{
			if (coinTable[j - coin] == std::numeric_limits<int>::max())
				continue;

			coinTable[j] = std::min(coinTable[j], coinTable[j - coin] + 1);
		}
	}

	return coinTable[target];
}

std::vector<std::string> Solution(const std::vector<std::string>& list)
{
	int lengthA = static_cast<int>(list[0].size()), lengthB = static_cast<int>(list[1].size());
	std::vector<std::vector<int>> table(lengthA + 1, std::vector<int>(lengthB + 1, 0));
	std::vector<std::string> result(2);
	int i = 1, j;

	for (; i <= lengthA; i++)
	{
		for (j = 1; j <= lengthB; j++)
		{
			if (list[0][i - 1] == list[1][j - 1])
				table[i][j] = table[i - 1][j - 1] + 1;
			else
				table[i][j] = std::max(table[i][j - 1], table[i - 1][j]);
		}
	}

	i--;
	j--;
	result[0] = std::to_string(table[i][j]);

	while (i > 0 && j > 0)
	{
		if (list[0][i - 1] == list[1][j - 1])
		{
			result[1].push_back(list[0][i - 1]);
			i--;
			j--;
		}
		else if (table[i][j - 1] >= table[i - 1][j])
			j--;
		else
			i--;
	}

	std::reverse(result[1].begin(), result[1].end());
	return result;
}

long long Solution(const std::vector<int>& list)
{
	long long answer = 0;
	std::multiset<int> bars;
	
	for (auto iterator = list.begin() + 1; iterator != list.end(); iterator++)
		bars.insert(*iterator);

	int min = *bars.begin();
	for (auto& item : bars)
		answer += static_cast<long long>(item - min);

	return answer;
}

int Solution(const std::vector<int>& list)
{
	int width = list[0], height = list[1];
	int checkPointX = list[2] - 1, checkPointY = list[3] - 1;
	std::vector<std::vector<long long>> table(height, std::vector<long long>(width, 0ll)), secondTable = table;
	table[checkPointY][checkPointX] = 1ll;
	secondTable[0][0] = 1ll;

	for (int y = checkPointY; y < height; y++)
	{
		for (int x = checkPointX; x < width; x++)
		{
			if (y != checkPointY)
				table[y][x] = (table[y][x] + table[y - 1][x]) % 1000007;
			if (x != checkPointX)
				table[y][x] = (table[y][x] + table[y][x - 1]) % 1000007;
		}
	}

	for (int y = 0; y <= checkPointY; y++)
	{
		for (int x = 0; x <= checkPointX; x++)
		{
			if (y != 0)
				secondTable[y][x] = (secondTable[y][x] + secondTable[y - 1][x]) % 1000007;
			if (x != 0)
				secondTable[y][x] = (secondTable[y][x] + secondTable[y][x - 1]) % 1000007;

		}
	}

	return (secondTable[checkPointY][checkPointX] * table[height - 1][width - 1]) % 1000007;
}

std::string Solution(const std::vector<int>& list)
{
	int length = list[0], left = 0, right = 0;
	const int* mag1 = &list[1];
	const int* mag2 = &list[1 + length];

	std::vector<int> table(length);

	for (int k = 0; k < length; k++)
	{
		for (auto& item : table)
			item = 1;

		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (mag1[(j + k) % length] < mag1[(i + k) % length])
					table[i] = std::max(table[i], table[j] + 1);
			}
		}

		for (int i = 0; i < length; ++i)
			right = std::max(right, table[i]);
	}

	for (int k = 0; k < length; k++)
	{
		for (auto& item : table)
			item = 1;

		for (int i = 0; i < length; i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (mag2[(j + k) % length] < mag2[(i + k) % length])
					table[i] = std::max(table[i], table[j] + 1);
			}
		}

		for (int i = 0; i < length; ++i)
			left = std::max(left, table[i]);
	}

	if (left > right)
		return "HG Win!";
	if (left < right)
		return "YJ Win!";

	return "Both Win!";
}

int Solutiontt(const std::vector<int>& list)
{
	int area = 0, reverse;
	const int& height = list[0],& width = list[1];
	std::vector<const int*> board(height);

	for (int i = 0; i < height; i++)
		board[i] = &list[2 + i * width];

	for (int i = 0; i < height; i++)
	{
		area += board[i][0] + board[i][width - 1];
		for (int j = 1; j < width; j++)
		{
			reverse = width - j - 1;

			if (board[i][j] > board[i][j - 1])
				area += board[i][j] - board[i][j - 1];
			if (board[i][reverse] > board[i][reverse + 1])
				area += board[i][reverse] - board[i][reverse + 1];


		}
	}

	for (int j = 0; j < width; j++)
	{
		area += board[0][j] + board[height - 1][j];
		for (int i = 1; i < height; i++)
		{
			reverse = height - i - 1;

			if (board[i][j] > board[i - 1][j])
				area += board[i][j] - board[i - 1][j];
			if (board[reverse][j] > board[reverse + 1][j])
				area += board[reverse][j] - board[reverse + 1][j];
		}
	}

	return area + height * width * 2;
}

double Solution(const std::vector<int>& list)
{
	int length = list[0];
	double area = 0.0;
	std::vector<std::pair<double, double>> vertices(length);
	for (int i = 0; i < length; i++)
	{
		vertices[i].first = static_cast<double>(list[i * 2 + 1]);
		vertices[i].second = static_cast<double>(list[i * 2 + 2]);
	}

	for (int i = 0; i < length; i++)
	{
		int next = (i + 1) % length; // tiling
		area += vertices[i].first * vertices[next].second;
		area -= vertices[i].second * vertices[next].first;
	}

	return std::abs(area) / 2.0;
}

int Solution(const std::vector<int>& list)
{
	int height = list[0], width = list[1], answer = 0;
	std::vector<const int*> board(height);
	std::vector<std::vector<int>> table(height, std::vector<int>(width, std::numeric_limits<int>::max()));
	std::list<std::pair<std::pair<int, int>, int>> queue;

	for (int i = 0; i < height; i++)
	{
		board[i] = &list[2 + i * width];
		for (int j = 0; j < width; j++)
		{
			if (board[i][j] == 1)
			{
				queue.push_back({ {i, j}, 0 });
				table[i][j] = 0;
			}
		}
	}

	while (!queue.empty())
	{
		std::pair<int, int> position = queue.front().first;
		int depth = queue.front().second + 1;
		queue.pop_front();

		for (int x = -1; x < 2; x++)
		{
			int X = position.first + x;
			if (X < 0 || X >= height)
				continue;

			for (int y = -1; y < 2; y++)
			{
				int Y = position.second + y;
				if (Y < 0 || Y >= width)
					continue;
				if (table[X][Y] <= depth)
					continue;
				table[X][Y] = depth;
				queue.push_back({ {X, Y}, depth });
			}
		}

	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
			answer = std::max(answer, table[i][j]);
	}

	return answer;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list)
{
	int nodeSize = list[0], length = list[1];
	std::vector<std::vector<int>> table(nodeSize, std::vector<int>(nodeSize, std::numeric_limits<int>::max()));

	for (int i = 0; i < nodeSize; i++)
		table[i][i] = 0;

	for (int i = 0; i < length; i++)
	{
		int& record = table[list[2 + i * 3] - 1][list[2 + i * 3 + 1] - 1];
		record = std::min(record, list[2 + i * 3 + 2]);
	}

	for (int k = 0; k < nodeSize; k++)
	{
		for (int i = 0; i < nodeSize; i++)
		{
			for (int j = 0; j < nodeSize; j++)
			{
				if (table[i][k] == std::numeric_limits<int>::max() || table[k][j] == std::numeric_limits<int>::max())
					continue;

				table[i][j] = std::min(table[i][j], table[i][k] + table[k][j]);
			}
		}
	}

	for (int i = 0; i < nodeSize; i++)
	{
		for (int j = 0; j < nodeSize; j++)
		{
			if (table[i][j] == std::numeric_limits<int>::max())
				table[i][j] = 0;
		}
	}

	return table;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int size = list[0], length = list[1];
	std::vector<int> result(length);
	std::vector<const int*> board(size);
	std::vector<std::vector<int>> table(size, std::vector<int>(size, 0));
	const int* caseList = &list[size * size + 2];

	for (int i = 0; i < size; i++)
		board[i] = &list[2 + i * size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			table[i][j] = board[i][j];
			if (i != 0)
				table[i][j] += table[i - 1][j];
			if (j != 0)
				table[i][j] += table[i][j - 1];
			if (i != 0 && j != 0)
				table[i][j] -= table[i - 1][j - 1];
		}
	}

	for (int i = 0; i < length; i++)
	{
		int minX = caseList[i * 4] - 1;
		int minY = caseList[i * 4 + 1] - 1;
		int maxX = caseList[i * 4 + 2] - 1;
		int maxY = caseList[i * 4 + 3] - 1;

		result[i] = table[maxX][maxY];

		if (minX != 0)
			result[i] -= table[minX - 1][maxY];
		if (minY != 0)
			result[i] -= table[maxX][minY - 1];
		if (minX != 0 && minY != 0)
			result[i] += table[minX - 1][minY - 1];
	}

	return result;
}

int Solution(const std::vector<std::string>& list)
{
	int gridLength = std::stoi(list[1]), gridHeight = std::stoi(list[0]);
	std::vector<std::vector<int>> board(gridHeight, std::vector<int>(gridLength));
	std::vector<std::vector<int>> table(gridHeight, std::vector<int>(gridLength, -1));
	std::vector<std::vector<bool>> trace(gridHeight, std::vector<bool>(gridLength, false));
	int max = 0;
	bool escape = false;

	std::function<void(int, int, int)> DFS = [&](int x, int y, int depth) -> void
		{
			if (escape)
				return;

			if (x < 0 || x >= gridHeight || y < 0 || y >= gridLength || board[x][y] == -1)
			{
				max = std::max(max, depth);
				return;
			}

			if (trace[x][y])
			{
				escape = true;
				max = -1;
				return;
			}

			if (table[x][y] >= depth)
				return;
			else
				table[x][y] = depth;

			trace[x][y] = true;

			DFS(x - board[x][y], y, depth + 1);
			DFS(x + board[x][y], y, depth + 1);
			DFS(x, y - board[x][y], depth + 1);
			DFS(x, y + board[x][y], depth + 1);

			trace[x][y] = false;
		};


	for (int i = 0; i < gridHeight; i++)
	{
		for (int j = 0; j < gridLength; j++)
		{
			if (list[i + 2][j] == 'H')
				board[i][j] = -1;
			else
				board[i][j] = static_cast<int>(list[i + 2][j] - '0');

		}
	}

	DFS(0, 0, 0);

	return max;
}

std::vector<long long> Solution(const std::vector<int>& list)
{
	class SegmentTree // 1 base index
	{
	public:
		SegmentTree(void) = default;
		SegmentTree(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end, int nodeLength) : array(begin, end), size(nodeLength), tree(nodeLength * 4)
		{
			array.insert(array.begin(), 0);
			Build(array, 1, 1, size);
		}

		void Update(int index, int value)
		{
			array[index] = value;
			Update(1, 1, size, index, static_cast<long long>(value));
		}

		long long Search(int left, int right) { if (left > right) std::swap(left, right); return Search(1, 1, size, left, right); }

	private:
		void Build(const std::vector<int>& arr, int node, int start, int end)
		{
			if (start == end)
			{
				tree[node] = static_cast<long long>(arr[start]);
				return;
			}

			int middle = (start + end) / 2;
			Build(arr, node * 2, start, middle);
			Build(arr, node * 2 + 1, middle + 1, end);
			tree[node] = tree[node * 2] + tree[node * 2 + 1];
		}

		void Update(int node, int start, int end, int index, long long value)
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

		long long Search(int node, int start, int end, int left, int right)
		{
			if (right < start || end < left)
				return 0ll; // outside
			if (left <= start && end <= right)
				return tree[node]; // inside

			int middle = (start + end) / 2;
			return Search(node * 2, start, middle, left, right) + Search(node * 2 + 1, middle + 1, end, left, right);
		}

		std::vector<int> array;
		std::vector<long long> tree;
		int size;
	};

	int length = list[0], turn = list[1];
	SegmentTree segmentTree(list.begin() + 2, list.begin() + (length + 2), length);
	std::vector<long long> result(turn);
	int pointer = length + 2;

	for (int i = 0; i < turn; i++)
	{
		result[i] = segmentTree.Search(list[pointer], list[pointer + 1]);
		segmentTree.Update(list[pointer + 2], list[pointer + 3]);
		pointer += 4;

	}

	return result;
}

void TopologicalSort_Khan(const std::vector<std::vector<int>>& graph, std::vector<int>& sorted)
{
	if (graph.empty())
		return;

	int size = static_cast<int>(graph.size());
	sorted.clear();
	sorted.reserve(size);
	std::list<int> queue;

	std::vector<std::pair<int, bool>> input(size, { 0, false });
	for (int i = 0; i < size; i++)
	{
		for (auto& item : graph[i])
			input[item].first++;
	}
	for (int i = 0; i < size; i++)
	{
		if (input[i].first == 0)
		{
			queue.push_back(i);
			input[i].second = true;
		}
	}

	while (!queue.empty())
	{
		int current = queue.front();
		queue.pop_front();

		if (input[current].first == 0)
		{
			sorted.push_back(current);
			for (auto& item : graph[current])
			{
				input[item].first--;
				if (input[item].second)
					continue;

				input[item].second = true;
				queue.push_back(item);
			}
		}
		else
			queue.push_back(current);
	}
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int testCaseLength = list[0], pointer = 1;
	std::vector<int> result(testCaseLength, 0);
	std::vector<int> table;
	std::vector<int> sorted;

	std::vector<std::vector<int>> graph;

	for (int i = 0; i < testCaseLength; i++)
	{
		int nodeLength = list[pointer], lineLength = list[pointer + 1];
		const int* cost = &list[pointer + 2];
		pointer += 2 + nodeLength;


		if (table.size() < nodeLength)
		{
			table.resize(nodeLength);
			graph.resize(nodeLength);
		}

		for (int j = 0; j < nodeLength; j++)
		{
			table[j] = 0;
			graph[j].clear();
		}

		for (int j = 0; j < lineLength; j++)
		{
			int storage = pointer + j * 2;
			graph[list[storage] - 1].push_back(list[storage + 1] - 1);
		}

		TopologicalSort_Khan(graph, sorted);

		for (auto& item : sorted)
		{
			for (auto& next : graph[item])
				table[next] = std::max(table[next], table[item] + cost[item]);
		}

		pointer += lineLength * 2;
		int root = list[pointer++] - 1;
		result[i] = table[root] + cost[root];
	}

	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int testCaseLength = list[0];
	std::vector<int> result(testCaseLength);
	std::vector<int> table;


	for (int step = 0; step < testCaseLength; step++)
	{
		int number = list[step + 1];
		int length = number + 1;

		if (static_cast<int>(table.size()) < number)
		{
			table.resize(length, 0);
			length = static_cast<int>(table.size());
		}

		table[0] = 1;
		for (int i = 1; i < length; i++)
			table[i] = 0;

		for (int i = 1; i < 4; i++)
		{
			for (int j = i; j <= number; j++)
				table[j] += table[j - i];
		}

		result[step] = table[number];
	}

	return result;
}

int Solution(const std::vector<std::string>& list)
{
	int height = std::stoi(list[0]), width = std::stoi(list[1]), turn = 1;
	const std::string* boardOrigin = &list[2];
	std::vector<std::string> board(height);
	std::vector<std::vector<int>> table(height, std::vector<int>(width, -2));
	std::pair<int, int> start, end, position;
	std::list<std::pair<int, int>> queue, storage, secondStorage;


	for (int i = 0; i < height; i++)
		board[i] = boardOrigin[i];

	auto Setter = [&](const std::pair<int, int>& position, std::function<bool(const std::pair<int, int>&)>& condition, std::list<std::pair<int, int>>& queue) -> void
		{
			if (position.first < 0 || position.first >= height || position.second < 0 || position.second >= width)
				return;

			if (condition(position))
				queue.push_back(position);
		};
	std::function<bool(const std::pair<int, int>&)> iceCondition = [&](const std::pair<int, int>& position) -> bool
		{
			if (table[position.first][position.second] != -2)
				return false;

			table[position.first][position.second] = 1;

			if (board[position.first][position.second] == 'X')
			{
				storage.push_back(position);
				return false;
			}

			return true;
		};

	std::function<bool(const std::pair<int, int>&)> pathCondition = [&](const std::pair<int, int>& position) -> bool
		{
			if (table[position.first][position.second] == 2)
				return false;

			table[position.first][position.second] = 2;

			if (board[position.first][position.second] == 'X')
			{
				secondStorage.push_back(position);
				return false;
			}

			return true;
		};

	bool flag = false;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			if (board[i][j] != 'X')
			{
				if (table[i][j] == -2)
					queue.push_back({ i, j });

				while (!queue.empty())
				{
					position = queue.front();
					queue.pop_front();

					Setter({ position.first - 1, position.second }, iceCondition, queue);
					Setter({ position.first + 1, position.second }, iceCondition, queue);
					Setter({ position.first, position.second - 1 }, iceCondition, queue);
					Setter({ position.first, position.second + 1 }, iceCondition, queue);
				}

			}
			if (board[i][j] == 'L')
			{
				if (flag)
				{
					if (table[i][j] == 2)
						return 0;
					end = { i, j };
					continue;
				}
				flag = true;
				start = { i, j };

				queue.push_back({ i, j });
				table[i][j] = 2;

				while (!queue.empty())
				{
					position = queue.front();
					queue.pop_front();

					Setter({ position.first - 1, position.second }, pathCondition, queue);
					Setter({ position.first + 1, position.second }, pathCondition, queue);
					Setter({ position.first, position.second - 1 }, pathCondition, queue);
					Setter({ position.first, position.second + 1 }, pathCondition, queue);
				}
			}
		}
	}

	while (turn)
	{
		queue.splice(queue.end(), storage, storage.begin(), storage.end());

		for (auto& item : queue)
			board[item.first][item.second] = '.';

		while (!queue.empty())
		{
			position = queue.front();
			queue.pop_front();

			Setter({ position.first - 1, position.second }, iceCondition, queue);
			Setter({ position.first + 1, position.second }, iceCondition, queue);
			Setter({ position.first, position.second - 1 }, iceCondition, queue);
			Setter({ position.first, position.second + 1 }, iceCondition, queue);
		}

		queue.splice(queue.end(), secondStorage, secondStorage.begin(), secondStorage.end());

		while (!queue.empty())
		{
			position = queue.front();
			queue.pop_front();


			if (end == position)
			{
				return turn;
			}

			Setter({ position.first - 1, position.second }, pathCondition, queue);
			Setter({ position.first + 1, position.second }, pathCondition, queue);
			Setter({ position.first, position.second - 1 }, pathCondition, queue);
			Setter({ position.first, position.second + 1 }, pathCondition, queue);
		}

		turn++;
	}

	return -1;
}

std::vector<long long> Solution(const std::vector<long long>& list)
{
	struct SegmentTree
	{
		long long length;
		const long long* table;
		std::vector<int> tree;

		void CreateTree(const long long* tablePointer, long long newLength)
		{
			length = newLength;
			table = tablePointer;
			tree.resize(4ll * length);
			for (long long i = 0ll; i < length; i++)
				tree[i] = 0ll;
			Build(1, 0, static_cast<int>(length) - 1);
		}

		int Build(int currentNode, int startNode, int endNode)
		{
			if (startNode == endNode)
			{
				tree[currentNode] = startNode;
				return startNode;
			}

			int middle = (startNode + endNode) / 2;
			int left = Build(currentNode * 2, startNode, middle);
			int right = Build(currentNode * 2 + 1, middle + 1, endNode);

			tree[currentNode] = (table[left] < table[right]) ? left : right;
			return tree[currentNode];
		}

		int Search(int node, int start, int end, int leftNode, int rightNode)
		{
			if (rightNode < start || end < leftNode)
				return -1;

			if (leftNode <= start && end <= rightNode)
				return tree[node];

			int middle = (start + end) / 2;
			int left = Search(node * 2, start, middle, leftNode, rightNode);
			int right = Search(node * 2 + 1, middle + 1, end, leftNode, rightNode);

			if (left == -1)
				return right;
			if (right == -1)
				return left;

			return (table[left] < table[right]) ? left : right;
		}

		long long GetResult(int start, int end)
		{
			if (start > end)
				return 0;

			int minIndex = Search(1, 0, static_cast<int>(length) - 1, start, end);
			long long left = GetResult(start, minIndex - 1);
			long long right = GetResult(minIndex + 1, end);
			return std::max({ table[minIndex] * (end - start + 1), left, right });
		}
	};


	SegmentTree segmentTree;
	std::vector<long long>::const_iterator iterator = list.begin();
	std::vector<long long> result;

	while (iterator != list.end() && *iterator != 0)
	{
		segmentTree.CreateTree(&(*(iterator + 1)), *iterator);
		iterator += segmentTree.length + 1;
		result.push_back(segmentTree.GetResult(0, static_cast<int>(segmentTree.length) - 1));
	}

	return result;
}

std::vector<std::string> Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], lineLength = list[1], start = list[2] - 1;
	std::vector<std::string> result(nodeLength);
	std::vector<int> table(nodeLength, std::numeric_limits<int>::max());
	std::vector<std::vector<std::pair<int, int>>> graph(nodeLength);
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> queue;
	queue.push({ 0, start });
	table[start] = 0;

	for (int i = 0; i < lineLength; i++)
	{
		int index = i * 3 + 3;
		graph[list[index] - 1].push_back({ list[index + 1] - 1, list[index + 2] });
	}

	while (!queue.empty())
	{
		int index = queue.top().second;

		if (queue.top().first > table[index])
		{
			queue.pop();
			continue;
		}

		queue.pop();

		for (auto& node : graph[index])
		{
			int cost = table[index] + node.second;
			if (table[node.first] < cost)
				continue;

			table[node.first] = cost;
			queue.push({ cost, node.first });
		}
	}

	for (int i = 0; i < nodeLength; i++)
	{
		if (table[i] == std::numeric_limits<int>::max())
			result[i] = "INF";
		else
			result[i] = std::to_string(table[i]);
	}

	return result;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list)
{
	int width = list[1], height = list[0], additionalWidth = list[3], additionalHeight = list[2];
	std::vector<std::vector<int>> result(height, std::vector<int>(width));
	std::vector<const int*> table(height + additionalHeight);

	int endHeight = height + additionalHeight;
	int endWidth = width + additionalWidth;

	for (size_t i = 0; i < table.size(); i++)
		table[i] = &list[i * (endWidth) + 4];

	for (int i = 0; i < additionalHeight && i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			result[i][j] = table[i][j];
			result[height - i - 1][j] = table[endHeight - i - 1][j + additionalWidth];
		}
	}


	int widthOverlapped = width - additionalWidth;

	for (int i = 0; i < width; i++)
	{
		result[0][i] = table[0][i];
		result[height - 1][i] = table[height + additionalHeight - 1][i + additionalWidth];
	}

	for (int i = additionalHeight; i < height - 1; i++)
	{
		int baseHeight = i - additionalHeight;
		int currentHeight = i + additionalHeight;

		for (int j = 0; j < additionalWidth; j++)
		{
			result[i][j] = table[i][j];
			result[i][width - j - 1] = table[currentHeight][endWidth - j - 1];
		}

		for (int j = additionalWidth; j < widthOverlapped; j++)
			result[i][j] = table[i][j] - result[baseHeight][j - additionalWidth];

	}

	return result;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list) // 13913
{
	std::vector<std::vector<int>> result(2);
	int start = list[0], end = list[1];
	if (start >= end)
	{
		for (int i = start; i >= end; i--)
			result[1].push_back(i);

		result[0].push_back(static_cast<int>(result[1].size()) - 1);
		return result;
	}
	int current, max = end * 2;
	std::vector<std::pair<int, int>> table(max + 1, {std::numeric_limits<int>::max(), -1});
	std::list<int> queue{ start };
	table[start] = { 0, -1 };

	while (!queue.empty())
	{
		current = queue.front();
		queue.pop_front();

		if (current == end)
			break;

		int cost = table[current].first + 1;
		int teleport = current * 2;

		if (teleport <= max && table[teleport].first > cost)
		{
			table[teleport] = { cost, current };
			queue.push_back(teleport);
		}

		if (current > 0 && table[current - 1].first > cost)
		{
			table[current - 1] = { cost, current };
			queue.push_back(current - 1);
		}
		if (current < max && table[current + 1].first > cost)
		{
			table[current + 1] = { cost, current };
			queue.push_back(current + 1);
		}
	}

	current = end;
	result[0].push_back(table[end].first);
	result[1].push_back(current);

	while (table[current].second != -1)
	{
		current = table[current].second;
		result[1].push_back(current);
	}
	std::reverse(result[1].begin(), result[1].end());

	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int length = list[0];
	const int* nodeList = &list[1];
	std::vector<int> result(length);

	result[0] = list[1];
	std::multiset<int> left{ list[1] };
	std::multiset<int> right;

	for (int i = 1; i < length; i++)
	{
		if (*left.rbegin() > nodeList[i])
			left.insert(nodeList[i]);
		else
			right.insert(nodeList[i]);

		if (left.size() > right.size() + 1)
		{
			right.insert(*left.rbegin());
			left.erase(std::prev(left.end()));
		}
		else if (left.size() < right.size())
		{
			left.insert(*right.begin());
			right.erase(right.begin());
		}

		result[i] = *left.rbegin();
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	struct Node
	{
		int index, x, y;
		int moveX{ 0 }, moveY{ 0 };
	};
	struct Cell
	{
		int cellInfomation;
		std::list<int> nodes;
	};

	int boardSize = list[0], nodeLength = list[1], turn = 0;
	int* boardList = const_cast<int*>(&list[2]);
	int* nodeList = const_cast<int*>(&list[2 + boardSize * boardSize]);
	std::vector<std::vector<Cell>> board(boardSize + 2, std::vector<Cell>(boardSize + 2));
	std::vector<Node> nodes(nodeLength);
	std::list<int> temperalList;

	for (int i = 0; i < boardSize + 2; i++)
	{
		board[i][0].cellInfomation = 2;
		board[i][boardSize + 1].cellInfomation = 2;
		board[0][i].cellInfomation = 2;
		board[boardSize + 1][i].cellInfomation = 2;
	}

	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			board[i + 1][j + 1].cellInfomation = boardList[i * boardSize + j];
		}
	}

	for (int i = 0; i < nodeLength; i++)
	{
		Node& currentNode = nodes[i];
		currentNode.index = i;
		int index = i * 3;
		currentNode.x = nodeList[index];
		currentNode.y = nodeList[index + 1];
		Cell& currentCell = board[currentNode.x][currentNode.y];

		switch (nodeList[index + 2])
		{
		case 1:
			currentNode.moveX = 0;
			currentNode.moveY = 1;
			break;
		case 2:
			currentNode.moveX = 0;
			currentNode.moveY = -1;
			break;
		case 3:
			currentNode.moveX = -1;
			currentNode.moveY = 0;
			break;
		case 4:
			currentNode.moveX = 1;
			currentNode.moveY = 0;
			break;
		default:
			break;
		}

		currentCell.nodes.push_back(i);
		if (currentCell.nodes.size() >= 4)
			return 0;
	}


	while (turn < 1000)
	{
		turn++;

		for (int i = 0; i < nodeLength; i++)
		{
			Node& currentNode = nodes[i];

			int nextX = currentNode.x + currentNode.moveX;
			int nextY = currentNode.y + currentNode.moveY;

			Cell& currentCell = board[currentNode.x][currentNode.y];
			Cell* NextCell = &board[nextX][nextY];

			if (NextCell->cellInfomation == 2)
			{
				currentNode.moveX *= -1;
				currentNode.moveY *= -1;
				nextX = currentNode.x + currentNode.moveX;
				nextY = currentNode.y + currentNode.moveY;
				NextCell = &board[nextX][nextY];

				if (NextCell->cellInfomation == 2)
					continue;
			}

			std::list<int>::iterator iterator = currentCell.nodes.begin();
			while (*iterator != i && iterator != currentCell.nodes.end())
				iterator++;


			for (auto innerIterator = iterator; innerIterator != currentCell.nodes.end(); innerIterator++)
			{
				nodes[*innerIterator].x = nextX;
				nodes[*innerIterator].y = nextY;
			}

			if (NextCell->cellInfomation == 1)
			{
				temperalList.splice(temperalList.end(), currentCell.nodes, iterator, currentCell.nodes.end());
				temperalList.reverse();
				NextCell->nodes.splice(NextCell->nodes.end(), temperalList, temperalList.begin(), temperalList.end());
			}
			else
				NextCell->nodes.splice(NextCell->nodes.end(), currentCell.nodes, iterator, currentCell.nodes.end());

			if (NextCell->nodes.size() >= 4)
				return turn;
		}
	}

	return -1;
}

std::vector<std::string> Solution(const std::vector<int>& list)
{
	int length = list[0], maxLength = -1, index = -1;
	const int* nodeList = &list[1];
	std::vector<std::pair<int, int>> table(length, { -1, 0 });
	std::vector<int> result;

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (nodeList[i] <= nodeList[j])
				continue;

			if (table[i].second >= table[j].second + 1)
				continue;

			table[i].first = j;
			table[i].second = table[j].second + 1;
		}

		if (maxLength < table[i].second)
		{
			maxLength = table[i].second;
			index = i;
		}
	}

	std::pair<int, int>& current = table[index];
	result.push_back(index);

	while (current.first != -1)
	{
		result.push_back(current.first);
		current = table[current.first];
	}

	reverse(result.begin(), result.end());

	index = static_cast<int>(result.size());
	std::vector<std::string> output(index + 1);
	output[0] = std::to_string(index) + "\n";

	for (int i = 0; i < index; i++)
	{
		output[i + 1] = std::to_string(nodeList[result[i]]);
		if (i != index - 1)
			output[i + 1] += " ";
	}

	return output;
}

int Solution(const std::vector<int>& list)
{
	struct Vector2
	{
		int x, y;
		static int Distance(const Vector2& one, const Vector2& other)
		{
			int x = one.x - other.x;
			int y = one.y - other.y;
			return x * x + y * y;
		}
	};
	int length = list[0];
	std::vector<Vector2> positionList(length);

	std::function<int(int, int)> recursive = [&](int left, int right) -> int
		{
			int minimum, middle;

			if (right - left <= 3)
			{
				minimum = std::numeric_limits<int>::max();
				for (int i = left; i <= right; i++)
				{
					for (int j = i + 1; j <= right; j++)
					{
						minimum = std::min(minimum, Vector2::Distance(positionList[i], positionList[j]));
					}
				}

				return minimum;
			}

			middle = (left + right) / 2;
			minimum = recursive(left, middle);
			minimum = std::min(minimum, recursive(middle + 1, right));

			std::vector<Vector2> candidates;
			candidates.reserve(right - left + 1);

			for (int i = left; i <= right; i++)
			{
				int xDistance = positionList[i].x - positionList[middle].x;
				if (xDistance * xDistance < minimum)
					candidates.push_back(positionList[i]);
			}
			int length = static_cast<int>(candidates.size());
			std::sort(candidates.begin(), candidates.end(), [&](const Vector2& left, const Vector2& right) { return left.y < right.y; });
			for (int i = 0; i < length; i++)
			{
				for (int j = i + 1; j < length; j++)
				{
					int yDistance = candidates[i].y - candidates[j].y;
					yDistance = yDistance * yDistance;

					if (yDistance > minimum)
						break;

					int xDistance = candidates[i].x - candidates[j].x;
					minimum = std::min(minimum, (yDistance + xDistance * xDistance));
				}
			}

			return minimum;
		};


	for (int i = 0; i < length; i++)
	{
		positionList[i].x = list[i * 2 + 1];
		positionList[i].y = list[i * 2 + 2];
	}

	std::sort(positionList.begin(), positionList.end(), [&](const Vector2& left, const Vector2& right) { return left.x < right.x; });

	return recursive(0, length - 1);
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], nodeLength = static_cast<int>(list.size()) - 2, debuff = list[1], result = 0;
	const int* table = &list[2];
	std::vector<bool> visitedTable(nodeLength, false);
	
	std::function<void(int, int)> backTracking = [&](int currentStep, int currentCost) -> void
		{
			if (currentCost < 500)
				return;

			if (currentStep == nodeLength)
			{
				result++;
				return;
			}

			for (int i = 0; i < nodeLength; i++)
			{
				if (visitedTable[i])
					continue;

				visitedTable[i] = true;
				backTracking(currentStep + 1, currentCost + table[i] - debuff);
				visitedTable[i] = false;
			}
		};

	backTracking(0, 500);
	return result;
}

int Solution(const std::vector<int>& list)
{
	struct Data
	{
		int cost, time;
		std::vector<bool> trackTable;
	};
	int length = list[0], nodeLength = static_cast<int>(list.size()) - 2, debuff = list[1], result = 0;
	const int* table = &list[2];

	std::vector<Data> stack{ {500, 0, std::vector<bool>(nodeLength, false)} };

	while (!stack.empty())
	{
		Data current = stack.back();
		stack.pop_back();

		if (current.time >= nodeLength)
			result++;

		current.time++;

		for (int i = 0; i < nodeLength; i++)
		{
			if (current.trackTable[i])
				continue;
			int cost = current.cost + table[i] - debuff;
			if (cost < 500)
				continue;

			stack.push_back(current);
			Data& back = stack.back();
			back.cost = cost;
			back.trackTable[i] = true;
		}

	}

	return result;
}

std::string Solution(const std::vector<std::string>& list)
{
	std::map<char, int> operators{ {'+', 0}, {'-', 0}, {'*', 1}, {'/', 1} };
	std::string result;
	std::vector<char> operatorStack;
	operatorStack.reserve(list.size());
	result.reserve(list.size());

	int brackets = 0;
	for (auto& character : list[0])
	{
		if (character - 'A' >= 0)
			result.push_back(character);

		else if (operatorStack.empty() || character == '(')
			operatorStack.push_back(character);

		else if (character == ')')
		{
			while (!operatorStack.empty() && operatorStack.back() != '(')
			{
				result.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.pop_back();
		}

		else
		{
			while (!operatorStack.empty() && operatorStack.back() != '(' && operators[operatorStack.back()] >= operators[character])
			{
				result.push_back(operatorStack.back());
				operatorStack.pop_back();
			}
			operatorStack.push_back(character);
		}
	}

	while (!operatorStack.empty())
	{
		if (operatorStack.back() != '(')
			result.push_back(operatorStack.back());
		operatorStack.pop_back();
	}

	return result;
}

std::vector<std::vector<int>> Solution(const std::vector<int>& list)
{
	std::function<void(std::vector<int>&, const int*, const int*, std::vector<int>&, int, int, int)> postorder = [&](std::vector<int>& output, const int* preorder, const int* inorder, std::vector<int>& inorderIndex, int preStart, int inStart, int inEnd)
		{
			if (inStart > inEnd)
				return;

			int root = preorder[preStart];
			int rootIndex = inorderIndex[root];
			int leftSize = rootIndex - inStart;

			if (leftSize > 0)
				postorder(output, preorder, inorder, inorderIndex, preStart + 1, inStart, rootIndex - 1); // left subtree
			if (rootIndex < inEnd)
				postorder(output, preorder, inorder, inorderIndex, preStart + 1 + leftSize, rootIndex + 1, inEnd); // right subtree

			output.push_back(root);
		};


	int caseLength = list[0], pointer = 1;
	std::vector<std::vector<int>> result(caseLength);

	for (int i = 0; i < caseLength; i++)
	{
		int nodeLength = list[pointer];
		pointer++;
		std::vector<int> inorderIndex(nodeLength + 1);
		const int* preorder = &list[pointer];
		const int* inorder = &list[pointer + nodeLength];

		for (int j = 0; j < nodeLength; j++)
			inorderIndex[inorder[j]] = j;

		postorder(result[i], preorder, inorder, inorderIndex, 0, 0, nodeLength - 1);

		pointer += nodeLength * 2;
	}
	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], length = list[1];
	std::vector<std::vector<int>> table(nodeLength, std::vector<int>(nodeLength, 0));
	std::vector<int> result(nodeLength, -1);
	
	for (int i = 0; i < length; i++)
	{
		int pointer = i * 2 + 2;
		int heavy = list[pointer] - 1;
		int light = list[pointer + 1] - 1;
		table[heavy][light] = 1;
		table[light][heavy] = -1;
	}


	for (int k = 0; k < nodeLength; k++) // 중간 대상
	{
		for (int i = 0; i < nodeLength; i++) // 출발 대상
		{
			if (k == i)
				continue;

			for (int j = 0; j < nodeLength; j++) // 도착 대상
			{
				if (i == j || j == k)
					continue;

				if (table[i][k] == 1 && table[k][j] == 1) // 출발 > 중간 > 도착
				{
					table[i][j] = 1;
					table[j][i] = -1;
				}
				else if (table[i][k] == -1 && table[k][j] == -1)
				{
					table[i][j] = -1;
					table[j][i] = 1;
				}
			}
		}
	}

	for (int i = 0; i < nodeLength; i++)
	{
		for (auto& object : table[i])
		{
			if (object == 0)
				result[i]++;

		}
	}

	return result;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], current;

	std::vector<std::vector<int>> tree(length);
	std::vector<int> table(length, 0);

	std::list<int> queue{ 0 };
	std::list<int> secondQueue;

	for (int i = 1; i < length; i++)
	{
		tree[list[i + 1]].push_back(i);
	}

	while (!queue.empty())
	{
		current = queue.front();
		queue.pop_front();

		secondQueue.push_front(current);

		for (auto& child : tree[current])
			queue.push_back(child);
	}

	while (!secondQueue.empty())
	{
		current = secondQueue.front();
		secondQueue.pop_front();
		length = static_cast<int>(tree[current].size());
		std::sort(tree[current].begin(), tree[current].end(), [&](const auto& left, const auto& right) { return table[left] > table[right]; });

		for (int i = 0; i < length; i++)
			table[current] = std::max(table[current], table[tree[current][i]] + i + 1);
	}

	return table[0];
}

int Solution(const std::vector<int>& list)
{
	int nodeNumber = list[0] + 1, groupNumber = list[1], length = list[2], pointer;
	std::vector<bool> state(nodeNumber, false);
	std::vector<std::set<int>> group(nodeNumber);
	std::vector<std::vector<int>> partyGroup(nodeNumber);
	std::set<int> groupSet;
	std::list<int> stack;

	for (int i = 0; i < length; i++)
	{
		state[list[3 + i]] = true;
		stack.push_back(list[3 + i]);
	}

	pointer = 3 + length;
	for (int i = 0; i < groupNumber; i++)
	{
		length = list[pointer];
		pointer++;
		for (int j = 0; j < length; j++)
		{
			int currentNode = list[pointer + j];
			partyGroup[currentNode].push_back(i);
			for (int k = j + 1; k < length; k++)
			{
				group[currentNode].insert(list[pointer + k]);
				group[list[pointer + k]].insert(currentNode);
			}
		}

		pointer += length;
	}

	while (!stack.empty())
	{
		pointer = stack.back();
		stack.pop_back();

		for (auto& item : partyGroup[pointer])
			groupSet.insert(item);

		for (auto& item : group[pointer])
		{
			if (state[item])
				continue;
			state[item] = true;
			stack.push_back(item);
		}
	}

	return groupNumber - static_cast<int>(groupSet.size());
}
std::vector<int> Solution(const std::vector<int>& list)
{
	int length = list[0], totalSum = 0;
	std::vector<int> result(length, 0);
	std::map<int, int> colorIndex;
	std::map<int, int>::iterator colorIterator;
	int ballColor, ballWeight;

	std::vector<std::pair<int, std::pair<int, int>>> balls(length);

	for (int i = 0; i < length; i++)
		balls[i] = { i, {list[i * 2 + 1], list[i * 2 + 2]} };

	std::sort(balls.begin(), balls.end(), [](const std::pair<int, std::pair<int, int>>& left, const std::pair<int, std::pair<int, int>>& right) { if(left.second.second == right.second.second) return left.second.first < right.second.first; return left.second.second < right.second.second; });

	for (int i = 0; i < length; i++)
	{
		ballColor = balls[i].second.first;
		ballWeight = balls[i].second.second;

		colorIterator = colorIndex.find(ballColor);
		if (colorIterator == colorIndex.end())
		{
			colorIndex[ballColor] = 0;
			colorIterator = colorIndex.find(ballColor);
		}

		result[balls[i].first] = totalSum - colorIterator->second;
		int index = i - 1;
		while (index >= 0 && balls[index].second.second == ballWeight)
		{
			if(balls[index].second.first != balls[i].second.first)
				result[balls[i].first] -= ballWeight;
			index--;
		}

		colorIterator->second += ballWeight;
		totalSum += ballWeight;
	}

	return result;
}

int Solution(const std::vector<std::string>& list)
{
	int width = std::stoi(list[0]), height = std::stoi(list[1]);
	struct Status
	{
		int x, y;
		int direction;// plusX 0, minusX 1, plusY 2, minusY 3;
		int count;
	};

	std::list<Status> queue;
	std::vector<std::vector<std::vector<int>>> visited(height, std::vector<std::vector<int>>(width, std::vector<int>(4, std::numeric_limits<int>::max())));
	Status end;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (list[y + 2][x] == 'C')
			{
				if (queue.size() != 0)
				{
					end = { x, y, 0, 0 };
					break;
				}
				queue.push_back({ x, y, 0, 0 });
				queue.push_back({ x, y, 1, 0 });
				queue.push_back({ x, y, 2, 0 });
				queue.push_back({ x, y, 3, 0 });
			}
		}
	}

	for (int i = 0; i < 4; i++)
		visited[queue.front().y][queue.front().x][i] = 0;


	auto CheckNode = [&](int x, int y, int direction, int count, int nextDirection) -> void
		{
			if (x < 0 || y < 0 || x >= width || y >= height || list[y + 2][x] == '*')
				return;
			bool streight = (nextDirection == direction);
			if (!streight)
				count++;
			if (list[y + 2][x] == 'C')
				visited[y][x][0] = std::min(visited[y][x][0], count);
			else if (visited[y][x][nextDirection] > count)
			{
				visited[y][x][nextDirection] = count;
				if (streight)
					queue.push_front({x, y, nextDirection, count });
				else
					queue.push_back({ x, y, nextDirection, count });
			}
		};

	while (!queue.empty())
	{
		Status current = queue.front();
		queue.pop_front();
		CheckNode(current.x + 1, current.y, current.direction, current.count, 0);
		CheckNode(current.x - 1, current.y, current.direction, current.count, 1);
		CheckNode(current.x, current.y + 1, current.direction, current.count, 2);
		CheckNode(current.x, current.y - 1, current.direction, current.count, 3);
	}

	return visited[end.y][end.x][0];
}

int Solution(const std::vector<int>& list)
{
	int nodeLength = list[0], limit = list[1], infomationLength = list[2];
	std::vector<std::pair<std::pair<int, int>, int>> nodeInfomation(infomationLength);
	std::vector<int> cargo(nodeLength);
	int weight = 0, sum = 0;

	for (int i = 0; i < infomationLength; i++)
	{
		int baseIndex = i * 3 + 3;
		nodeInfomation[i] = { {list[baseIndex] - 1, list[baseIndex + 1] - 1}, list[baseIndex + 2] };
	}

	std::sort(nodeInfomation.begin(), nodeInfomation.end(), [](const std::pair<std::pair<int, int>, int>& left, const std::pair<std::pair<int, int>, int>& right) { return left.first.second < right.first.second; });

	for (int i = 0; i < infomationLength; i++)
	{
		int end = nodeInfomation[i].first.second;
		int cargoStatus = 0;

		for (int j = nodeInfomation[i].first.first; j < end; j++)
			cargoStatus = std::max(cargoStatus, cargo[j]);

		cargoStatus = std::min(limit - cargoStatus, nodeInfomation[i].second);

		for (int j = nodeInfomation[i].first.first; j < end; j++)
			cargo[j] += cargoStatus;

		sum += cargoStatus;
	}

	return sum;
}

int Solution(const std::vector<std::string>& list)
{
	int length = stoi(list[0]), first, second;

	auto Flip = [](char& item) -> void { item = (item == '1') ? '0' : '1'; };
	auto Calculate = [&](const std::string& target, std::string& object, int length) -> int
		{
			int result = 0;
			for (int i = 0; i < length - 1; i++)
			{
				if (target[i] != object[i])
				{
					Flip(object[i]);
					Flip(object[i + 1]);
					if(i < length - 2)
						Flip(object[i + 2]);
					result++;
				}
			}

			if (target.back() != object.back())
				return -1;

			return result;
		};

	std::string buffer = list[1];
	first = Calculate(list[2], buffer, length);
	if(first == -1)
		first = std::numeric_limits<int>::max();

	buffer = list[1];
	Flip(buffer[0]);
	if(length > 1)
	Flip(buffer[1]);

	second = Calculate(list[2], buffer, length);
	if (second == -1)
		second = std::numeric_limits<int>::max();
	else
		second++;

	int result = std::min(first, second);
	return (result == std::numeric_limits<int>::max()) ? -1 : result;
}
int Solution(const std::vector<int>& list)
{
	int length = list[0], bridgeLength = list[1], limit = list[2];
	std::list<std::pair<int, int>> queue;
	int currentWeight = 0, time = 0;
	for (int i = 0; i < length; i++)
	{
		time++;
		int weight = list[i + 3];
		if (static_cast<int>(queue.size()) >= bridgeLength)
		{
			time = std::max(time, queue.front().first + bridgeLength);
			currentWeight -= queue.front().second;
			queue.pop_front();
		}

		while (currentWeight + weight > limit)
		{
			time = std::max(time, queue.front().first + bridgeLength);
			currentWeight -= queue.front().second;
			queue.pop_front();
		}
		currentWeight += weight;
		queue.push_back({ time, weight });
	}

	time = std::max(time, queue.back().first + bridgeLength);

	return time;
}

int Solution(int input)
{
	constexpr int max = std::numeric_limits<int>::max();
	std::vector<int> table(input + 6, max);
	table[0] = 0;

	for (int i = 0; i <= input; i++)
	{
		if (table[i] == max)
			continue;

		table[i + 3] = std::min(table[i + 3], table[i] + 1);
		table[i + 5] = std::min(table[i + 5], table[i] + 1);
	}

	if(table[input] == max)
		return -1;

	return table[input];
}

double Solution(const std::vector<double>& list)
{
	double jumpRange = list[2], jumpCost = list[3];
	std::pair<double, double> departure(list[0], list[1]);
	double distance = std::sqrt(departure.first * departure.first + departure.second * departure.second);
	if (jumpRange <= jumpCost)
		return distance;

	double result = distance;
	double JumpCount = static_cast<double>(static_cast<int>(distance / jumpRange));


	double case1 = (distance - jumpRange * JumpCount) + jumpCost * JumpCount; // 그냥 점프 후 이동 (다만 가능할 시 딱 맞춰서 점프 이동)
	result = std::min(result, case1);
	double case2 = (jumpRange * (JumpCount + 1) - distance) + jumpCost * (JumpCount + 1); // 오버슛
	result = std::min(result, case2);
	if (JumpCount == 0)// 돌아서 점프 이동
		result = std::min(result, jumpCost * 2);
	else
		result = std::min(result, jumpCost * (JumpCount + 1));

	return result;
}

std::vector<int> Solution(const std::vector<int>& list)
{
	int pointer = 1;
	int testCaseLength = list[0];
	std::vector<int> result(testCaseLength, 0);

	for (int i = 0; i < testCaseLength; i++)
	{
		int iLength = list[pointer], jLength = list[pointer + 1], length = list[pointer + 2];
		pointer += 3;

		std::vector<std::vector<int>> grid(iLength, std::vector<int>(jLength, 0));
		std::vector<std::pair<int, int>> point(length);
		std::list<std::pair<int, int>> stack;

		for (int j = 0; j < length; j++)
		{
			int baseIndex = pointer + j * 2;
			point[j].first = list[baseIndex];
			point[j].second = list[baseIndex + 1];
			grid[point[j].first][point[j].second] = 1;
		}

		for (auto& position : point)
		{
			if (grid[position.first][position.second] == 2)
				continue;

			result[i]++;

			stack.push_back(position);
			while (!stack.empty())
			{
				std::pair<int, int> currentPosition = stack.back();
				stack.pop_back();

				for (int x = -1; x < 2; x++)
				{
					for (int y = -1; y < 2; y++)
					{
						if (std::abs(x + y) != 1)
							continue;
						int currentX = x + currentPosition.first;
						int currentY = y + currentPosition.second;
						if (currentX < 0 || currentY < 0 || currentX >= iLength || currentY >= jLength)
							continue;

						if (grid[currentX][currentY] != 1)
							continue;

						grid[currentX][currentY] = 2;
						stack.push_back({currentX, currentY});
					}
				}

			}
		}


		pointer += length * 2;
	}

	return result;
}
std::vector<int> Solution(const std::vector<int>& list)
{
	int iLength = list[0], jLength = list[1], length = list[list[0] * list[1] + 2];

	int baseIndex = list[0] * list[1] + 2 + 1;
	auto GetIndex = [&](int i, int j) { return 2 + i * jLength + j; };
	std::vector<int> result(length, 0);

	for (int i = 0; i < length; i++)
	{
		int currentBaseIndex = i * 4 + baseIndex;
		int xMax = list[currentBaseIndex + 2];
		int yMax = list[currentBaseIndex + 3];

		for (int x = list[currentBaseIndex]; x <= xMax; x++)
		{
			for (int y = list[currentBaseIndex + 1]; y <= yMax; y++)
			{
				result[i] += list[GetIndex(x - 1, y - 1)];
			}
		}
	}
	return result;
}

int Solution(const std::vector<int>& list)
{
	int number = list[0], answer = 0;
	std::vector<std::pair<int, int>> lines(number);
	std::vector<std::pair<int, int>> mergedLines;


	for (int i = 0; i < number; i++)
		lines[i] = { list[i * 2 + 1], list[i * 2 + 2] };

	std::sort(lines.begin(), lines.end(), [](const std::pair<int, int>& left, const std::pair<int, int>& right) { if (left.first == right.first) return left.secondcond < right.secondcond; return left.first < right.first; });
	mergedLines.push_back(*lines.begin());
	for (int i = 1; i < number; i++)
	{
		std::pair<int, int>& current = mergedLines.back();
		if (current.secondcond <= lines[i].first)
		{
			if (current.secondcond == lines[i].first)
				current.secondcond = lines[i].secondcond;
			elsecond
				mergedLines.push_back(lines[i]);
		}
		elsecond //if (current.secondcond < lines[i].secondcond)
			current.secondcond = std::max(current.secondcond, lines[i].secondcond);
	}

	for (auto& line : mergedLines)
		answer += line.secondcond - line.first;

	return answer;
}

int Solution(const std::vector<int>& list)
{
	int number = list[0];
	std::vector<int> table(number + 1);

	for (int i = 2; i <= number; i++)
	{
		table[i] = table[i - 1] + 1;

		if(i % 3 == 0)
			table[i] = std::min(table[i], table[i / 3] + 1);
		if (i % 2 == 0)
			table[i] = std::min(table[i], table[i / 2] + 1);
	}

	return table[number];
}


int Solution(const std::vector<std::string>& list)
{
	int length = stoi(list[0]), groupCount = -1, link = 0;
	if (length == 1) return 0;
	std::vector<std::vector<int>> graph(length, std::vector<int>());
	std::vector<bool> visited(length, falsecond);


	for (int i = 0; i < length; i++)
	{
		for (int j = i + 1; j < length; j++)
		{
			if (list[i + 1][j] == 'N')
				continue;
			link++;
			graph[i].push_back(j);
			graph[j].push_back(i);
		}
	}

	if (link < length - 1)
		return -1;

	int leftLink = 0;
	std::list<int> stack;
	for (int i = 0; i < length; i++)
	{
		if (graph[i].empty())
			return -1;
		if (visited[i])
			continue;
		visited[i] = true;
		stack.push_back(i);
		groupCount++;
		link = 0;
		int nodeCount = 1;
		while (!stack.empty())
		{
			int currentNode = stack.back();
			stack.pop_back();
			for (auto& child : graph[currentNode])
			{
				link++;
				if (visited[child])
					continue;
				visited[child] = true;
				stack.push_back(child);
				nodeCount++;
			}
		}
		leftLink += link / 2 - (nodeCount - 1);
	}

	if (groupCount - 1> leftLink)
		return -1;

	return groupCount;
}

int Solution(const std::vector<int>& list)
{
	int length = list[0], root = 0, answer = 0, target = list.back();
	std::vector<std::vector<int>> tree(length);


	for (int i = 0; i < length; i++)
	{
		if (list[i + 1] == -1)
		{
			root = i;
			continue;
		}

		tree[list[i + 1]].push_back(i);
	}

	if (target == root) return 0;

	auto& subtree = tree[list[target + 1]];
	for (auto iterator = subtree.begin(); iterator != subtree.end(); iterator++)
	{
		if (*iterator == target)
		{
			subtree.erasecond(iterator);
			break;
		}
	}

	std::list<int> stack{root};

	while (!stack.empty())
	{
		int currentNode = stack.back();
		stack.pop_back();

		if (tree[currentNode].empty())
		{
			answer++;
			continue;
		}

		for (auto& child : tree[currentNode])
			stack.push_back(child);

	}

	return answer;
}

std::vector<int> Solution(const std::vector<int>& input)
{
	std::vector<int> list(std::next(input.begin()), input.end());
	int length = static_cast<int>(list.size()), sum = 0;
	std::sort(list.begin(), list.end());
	std::map<int, int> frequency;
	std::map<int, int>::iterator frequencyIterator;
	std::vector<std::map<int, int>::iterator> forSort;
	std::vector<int> answer(4);

	for (auto& item : list)
	{
		frequency[item]++;
		sum += item;
	}

	forSort.resecondrve(frequency.size());
	for (auto iterator = frequency.begin(); iterator != frequency.end(); iterator++)
		forSort.push_back(iterator);

	std::sort(forSort.begin(), forSort.end(), [&](const std::map<int, int>::iterator& left, const std::map<int, int>::iterator& right) { if(left->secondcond == right->secondcond) return left->first < right->first; return left->secondcond > right->secondcond; });

	if(forSort.size() > 1 && forSort[0]->secondcond == forSort[1]->secondcond)
		answer[2] = forSort[1]->first;
	elsecond
		answer[2] = forSort[0]->first;

	float result = static_cast<float>(sum) / static_cast<float>(length);
	answer[0] = static_cast<int>(result + ((result > 0) ? 0.5f : -0.5f));
	answer[1] = list[length / 2];
	answer[3] = list.back() - list.front();

	return answer;
}

int Solution(const std::vector<std::string>& list)
{
	int answer = 0;
	std::vector<std::string>::const_iterator listIterator = std::next(list.begin());

	std::map<std::string, bool> dancing;
	std::map<std::string, bool>::iterator dancingLeft;
	std::map<std::string, bool>::iterator dancingRight;
	dancing["ChongChong"] = true;


	while (listIterator != list.end())
	{
		dancingLeft = dancing.find(*listIterator);
		if (dancingLeft == dancing.end())
		{
			dancing[*listIterator] = falsecond;
			dancingLeft = dancing.find(*listIterator);
		}
		listIterator++;
		dancingRight = dancing.find(*listIterator);
		if (dancingRight == dancing.end())
		{
			dancing[*listIterator] = falsecond;
			dancingRight = dancing.find(*listIterator);
		}
		listIterator++;

		dancingLeft->secondcond |= dancingRight->secondcond;
		dancingRight->secondcond |= dancingLeft->secondcond;

	}
	for (auto iterator = dancing.begin(); iterator != dancing.end(); iterator++)
	{
		if (iterator->secondcond)
			answer++;
	}

	return answer;
}
int Solution(const std::vector<std::string>& list)
{
	int answer = 0;
	const std::string enter = "ENTER";
	std::vector<std::string>::const_iterator listIterator = std::next(list.begin());
	std::secondt<std::string> anyeong;
	while (listIterator != list.end())
	{
		if (*listIterator == enter)
		{
			anyeong.clear();
			listIterator++;
			continue;
		}

		if (anyeong.find(*listIterator) != anyeong.end())
		{
			listIterator++;
			continue;
		}

		anyeong.insecondrt(*listIterator);
		answer++;
		listIterator++;
	}

	return answer;
}


std::vector<std::string> Solution(const std::vector<int>& list)
{
	const static std::string Equilateral = "Equilateral", Scalene = "Scalene", Invalid = "Invalid", Isosceles = "Isosceles";
	int trueLength = static_cast<int>(list.size()),length = trueLength / 3;
	std::vector<std::string> answer(length);
	std::vector<int> triangle(3);
	for (int i = 0; i < length - 1; i++)
	{
		triangle[0] = list[i * 3];
		triangle[1] = list[i * 3 + 1];
		triangle[2] = list[i * 3 + 2];

		std::sort(triangle.begin(), triangle.end());

		if (triangle[0] + triangle[1] <= triangle[2])
			answer[i] = Invalid;

		elsecond if (triangle[0] == triangle[1])
		{
			if (triangle[1] == triangle[2])
				answer[i] = Equilateral;
			elsecond
				answer[i] = Isosceles;
		}
		elsecond if (triangle[1] == triangle[2])
			answer[i] = Isosceles;
		elsecond
			answer[i] = Scalene;
	}
	return answer;
}


std::vector<std::string> solution(const std::vector<int>& list)
{
	const static std::string factor = "factor" ,multiple = "multiple" ,neither = "neither";
	int length = list.size();
	std::vector<std::string> answer(length / 2);

	for (int i = 0; i < length; i+= 2)
	{
		int one = list[i];
		int other = list[i + 1];

		if (one > other)
		{
			if (other == 0)
				continue;
			answer[i / 2] = (one % other == 0) ? multiple : neither;
		}
		elsecond
		{
			if (one == 0)
				continue;
			answer[i / 2] = (other % one == 0) ? factor : neither;
		}
	}
	return answer;
}

int solution(const std::vector<int>& list)
{
	int target = list[0];

	if (target % 400 == 0)
		return 1;

	if (target % 4 == 0 && target % 100 != 0)
		return 1;

	return 0;
}

char solution(const std::vector<int>& list)
{
	int target = list[0];
	if(target > 89)
		return 'A';
	if (target > 79)
		return 'B';
	if (target > 69)
		return 'C';
	if (target > 59)
		return 'D';
	return 'F';
}
std::string solution(const std::vector<int>& list)
{
	if (list[0] > list[1])
		return ">";
	if (list[0] < list[1])
		return "<";

	return "==";
}
int solution(const std::vector<int>& list)
{
	return list[0] + list[1] + list[2];
}

std::vector<int> solution(const std::vector<int>& list)
{
	std::string number = std::to_string(list[1]);
	int one = std::stoi(number.substr(2, 1)) * list[0];
	int ten = std::stoi(number.substr(1, 1)) * list[0];
	int hundred = std::stoi(number.substr(0, 1)) * list[0];

	return {one, ten, hundred, one + ten * 10 + hundred + 100};
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



template<typename Type>
std::vector<Type> Input(int length)
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::vector<Type> output;
	Type input;

	for (int i = 0; i < length; i++)
	{
		std::cin >> input;
		output.push_back(input);
	}

	return output;
}
*/