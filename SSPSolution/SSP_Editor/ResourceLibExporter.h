#ifndef SSP_EDITOR_RESOURCELIBEXPORTER_H
#define SSP_EDITOR_RESOURCELIBEXPORTER_H
#include "../ResourceLib/FileHeaders.h"
#include <string>
#include "DataHandler.h"
#include <vector>
#include "FileImporter.h"
#include <fstream>
/*
	Author: Martin Clementson & David Wigelius
	This Class takes all the raw data that is on the server and creates a
	library file along with a registry file.

	The output .bpf file will be written by writing parts and then jumping back.
	The output file will be as following:
	-RegistryHeader-
	number of registry items
	----------------
	-RegistryItems-
	nr of registry items will be based on how many files are
	on the server, will convert all of them into the .bpf file
	----------------
	-bbf data-
	Raw bbf data taken straight from the server, the registry tells
	the application where the items are
	----------------

	The output file will be built in the following manner:
	1. Registry header
	2. Allocate space for the registry
	3. Jump to a point in the ouputfile after the allocated registry memory
	4. Iterate all the .bbf files
		-Find the corresponding registry item to the file
		-Fill the registry item with the startbit and the size of memory it will occopy
		-Write the .bbf file to the .bpf file
	5. All the registryItems are now filled with information
	6. Jump to a point after the Header (Where there's allocated "trash" memory)
	7. Write the registry items to the .bpf file
*/
class ResourceLibExporter
{
private:
	enum OverWriting
	{
		ONCE_FIRST,
		ONCE,
		NOTHING,
		ALL
	};

	ResourceLibExporter();
	std::string m_DestinationPath = "../Assets/AssetFile.bpf";

	std::vector<RegistryItem> m_Items;
	std::ofstream* m_Output;
	unsigned int m_Offset = sizeof(RegistryHeader);
	OverWriting m_overWrite = ONCE_FIRST;

	DataHandler* m_Data = DataHandler::GetInstance();
	QProgressBar* m_ProgressBar = nullptr;
	FileImporter* m_FileImporter = nullptr;
public:
	~ResourceLibExporter();

	static ResourceLibExporter* GetInstance();
	
	void Initialize(FileImporter* m_FileImporter, QProgressBar* m_ProgressBar);
	void ExportBPF();
private:
	void BuildRegistry();
	void WriteToBPF(char* m_BBF_File, const unsigned int fileSize);
	void WriteMatToBPF(char* m_BBF_File, const unsigned int fileSize);
	void CopyTextureFile(std::string *file);
	void HandleSceneData();
	void WriteRegistry();
	bool TextureExists(const std::string& filename);
	bool overWrite(const std::string& filename);

	bool Open();
	bool Close();
};

#endif