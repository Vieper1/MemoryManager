#include "FixedSizeAllocator.h"
#include <iostream>
#include <cinttypes>


size_t FixedSizeAllocator::calcSpaceForManager(const size_t bits, const size_t blockSize)
{
	return sizeof(FixedSizeAllocator) + BitArray::calcSpaceForArray(bits) + (blockSize * bits);
}

void* FixedSizeAllocator::_alloc()
{
	unsigned long freeIndex = 0;
	const bool isFreeFound = bit_array_.findFirstSetBit(freeIndex);
	if (!isFreeFound)
	{
		printf("No free blocks found here!");
		return nullptr;
	}
	bit_array_.set(freeIndex, 0);
	return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(start_) + block_size_ * freeIndex);
}

void FixedSizeAllocator::_free(void* ptr)
{
	const size_t index = (reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(start_)) / block_size_;
	bit_array_.set(index, 1);
}

bool FixedSizeAllocator::_contains(const void* ptr) const
{
	const uintptr_t ptrAddress = reinterpret_cast<uintptr_t>(ptr);
	const uintptr_t startAddress = reinterpret_cast<uintptr_t>(start_);
	const uintptr_t endAddress = reinterpret_cast<uintptr_t>(this) + heap_size_;
	if (ptrAddress > startAddress && ptrAddress < endAddress)
		return true;
	return false;
}

// PRINT / DEBUG
void FixedSizeAllocator::ShowFreeBlocks(const FixedSizeAllocator* manager)
{
	printf("\nPRINTING ALL FREE BLOCKS\n=========================\n");
	/*for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
		if (manager->_freeDesc[i].offset != 0) {
			printf("Address => [%p] | From => [%d] | Size => [%d]\n", &manager->_freeDesc[i], manager->_freeDesc[i].offset, manager->_freeDesc[i].size);
		}
	}*/
}
void FixedSizeAllocator::ShowOutstandingAllocations(const FixedSizeAllocator* manager)
{
	printf("\nPRINTING ALL ALLOCATED BLOCKS\n=========================\n");
	/*for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
		if (manager->_usedDesc[i].offset != 0) {
			printf("Address => [%p] | From => [%d] | Size => [%d]\n", &manager->_usedDesc[i], manager->_usedDesc[i].offset, manager->_usedDesc[i].size);
		}
	}*/
}