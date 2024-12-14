#pragma once

#include <random>

class RandomManager
{
public:
	RandomManager() : randomDevice(), generator(randomDevice()) {}
	~RandomManager() = default;


	int randomInt();
	int randomIntRange(int min, int max);



private:
	std::random_device randomDevice;
	std::mt19937 generator;

};
int RandomManager::randomInt()
{
	std::uniform_int_distribution<int> distributor;
	return distributor(generator);
}

int RandomManager::randomIntRange(int min, int max)
{
	std::uniform_int_distribution<int> distributor(min, max);
	return distributor(generator);
}
