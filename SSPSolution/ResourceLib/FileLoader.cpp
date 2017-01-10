#include "FileLoader.h"
#include "FileHeaders.h"

Resources::FileLoader::FileLoader()
{
	this->mem_manager.Alloc(Resources::Memory::MEM_LEVEL, LEVEL_MEMORY);
	this->mem_manager.Alloc(Resources::Memory::MEM_RES, RESOURCE_MEMORY);
	
	filePaths[BPF_FILE] = std::string("../ResourceLib/AssetFiles/AssetFile.bpf");

	fileHandles[BPF_FILE].rdbuf()->pubsetbuf(0, 0);	 //Disable streaming buffers

	for (size_t i = 0; i < NUM_FILES; i++)
	{
		fileStates[i] = CLOSED;
	}

	LoadRegistryFile();
}


Resources::FileLoader * Resources::FileLoader::GetInstance()
{
	static FileLoader instance;
	return &instance;
}

Resources::FileLoader::~FileLoader()
{
}

bool Resources::FileLoader::OpenFile(Files file)
{
	if (fileStates[file] == CLOSED)
	{
		fileHandles[file].open(filePaths[file].c_str(), std::fstream::binary);
		if (fileHandles[file])
		{
			fileStates[file] = OPEN;
			return true;
		}
		else
			return false;
	}
#ifdef _DEBUG
	std::cout << "File was already open, did you forget to close?" << std::endl;
#endif // _DEBUG

	return true;
}

bool Resources::FileLoader::CloseFile(Files file)
{

	if (fileStates[file] == OPEN)
	{
		fileHandles[file].close();
		fileStates[file] = CLOSED;
	}
	assert(!fileHandles[file].is_open());
	return true;
}

RegistryItem * Resources::FileLoader::GetRegistryIndex(const unsigned int & objectId)
{

	std::unordered_map<unsigned int, RegistryItem>::iterator got = m_fileRegistry.find(objectId);
	if (got == m_fileRegistry.end()) { // if the index does not exists in memory

		return nullptr;
	}
	else {
		return &got->second;
	}
}

Resources::Status Resources::FileLoader::LoadResource(const unsigned int& id, char*& data, size_t * size)
{
	if (fileStates[BPF_FILE] == CLOSED)
	{
		std::cout << "File is closed" << std::endl;
		return Status::ST_FILE_CLOSED;
	}

	RegistryItem* itemPtr = this->GetRegistryIndex(id);	//Get Registry info for this resource
	if (itemPtr == nullptr)
		return Status::ST_RES_MISSING;

	std::ifstream* infile = &fileHandles[BPF_FILE];
	infile->seekg(itemPtr->startBit); // Jump to the start of the resource data in the library

	mem_manager.Clear(Resources::Memory::MEM_RES);							// Clear Resource Mem
	data = mem_manager.Store(Resources::Memory::MEM_RES, itemPtr->byteSize);// Request new mem for this resource

	infile->read(data, itemPtr->byteSize); //Read the whole item data

//MainHeader mainHeader;
//infile->read((char*)&mainHeader, sizeof(MainHeader)); //Read main header
//
//MeshHeader meshHeader;
//
//infile->read((char*)&meshHeader, sizeof(MeshHeader)); 
//size_t sizetoRead = sizeof(Resource::RawResourceData)+ sizeof(MeshHeader) + (sizeof(Mesh::Vertex) * meshHeader.numVerts) + (sizeof(UINT) * meshHeader.indexLength);
//Resource::RawResourceData tempRes;
//tempRes.m_id = id;
//tempRes.m_resType = Resources::ResourceType::RES_MESH;
//
//mem_manager.Clear(Resources::Memory::MEM_RES);
//
//data = mem_manager.Store(Resources::Memory::MEM_RES, sizetoRead);
//
//memcpy((char*)data, (char*)&tempRes, sizeof(Resource::RawResourceData));
//memcpy((char*)data + sizeof(Resource::RawResourceData), (char*)&meshHeader, sizeof(MeshHeader));
//
//UINT offset = sizeof(Resource::RawResourceData) + sizeof(MeshHeader);
//infile->read(data + offset, sizetoRead);

	return Resources::Status::ST_OK;

}

Resources::Status Resources::FileLoader::LoadPlaceHolderMesh(std::string& path, char *& data, size_t * size)
{
	std::ifstream infile;
	infile.open(path.c_str(), std::fstream::binary);
	if (!infile)
		return Status::ST_ERROR_OPENING_FILE;

	MainHeader mainHeader;
	infile.read((char*)&mainHeader, sizeof(MainHeader));

	MeshHeader meshHeader;

	infile.read((char*)&meshHeader, sizeof(MeshHeader));
	size_t sizetoRead = sizeof(Resource::RawResourceData) + sizeof(MeshHeader) + (sizeof(Mesh::Vertex) * meshHeader.numVerts) + (sizeof(UINT) * meshHeader.indexLength);

	Resource::RawResourceData tempRes;
	tempRes.m_id = -11;

	tempRes.m_resType = Resources::ResourceType::RES_MESH;

	mem_manager.Clear(Resources::Memory::MEM_RES);

	data = mem_manager.Store(Resources::Memory::MEM_RES, sizetoRead);

	memcpy((char*)data, (char*)&tempRes, sizeof(Resource::RawResourceData));
	memcpy((char*)data + sizeof(Resource::RawResourceData), (char*)&meshHeader, sizeof(MeshHeader));

	UINT offset = sizeof(Resource::RawResourceData) + sizeof(MeshHeader);
	infile.read(data + offset, sizetoRead);
	infile.close();
	return Resources::Status::ST_OK;
}

Resources::Status Resources::FileLoader::LoadFile(std::string & path, char *& data, size_t * size)
{
	std::streampos fsize;

	std::ifstream infile;
	infile.open(path.c_str(), std::fstream::binary | std::ios::ate);
	if (!infile)
		return Status::ST_ERROR_OPENING_FILE;

	fsize = infile.tellg();

	*size = static_cast<size_t>(fsize);

	mem_manager.Clear(Resources::Memory::MEM_RES);

	data = mem_manager.Store(Resources::Memory::MEM_RES, *size);
	infile.seekg(0,std::ios::beg);
	infile.read(data, *size);
	infile.close();
	return Resources::Status::ST_OK;
}

Resources::Status Resources::FileLoader::LoadLevel(std::string & path, LevelData::Level *& levelPtr)
{
	
	static LevelData::Level level;
	std::fstream file;
	file.open(path, std::fstream::in | std::fstream::binary);
	if (!file.is_open())
		return Resources::Status::ST_ERROR_OPENING_FILE;

	mem_manager.Clear(Resources::Memory::MEM_LEVEL);

	LevelData::MainLevelHeader header;
	file.read((char*)&header, sizeof(LevelData::MainLevelHeader)); //Read file header

	size_t totalMem = header.resAmount * sizeof(LevelData::ResourceHeader) + //Calculate file size
		header.entityAmount * sizeof(LevelData::EntityHeader) +
		header.lightAmount * sizeof(LevelData::LightHeader);

	char* data = mem_manager.Store(Resources::Memory::MEM_LEVEL, totalMem); //get memorychunk
	size_t offset = 0;
	
	
	level.numResources  = header.resAmount;
	level.numLights		= header.lightAmount;
	level.numEntities   = header.entityAmount;

	//Resource data
	size_t resSize = sizeof(LevelData::ResourceHeader)* header.resAmount; //size of resource data
	file.read(data, resSize);						     //Read res data
	level.resources = (LevelData::ResourceHeader*) data; //put data into level variable
	offset += resSize;

	//Model Entities
	size_t modelSize = sizeof(LevelData::EntityHeader) * header.entityAmount;	  //memsize
	file.read(data + offset , modelSize);										  // read all the entity data
	level.entities = (LevelData::EntityHeader*) (data + offset);
	offset += modelSize;

	//Lights
	/*
		size_t lightSize = sizeof(LevelData::LightHeader) * header.lightAmount;	  //memsize
		file.read(data + offset , lightSize);
		level.lights = (LevelData::LightHeader*) data + offset;
	*/
	
	file.close();

	levelPtr = &level;
	return  Resources::Status::ST_OK;
}

Resources::Status Resources::FileLoader::LoadRegistryFile()
{

	if (!OpenFile(Files::BPF_FILE)) //open BPF file
	{	
#ifdef _DEBUG
		MessageBox(NULL, TEXT("No BPF file found"), TEXT("Critical error!"), MB_OK);
#endif // _DEBUG
		return Status::ST_ERROR_OPENING_FILE;
	}

	
	RegistryHeader regHead;
	fileHandles[BPF_FILE].read((char*)&regHead, sizeof(RegistryHeader));
	m_fileRegistry.reserve((size_t)regHead.numIds);
	for (size_t i = 0; i < regHead.numIds; i++)
	{
		RegistryItem item;
		fileHandles[BPF_FILE].read((char*)&item, sizeof(RegistryItem));
		this->m_fileRegistry[item.id] = item;
	}

	CloseFile(Files::BPF_FILE);
	return Resources::Status::ST_OK;
}
