#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <string>
#include <map>

int �����佺�׳׽�����ó��(int size) // �⺻, O(n log n), n�� 10^6���� ����
{
	int answer = 0;
	std::vector<int> factors(size + 1, 0);

	for (int i = 1; i <= size; i++)
	{
		for (int j = i; j <= size; j += i)
		{
			factors.push_back(j);
		}
	}

	return answer;
}
std::vector<std::vector<int>> �����佺�׳׽�����ó��_��ü(int size)
{
	std::vector<std::vector<int>> divisors(size + 1);
	for (int i = 1; i <= size; i++)
	{
		for (int j = i; j <= size; j += i)
		{
			divisors[j].push_back(i);
		}
	}
	return divisors;
}

std::vector<int> �����佺�׳׽�����ó��_�ϳ�(int size)
{
	std::vector<int> divisors(size + 1);
	for (int i = 1; i <= size; i++)
	{
		for (int j = i; j <= size; j += i)
		{
			divisors.push_back(i);
		}
	}
	return divisors;
}

std::vector<int> ������Ž��(int size)//O(��n), n�� 10^9���� ������ ��� ����
{
	std::vector<int> result;
	for (int i = 1; i * i <= size; i++)
	{
		if (size % i == 0)
		{
			result.push_back(i);
			if (i != size / i) result.push_back(size / i); // ��Ī ���
		}
	}
	std::sort(result.begin(), result.end()); // ����
	return result;
}


std::vector<int> ���μ�����(long long size)
{
	std::vector<std::pair<long long, int>> factors; // (�Ҽ�, ����)
	for (long long i = 2; i * i <= size; i++)
	{
		int cnt = 0;
		while (size % i == 0)
		{
			size /= i;
			cnt++;
		}
		if (cnt > 0) factors.push_back({ i, cnt });
	}
	if (size > 1) factors.push_back({ size, 1 });

	std::vector<int> result = { 1 };
	for (auto& factor : factors)
	{
		auto p = factor.first;   // �Ҽ�
		auto cnt = factor.second; // ����

		std::vector<int> tmp;
		for (auto x : result)
		{
			long long val = x;
			for (int i = 0; i < cnt; i++)
			{
				val *= p;
				tmp.push_back(val);
			}
		}
		result.insert(result.end(), tmp.begin(), tmp.end());
	}
	sort(result.begin(), result.end());
	return result;
}