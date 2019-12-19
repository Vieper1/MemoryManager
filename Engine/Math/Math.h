#pragma once

#include "time.h"
#include "Vector.h"
#include <iostream>
#include <random>


class Math {
public:
	
	
	static inline int random(const int min, const int max) {
		if (max < min) return -1;
		srand((unsigned)time(NULL));
		return min + (rand() % (max - min));
	}

	//static inline int randomNew(const int min, const int max)
	//{
	//	std::random_device rd; // obtain a random number from hardware
	//	std::mt19937 eng(rd()); // seed the generator
	//	std::uniform_int_distribution<> distr(25, 63); // define the range

	//	return min + (distr(eng) / 100 * (max - min));
	//}
};

