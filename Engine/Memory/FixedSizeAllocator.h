#pragma once

#include "BitArray.h"

// 1 => FREE
// 0 => USED
class FixedSizeAllocator
{
public:
	static FixedSizeAllocator* create(void* i_pHeapMemory, size_t i_HeapMemorySize, size_t i_blockSize, size_t i_bits)
	{
		FixedSizeAllocator* manager = static_cast<FixedSizeAllocator*>(i_pHeapMemory);
		manager->block_size_ = i_blockSize;
		manager->heap_size_ = i_HeapMemorySize;
		manager->start_ = reinterpret_cast<void*>(
			reinterpret_cast<uintptr_t>(i_pHeapMemory) +
			sizeof(FixedSizeAllocator) +
			BitArray::calcSpaceForArray(i_bits)
		);

		int* test = (int*)manager->start_;
		*test = 1911;

		manager->bit_array_.setAll(i_bits);

		return manager;
	}

	static size_t calcSpaceForManager(const size_t bits, const size_t blockSize);
	void* _alloc();
	void _free(void* ptr);
	bool _contains(const void* ptr) const;


	void ShowFreeBlocks(const FixedSizeAllocator* i_pManager);
	void ShowOutstandingAllocations(const FixedSizeAllocator* i_pManager);

private:
	size_t block_size_;
	void* start_;
	size_t heap_size_;
	BitArray bit_array_;	// Always at the end
};