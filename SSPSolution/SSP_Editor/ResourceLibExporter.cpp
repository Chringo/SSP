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
	if (Open())
	{

		/*building the registry*/
		BuildRegistry();

		/*handles all files in the scene and exports the raw data from the .bbf files
		to the .bpf file*/
		HandleSceneData();

		/*writing the registry to .bpf file*/
		WriteRegistry();

		Close();
	}
}

void ResourceLibExporter::BuildRegistry()
{
	RegistryHeader m_Header{ m_FileImporter->GetFilePaths()->size() };
	m_Items.reserve(m_Header.numIds);
	
	/*iterating over all the models*/
	std::vector<Resources::Model*>* sceneModels = m_Data->GetModels();
	for (int i = 0; i < sceneModels->size(); ++i)
	{
		RegistryItem item;
		item.byteSize = 0;
		item.startBit = 0;
		item.id = sceneModels->at(i)->GetId();
		m_Items.push_back(item);
	}

	/*iterating over all the meshes*/
	std::vector<Resources::Mesh*>* sceneMeshes = m_Data->GetMeshes();
	for (int i = 0; i < sceneMeshes->size(); ++i)
	{
		RegistryItem item;
		item.byteSize = 0;
		item.startBit = 0;
		item.id = sceneMeshes->at(i)->GetId();
		m_Items.push_back(item);
	}

	/*iterating over all the materials*/
	std::vector<Resources::Material*>* sceneMaterials = m_Data->GetMaterials();
	for (int i = 0; i < sceneMaterials->size(); ++i)
	{
		RegistryItem item;
		item.byteSize = 0;
		item.startBit = 0;
		item.id = sceneMaterials->at(i)->GetId();
		m_Items.push_back(item);
	}

	/*iterating over all the textures*/
	std::unordered_map<std::string, Resources::Texture*>* sceneTextures = m_Data->GetTextures();
	for (auto iterator = sceneTextures->begin(); iterator != sceneTextures->end(); ++iterator)
	{
		Resources::Texture * node = iterator->second;
		RegistryItem item;
		item.byteSize = 0;
		item.startBit = 0;
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
	m_Offset = m_Offset + (sizeof(RegistryItem)*m_Header.numIds);
	m_Output->write((char*)&m_Header, sizeof(RegistryHeader));

	/*			skipping over the registry to write later
	I---------------------------------------------------------------------I
	The files will be written in a space after the allocated "trash" memory
	that the registry will occopy later
	I---------------------------------------------------------------------I
	*/
	long pos = m_Output->tellp();
	pos += sizeof(RegistryItem)*m_Items.size();
	m_Output->seekp(pos);
}

void ResourceLibExporter::WriteToBPF(char * m_BBF_File, const unsigned int fileSize)
{
	Resources::Resource::RawResourceData* u_data = (Resources::Resource::RawResourceData*)m_BBF_File;
	for (int i = 0; i < m_Items.size(); ++i)
	{
		/*finding the corresponding item to the current bbf file and adding the startpoint and size*/
		if (m_Items.at(i).id == u_data->m_id)
		{
			m_Items.at(i).startBit = this->m_Output->tellp();
			m_Items.at(i).byteSize = fileSize;
			break;
		}
	}
	this->m_Output->write(m_BBF_File, fileSize);
}

void ResourceLibExporter::WriteMatToBPF(char * m_BBF_File, const unsigned int fileSize)
{
	/*special case function that converts .mat files to raw data in the .bpf file*/

}

void ResourceLibExporter::HandleSceneData()
{
	/*getting a pointer to the fileloader singleton, will be used to load the .bbf files*/
	Resources::FileLoader* fromServer =	Resources::FileLoader::GetInstance();
	std::vector<std::string>* serverFiles = m_FileImporter->GetFilePaths();
	char* data; size_t dataSize;

	/*iterating over all the serverfiles to write them to the .bpf file*/
	for (int i = 0; i < serverFiles->size(); ++i)
	{
		//need to check if it's a material or texture;
		if (fromServer->LoadFile(serverFiles->at(i), data, &dataSize) == Resources::Status::ST_OK)
		{
			std::string dotName = serverFiles->at(i).substr(serverFiles->at(i).rfind(".")).c_str();

			if (dotName != ".mat" && dotName != ".dds")
			{
				WriteToBPF(data, (const unsigned int)dataSize);
			}
			else if (dotName == ".mat")
			{

			}
		}
	}
}

void ResourceLibExporter::WriteRegistry()
{
	/*setting the writing position to after the header*/
	long pos = sizeof(RegistryHeader);
	m_Output->seekp(pos);

	/*writing the registry now that it has all the information*/
	m_Output->write((char*)m_Items.data(), sizeof(RegistryItem)*m_Items.size());
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
