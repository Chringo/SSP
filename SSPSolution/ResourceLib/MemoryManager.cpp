#include "MemoryManager.h"
using namespace Resources;

MemoryManager::MemoryManager()
{

	for (int i = 0; i < Memory::MEM_CHUNKS; i++) //initialize sizes as 0 and **ptrs as null
	{
		size[i] = 0;
		freeSpace[i] = 0;
		memory[i] = nullptr;
	}


}

MemoryManager::~MemoryManager()
{
	for (int i = 0; i < Memory::MEM_CHUNKS; i++)
	{
		if (memory[i] != nullptr)
		{
			delete memory[i];
		}
	};
}

void MemoryManager::Alloc(Memory which, size_t memory_size)
{
	assert(memory_size % 16 == 0);
	memory_size = memory_size << 10;

	memory[which] = new char[memory_size];
	size[which] = memory_size;
	freeSpace[which] = memory_size;

}

size_t Resources::MemoryManager::GetSize(Memory which) const
{
	return size[which];
}

char * Resources::MemoryManager::GetMemory(Memory which)
{
	return memory[which];
}

char *& Resources::MemoryManager::Store(Memory which, size_t size)
{
	if (size > freeSpace[which])
	{
		size_t newSize = RoundUp(this->size[which] + (size - freeSpace[which]));
		char * newMem = new char[newSize];

		memcpy(newMem, memory[which], this->size[which]);
		delete[] memory[which];

		freeSpace[which] += (newSize - this->size[which]);
		this->size[which] = newSize;
		
		memory[which] = newMem;
#ifdef _DEBUG
		std::cout << "expanded the size of the resource memory buffer" << std::endl;
#endif // _DEBUG

	}

	freeSpace[which] -= size;
	
	return memory[which];
}

void Resources::MemoryManager::Clear(Memory which)
{
	freeSpace[which] = size[which];
}

size_t Resources::MemoryManager::RoundUp(size_t num)
{
	assert(multiple);
	return ((num + multiple - 1) / multiple) * multiple;
}
