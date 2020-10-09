#pragma once

#include <ctime>


class Math {
public:
	
	
	static inline int random(const int min, const int max) {
		if (max < min) return -1;
		srand((unsigned)time(NULL));
		return min + (rand() % (max - min));
	}

	static inline bool IsNearlyEqual(const float a, const float b, const float minPrecision = 0.0001f)
	{
		const float diff = fabs(a - b);
		const float absA = fabs(a);
		const float absB = fabs(b);

		const float largest = (absB > absA) ? absB : absA;
		if (diff <= largest * minPrecision)
			return true;
		return false;
	}

	static inline bool IsNearlyZero(const float value, const float errorTolerance = 0.0001f)
	{
		return fabs(value) <= errorTolerance;
	}

	static inline bool IsNaN(const float value)
	{
		if (value != value)
			return true;
		return false;
	}

	static inline float clamp(const float input, const float min, const float max)
	{
		if (input < min)
			return min;
		if (input > max)
			return max;
		return input;
	}

	static inline bool clampRef(float & input, const float min, const float max)
	{
		if (input < min)
		{
			input = min;
			return true;
		}
		if (input > max)
		{
			input = max;
			return true;
		}
		return false;
	}
};

