#include "GraphicsHandler.h"

GraphicsHandler::GraphicsHandler()
{
	this->m_d3dHandler = nullptr;
	this->m_deferredSH = nullptr;
	this->m_lightSH = nullptr;
	this->m_indexBuffer = nullptr;
	this->m_vertexBuffer = nullptr;
	this->m_camera = nullptr;
}


GraphicsHandler::~GraphicsHandler()
{
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMINT2& resolution)
{
	this->m_d3dHandler = new Direct3DHandler;
	if (this->m_d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}

	this->m_deferredSH = new DeferredShaderHandler;
	if (this->m_deferredSH->Initialize(this->m_d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	this->m_lightSH = new LightShaderHandler;
	if (this->m_lightSH->Initialize(this->m_d3dHandler->GetDevice(), windowHandle, resolution))
	{
		return 1;
	}

	this->m_camera = new Camera;
	this->m_camera->Initialize();

	//Setup projection matrix
	//fieldOfView = 3.141592654f / 4.0f;
	float fieldOfView = (float)DirectX::XM_PI / 4.0f;
	float screenAspect = (float)resolution.x / (float)resolution.y;

	this->m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);

	this->CreateTriangle();

	return 0;
}

int GraphicsHandler::SetCamera(Camera * newCamera)
{
	int result = 1;
	delete this->m_camera;
	this->m_camera = newCamera;
	return result;
}

int GraphicsHandler::Render()
{

	this->m_deferredSH->ClearRenderTargetViews(this->m_d3dHandler->GetDeviceContext());

	DirectX::XMMATRIX viewMatrix;
	this->m_camera->GetViewMatrix(viewMatrix);
	DirectX::XMFLOAT3 cameraPos;
	this->m_camera->GetCameraPos(cameraPos);

	this->SetTriangle();

	this->m_deferredSH->SetActive(this->m_d3dHandler->GetDeviceContext(), ShaderLib::ShaderType::Normal);

	ShaderLib::DeferredConstantBuffer* shaderParams = new ShaderLib::DeferredConstantBuffer;
	shaderParams->worldMatrix = DirectX::XMMatrixIdentity();
	shaderParams->viewMatrix = viewMatrix;
	shaderParams->projectionMatrix = this->m_projectionMatrix;
	shaderParams->diffColor = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	shaderParams->camPos = cameraPos;

	this->m_deferredSH->SetShaderParameters(this->m_d3dHandler->GetDeviceContext(), shaderParams);
	delete shaderParams;
	this->m_d3dHandler->GetDeviceContext()->DrawIndexed(3, 0, 0);

	this->m_d3dHandler->ClearDepthAndRTV(this->m_deferredSH->GetDSV());
	this->m_d3dHandler->SetBackBuffer(this->m_deferredSH->GetDSV());
	this->m_lightSH->SetActive(this->m_d3dHandler->GetDeviceContext(), ShaderLib::ShaderType::Normal);

	ShaderLib::LightConstantBuffer* lShaderParams = new ShaderLib::LightConstantBuffer;

	lShaderParams->viewMatrix = viewMatrix;
	lShaderParams->projectionMatrix = this->m_projectionMatrix;

	lShaderParams->camPos = cameraPos;

	this->m_lightSH->SetShaderParameters(this->m_d3dHandler->GetDeviceContext(), lShaderParams, this->m_deferredSH->GetShaderResourceViews());
	delete lShaderParams;
	this->m_d3dHandler->GetDeviceContext()->DrawIndexed(6, 0, 0);

	this->m_lightSH->ResetPSShaderResources(this->m_d3dHandler->GetDeviceContext());

	this->m_d3dHandler->PresentScene();

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
	if (this->m_lightSH)
	{
		this->m_lightSH->Shutdown();
		delete this->m_lightSH;
		this->m_lightSH = nullptr;
	}
	if (this->m_indexBuffer)
	{
		this->m_indexBuffer->Release();
		delete this->m_indexBuffer;
		this->m_indexBuffer = nullptr;
	}
	if (this->m_vertexBuffer) 
	{
		this->m_vertexBuffer->Release();
		delete this->m_vertexBuffer;
		this->m_vertexBuffer = nullptr;
	}
	if (this->m_windowHandle)
	{
		this->m_windowHandle = nullptr;
	}
	if (this->m_camera)
	{
		delete this->m_camera;
		this->m_camera = nullptr;
	}
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
