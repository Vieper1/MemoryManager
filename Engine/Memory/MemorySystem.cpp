#pragma once

#include "PoolAllocator.h"
#include "FixedSizeAllocator.h"
#include "Windows.h"
#include <iostream>
#include <cinttypes>
#include "malloc.h"

#define LIMIT_1 16
#define LIMIT_2 32
#define LIMIT_3 96


namespace HeapManagerProxy
{
	static bool MsgsEnabled = true;
	void EnableDebugMessages(bool value)
	{
		MsgsEnabled = value;
	}
	
	
	static PoolAllocator* pool;
	static FixedSizeAllocator* fsa_1;
	static FixedSizeAllocator* fsa_2;
	static FixedSizeAllocator* fsa_3;
	

	void* GroupAlloc(size_t i_size)
	{
		// Alignment = 4
		if (i_size <= LIMIT_1)
		{
			return fsa_1->_alloc();
		}
		if (i_size <= LIMIT_2)
		{
			return fsa_2->_alloc();
		}
		if (i_size <= LIMIT_3)
		{
			return fsa_3->_alloc();
		}
		return pool->_alloc(i_size, 4);
	}

	void GroupFree(void* i_ptr)
	{
		if (fsa_1->_contains(i_ptr))
		{
			fsa_1->_free(i_ptr);
		}
		if (fsa_2->_contains(i_ptr))
		{
			fsa_2->_free(i_ptr);
		}
		if (fsa_3->_contains(i_ptr))
		{
			fsa_3->_free(i_ptr);
		}
		if (pool->Contains(i_ptr))
		{
			pool->_free(i_ptr);
		}
	}


	








	

	void InitializeMemorySystem()
	{
		// POOL ALLOCATOR INIT
		size_t heapSize = 1024 * 1024;
		const unsigned int numDescriptors = 2048;

		void* pHeapMemory = HeapAlloc(GetProcessHeap(), 0, heapSize);
		pool = PoolAllocator::create(pHeapMemory, heapSize, numDescriptors);
		
		// FIXED SIZE ALLOCATOR INIT
		size_t nBits = 100;
		size_t blockSize = 16;
		heapSize = FixedSizeAllocator::calcSpaceForManager(nBits, blockSize);
		pHeapMemory = HeapAlloc(GetProcessHeap(), 0, heapSize);
		fsa_1 = FixedSizeAllocator::create(pHeapMemory, heapSize, blockSize, nBits);

		nBits = 200;
		blockSize = 32;
		heapSize = FixedSizeAllocator::calcSpaceForManager(nBits, blockSize);
		pHeapMemory = HeapAlloc(GetProcessHeap(), 0, heapSize);
		fsa_2 = FixedSizeAllocator::create(pHeapMemory, heapSize, blockSize, nBits);

		nBits = 400;
		blockSize = 96;
		heapSize = FixedSizeAllocator::calcSpaceForManager(nBits, blockSize);
		pHeapMemory = HeapAlloc(GetProcessHeap(), 0, heapSize);
		fsa_3 = FixedSizeAllocator::create(pHeapMemory, heapSize, blockSize, nBits);
	}

	void Collect()
	{
		pool->collect();
	}

	void DestroyMemorySystem()
	{
		HeapFree(GetProcessHeap(), 0, pool);
		HeapFree(GetProcessHeap(), 0, fsa_1);
		HeapFree(GetProcessHeap(), 0, fsa_2);
		HeapFree(GetProcessHeap(), 0, fsa_3);
	}




	// malloc/free
	void* _cdecl CustomMalloc(size_t i_size)
	{
		if (MsgsEnabled) printf("malloc %zu\n", i_size);
		return HeapManagerProxy::GroupAlloc(i_size);
	}
	void _cdecl CustomFree(void * i_ptr)
	{
		if (MsgsEnabled) printf("free 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
		return HeapManagerProxy::GroupFree(i_ptr);
	}
}




// Interface for allocation


// new/delete
void * operator new(size_t i_size)
{
	if (HeapManagerProxy::MsgsEnabled) printf("new %zu\n", i_size);
	return HeapManagerProxy::GroupAlloc(i_size);
}
void operator delete(void * i_ptr)
{
	if (HeapManagerProxy::MsgsEnabled) printf("delete 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return HeapManagerProxy::GroupFree(i_ptr);
}

// new[]/delete[]
void * operator new[](size_t i_size)
{
	if (HeapManagerProxy::MsgsEnabled) printf("new [] %zu\n", i_size);
	return HeapManagerProxy::GroupAlloc(i_size);
}
void operator delete[](void * i_ptr)
{
	if (HeapManagerProxy::MsgsEnabled) printf("delete [] 0x%" PRIXPTR "\n", reinterpret_cast<uintptr_t>(i_ptr));
	return HeapManagerProxy::GroupFree(i_ptr);
}

// custom new to force usage of PoolAllocator
void * operator new(size_t i_size, bool shouldForcePool)
{
	if (HeapManagerProxy::MsgsEnabled) printf("new %zu\n", i_size);
	if (shouldForcePool)
		return HeapManagerProxy::pool->_alloc(i_size, 4);
	else
		return HeapManagerProxy::GroupAlloc(i_size);
}