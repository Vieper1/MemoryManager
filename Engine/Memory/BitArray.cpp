#include "BitArray.h"
#include <stdexcept>


// Init
BitArray::BitArray(const size_t bits) :
	bits_(bits),
	array_length_((uint_bits_ + bits - 1) / uint_bits_),
	array_(reinterpret_cast<uint_x*>(reinterpret_cast<uintptr_t>(this) + sizeof(BitArray)))
{
	resetBits(1);
}
BitArray* BitArray::init(void*& placementPtr, const size_t bits)
{
	auto _mapper = reinterpret_cast<BitArray*>(placementPtr);
	_mapper->bits_ = bits;
	_mapper->array_length_ = (uint_bits_ + bits - 1) / uint_bits_;
	_mapper->array_ = reinterpret_cast<uint_x*>(reinterpret_cast<uintptr_t>(placementPtr) + sizeof(BitArray));
	_mapper->resetBits(0);

	return _mapper;
}
size_t BitArray::calcSpaceForArray(const size_t bits)
{
	const size_t arrayLength = (uint_bits_ + bits - 1) / uint_bits_;
	return sizeof(uint_x) * arrayLength;
}

void BitArray::setAll(const size_t bits)
{
	bits_ = bits;
	array_length_ = (uint_bits_ + bits - 1) / uint_bits_;
	array_ = reinterpret_cast<uint_x*>(reinterpret_cast<uintptr_t>(this) + sizeof(BitArray));
	resetBits(1);
}

void BitArray::resetBits(const bool value)
{
	for (size_t i = 0; i < array_length_; i++)
	{
		array_[i] = value ? -1 : 0;
	}
}
size_t BitArray::getNBits() const
{
	return bits_;
}
size_t BitArray::getArrayLength() const
{
	return array_length_;
}


// Runtime
void BitArray::set(size_t index, bool value) const
{
	const uint_x i = index / uint_bits_;
	if (i > array_length_) throw std::out_of_range("Index out of range!");
	const uint_x num = array_[i];

#ifdef WIN32
	const uint_x mask = 1u << (index % uint_bits_);
#else
	const uint_x mask = 1ULL << (index % uint_bits_);
#endif
	
	array_[i] = value ? num | mask : num & ~mask;
}
bool BitArray::get(size_t index) const
{
	const uint_x i = index / uint_bits_;
	if (i > array_length_ - 1) throw std::out_of_range("Index out of range!");
	const uint_x num = array_[i];

	
#ifdef WIN32
	const uint_x mask = 1u << (index % uint_bits_);
#else
	const uint_x mask = 1ULL << (index % uint_bits_);
#endif
	
	return num & mask;
}
bool BitArray::findFirstSetBit(unsigned long &index) const
{
	unsigned long tempIndex = 0;
	for (unsigned long i = 0; i < array_length_; i++)
	{
#ifdef WIN32
		const char isNonZero = _BitScanForward(&tempIndex, static_cast<unsigned long>(array_[i]));
#else
		const char isNonZero = _BitScanForward64(&tempIndex, static_cast<unsigned long>(array_[i]));
#endif
		if (isNonZero)
		{
			index = tempIndex + (static_cast<unsigned long>(uint_bits_) * i);
			return true;
		}
	}
	return false;
}

