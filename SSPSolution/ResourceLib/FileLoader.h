#ifndef RESOURCELIB_FILELOADER_FILELOADER_H
#define RESOURCELIB_FILELOADER_FILELOADER_H
#include <fstream>
#include "Mesh.h"
#include "MemoryManager.h"
#include "FileHeaders.h"
namespace Resources
{
	/*
	AUTHOR: Martin Clementson
	
	This is the main file loader. It reads the BPF file by getting an id of a resource
	It reads the registry to get a RegIndex of that id. The Regindex says where the asset is in the BPF file.

	It uses preallocated memory buffers, provided by "MemoryManager" to hold the file data. 
	This nullifies any new and delete operations during runtime.
	In case of an asset is larger than the memorybuffers, they will automatically resize. 
	However, one should set the static variables below do avoid these situations.

	The function "LoadFile" Is a backdoor to directly read one file. This was used during testing
	when we had no BPF, Registry or level file.
	The function reads the whole file and puts it into a char buffer.
	*/

	class FileLoader
	{
	public:
		enum Files
		{
			BPF_FILE,

			NUM_FILES
		};
		enum FileState
		{
			CLOSED = 0,	//This is used to keep a file open. 
			OPEN   = 1	//When loading a level, we dont want to open/close the BPF for every function call.
		};
		
	private:
		static const size_t LEVEL_MEMORY	= 128; //kb
		static const size_t RESOURCE_MEMORY = 256; //kb
							 
		FileState     fileStates [NUM_FILES]; 
		std::string	  filePaths  [NUM_FILES];
		std::ifstream fileHandles[NUM_FILES];

		std::unordered_map<unsigned int, RegistryItem> m_fileRegistry;
		MemoryManager mem_manager;
		FileLoader();
	public:
		DLL_OPERATION static FileLoader* GetInstance(); //Singleton
		DLL_OPERATION virtual ~FileLoader();

		DLL_OPERATION bool OpenFile(Files file);
		DLL_OPERATION bool CloseFile(Files file);
		DLL_OPERATION RegistryItem* GetRegistryIndex(const unsigned int& objectId);
		DLL_OPERATION Resources::Status LoadResource(const unsigned int& id, char*& data, size_t* size);
		DLL_OPERATION Resources::Status LoadPlaceHolderMesh(std::string& path, char*& data, size_t* size);
		DLL_OPERATION Resources::Status LoadFile(std::string& path, char*& data, size_t* size);

	private:
		Resources::Status LoadRegistryFile(); //Load registry into memory on startup
	};

}

#endif
