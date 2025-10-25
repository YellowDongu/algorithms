
#include <iostream>
#include "NumberOfCases.h"

int main()
{
	int size = 4; // 전체 항목 수
	int k = 3; // 선택할 항목 수

	std::vector<std::vector<int>> test = binaryCase(size, k);

	for (int i = 0; i < test.size(); i++)
	{
		for (int j = 0; j < test[i].size(); j++)
		{
			std::cout << test[i][j] << ":";
		}
		std::cout << std::endl;
	}

	return 0;
}


