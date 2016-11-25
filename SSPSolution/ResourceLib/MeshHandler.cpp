#include "MeshHandler.h"



Resources::MeshHandler::MeshHandler()
{

}

Resources::MeshHandler::MeshHandler(size_t meshAmount, ID3D11Device* device)
{
	this->m_emptyContainers.resize(meshAmount);
	this->m_meshes.reserve(meshAmount);
	this->m_containers.reserve(meshAmount);
	this->m_containers.insert(m_containers.begin(), meshAmount, Mesh());
	for (size_t i = 0; i < meshAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(i);
	}

	if (device != nullptr) {
		this->m_device = device;
	}

}


Resources::MeshHandler::~MeshHandler()
{
	delete m_placeHolder;
}

Resources::Mesh * Resources::MeshHandler::GetPlaceHolderMesh()
{

	if (this->m_placeHolder == nullptr)
	{
		m_placeHolder = new Mesh();
		this->LoadPlaceHolderMesh();
	}

	return m_placeHolder;
	
}

Resources::Status Resources::MeshHandler::GetMesh(const unsigned int & id, ResourceContainer *& meshPtr)
{


	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_meshes.find(id);
	if (got == m_meshes.end()) // if the model does not exists in memory
	{
		return Resources::Status::ST_RES_MISSING;
	}
	else
	{
		meshPtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::MeshHandler::LoadMesh(const unsigned int & id, ResourceContainer *& meshPtr)
{

	char* data = nullptr;
	size_t dataSize = 0;
	Status st = FileLoader::GetInstance()->LoadResource(id, data, &dataSize);
	if (st != ST_OK)
		return st;
	
	//additional headers could be added here,
	Resource::RawResourceData* resData  = (Resource::RawResourceData*)data;
	if (resData->m_resType != RES_MESH)
	{
#ifdef _DEBUG
		std::cout << "Wrong resource type. Wanted mesh, got type: " << resData->m_id << std::endl;
#endif // _DEBUG

		return ST_WRONG_RESTYPE;
	}
	
	MeshHeader* meshData = (MeshHeader*)(data + sizeof(Resource::RawResourceData));
	unsigned int*			   indices  = nullptr;
	
#ifdef _DEBUG
	std::cout << "Loading new Mesh | ID : " << resData->m_id << std::endl;
#endif // DEBUG

	Mesh* newMesh = m_emptyContainers.front();		//Get an empty container
	st = newMesh->Create((Resource::RawResourceData*)data); //Initialize it with data

	if (st != ST_OK)
		return st;

	if (meshData->skeleton)
	{
		Mesh::VertexAnim* vertices = (Mesh::VertexAnim*)((char*)meshData + sizeof(MeshHeader));
		newMesh->SetVertices(vertices,m_device, meshData->numVerts);
		indices = (unsigned int*) ((char*)vertices + (sizeof(Mesh::VertexAnim)* meshData->numVerts));
		
#ifdef _DEBUG
		for (size_t i = 0; i < meshData->numVerts; i++)
		{
			std::cout << vertices[i].position[0] << ","
				<< vertices[i].position[1] << ","
				<< vertices[i].position[2] << std::endl;
		}
#endif // _DEBUG
	}
	else
	{
		Mesh::Vertex *vertices = (Mesh::Vertex*)((char*)meshData + sizeof(MeshHeader));
		newMesh->SetVertices(vertices, m_device, meshData->numVerts);
		indices = (unsigned int*)((char*)vertices + (sizeof(Mesh::Vertex)* meshData->numVerts));
#ifdef _DEBUG
	//for (size_t i = 0; i < meshData->numVerts; i++)
	//{
	//	std::cout << vertices[i].position[0] << ","
	//		<< vertices[i].position[1] << ","
	//		<< vertices[i].position[2] << std::endl;
	//}


		for (size_t i = 0; i < meshData->indexLength; i++)
		{
			//std::cout << vertices[i].position[0] << ","
			//	<< vertices[i].position[1] << ","
			//	<< vertices[i].position[2] << std::endl;


			std::cout << indices[i] << std::endl;
		}
#endif
	}

	if( !newMesh->SetIndices(indices, meshData->indexLength, m_device) )
		st =  Status::ST_BUFFER_ERROR;

	m_meshes[id] = ResourceContainer(newMesh, 1); // put it into the map
	m_emptyContainers.pop_front(); //remove from empty container queue;


	meshPtr = &m_meshes[id];
	
	

	return st;
}

Resources::Status Resources::MeshHandler::UnloadMesh(const unsigned int & id)
{

	ResourceContainer* meshRes = nullptr;

	Status st = GetMesh(id, meshRes);
	switch (st)
	{

		case ST_OK:
			meshRes->refCount -= 1;
			if (meshRes->refCount <= 0)
			{
				((Mesh*)meshRes->resource)->Destroy();
				m_emptyContainers.push_back(((Mesh*)meshRes->resource));
				m_meshes.erase(id);
#ifdef _DEBUG
				std::cout << "MESH : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
			}
		default:
			return st;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::MeshHandler::LoadPlaceHolderMesh()
{


	std::string path = "placeHolder.bbf";
	char* data = nullptr;
	size_t dataSize = 0;
	Status st = FileLoader::GetInstance()->LoadPlaceHolderMesh(path, data, &dataSize);
	if (st != ST_OK)
		return st;

	//additional headers could be added here,
	Resource::RawResourceData* resData = (Resource::RawResourceData*)data;
	if (resData->m_resType != RES_MESH){
		delete data;
		return ST_WRONG_RESTYPE;
	}

	MeshHeader* meshData = (MeshHeader*)(data + sizeof(Resource::RawResourceData));
	unsigned int*			   indices = nullptr;

#ifdef _DEBUG
	std::cout << "Loading placeHolder mesh" << std::endl;
#endif // DEBUG
	st = m_placeHolder->Create((Resource::RawResourceData*)data); //Initialize it with data

	if (st != ST_OK)
		return st;

	if (meshData->skeleton){
		Mesh::VertexAnim* vertices = (Mesh::VertexAnim*)((char*)meshData + sizeof(MeshHeader));
		m_placeHolder->SetVertices(vertices, m_device, meshData->numVerts);
		indices = (unsigned int*)((char*)vertices + (sizeof(Mesh::VertexAnim)* meshData->numVerts));

#ifdef _DEBUG
		for (size_t i = 0; i < meshData->numVerts; i++)
		{
			std::cout << vertices[i].position[0] << ","
				<< vertices[i].position[1] << ","
				<< vertices[i].position[2] << std::endl;
		}
#endif // _DEBUG
	}
	else{
		Mesh::Vertex *vertices = (Mesh::Vertex*)((char*)meshData + sizeof(MeshHeader));
		m_placeHolder->SetVertices(vertices, m_device, meshData->numVerts);
		indices = (unsigned int*)((char*)vertices + (sizeof(Mesh::Vertex)* meshData->numVerts));
#ifdef _DEBUG
		for (size_t i = 0; i < meshData->indexLength; i++)
		{
			std::cout << indices[i] << std::endl;
		}
#endif
	}

	if (!m_placeHolder->SetIndices(indices, meshData->indexLength, m_device))
		st = Status::ST_BUFFER_ERROR;

	return st;
}
