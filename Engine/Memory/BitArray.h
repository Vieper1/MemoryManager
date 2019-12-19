#pragma once

#include <climits>
#include <cstdint>

#ifdef WIN32
typedef uint32_t uint_x;
#else
typedef uint64_t uint_x;
#endif

class BitArray
{
public:
	BitArray(const size_t bits);
	
	static BitArray* init(void* &placementPtr, const size_t bits);
	static size_t calcSpaceForArray(const size_t bits);
	void setAll(const size_t bits);
	void resetBits(const bool value);
	void set(size_t index, bool value) const;
	bool get(size_t index) const;
	bool findFirstSetBit(unsigned long &index) const;
	size_t getNBits() const;
	size_t getArrayLength() const;


	static const uint_x uint_bits_ = CHAR_BIT * sizeof(uint_x);

private:
	size_t bits_ = 0;
	size_t array_length_ = 0;
	uint_x* array_ = nullptr;
};
