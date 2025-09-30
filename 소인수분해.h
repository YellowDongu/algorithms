#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <string>
#include <map>

int �����佺�׳׽�����ó��(int n) // �⺻, O(n log n), n�� 10^6���� ����
{
	int answer = 0;
	std::vector<int> factors(n + 1, 0);

	for (int i = 1; i <= n; i++)
	{
		for (int j = i; j <= n; j += i)
		{
			factors.push_back(j);
		}
	}

	return answer;
}
std::vector<std::vector<int>> �����佺�׳׽�����ó��_��ü(int n)
{
	std::vector<std::vector<int>> divisors(n + 1);
	for (int i = 1; i <= n; i++)
	{
		for (int j = i; j <= n; j += i)
		{
			divisors[j].push_back(i);
		}
	}
	return divisors;
}

std::vector<int> �����佺�׳׽�����ó��_�ϳ�(int n)
{
	std::vector<int> divisors(n + 1);
	for (int i = 1; i <= n; i++)
	{
		for (int j = i; j <= n; j += i)
		{
			divisors.push_back(i);
		}
	}
	return divisors;
}

std::vector<int> ������Ž��(int n)//O(��n), n�� 10^9���� ������ ��� ����
{
	std::vector<int> result;
	for (int i = 1; i * i <= n; i++)
	{
		if (n % i == 0)
		{
			result.push_back(i);
			if (i != n / i) result.push_back(n / i); // ��Ī ���
		}
	}
	std::sort(result.begin(), result.end()); // ����
	return result;
}


std::vector<int> ���μ�����(long long n)
{
	std::vector<std::pair<long long, int>> factors; // (�Ҽ�, ����)
	for (long long i = 2; i * i <= n; i++)
	{
		int cnt = 0;
		while (n % i == 0)
		{
			n /= i;
			cnt++;
		}
		if (cnt > 0) factors.push_back({ i, cnt });
	}
	if (n > 1) factors.push_back({ n, 1 });

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