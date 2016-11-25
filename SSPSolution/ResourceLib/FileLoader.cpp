#include "FileLoader.h"
#include "FileHeaders.h"


Resources::FileLoader::FileLoader()
{
	this->mem_manager.Alloc(Resources::Memory::MEM_LEVEL, LEVEL_MEMORY);
	this->mem_manager.Alloc(Resources::Memory::MEM_RES, RESOURCE_MEMORY);
	
	filePaths[RESOURCE_FILE] = std::string("pillar.BBF");
	filePaths[REG_FILE] = std::string("regfile.reg");

	fileHandles[RESOURCE_FILE].rdbuf()->pubsetbuf(0, 0);	 //Disable streaming buffers
	fileHandles[REG_FILE].rdbuf()->pubsetbuf(0, 0);			 //Disable streaming buffers

	for (size_t i = 0; i < NUM_FILES; i++)
	{
		fileStates[i] = CLOSED;
	}
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
	assert(!fileHandles[file]);
	return true;
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
	tempRes.m_name[0] = 'M';
	tempRes.m_name[1] = '\0';
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

	char name[256] = { 'P','l','a','c','e','H','o','l','d','e','r','\0' };
	Resource::RawResourceData tempRes;
	tempRes.m_id = -11;
	memcpy(tempRes.m_name, name, 256);
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
