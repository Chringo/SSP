#ifndef RESOURCELIB_FILELOADER_FILELOADER_H
#define RESOURCELIB_FILELOADER_FILELOADER_H
#include <fstream>
#include "Mesh.h"
#include "MemoryManager.h"
namespace Resources
{

	class FileLoader{
	public:
		enum Files{
			RESOURCE_FILE = 0,
			REG_FILE = 1
		};
		enum FileState{
			CLOSED = 0,
			OPEN = 1
		};
	private:
		static const size_t NUM_FILES		= 2;
		static const size_t LEVEL_MEMORY	= 128; //kb
		static const size_t RESOURCE_MEMORY = 128; //kb
		
		FileState     fileStates[NUM_FILES];
		std::string	  filePaths[NUM_FILES];
		std::ifstream fileHandles[NUM_FILES];

		MemoryManager mem_manager;
		FileLoader();
	public:
		static FileLoader* GetInstance();
		virtual ~FileLoader();
	
		bool OpenFile(Files file);
		bool CloseFile(Files file);
		Resources::Status LoadResource(const unsigned int& id, char*& data, size_t* size);
	
		Resources::Status LoadPlaceHolderMesh(std::string& path, char*& data, size_t* size);
	};

}

#endif
