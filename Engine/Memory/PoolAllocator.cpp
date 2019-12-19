#include <iostream>
#include "PoolAllocator.h"


void* PoolAllocator::_alloc(size_t i_bytes)
{
	if (_nUsedDesc == _nMaxDesc)
	{
		printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
		return nullptr;
	}
	for (size_t i = 0; i < _nFreeDesc; i++)
	{
		if (_freeDesc[i].size > i_bytes)
		{
			void* ptr = PoolAllocator::getPointerOffset(_start, static_cast<unsigned int>(_freeDesc[i].offset + _freeDesc[i].size - i_bytes), false);

			if (writeUsedDescriptor(static_cast<unsigned int>(_freeDesc[i].offset + _freeDesc[i].size - i_bytes), i_bytes))
			{
				_freeDesc[i].size -= static_cast<unsigned int>(i_bytes);
			}
			return ptr;
		}
	}
	return nullptr;
}

void* PoolAllocator::_alloc(size_t i_bytes, unsigned int i_alignment)
{
	if (_nUsedDesc == _nMaxDesc)
	{
		printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
		return nullptr;
	}
	for (size_t i = 0; i < _nFreeDesc; i++)
	{
		void* ptr = PoolAllocator::getPointerOffset(_start, static_cast<unsigned int>(_freeDesc[i].offset + _freeDesc[i].size - i_bytes), false);
		unsigned int mod = reinterpret_cast<uintptr_t>(ptr) % i_alignment;
		if (_freeDesc[i].size > i_bytes + mod)
		{
			// Find an empty ALLOC descriptor to store data
			unsigned int allocIndex = int(UINT64_MAX);
			for (unsigned int j = 0; j < _nUsedDesc; j++)
			{
				if (_usedDesc[j].offset == 0)
				{
					allocIndex = j;
					break;
				}
			}
			if (_nUsedDesc == _nMaxDesc)
			{
				printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
				return nullptr;
			}
			if (allocIndex > _nUsedDesc)
			{
				allocIndex = _nUsedDesc;
				_nUsedDesc++;
			}
			// USED metadata set
			_usedDesc[allocIndex].offset = static_cast<unsigned int>(_freeDesc[i].offset + _freeDesc[i].size - i_bytes - mod);
			_usedDesc[allocIndex].size = static_cast<unsigned int>(i_bytes + mod);
			// FREE metadata set
			_freeDesc[i].size -= static_cast<unsigned int>(i_bytes + mod);
			ptr = PoolAllocator::getPointerOffset(ptr, mod, true);
			return ptr;
		}
	}
	return nullptr;
}



bool PoolAllocator::writeUsedDescriptor(unsigned int offset, size_t i_bytes)
{
	// Find an empty descriptor to store data
	unsigned int allocIndex = int(UINT64_MAX);
	for (unsigned int j = 0; j < _nUsedDesc; j++)
	{
		if (_usedDesc[j].offset == 0)
		{
			allocIndex = j;
			break;
		}
	}
	if (allocIndex > _nUsedDesc)
	{
		allocIndex = _nUsedDesc;
		_nUsedDesc++;
	}
	_usedDesc[allocIndex].offset = static_cast<unsigned int>(offset);
	_usedDesc[allocIndex].size = static_cast<unsigned int>(i_bytes);

	return true;
}

bool PoolAllocator::_free(void * i_ptr)
{
	unsigned int offset = static_cast<unsigned int>(static_cast<char*>(i_ptr) - static_cast<char*>(_start));
	for (unsigned int i = 0; i < _nUsedDesc; i++)
	{
		if (_usedDesc[i].offset == offset)
		{
			for (unsigned int j = 0; j < _nFreeDesc; j++)
			{
				if (_freeDesc[j].offset == 0)
				{
					_freeDesc[j].offset = _usedDesc[i].offset;
					_freeDesc[j].size = _usedDesc[i].size;
					_usedDesc[i].offset = 0;
					collect();
					return true;
				}
			}
			if (_nFreeDesc == _nMaxDesc)
			{
				printf("\nERROR: Reached maximum number of FREE descriptors, defragmenting! %s @ %d", __FILE__, __LINE__);
				collect();
				return false;
			}
			else
			{
				_freeDesc[_nFreeDesc].offset = _usedDesc[i].offset;
				_freeDesc[_nFreeDesc].size = _usedDesc[i].size;
				_usedDesc[i].offset = 0;
				_nFreeDesc++;
				collect();
				return true;
			}
		}
	}
	return false;
}

void PoolAllocator::collect()
{
	for (unsigned int i = 0; i < _nFreeDesc; i++)
	{
		if (_freeDesc[i].offset == 0) continue;
		for (unsigned int j = 0; j < _nFreeDesc; j++)
		{
			if (_freeDesc[j].offset == 0) continue;
			if (_freeDesc[i].offset + _freeDesc[i].size == _freeDesc[j].offset)
			{
				_freeDesc[i].size += _freeDesc[j].size;
				_freeDesc[j].offset = 0;
				_freeDesc[j].size = 0;
			}
		}
	}
}

size_t PoolAllocator::getLargestFreeBlock()
{
	size_t largestFree = 0;
	for (unsigned int i = 0; i < _nFreeDesc; i++)
	{
		if (_freeDesc[i].size > largestFree)
		{
			largestFree = _freeDesc[i].size;
		}
	}
	return largestFree;
}

size_t PoolAllocator::getTotalFreeMemory()
{
	size_t temp = 0;
	for (unsigned int i = 0; i < _nFreeDesc; i++)
	{
		if (_freeDesc[i].size != 0)
		{
			temp += _freeDesc[i].size;
		}
	}
	return temp;
}

bool PoolAllocator::Contains(const void * i_ptr)
{
	if (i_ptr > _start && i_ptr < _end)
	{
		return true;
	}
	return false;
}

bool PoolAllocator::IsAllocated(const void * i_ptr)
{
	size_t diff = reinterpret_cast<uintptr_t>(i_ptr) - reinterpret_cast<uintptr_t>(_start);
	for (unsigned int i = 0; i < _nUsedDesc; i++)
	{
		if (_usedDesc[i].offset == diff)
		{
			return true;
		}
	}
	return false;
}

BlockDesc* PoolAllocator::FindAllocated(const void* i_ptr)
{
	size_t diff = (char*)i_ptr - (char*)_start;
	for (unsigned int i = 0; i < _nUsedDesc; i++)
	{
		if (_usedDesc[i].offset == diff)
		{
			return &_usedDesc[i];
		}
	}
	return nullptr;
}







// PRINT / DEBUG
void PoolAllocator::ShowFreeBlocks()
{
	printf("\nPRINTING ALL FREE BLOCKS\n=========================\n");
	for (unsigned int i = 0; i < _nFreeDesc; i++)
	{
		if (_freeDesc[i].offset != 0)
		{
			printf("Address => [%p] | From => [%d] | Size => [%d]\n", &_freeDesc[i], _freeDesc[i].offset, _freeDesc[i].size);
		}
	}
}
void PoolAllocator::ShowOutstandingAllocations()
{
	printf("\nPRINTING ALL ALLOCATED BLOCKS\n=========================\n");
	for (unsigned int i = 0; i < _nUsedDesc; i++)
	{
		if (_usedDesc[i].offset != 0)
		{
			printf("Address => [%p] | From => [%d] | Size => [%d]\n", &_usedDesc[i], _usedDesc[i].offset, _usedDesc[i].size);
		}
	}
}
void PoolAllocator::clearBits(void* ptr, size_t size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		char* temp = (char*)ptr + i;
		*temp = '/';
	}
}

// MATH HELPERS
void* PoolAllocator::getPointerOffset(void* ptr, unsigned int offset, bool isReverse)
{
	if (isReverse)
	{
		return reinterpret_cast<void*>((char*)ptr - offset);
	}
	else
	{
		return reinterpret_cast<void*>((char*)ptr + offset);
	}
}