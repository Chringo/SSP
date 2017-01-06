#ifndef SSP_EDITOR_RESOURCELIBEXPORTER_H
#define SSP_EDITOR_RESOURCELIBEXPORTER_H
#include "../ResourceLib/FileHeaders.h"

/*
	Author: Martin Clementson
	This Class takes all the raw data that is on the server and creates a
	library file along with a registry file.
*/
class ResourceLibExporter
{
public:
	ResourceLibExporter();
	~ResourceLibExporter();

	static ResourceLibExporter* GetInstance();
};

#endif