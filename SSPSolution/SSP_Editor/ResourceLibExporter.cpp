#include "ResourceLibExporter.h"



ResourceLibExporter::ResourceLibExporter()
{
	m_Output = new std::ofstream();
}


ResourceLibExporter::~ResourceLibExporter()
{
	delete m_Output;
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
	Open();

	BuildRegistry();
	
	Close();
}

void ResourceLibExporter::BuildRegistry()
{
	RegistryHeader m_Header{ m_FileImporter->GetFilePaths()->size() };
	m_Items.reserve(m_Header.numIds);
	
	std::vector<Resources::Model*>* sceneModels = m_Data->GetModels();
	
	for (int i = 0; i < sceneModels->size(); ++i)
	{
		RegistryItem item;
		item.id = sceneModels->at(i)->GetId();
		m_Items.push_back(item);
	}

	std::vector<Resources::Mesh*>* sceneMeshes = m_Data->GetMeshes();
	for (int i = 0; i < sceneMeshes->size(); ++i)
	{
		RegistryItem item;
		item.id = sceneMeshes->at(i)->GetId();
		m_Items.push_back(item);
	}

	std::vector<Resources::Material*>* sceneMaterials = m_Data->GetMaterials();
	for (int i = 0; i < sceneMaterials->size(); ++i)
	{
		RegistryItem item;
		item.id = sceneMaterials->at(i)->GetId();
		m_Items.push_back(item);
	}

	std::unordered_map<std::string, Resources::Texture*>* sceneTextures = m_Data->GetTextures();
	for (auto iterator = sceneTextures->begin(); iterator != sceneTextures->end(); ++iterator)
	{
		Resources::Texture * node = iterator->second;
		RegistryItem item;
		item.id = node->GetId();
		m_Items.push_back(item);
	}

	/*add skeleton here*/
	/*add animations here*/

	/*Assigning the right amount of space required, just in case someone "accidentally" put
	a file on the server in one of the folders who are meant for binary files. Like hey!
	I know! I'm going to put this file, which has nothing to do with our binary files in
	the binary files folders! Smart. Really smart.*/
	m_Header.numIds = m_Items.size();
	m_Output->write((char*)&m_Header, sizeof(RegistryHeader));
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
