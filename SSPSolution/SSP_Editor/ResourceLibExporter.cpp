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
	Resources::Resource::RawResourceData* fileData = (Resources::Resource::RawResourceData*)m_BBF_File;
	for (int i = 0; i < m_Items.size(); ++i)
	{
		/*finding the corresponding item to the current bbf file and adding the startpoint and size*/
		if (m_Items.at(i).id == fileData->m_id)
		{
			m_Items.at(i).startBit = this->m_Output->tellp();
			m_Items.at(i).byteSize = sizeof(Resources::Resource::RawResourceData) + sizeof(MaterialHeader);
			break;
		}
	}

	/*writing the material to the .bpf file*/
	this->m_Output->write(m_BBF_File, (sizeof(Resources::Resource::RawResourceData) + sizeof(MaterialHeader)));
	m_BBF_File += sizeof(Resources::Resource::RawResourceData);
	MaterialHeader* exportMaterial = (MaterialHeader*)(m_BBF_File);
	m_BBF_File += sizeof(MaterialHeader);

	/*getting the texture name lengths and stepping to the texture names*/
	unsigned int* textureNameLength[5];
	for (int i = 0; i < 5; ++i)
	{
		textureNameLength[i] = (unsigned int*)m_BBF_File;
		m_BBF_File += sizeof(unsigned int);
	}

	/*iterating over the 5 textures and writing the textures found in
	the current materials texture id*/
	for (int i = 0; i < 5; ++i)
	{
		std::string textureName = m_BBF_File;

		for (int j = 0; j < m_Items.size(); ++j)
		{
			/*if the texture being processed corresponds to a texture in the
			material, the function creates a RawResourceData struct and writes
			the texture information to the .bpf file*/
			if (m_Items.at(j).id == exportMaterial->textureIDs[i])
			{
				Resources::Resource::RawResourceData textureData;
				textureData.m_id = exportMaterial->textureIDs[i];
				textureData.m_resType = Resources::ResourceType::RES_TEXTURE;

				std::string substring = textureName.substr(textureName.rfind("/")+1);
				substring += '\0';
				m_Items.at(j).byteSize = (unsigned int)substring.length() + sizeof(Resources::Resource::RawResourceData);
				m_Items.at(j).startBit = this->m_Output->tellp();
				//CopyTextureFile(&textureName);

				m_Output->write((char*)&textureData, sizeof(Resources::Resource::RawResourceData));
				m_Output->write((char*)substring.c_str(), substring.length());
				break;
			}
		}

		/*even if the texture didn't exist in the material or did, the function jumps 
		to the next texture in the .mat file to see if it needs to be processed*/
		m_BBF_File += *textureNameLength[i];
	}
}

void ResourceLibExporter::CopyTextureFile(std::string * file)
{
	std::string newFilePath = m_DestinationPath.substr(0, m_DestinationPath.rfind(".")) + file->substr(file->rfind("/"));
	
	std::wstring oldPath(file->begin(), file->end());
	std::wstring newPath(newFilePath.begin(), newFilePath.end());

	/*edit bool if the desire for a check exists*/
	CopyFile(oldPath.c_str(), newPath.c_str(), false);
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
		std::string dotName = serverFiles->at(i).substr(serverFiles->at(i).rfind(".")).c_str();
		if (dotName != ".dds")
		{
			if (fromServer->LoadFile(serverFiles->at(i), data, &dataSize) == Resources::Status::ST_OK)
			{

				if (dotName != ".mat" && dotName == ".model" || dotName != ".mat" && dotName == ".bbf")
				{
					WriteToBPF(data, (const unsigned int)dataSize);
				}
				else if (dotName == ".mat")
				{
					WriteMatToBPF(data, (const unsigned int)dataSize);
				}
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
	m_Items.clear();
	m_Output->close();
	if(!m_Output->is_open())
		return true;
	return false;
}
