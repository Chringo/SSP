#include "GraphicsHandler.h"

GraphicsHandler::GraphicsHandler()
{
}


GraphicsHandler::~GraphicsHandler()
{
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMFLOAT2& resolution)
{
	this->d3dHandler = new Direct3DHandler;
	if (this->d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}

	this->deferredSH = new DeferredShaderHandler;
	if (this->deferredSH->Initialize(this->d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	this->lightSH = new LightShaderHandler;
	if (this->lightSH->Initialize(this->d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	return 0;
}

int GraphicsHandler::Render(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMFLOAT3& cameraPos)
{
	
	return 0;
}

void GraphicsHandler::Shutdown()
{
	if (this->d3dHandler)
	{
		this->d3dHandler->Shutdown();
		this->d3dHandler = nullptr;
	}
	if (this->deferredSH)
	{
		this->deferredSH->Shutdown();
		this->deferredSH = nullptr;
	}
	if (this->lightSH)
	{
		this->lightSH->Shutdown();
		this->lightSH = nullptr;
	}
	if (this->m_indexBuffer)
	{
		this->m_indexBuffer->Release();
	}
	if (this->m_vertexBuffer) 
	{
		this->m_vertexBuffer->Release();
	}
}

int GraphicsHandler::SetTriangle()
{
	DirectX::XMFLOAT3 vertices[3];
	unsigned long indices[3];
	int sizeVertices = 3;
	int sizeIndices = 3;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT hresult;
	bool result;

	vertices[0] = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f);  //bottom left

	vertices[1] = DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f);  //top min

	vertices[2] = DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f);  //bottom right

										   //Load the index array with data
	for (int i = 0; i < sizeIndices; i++)
	{
		indices[i] = i;
	}

	//Set the description of the static vertex buffer
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT3) * sizeVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	hresult = this->d3dHandler->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	if (FAILED(hresult)) {
		return false;
	}

	//Set up the description of the static index buffer
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * sizeIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	hresult = this->d3dHandler->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &this->m_indexBuffer);
	if (FAILED(hresult)) {
		return false;
	}

	return 0;
}
