#pragma once



// SIZE = 8 bytes
struct BlockDesc
{
	unsigned int offset;
	unsigned int size;
};

class PoolAllocator
{
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






	static PoolAllocator* create(void* i_pHeapMemory, size_t i_HeapMemorySize, unsigned int i_numDescriptors)
	{
		PoolAllocator* manager = static_cast<PoolAllocator*>(i_pHeapMemory);
		manager->_start = static_cast<void*>((char*)manager + sizeof(PoolAllocator));
		manager->_heapSize = i_HeapMemorySize;
		manager->_nMaxDesc = i_numDescriptors;
		manager->_memUsed = 0;
		manager->_end = (char*)manager->_start + manager->_heapSize - (static_cast<size_t>(manager->_nMaxDesc) * 2 * sizeof(BlockDesc)) - sizeof(PoolAllocator);

		// Init Descriptors
		manager->_usedDesc = static_cast<BlockDesc*>(manager->_end);
		manager->_nUsedDesc = 0;

		manager->_freeDesc = static_cast<BlockDesc*>((void*)((char*)manager->_end + (manager->_nMaxDesc * sizeof(BlockDesc))));
		manager->_nFreeDesc = 1;
		manager->_freeDesc[0].offset = 4;
		manager->_freeDesc[0].size = static_cast<int>((char*)manager->_end - (char*)manager->_start);

		return manager;
	}
	void destroy()
	{
		
	}
	
	void* _alloc(size_t i_bytes);
	void* _alloc(size_t i_bytes, unsigned int i_alignment);
	bool _free(void* i_ptr);
	void collect();
	size_t getLargestFreeBlock();
	size_t getTotalFreeMemory();
	bool Contains(const void * i_ptr);
	bool IsAllocated(const void* i_ptr);
	BlockDesc* FindAllocated(const void* i_ptr);
	void ShowFreeBlocks();
	void ShowOutstandingAllocations();
	void clearBits(void* ptr, size_t size);

	bool writeUsedDescriptor(unsigned int offset, size_t i_bytes);

	static void* getPointerOffset(void* ptr, unsigned int offset, bool isReverse);
};