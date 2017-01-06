#include "ResourceLibExporter.h"



ResourceLibExporter::ResourceLibExporter()
{
	
}


ResourceLibExporter::~ResourceLibExporter()
{
	for (int i = 0; i < m_Items.size(); ++i)
		delete m_Items.at(i);
}

ResourceLibExporter * ResourceLibExporter::GetInstance()
{
	static ResourceLibExporter resourceLibExporter;

	return &resourceLibExporter;
}

void ResourceLibExporter::ExportBPF()
{
}
