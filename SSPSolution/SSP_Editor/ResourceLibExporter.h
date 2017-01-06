#ifndef SSP_EDITOR_RESOURCELIBEXPORTER_H
#define SSP_EDITOR_RESOURCELIBEXPORTER_H
#include "../ResourceLib/FileHeaders.h"
#include <string>
#include "DataHandler.h"
#include <vector>
#include "FileImporter.h"
/*
	Author: Martin Clementson
	This Class takes all the raw data that is on the server and creates a
	library file along with a registry file.
*/
class ResourceLibExporter
{
private:
	ResourceLibExporter();
	std::string m_DestinationPath = "../ResourceLib/AssetFiles/";
	std::vector<RegistryItem*> m_Items;

	DataHandler* m_Data = DataHandler::GetInstance();
	unsigned int m_Offset = 0;

public:
	~ResourceLibExporter();

	static ResourceLibExporter* GetInstance();
	void ExportBPF();
private:
	void BuildRegistry();

};

#endif