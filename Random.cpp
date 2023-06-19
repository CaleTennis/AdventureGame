#include "Random.hpp"
#pragma warning(disable : 4244) // Type conversion warning: possible data loss

long long Random::seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
std::mt19937 Random::mt_rand(seed);

long long Random::unbounded_rand()
{
	return mt_rand();
}

bool Random::fifty_fifty()
{
	auto gen = std::bind(std::uniform_real_distribution<double>(0, 1), std::ref(mt_rand));
	if (gen() >= 0.5)
		return true;
	else
		return false;
}

int Random::rand_int(size_t leftBound, size_t rightBound)
{
	auto gen = std::bind(std::uniform_real_distribution<float>(leftBound, rightBound), std::ref(mt_rand));
	return floor(gen());
}