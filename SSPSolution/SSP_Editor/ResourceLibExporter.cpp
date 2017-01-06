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

void ResourceLibExporter::Initialize(FileImporter * m_FileImporter)
{
	this->m_FileImporter = m_FileImporter;
}

void ResourceLibExporter::ExportBPF()
{
}

void ResourceLibExporter::BuildRegistry()
{
	RegistryHeader m_Header{ m_FileImporter->GetFilePaths()->size() };
	m_Items.reserve(m_Header.numIds);

}

void ResourceLibExporter::WriteToBPF(char * m_BBF_File, const unsigned int fileSize)
{
}

bool ResourceLibExporter::Open()
{
	return true;
}

bool ResourceLibExporter::Close()
{
	return true;
}

char * ResourceLibExporter::ImportFromServer(unsigned int index, unsigned int & FileSize)
{
	return nullptr;
}
