#include "FileLoader.h"
#include "FileHeaders.h"

Resources::FileLoader::FileLoader()
{
	this->mem_manager.Alloc(Resources::Memory::MEM_LEVEL, LEVEL_MEMORY);
	this->mem_manager.Alloc(Resources::Memory::MEM_RES, RESOURCE_MEMORY);
	
	filePaths[RESOURCE_FILE] = std::string("../ResourceLib/AssetFiles/grid.bbf");
	filePaths[REG_FILE] = std::string("../ResourceLib/AssetFiles/regfile.reg");

	fileHandles[RESOURCE_FILE].rdbuf()->pubsetbuf(0, 0);	 //Disable streaming buffers
	fileHandles[REG_FILE].rdbuf()->pubsetbuf(0, 0);			 //Disable streaming buffers

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

Resources::FileLoader::RegIndex * Resources::FileLoader::GetRegistryIndex(const unsigned int & objectId)
{

	std::unordered_map<unsigned int, RegIndex>::iterator got = m_fileRegistry.find(objectId);
	if (got == m_fileRegistry.end()) { // if the index does not exists in memory

		return nullptr;
	}
	else {
		return &got->second;
	}
}

Resources::Status Resources::FileLoader::LoadResource(const unsigned int& id, char*& data, size_t * size)
{
	if (fileStates[RESOURCE_FILE] == CLOSED)
	{
		std::cout << "File is closed" << std::endl;
		return Status::ST_FILE_CLOSED;

	}


	//unsigned int resourcePointer = Registry->GetPointerInFile(id);
	std::ifstream* infile = &fileHandles[RESOURCE_FILE];

	MainHeader mainHeader;
	infile->read((char*)&mainHeader, sizeof(MainHeader));

	MeshHeader meshHeader;
	
	infile->read((char*)&meshHeader, sizeof(MeshHeader));
	size_t sizetoRead = sizeof(Resource::RawResourceData)+ sizeof(MeshHeader) + (sizeof(Mesh::Vertex) * meshHeader.numVerts) + (sizeof(UINT) * meshHeader.indexLength);
	Resource::RawResourceData tempRes;
	tempRes.m_id = id;
	tempRes.m_resType = Resources::ResourceType::RES_MESH;

	mem_manager.Clear(Resources::Memory::MEM_RES);
	
	data = mem_manager.Store(Resources::Memory::MEM_RES, sizetoRead);

	memcpy((char*)data, (char*)&tempRes, sizeof(Resource::RawResourceData));
	memcpy((char*)data + sizeof(Resource::RawResourceData), (char*)&meshHeader, sizeof(MeshHeader));

	UINT offset = sizeof(Resource::RawResourceData) + sizeof(MeshHeader);
	infile->read(data + offset, sizetoRead);

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

Resources::Status Resources::FileLoader::LoadRegistryFile()
{

	if (!OpenFile(Files::REG_FILE))
	{	
#ifdef _DEBUG
		//MessageBox(NULL, TEXT("No registry file found"), TEXT("Critical error!"), MB_OK);
#endif // _DEBUG
		return Status::ST_ERROR_OPENING_FILE;
	}

	mem_manager.Clear(Resources::Memory::MEM_RES);
//	char* data = mem_manager.Store(Resources::Memory::MEM_RES, *size);


	//fileHandles[REG_FILE].read()

	CloseFile(Files::REG_FILE);
	return Resources::Status::ST_OK;
}
