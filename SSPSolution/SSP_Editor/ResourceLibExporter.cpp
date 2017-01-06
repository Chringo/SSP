#include "ResourceLibExporter.h"



ResourceLibExporter::ResourceLibExporter()
{
	m_Output = new std::ofstream();
}


ResourceLibExporter::~ResourceLibExporter()
{
	delete m_Output;
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
	m_Output->open(m_DestinationPath, std::fstream::binary);
	if(m_Output->is_open())
		return true;
	return false;
}

bool ResourceLibExporter::Close()
{
	m_Output->close();
	if(!m_Output->is_open())
		return true;
	return false;
}

char * ResourceLibExporter::ImportFromServer(unsigned int index, unsigned int & FileSize)
{
	return nullptr;
}
