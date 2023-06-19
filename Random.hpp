#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <random>
#include <functional>
#include <chrono>


class Random
{
private:
	Random();
public:
	static long long seed;
	static std::mt19937 mt_rand;

	static long long unbounded_rand();
	static bool fifty_fifty();
	static int rand_int(size_t leftBound, size_t rightBound);
};

#endif