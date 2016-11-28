#include "Mesh.h"


Resources::Mesh::Mesh(Resource::RawResourceData resData, RawMeshData meshData, ID3D11Device* dev, bool keepRawData) :
	Resource(resData)
{
	this->m_resourceData.m_resType = ResourceType::RES_MESH;
	m_meshData = meshData;

	if(meshData.hasAnimation)
		this->SetVertices(meshData.m_animVertices, dev,meshData.m_numVerts,keepRawData);
	else
		this->SetVertices(meshData.m_vertices, dev, meshData.m_numVerts,keepRawData);

	SetIndices(meshData.m_indices, meshData.m_numIndices,dev,keepRawData);
}

Resources::Mesh::Mesh(Resource::RawResourceData resData)
	:Resource(resData)
{
	this->m_resourceData.m_resType = ResourceType::RES_MESH;
}

Resources::Mesh::Mesh() : Resource()
{

	m_resourceData.m_resType = ResourceType::RES_MESH;

}

Resources::Status Resources::Mesh::Create(Resource::RawResourceData * resData, ID3D11Device* dev , RawMeshData * meshData, bool keepRawData)
{
	this->Destroy();
	memcpy((char*)m_resourceData.m_name, (char*)resData->m_name, 256);
	m_resourceData.m_id = resData->m_id;
	m_resourceData.m_resType = RES_MESH;
	if (meshData != nullptr)
	{
		if(!this->SetIndices(meshData->m_indices, meshData->m_numIndices,dev, keepRawData))	
			return Status::ST_BUFFER_ERROR;
		if (meshData->hasAnimation){
			if (!SetVertices(meshData->m_animVertices,dev, meshData->m_numVerts, keepRawData)) 
				return Status::ST_BUFFER_ERROR;
		}
		else{
			if (!SetVertices(meshData->m_vertices,dev, meshData->m_numVerts, keepRawData)) return Status::ST_BUFFER_ERROR;
		}
		if (keepRawData == true)
		{
			m_meshData.hasAnimation = meshData->hasAnimation;
			m_meshData.m_numIndices = meshData->m_numIndices;
			m_meshData.m_numVerts   = meshData->m_numVerts;
			m_meshData.m_indices	= new unsigned int[meshData->m_numIndices];
			memcpy((char*)m_meshData.m_indices, meshData->m_indices, sizeof(unsigned int)*  meshData->m_numIndices);
		
			if (meshData->hasAnimation)
			{
				m_meshData.m_animVertices = new VertexAnim[meshData->m_numVerts];
				memcpy((char*)m_meshData.m_animVertices, meshData->m_animVertices, sizeof(VertexAnim)*  meshData->m_numVerts);

			}
			else
			{
				m_meshData.m_vertices = new Vertex[meshData->m_numVerts];
				memcpy((char*)m_meshData.m_vertices, meshData->m_vertices, sizeof(Vertex)*  meshData->m_numVerts);
			}
		}
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::Mesh::Destroy()
{
	if (!EraseMeshData())
		return Status::ST_BUFFER_ERROR;
	char name[5] = { 'N', 'O', 'N', 'E','\0' };
	memcpy(m_resourceData.m_name, name, sizeof(char) * 5);
	this->m_resourceData.m_id = 0;

	return Resources::Status::ST_OK;
}

Resources::Mesh::~Mesh()
{
	EraseMeshData();
}

bool Resources::Mesh::SetVertices(Vertex * data, ID3D11Device* dev, unsigned int numVerts, bool keepRawData)
{

	delete m_meshData.m_vertices;
	m_meshData.m_vertices = nullptr;

	delete m_meshData.m_animVertices;
	m_meshData.m_animVertices = nullptr;


	if (!Resources::SAFE_RELEASE(m_AnimVertBuffer)){
		Resources::OutputErrorString(this, std::string("could not release animBuffer")); 
		return false;
	}
	if (!Resources::SAFE_RELEASE(m_vertBuffer)) {
		Resources::OutputErrorString(this, std::string("could not release vertbuffer")); 
		return false;
	}


	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* numVerts;


	D3D11_SUBRESOURCE_DATA b_data;
	b_data.pSysMem = data;
	HRESULT hr;
	hr = dev->CreateBuffer(&bufferDesc, &b_data, &m_vertBuffer);

	if (FAILED(hr))
		return false;

	if (keepRawData) {
		m_meshData.m_vertices = new Vertex[numVerts];
		memcpy(m_meshData.m_vertices,&data,sizeof(Vertex)* numVerts);
	}

	m_meshData.m_numVerts = numVerts;
	m_meshData.hasAnimation = false;


	return true;
}

bool Resources::Mesh::SetVertices(VertexAnim * data, ID3D11Device* dev, unsigned int numVerts, bool keepRawData)
{
	if (numVerts = 0) 
		return false;

	
	delete m_meshData.m_vertices;	    m_meshData.m_vertices	  = nullptr;
	delete m_meshData.m_animVertices;	m_meshData.m_animVertices = nullptr;
	

	if (!Resources::SAFE_RELEASE(m_AnimVertBuffer)) {
		Resources::OutputErrorString(this, std::string("could not release animBuffer")); 
		return false;
	}
	if (!Resources::SAFE_RELEASE(m_vertBuffer)) {
		Resources::OutputErrorString(this, std::string("could not release vertbuffer"));
		return false;
	}
	if (dev != nullptr){
		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexAnim)* numVerts;
	
	
		D3D11_SUBRESOURCE_DATA b_data;
		b_data.pSysMem = data;
		HRESULT hr = S_OK;
		hr = dev->CreateBuffer(&bufferDesc, &b_data, &m_AnimVertBuffer);
	
		if (FAILED(hr))
			return false;
	}

	if (keepRawData)
		m_meshData.m_animVertices = data;
	else {
		delete data; data = nullptr;
	}
	m_meshData.hasAnimation = true;
	m_meshData.m_numVerts = numVerts;
	return true;

}

bool Resources::Mesh::SetIndices(unsigned int * indices, unsigned int numIndices, ID3D11Device* dev, bool keepRawData)
{
	HRESULT hr = S_OK;
	Resources::SAFE_RELEASE(m_indexBuffer);

	delete m_meshData.m_indices; m_meshData.m_indices = nullptr;

	if (dev != nullptr)
	{
		D3D11_BUFFER_DESC ibd;

		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = sizeof(unsigned int) * numIndices;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA ibdData;
		ibdData.pSysMem = indices;
		hr = dev->CreateBuffer(&ibd, &ibdData, &m_indexBuffer);
		if (FAILED(hr))
			return false;
	}

	if (keepRawData)
		m_meshData.m_indices = indices;

	m_meshData.m_numIndices = numIndices;
	

	return true;
}

std::shared_ptr<char> Resources::Mesh::GetDataAsBinary(size_t * size, bool * result)
{
	return std::shared_ptr<char>();
}

bool Resources::Mesh::EraseMeshData()
{
	delete m_meshData.m_animVertices;	 m_meshData.m_animVertices = nullptr;
	delete m_meshData.m_vertices;		 m_meshData.m_vertices	   = nullptr;
	delete m_meshData.m_indices;		 m_meshData.m_indices	   = nullptr;

	m_meshData.m_numVerts	= 0;
	m_meshData.m_numIndices = 0;
	m_meshData.hasAnimation = false;

	if (!Resources::SAFE_RELEASE(m_AnimVertBuffer)){
		Resources::OutputErrorString(this, std::string("could not release animBuffer")  ); return false;
	}
	if (!Resources::SAFE_RELEASE(m_vertBuffer)) {
		Resources::OutputErrorString(this, std::string("could not release vertbuffer")  ); return false;
	}
	if (!Resources::SAFE_RELEASE(m_indexBuffer)) {
		Resources::OutputErrorString(this, std::string("could not release indexbuffer")); return false;
	}
	
	std::cout << "Erasin mesh data on : " << this->GetName() << std::endl;


	m_AnimVertBuffer  = nullptr;
		m_vertBuffer  = nullptr;
		m_indexBuffer = nullptr;
		return true;
}

bool Resources::Mesh::SetMeshData(RawMeshData* newMeshData, ID3D11Device* dev,bool keepRawData)
{
	if (!EraseMeshData()) {
		Resources::OutputErrorString(this, std::string("could not erase MeshData"));
		return false;
	}

		m_meshData.hasAnimation = newMeshData->hasAnimation;
		m_meshData.m_numIndices = newMeshData->m_numIndices;
		m_meshData.m_numVerts   = newMeshData->m_numVerts;

	if (newMeshData->hasAnimation){
		if (!SetVertices(newMeshData->m_animVertices, dev, newMeshData->m_numVerts, keepRawData)) 
			return false;
	}
	else {
		if (!SetVertices(newMeshData->m_vertices, dev, newMeshData->m_numVerts,keepRawData)) 
			return false;
	}


	if (!SetIndices(m_meshData.m_indices, m_meshData.m_numIndices, dev, keepRawData))
		return false;


	return true;
}
