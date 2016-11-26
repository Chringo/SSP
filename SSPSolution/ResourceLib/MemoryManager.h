#pragma once

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stdio.h>
#include <cassert>
#include "DefineHeader.h"
#include "Enumerations.h"


namespace Resources {
	class DLL_OPERATION MemoryManager

	{
	public:
		MemoryManager();
		~MemoryManager();

		void Alloc(Memory which, size_t memory_size); //size in KB, asserts a multiple of 16.
		unsigned int GetSize(Memory which) const; //returns allocated space for given memory in bytes
		char * GetMemory(Memory which); //returns a copy of pointer to given memory
		char *& Store(Memory which, size_t size); //returns reference to given memory location. If there's not enough space, more will be allocated.
		void Clear(Memory which); //resets free memory, use this to recycle the given memory. Unless you run this first, Store will just append data.
	private:
		size_t RoundUp(size_t num);
	
	private:
		const unsigned int multiple = 16;
		char * memory[Memory::MEM_CHUNKS];
		size_t size[Memory::MEM_CHUNKS];
		size_t freeSpace[Memory::MEM_CHUNKS];
		

	};
}



#endif // !MEMORYMANAGER_H