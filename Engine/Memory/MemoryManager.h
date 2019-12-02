#ifndef __HEAP_MANAGER_PROXY_H
#define __HEAP_MANAGER_PROXY_H

#pragma once

#include <cassert>
#include <iostream>


// SIZE = 8 bytes
struct BlockDesc {
	unsigned int offset;
	unsigned int size;
};

class MemoryManager {
public:	// Inaccessible when set to protected/private
	BlockDesc* _usedDesc;
	BlockDesc* _freeDesc;
	unsigned int _nMaxDesc = 0;
	unsigned int _nUsedDesc = 0;
	unsigned int _nFreeDesc = 0;
	size_t _heapSize = 0;
	size_t _memUsed = 0;
	void* _start;
	void* _end;
};



namespace HeapManagerProxy {
	static MemoryManager* create(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_numDescriptors) {
		MemoryManager* manager = static_cast<MemoryManager*>(i_pHeapMemory);
		manager->_start = static_cast<void*>((char*)manager + sizeof(MemoryManager));
		manager->_heapSize = i_HeapMemorySize;
		manager->_nMaxDesc = i_numDescriptors;
		manager->_memUsed = 0;
		manager->_end = (char*)manager->_start + manager->_heapSize - (static_cast<size_t>(manager->_nMaxDesc) * 2 * sizeof(BlockDesc)) - sizeof(MemoryManager);

		// Init Descriptors
		manager->_usedDesc = static_cast<BlockDesc*>(manager->_end);
		manager->_nUsedDesc = 0;

		manager->_freeDesc = static_cast<BlockDesc*>((void*)((char*)manager->_end + (manager->_nMaxDesc * sizeof(BlockDesc))));
		manager->_nFreeDesc = 1;
		manager->_freeDesc[0].offset = 4;
		manager->_freeDesc[0].size = static_cast<int>((char*)manager->_end - (char*)manager->_start);

		return manager;
	}
	static void destroy(MemoryManager* manager) {
		
	}
	void* _alloc(MemoryManager* i_pManager, size_t i_bytes);
	void* _alloc(MemoryManager* i_pManager, size_t i_bytes, unsigned int i_alignment);
	bool _free(MemoryManager* i_pManager, void* i_ptr);
	void collect(MemoryManager* i_pManager);
	size_t getLargestFreeBlock(const MemoryManager* i_pManager);
	size_t getTotalFreeMemory(const MemoryManager* i_pManager);
	bool Contains(const MemoryManager* i_pManager, void * i_ptr);
	bool IsAllocated(const MemoryManager* i_pManager, void* i_ptr);
	BlockDesc* FindAllocated(const MemoryManager* i_pManager, void* i_ptr);
	void ShowFreeBlocks(const MemoryManager* i_pManager);
	void printBlockState(MemoryManager* i_pManager, unsigned int alignment);
	void ShowOutstandingAllocations(const MemoryManager* i_pManager);
	void clearBits(MemoryManager* i_pManager, void* ptr, size_t size);

	bool writeUsedDescriptor(MemoryManager* manager, unsigned int offset, size_t i_bytes);

	static void* getPointerOffset(void* ptr, unsigned int offset, bool isReverse);
	static unsigned int getNearestAlign(const size_t value, const unsigned int alignment);
}

#endif // __HEAP_MANAGER_PROXY_H