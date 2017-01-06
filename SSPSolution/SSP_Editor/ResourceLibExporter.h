#ifndef SSP_EDITOR_RESOURCELIBEXPORTER_H
#define SSP_EDITOR_RESOURCELIBEXPORTER_H
#include "../ResourceLib/FileHeaders.h"
#include <string>
#include "DataHandler.h"
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
	DataHandler* m_Data = DataHandler::GetInstance();
	RegistryItem* m_Items = nullptr;

public:
	~ResourceLibExporter();

	static ResourceLibExporter* GetInstance();
	void ExportBPF();
private:
	void BuildRegistry();

};

#endif