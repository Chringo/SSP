#include "GraphicsHandler.h"

int GraphicsHandler::IncreaseArraySize()
{
	GraphicsComponent** newArray = new GraphicsComponent*[this->m_maxGraphicsComponents + ARRAY_INC];

	for (int i = 0; i < this->m_maxGraphicsComponents; i++)
	{
		newArray[i] = this->m_graphicsComponents[i];
	}
	delete[] this->m_graphicsComponents;
	this->m_graphicsComponents = newArray;
	this->m_maxGraphicsComponents += ARRAY_INC;

	return 1;
}

int GraphicsHandler::DecreaseArraySize()
{
	this->m_maxGraphicsComponents -= ARRAY_INC;
	GraphicsComponent** newArray = new GraphicsComponent*[this->m_maxGraphicsComponents];

	for (int i = 0; i < this->m_maxGraphicsComponents; i++)
	{
		newArray[i] = this->m_graphicsComponents[i];
	}

	for (int i = this->m_maxGraphicsComponents; i < this->m_maxGraphicsComponents + ARRAY_INC; i++)
	{
		if (this->m_graphicsComponents[i])
		{
			delete this->m_graphicsComponents[i];
		}
	}

	delete[] this->m_graphicsComponents;
	this->m_graphicsComponents = newArray;

	return 1;
}

GraphicsHandler::GraphicsHandler()
{
	this->m_d3dHandler = nullptr;
	this->m_deferredSH = nullptr;
	this->m_finalSH = nullptr;
	this->m_indexBuffer = nullptr;
	this->m_vertexBuffer = nullptr;
	this->m_camera = nullptr;
	this->m_graphicsComponents = nullptr;
	this->m_nrOfGraphicsComponents = 0;
	this->m_maxGraphicsComponents = 5;
	this->m_gridEnabled = false;
}


GraphicsHandler::~GraphicsHandler()
{
	
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMINT2& resolution)
{
	this->initCheck = false;
	this->simpleGravity = -1.000000f;
	this->m_d3dHandler = new Direct3DHandler;
	this->modelsPtr = new Resources::Model*[2];
	if (this->m_d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}
	Resources::ResourceHandler::GetInstance()->LoadLevel(UINT(1337)); //placeholder id

	this->m_deferredSH = new DeferredShaderHandler;
	if (this->m_deferredSH->Initialize(this->m_d3dHandler->GetDevice(), windowHandle, this->m_d3dHandler->GetDeviceContext(), resolution))
	{
		return 1;
	}

	this->m_finalSH = new FinalShaderHandler;
	if (this->m_finalSH->Initialize(this->m_d3dHandler->GetDevice(), windowHandle, this->m_d3dHandler->GetDeviceContext(), resolution))
	{
		return 1;
	}

	if (this->m_finalSH->SetRenderParameters(this->m_d3dHandler->GetBackbufferRTV(), this->m_deferredSH->GetShaderResourceViews()))
	{
		return 1;
	}

	this->m_camera = new Camera;
	this->m_camera->Initialize();

	//Setup projection matrix
	//fieldOfView = 3.141592654f / 4.0f;
	float fieldOfView = (float)DirectX::XM_PI / 4.0f;
	float screenAspect = (float)resolution.x / (float)resolution.y;

	DirectX::XMStoreFloat4x4(&m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f));

	this->m_graphicsComponents = new GraphicsComponent*[this->m_maxGraphicsComponents];
	for (int i = 0; i < this->m_maxGraphicsComponents; i++) {
		this->m_graphicsComponents[i] = nullptr;
	}

	DirectX::XMMATRIX tempWorld = DirectX::XMMatrixIdentity();
	//DirectX::XMFLOAT4X4 worldMatrix;
	//DirectX::XMStoreFloat4x4(&worldMatrix, tempWorld);

	this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

	tempWorld = DirectX::XMMatrixTranslation(1.f, 0.f, 6.f);
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixRotationZ(.3f));
	//DirectX::XMStoreFloat4x4(&worldMatrix, tempWorld);
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

	tempWorld = DirectX::XMMatrixTranslation(-1.f, 0.5f, 6.f);
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixRotationZ(.3f));
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f));
	//DirectX::XMStoreFloat4x4(&worldMatrix, tempWorld);
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

	this->m_deferredSH->SetGraphicsParameters(m_graphicsComponents, modelsPtr);
	this->InitializeGrid();
	
	/*TEMP MODELS*/
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(111337), modelsPtr[1]);

	return 0;
	
}

Camera* GraphicsHandler::SetCamera(Camera * newCamera)
{
	int result = 1;
	Camera* tempCam = this->m_camera;
	this->m_camera = newCamera;
	return tempCam;
}

int GraphicsHandler::Render()
{


	/*TEMP CBUFFER STUFF*/
	ShaderLib::DeferredConstantBufferVP* shaderParamsVP = new ShaderLib::DeferredConstantBufferVP;
	shaderParamsVP->viewMatrix = *this->m_camera->GetViewMatrix();
	shaderParamsVP->projectionMatrix = this->m_projectionMatrix;
	ShaderLib::DeferredConstantBufferWorldxm * shaderParamsXM = new ShaderLib::DeferredConstantBufferWorldxm;
	ShaderLib::CameraConstantBuffer* lShaderParams = new ShaderLib::CameraConstantBuffer;
	lShaderParams->camPos = this->m_camera->GetCameraPos();
	lShaderParams->camTar = this->m_camera->GetLookAt();
	/********************/



	this->m_deferredSH->SetActive(ShaderLib::ShaderType::Normal);
	this->m_deferredSH->SetShaderParameters(shaderParamsVP, ShaderLib::CB_VIEW_PROJECTION);
	m_deferredSH->Draw(ShaderLib::DRAW_STANDARD);

	/*TEMP*/
	if (m_gridEnabled)
	{
		int ett;
		float tva;
		this->RenderGrid(ett, tva);
	}
	/********/

 

	this->m_finalSH->SetActive(ShaderLib::ShaderType::Normal);
	this->m_finalSH->SetShaderParameters(lShaderParams);
	this->m_finalSH->Draw();

	/*TEMP CBUFFER STUFF*/
	delete shaderParamsXM;
	delete shaderParamsVP;
	delete lShaderParams;
	/*TEMP CBUFFER STUFF*/


	this->m_d3dHandler->PresentScene();
	return 0;
}

int GraphicsHandler::InitializeGrid()
{
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), modelsPtr[0]);
	m_d3dHandler->InitializeGridRasterizer();
	m_deferredSH->InitializeGridShader(this->m_d3dHandler->GetDevice());
	this->m_gridEnabled = true;
	return 0;
}

int GraphicsHandler::RenderGrid(int &align, float &scale) //will render the grid from said variables every frame, there will be a updategrid function for this instead later
{
		m_d3dHandler->SetRasterizerState(D3D11_FILL_WIREFRAME);
		this->m_deferredSH->Draw(ShaderLib::DRAW_GRID);
		m_d3dHandler->SetRasterizerState(D3D11_FILL_SOLID);

	return 0;
}

void GraphicsHandler::Shutdown()
{
	if (this->m_d3dHandler)
	{
		this->m_d3dHandler->Shutdown();
		delete this->m_d3dHandler;
		this->m_d3dHandler = nullptr;
	}
	if (this->m_deferredSH)
	{
		this->m_deferredSH->Shutdown();
		delete this->m_deferredSH;
		this->m_deferredSH = nullptr;
	}
	if (this->m_finalSH)
	{
		this->m_finalSH->Shutdown();
		delete this->m_finalSH;
		this->m_finalSH = nullptr;
	}
	if (this->m_indexBuffer)
	{
		this->m_indexBuffer->Release();
		this->m_indexBuffer = nullptr;
	}
	if (this->m_vertexBuffer) 
	{
		this->m_vertexBuffer->Release();
		this->m_vertexBuffer = nullptr;
	}
	if (this->m_windowHandle)
	{
		this->m_windowHandle = nullptr;
	}
	for (int i = 0; i < this->m_nrOfGraphicsComponents; i++)
	{
		if (this->m_graphicsComponents[i])
		{
			delete this->m_graphicsComponents[i];
			this->m_graphicsComponents[i] = nullptr;
		}
	}
	delete[] this->modelsPtr;
	delete[] this->m_graphicsComponents;
}

int GraphicsHandler::CreateTriangle()
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

	vertices[0] = DirectX::XMFLOAT3(-.5f, -.5f, 2.0f);  //bottom left

	vertices[1] = DirectX::XMFLOAT3(0.0f, .5f, 2.0f);  //top mid
											   
	vertices[2] = DirectX::XMFLOAT3(.5f, -.5f, 2.0f);  //bottom right

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
	hresult = this->m_d3dHandler->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_vertexBuffer);
	if (FAILED(hresult)) {
		return 1;
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
	hresult = this->m_d3dHandler->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &this->m_indexBuffer);
	if (FAILED(hresult)) {
		return 1;
	}

	return 0;
}

int GraphicsHandler::SetTriangle()
{
	unsigned int stride;
	unsigned offset;

	//Set vertex buffer stride and offset
	stride = sizeof(DirectX::XMFLOAT3);
	offset = 0;

	//Set the vertex buffer to active in the input assembly so it can rendered
	this->m_d3dHandler->GetDeviceContext()->IASetVertexBuffers(0, 1, &this->m_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	this->m_d3dHandler->GetDeviceContext()->IASetIndexBuffer(this->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type od primitiv that should be rendered from this vertex buffer, in this case triangles
	this->m_d3dHandler->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return 0;
}
