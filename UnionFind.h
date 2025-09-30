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