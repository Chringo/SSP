#include "ResourceLibExporter.h"



ResourceLibExporter::ResourceLibExporter()
{
	
}


ResourceLibExporter::~ResourceLibExporter()
{
	if (m_Items != nullptr)
		delete m_Items;
}

ResourceLibExporter * ResourceLibExporter::GetInstance()
{
	static ResourceLibExporter resourceLibExporter;

	return &resourceLibExporter;
}

void ResourceLibExporter::ExportBPF()
{
}
