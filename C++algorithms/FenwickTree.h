#pragma once
#include <vector>

struct FenwickTree
{
	std::vector<int> origin;
	std::vector<int> bit;
    int size = 0;

    FenwickTree(const std::vector<int>& list) : origin(list), size(static_cast<int>(list.size()))
    {
        bit.resize(size + 1, 0);
        for (int i = 1; i <= size; i++)
            UpdateValue(i, origin[i]);
    }

    FenwickTree(const std::vector<int>::iterator& begin, const std::vector<int>::iterator& end) : origin(begin, end), size(static_cast<int>(origin.size()))
    {
        origin.insert(origin.begin(), 0);
        bit.resize(size + 1, 0);
        for (int i = 1; i <= size; i++)
            UpdateValue(i, origin[i]);
    }

    void ChangeValue(int index, int value)
    {
        int difference = value - origin[index];
        origin[index] = value;
        UpdateValue(index, difference);
    }

    void UpdateValue(int index, int value)
    {
        while (index <= size)
        {
            bit[index] += value;
            index += index & -index;
        }
    }

    // 1~index sum
    int Sum(int index)
    {
        int sum = 0;
        while (index > 0)
        {
            sum += bit[index];
            index -= index & -index;
        }
        return sum;
    }

    // x~y sum
    int Sum(int x, int y)
    {
        return Sum(y) - Sum(x - 1);
    }
};



struct SegmentTree
{
    std::vector<int> tree;
    std::vector<int> origin;
    size_t size;

    SegmentTree(const std::vector<int>& list) : origin(list)
    {
        size = origin.size();
        tree.resize(4 * size);
        for (size_t i = 0; i < size; i++)
            tree[size + i] = origin[i];
        for (size_t i = size - 1; i > 0; i--)
            tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }
    SegmentTree(const std::vector<int>::const_iterator& begin, const std::vector<int>::const_iterator& end) : origin(begin, end)
    {
        size = origin.size();
        tree.resize(4 * size);
        for (size_t i = 0; i < size; i++)
            tree[size + i] = origin[i];
        for (size_t i = size - 1; i > 0; i--)
            tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }

    void update(size_t index, int value)
    {
        index += size;
        tree[index] = value;
        while (index > 1)
        {
            index >>= 1;
            tree[index] = tree[index << 1] + tree[index << 1 | 1];
        }
    }

    int query(int left, int right)
    {
        if (left > right)
            std::swap(left, right);
        left += size;
        right += size;
        int result = 0;
        while (left < right)
        {
            if (left & 1)
                result += tree[left++];
            if (right & 1)
                result += tree[--right];
            left >>= 1;
            right >>= 1;
        }
        return result;
    }
};