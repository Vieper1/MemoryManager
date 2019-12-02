#pragma once

#include "time.h"
#include "Vector.h"
#include <iostream>



class Math {
public:
	static inline int random(const int min, const int max) {
		if (max < min) return -1;
		srand((unsigned)time(NULL));
		return min + (rand() % (max - min));
	}
};

