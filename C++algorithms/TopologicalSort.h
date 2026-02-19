#pragma once
#include <vector>
#include <list>
#include <queue>
#include <stdexcept> // runtime_error

struct TopologicalSort
{
	std::vector<int> sorted;
	std::vector<std::vector<int>> graph;

	TopologicalSort() = delete;
	TopologicalSort(const std::vector<std::vector<int>>& DAG) : graph(DAG) { }

	void Sort(void)
	{

	}

	void Sort_Khan(const std::vector<std::vector<int>>& graph, std::vector<int>& sorted)
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

	void Sort_DFS(const std::vector<std::vector<int>>& graph, std::vector<int>& sorted)
	{
		if (graph.empty())
			return;

		int length = static_cast<int>(graph.size());
		sorted.clear();
		sorted.reserve(length);
		std::vector<int> stack;
		std::vector<int> visited(length, -1);

		for (int i = 0; i < length; i++)
		{
			if (visited[i] == -1)
				stack.push_back(i);

			while (!stack.empty())
			{
				int index = stack.back();

				if (visited[index] == 1)
				{
					sorted.push_back(index);
					stack.pop_back();
					continue;
				}

				visited[index] = 1;
				for (auto& item : graph[index])
				{
					if (visited[item] > -1)
						continue;
					visited[item] = 0;
					stack.push_back(item);
				}
			}
		}
	}

	std::vector<int> Sort_DFS_Recursive(const std::vector<std::vector<int>>& graph)
	{
		int length = static_cast<int>(graph.size());
		std::vector<int> visited(length, -1);
		std::vector<int> sorted;

		for (int i = 0; i < length; i++)
		{
			if (visited[i] == -1)
				Sort_DFS_Recursive(graph, visited, sorted, i);
		}

		return sorted;
	}

	void Sort_DFS_Recursive(const std::vector<std::vector<int>>& graph, std::vector<int>& visited, std::vector<int>& sorted, int index)
	{
		visited[index] = 0;

		for (auto& item : graph[index])
		{
			if (visited[item] > -1)
			{
				if (visited[item] == 0)
					throw std::runtime_error("Cycle detected!");
				continue;
			}
			Sort_DFS_Recursive(graph, visited, sorted, item);
		}

		visited[index] = 1;
		sorted.push_back(index);
	}

};


