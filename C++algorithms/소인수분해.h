#pragma once
#include <vector>
#include <algorithm>
#include <list>
#include <utility>
#include <unordered_map>
#include <cmath>
#include <string>
#include <map>

// 한글 입력을 굳이 할 기회가 없어서 여기다가 함 해봤음
// 어짜피 컴파일러가 기호화를 하면서 맹글링을 해서 규칙에 따라 이름이 아예 달라질 수도 있고 - 이거에 대한건 정확히는 모름
// 사용처와 생성처에서의 이름만 같다면 이걸 쓸 수는 있으니깐 개인 혹은 소규모 프로젝트에서 사용이 가능하겠지만
// 변수로 인해서 문제가 생길 수도 있고 국룰도 국룰이지만 한글에서는 띄어쓰기로 인한 의미변경같은 소통문제 등등 여러가지의 이유로 안하는게 나을 듯 싶다.

int 에라토스테네스식전처리(int size) // 기본, O(n log n), n이 10^6까지 가능
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
std::vector<std::vector<int>> 에라토스테네스식전처리_전체(int size)
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

std::vector<int> 에라토스테네스식전처리_하나(int size)
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

std::vector<int> 제곱근탐색(int size)//O(√n)이라 제곱근 탐색임. n이 10^9여도 빠르게 계산 가능
{
	std::vector<int> result;
	for (int i = 1; i * i <= size; i++)
	{
		if (size % i == 0)
		{
			result.push_back(i);
			if (i != size / i) result.push_back(size / i); // 대칭 약수
		}
	}
	std::sort(result.begin(), result.end()); // 정렬
	return result;
}


std::vector<int> 소인수분해(long long size)
{
	std::vector<std::pair<long long, int>> factors; // (소수, 지수)
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
		auto p = factor.first;   // 소수
		auto cnt = factor.second; // 지수

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