#include "ResourceLibExporter.h"



ResourceLibExporter::ResourceLibExporter()
{
	
}


ResourceLibExporter::~ResourceLibExporter()
{
}

ResourceLibExporter * ResourceLibExporter::GetInstance()
{
	static ResourceLibExporter resourceLibExporter;

	return &resourceLibExporter;
}
