#pragma once

#include "MemoryManager.h"
#include <iostream>


// I'm using unsigned int instead of uintptr_t in my Block Descriptor to save space
// The size of the block has to be limited under UINT64_MAX


namespace HeapManagerProxy {
	void* _alloc(MemoryManager* manager, size_t i_bytes) {
		if (manager->_nUsedDesc == manager->_nMaxDesc) {
			printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
			return nullptr;
		}
		for (size_t i = 0; i < manager->_nFreeDesc; i++) {
			if (manager->_freeDesc[i].size > i_bytes) {
				void* ptr = getPointerOffset(manager->_start, static_cast<unsigned int>(manager->_freeDesc[i].offset + manager->_freeDesc[i].size - i_bytes), false);

				if (writeUsedDescriptor(manager, static_cast<unsigned int>(manager->_freeDesc[i].offset + manager->_freeDesc[i].size - i_bytes), i_bytes)) {
					manager->_freeDesc[i].size -= static_cast<unsigned int>(i_bytes);
				}
				return ptr;
			}
		}
		return nullptr;
	}

	void* _alloc(MemoryManager* manager, size_t i_bytes, unsigned int i_alignment) {
		if (manager->_nUsedDesc == manager->_nMaxDesc) {
			printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
			return nullptr;
		}
		for (size_t i = 0; i < manager->_nFreeDesc; i++) {
			void* ptr = getPointerOffset(manager->_start, static_cast<unsigned int>(manager->_freeDesc[i].offset + manager->_freeDesc[i].size - i_bytes), false);
			unsigned int mod = reinterpret_cast<uintptr_t>(ptr) % i_alignment;
			if (manager->_freeDesc[i].size > i_bytes + mod) {
				// Find an empty ALLOC descriptor to store data
				unsigned int allocIndex = int(UINT64_MAX);
				for (unsigned int j = 0; j < manager->_nUsedDesc; j++) {
					if (manager->_usedDesc[j].offset == 0) {
						allocIndex = j;
						break;
					}
				}
				if (manager->_nUsedDesc == manager->_nMaxDesc) {
					printf("ERROR: Reached maximum number of ALLOC descriptors, free something to continue! %s @ %d", __FILE__, __LINE__);
					return nullptr;
				}
				if (allocIndex > manager->_nUsedDesc) {
					allocIndex = manager->_nUsedDesc;
					manager->_nUsedDesc++;
				}
				// USED metadata set
				manager->_usedDesc[allocIndex].offset = static_cast<unsigned int>(manager->_freeDesc[i].offset + manager->_freeDesc[i].size - i_bytes - mod);
				manager->_usedDesc[allocIndex].size = static_cast<unsigned int>(i_bytes + mod);
				// FREE metadata set
				manager->_freeDesc[i].size -= static_cast<unsigned int>(i_bytes + mod);
				ptr = getPointerOffset(ptr, mod, true);
				return ptr;
			}
		}
		return nullptr;
	}

	

	bool writeUsedDescriptor(MemoryManager* manager, unsigned int offset, size_t i_bytes) {
		// Find an empty descriptor to store data
		unsigned int allocIndex = int(UINT64_MAX);
		for (unsigned int j = 0; j < manager->_nUsedDesc; j++) {
			if (manager->_usedDesc[j].offset == 0) {
				allocIndex = j;
				break;
			}
		}
		if (allocIndex > manager->_nUsedDesc) {
			allocIndex = manager->_nUsedDesc;
			manager->_nUsedDesc++;
		}
		manager->_usedDesc[allocIndex].offset = static_cast<unsigned int>(offset);
		manager->_usedDesc[allocIndex].size = static_cast<unsigned int>(i_bytes);

		return true;
	}

	bool _free(MemoryManager* manager, void * i_ptr) {
		unsigned int offset = static_cast<unsigned int>(static_cast<char*>(i_ptr) - static_cast<char*>(manager->_start));
		for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
			if (manager->_usedDesc[i].offset == offset) {
				for (unsigned int j = 0; j < manager->_nFreeDesc; j++) {
					if (manager->_freeDesc[j].offset == 0) {
						manager->_freeDesc[j].offset = manager->_usedDesc[i].offset;
						manager->_freeDesc[j].size = manager->_usedDesc[i].size;
						manager->_usedDesc[i].offset = 0;
						collect(manager);
						return true;
					}
				}
				if (manager->_nFreeDesc == manager->_nMaxDesc) {
					printf("\nERROR: Reached maximum number of FREE descriptors, defragmenting! %s @ %d", __FILE__, __LINE__);
					collect(manager);
					return false;
				}
				else {
					manager->_freeDesc[manager->_nFreeDesc].offset = manager->_usedDesc[i].offset;
					manager->_freeDesc[manager->_nFreeDesc].size = manager->_usedDesc[i].size;
					manager->_usedDesc[i].offset = 0;
					manager->_nFreeDesc++;
					collect(manager);
					return true;
				}
			}
		}
		return false;
	}

	void collect(MemoryManager* manager) {
		for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
			if (manager->_freeDesc[i].offset == 0) continue;
			for (unsigned int j = 0; j < manager->_nFreeDesc; j++) {
				if (manager->_freeDesc[j].offset == 0) continue;
				if (manager->_freeDesc[i].offset + manager->_freeDesc[i].size == manager->_freeDesc[j].offset) {
					manager->_freeDesc[i].size += manager->_freeDesc[j].size;
					manager->_freeDesc[j].offset = 0;
					manager->_freeDesc[j].size = 0;
				}
			}
		}
	}

	size_t getLargestFreeBlock(const MemoryManager* manager) {
		size_t largestFree = 0;
		for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
			if (manager->_freeDesc[i].size > largestFree) {
				largestFree = manager->_freeDesc[i].size;
			}
		}
		return largestFree;
	}

	size_t getTotalFreeMemory(const MemoryManager* manager) {
		size_t temp = 0;
		for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
			if (manager->_freeDesc[i].size != 0) {
				temp += manager->_freeDesc[i].size;
			}
		}
		return temp;
	}

	bool Contains(const MemoryManager* manager, void * i_ptr) {
		if (i_ptr > manager->_start && i_ptr < manager->_end) {
			return true;
		}
		return false;
	}

	bool IsAllocated(const MemoryManager* manager, void * i_ptr) {
		size_t diff = reinterpret_cast<uintptr_t>(i_ptr) - reinterpret_cast<uintptr_t>(manager->_start);
		for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
			if (manager->_usedDesc[i].offset == diff) {
				return true;
			}
		}
		return false;
	}

	BlockDesc* FindAllocated(const MemoryManager* manager, void* i_ptr) {
		size_t diff = (char*)i_ptr - (char*)manager->_start;
		for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
			if (manager->_usedDesc[i].offset == diff) {
				return &manager->_usedDesc[i];
			}
		}
		return nullptr;
	}







	// PRINT / DEBUG
	void ShowFreeBlocks(const MemoryManager* manager) {
		printf("\nPRINTING ALL FREE BLOCKS\n=========================\n");
		for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
			if (manager->_freeDesc[i].offset != 0) {
				printf("Address => [%p] | From => [%d] | Size => [%d]\n", &manager->_freeDesc[i], manager->_freeDesc[i].offset, manager->_freeDesc[i].size);
			}
		}
	}
	void ShowOutstandingAllocations(const MemoryManager* manager) {
		printf("\nPRINTING ALL ALLOCATED BLOCKS\n=========================\n");
		for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
			if (manager->_usedDesc[i].offset != 0) {
				printf("Address => [%p] | From => [%d] | Size => [%d]\n", &manager->_usedDesc[i], manager->_usedDesc[i].offset, manager->_usedDesc[i].size);
			}
		}
	}
	void clearBits(MemoryManager* manager, void* ptr, size_t size) {
		for (unsigned int i = 0; i < size; i++) {
			char* temp = (char*)ptr + i;
			*temp = '/';
		}
	}
	
	// MATH HELPERS
	static void* getPointerOffset(void* ptr, unsigned int offset, bool isReverse) {
		if (isReverse) {
			return reinterpret_cast<void*>((char*)ptr - offset);
		}
		else {
			return reinterpret_cast<void*>((char*)ptr + offset);
		}
	}



	// Visualizer
	void printBlockState(MemoryManager* manager, unsigned int nBytesPerRow) {
		printf("\n\nMemory Block @ %p\n\n(/) = Unusable Space\n(.) = Free Useable Space\n(x) = Allocation & Metadata\n==============================\n", manager->_start);
		unsigned int sqLen = static_cast<unsigned int>(sqrt(manager->_heapSize));
		char* charArray = (char*)malloc(sizeof(char) * manager->_heapSize);		// FOR DEBUGGING PURPOSES ONLY | Cannot store HeapSize sized array in my heap

		for (unsigned int i = 0; i < manager->_heapSize; i++) {
			charArray[i] = '/';
		}

		size_t diff = (char*)manager->_start - (char*)manager;

		// Mark Allocated Spots
		for (size_t i = 0; i < manager->_nUsedDesc; i++) {
			size_t offset = manager->_usedDesc[i].offset;
			if (offset == 0) continue;
			size_t size = manager->_usedDesc[i].size;
			for (size_t j = 0; j < size; j++) {
				charArray[diff + offset + j] = 65 + (i % 25);
			}
		}

		// Mark Free Spots
		for (size_t i = 0; i < manager->_nFreeDesc; i++) {
			size_t offset = manager->_freeDesc[i].offset;
			size_t size = manager->_freeDesc[i].size;
			for (size_t j = 0; j < size; j++) {
				charArray[diff + offset + j] = '.';
			}
		}

		// Mark descriptors on screen
		for (unsigned int i = 0; i < manager->_nUsedDesc; i++) {
			BlockDesc* temp = &manager->_usedDesc[i];
			if (temp->offset == 0) continue;
			unsigned int offset = static_cast<unsigned int>((char*)temp - (char*)manager->_start);
			for (int j = 0; j < sizeof(BlockDesc); j++) {
				charArray[diff + offset + j] = 65 + (i % 25);
			}
		}
		for (unsigned int i = 0; i < manager->_nFreeDesc; i++) {
			BlockDesc* temp = &manager->_freeDesc[i];
			unsigned int offset = static_cast<unsigned int>((char*)temp - (char*)manager->_start);
			for (int j = 0; j < sizeof(BlockDesc); j++) {
				charArray[diff + offset + j] = 49 + i;
			}
		}

		// Print everything on screen
		unsigned int nRows = static_cast<unsigned int>(manager->_heapSize) / nBytesPerRow;
		for (unsigned int i = 0; i < nRows; i++) {
			for (unsigned int j = 0; j < nBytesPerRow; j++) {
				printf("%c", charArray[(i * nBytesPerRow) + j]);
			}
			printf("\n");
		}

		free(charArray);
	}
}