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
	
	std::vector<Resources::Model*>* sceneModels = m_Data->GetModels();
	
	/*for (int i = 0; i < sceneModels->size(); ++i)
	{
		RegistryItem modelId;
		modelId->id = sceneModels->at(i)->GetId();
		m_Items.push_back(modelId);

		if (sceneModels->at(i)->GetMesh() != nullptr)
		{
			RegistryItem* meshId = new RegistryItem;
			meshId->id = sceneModels->at(i)->GetMesh()->GetId();
			m_Items.push_back(meshId);
		}
		if (sceneModels->at(i)->GetMaterial() != nullptr)
		{

			RegistryItem* materialId = new RegistryItem;
			materialId->id = sceneModels->at(i)->GetMaterial()->GetId();
			m_Items.push_back(materialId);
		}
	}*/
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
