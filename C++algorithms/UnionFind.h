#pragma once
#include <vector>

struct UnionFind
{
    UnionFind(int number) : size(number)
    {
        parent.resize(size);
        for (int i = 0; i < size; i++)
            parent[i] = i;
    }

    int size = -1;
    std::vector<int> parent;

    int Find(int current)
    {
        while (parent[current] != current)
            current = parent[current];

        return current;
    }

    int Find_PathCompression(int target)
    {
        int current = target;
        while (parent[current] != current)
            current = parent[current];

        while (target != current)
        {
            int storage = parent[target];
            parent[target] = current;
            target = storage;
        }

        return current;
    }

    void Unite(int x, int y, bool withPathCompression = false)
    {
        if (withPathCompression)
            parent[Find_PathCompression(y)] = Find_PathCompression(x);
        else
            parent[Find(y)] = Find(x);
    }
};

struct UnionFindAdvenced
{
    UnionFindAdvenced(int number) : size(number)
    {
        parent.resize(size);
        rank.resize(size, 0);
        for (int i = 0; i < size; i++)
            parent[i] = i;
    }

    int size = -1;
    std::vector<int> parent;
    std::vector<int> rank;


    int Find(int current)
    {
        while (parent[current] != current)
            current = parent[current];

        return current;
    }

    int Find_PathCompression(int target)
    {
        int current = target;
        while (parent[current] != current)
            current = parent[current];

        while (target != current)
        {
            int storage = parent[target];
            parent[target] = current;
            target = storage;
        }

        return current;
    }

    void Unite(int x, int y, bool withPathCompression = false)
    {
        if (withPathCompression)
        {
            y = Find_PathCompression(y);
            x = Find_PathCompression(x);
        }
        else
        {
            x = Find(x);
            y = Find(y);
        }

        if (rank[x] < rank[y])
            parent[x] = y;
        else
        {
            parent[y] = x;
            if (rank[x] == rank[y])
                rank[x]++;
        }
    }
};


//https://gmlwjd9405.github.io/2018/08/31/algorithm-union-find.html





struct Edge
{
    int to;
    int weight;
    bool operator>(const Edge& other) const
    {
        return weight > other.weight;
    }
};
#include <queue>
int prim(int start, const std::vector<std::vector<std::pair<int, int/*weight, to*/>>>& graph)
{
    int n = graph.size();
    std::vector<bool> visited(n, false);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    int totalCost = 0;

    // 시작 정점의 간선들을 넣기
    visited[start] = true;
    for (auto& edge : graph[start])
    {
        pq.push(edge);
    }

    // 트리 확장
    while (!pq.empty())
    {
        std::pair<int, int> cur = pq.top();
        pq.pop();

        if (visited[cur.second]) continue;

        visited[cur.second] = true;
        totalCost += cur.first;

        for (auto& next : graph[cur.second])
        {
            if (!visited[next.second])
            {
                pq.push(next);
            }
        }
    }

    return totalCost;
}


