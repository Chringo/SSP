#include "GraphicsHandler.h"

int GraphicsHandler::IncreaseArraySize()
{
	GraphicsComponent** newArray = new GraphicsComponent*[this->m_maxGraphicsComponents + ARRAY_INC];

	for (int i = 0; i < this->m_maxGraphicsComponents + ARRAY_INC; i++)
	{
		if (i < this->m_nrOfGraphicsComponents)
		{
			newArray[i] = this->m_graphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}
	delete[] this->m_graphicsComponents;
	this->m_graphicsComponents = newArray;
	this->m_maxGraphicsComponents += ARRAY_INC;

	return 1;
}

int GraphicsHandler::IncreaseArraySize(int increaseTo)
{
	GraphicsComponent** newArray = new GraphicsComponent*[increaseTo];

	for (int i = 0; i < increaseTo; i++)
	{
		if (i < this->m_nrOfGraphicsComponents)
		{
			newArray[i] = this->m_graphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}
	delete[] this->m_graphicsComponents;
	this->m_graphicsComponents = newArray;
	this->m_maxGraphicsComponents = increaseTo;

	return 1;
}

int GraphicsHandler::DecreaseArraySize()
{
	this->m_maxGraphicsComponents -= ARRAY_INC;
	GraphicsComponent** newArray = new GraphicsComponent*[this->m_maxGraphicsComponents];

	for (int i = 0; i < this->m_maxGraphicsComponents; i++)
	{
		if (i < this->m_nrOfGraphicsComponents)
		{
			newArray[i] = this->m_graphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
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

int GraphicsHandler::DecreaseArraySize(int decreaseTo)
{
	GraphicsComponent** newArray = new GraphicsComponent*[decreaseTo];

	for (int i = 0; i < decreaseTo; i++)
	{
		if (i < this->m_nrOfGraphicsComponents)
		{
			newArray[i] = this->m_graphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}

	for (int i = decreaseTo; i < this->m_maxGraphicsComponents; i++)
	{
		if (this->m_graphicsComponents[i])
		{
			delete this->m_graphicsComponents[i];
		}
	}

	delete[] this->m_graphicsComponents;
	this->m_graphicsComponents = newArray;
	this->m_maxGraphicsComponents = decreaseTo;

	return 1;
}

GraphicsHandler::GraphicsHandler()
{
	this->m_d3dHandler			   = nullptr;
	this->m_deferredSH			   = nullptr;
	this->m_finalSH				   = nullptr;
	this->m_indexBuffer			   = nullptr;
	this->m_vertexBuffer		   = nullptr;
	this->m_camera				   = nullptr;
	this->m_graphicsComponents	   = nullptr;
	this->m_shaderControl		   = nullptr;
	this->m_nrOfGraphicsComponents = 0;
	this->m_maxGraphicsComponents  = 5;
	this->m_gridEnabled			   = false;
}


GraphicsHandler::~GraphicsHandler()
{
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMINT2& resolution)
{
	this->m_d3dHandler = new Direct3DHandler;
	this->m_modelsPtr = new Resources::Model*[2];
	if (this->m_d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}
	Resources::ResourceHandler::GetInstance()->LoadLevel(UINT(1337)); //placeholder id

	this->m_shaderControl = new ShaderControl;
	m_shaderControl->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext(), resolution);
	m_shaderControl->SetBackBuffer(m_d3dHandler->GetBackbufferRTV(), m_d3dHandler->GetBackbufferSRV());

	ConstantBufferHandler::GetInstance()->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext());

	this->m_camera = new Camera;
	this->m_camera->Initialize();

	this->m_graphicsComponents = new GraphicsComponent*[this->m_maxGraphicsComponents];
	for (int i = 0; i < this->m_maxGraphicsComponents; i++) {
		this->m_graphicsComponents[i] = new GraphicsComponent;
	}

	DirectX::XMMATRIX tempWorld = DirectX::XMMatrixIdentity();

	//this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

	tempWorld = DirectX::XMMatrixTranslation(1.f, 0.f, 6.f);
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixRotationZ(.3f));
	//DirectX::XMStoreFloat4x4(&worldMatrix, tempWorld);
	//this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

	tempWorld = DirectX::XMMatrixTranslation(-1.f, 0.5f, 6.f);
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixRotationZ(.3f));
	tempWorld = DirectX::XMMatrixMultiply(tempWorld, DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f));
	//DirectX::XMStoreFloat4x4(&worldMatrix, tempWorld);
	//this->m_graphicsComponents[this->m_nrOfGraphicsComponents] = new GraphicsComponent;
	this->m_graphicsComponents[this->m_nrOfGraphicsComponents]->worldMatrix = tempWorld;
	this->m_nrOfGraphicsComponents++;

//	this->m_deferredSH->SetGraphicsParameters(m_graphicsComponents, this->m_modelsPtr);
	
	this->InitializeGrid();
	
	/*TEMP MODELS*/
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(13337), m_modelsPtr[0]);
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), m_modelsPtr[1]);

#ifdef _DEBUG
	 m_debugRender.Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext(), resolution);
#endif // _DEBUG

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
	m_shaderControl->ClearFrame();

	/*TEMP CBUFFER STUFF*/
	ConstantBufferHandler::ConstantBuffer::camera::cbData cam;
	this->m_camera->GetCameraPos(cam.cPos);
	this->m_camera->GetViewMatrix(cam.cView);
	cam.cProjection = DirectX::XMLoadFloat4x4(m_camera->GetProjectionMatrix());
	/********************/
	ConstantBufferHandler::GetInstance()->camera.UpdateBuffer(&cam);
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);
	for (int i = 1; i < 3; i++) //FOR EACH NORMAL GEOMETRY
	{
		ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&this->m_graphicsComponents[i]->worldMatrix);
		m_shaderControl->Draw(m_modelsPtr[0]);
	}
	//for (int i = 0; i < 0; i++) //FOR EACH "OTHER TYPE OF GEOMETRY" ETC...
	//{
	//}
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Animated);
	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&this->m_graphicsComponents[0]->worldMatrix);
	m_shaderControl->Draw(m_modelsPtr[1]);



	if (m_gridEnabled)
	{
		int ett;
		float tva;
		this->RenderGrid(ett, tva);
	}
	/********/


	m_shaderControl->DrawFinal();

	/*TEMP CBUFFER STUFF*/

	/*TEMP CBUFFER STUFF*/


	if (postProcessing)
	{
		ID3D11DeviceContext* context = m_d3dHandler->GetDeviceContext();
		ID3D11RenderTargetView* temp = nullptr;
		context->OMSetRenderTargets(1, &temp, NULL);
		ID3D11ShaderResourceView* srv = m_d3dHandler->GetBackbufferSRV();
		context->PSSetShaderResources(6,1,&srv);

		m_shaderControl->PostProcess();
		ID3D11ShaderResourceView * tab[1];
		tab[0] = NULL;
		context->PSSetShaderResources(6, 1, tab);
	}
	
#ifdef _DEBUG
	AABB box;
	m_debugRender.SetActive();

	m_debugRender.Render(box);
	//Draw Debug.
#endif // _DEBUG
	this->m_d3dHandler->PresentScene();
	return 0;
}

int GraphicsHandler::InitializeGrid()
{
	//Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), m_modelsPtr[0]);
	//m_d3dHandler->InitializeGridRasterizer();
	//m_deferredSH->InitializeGridShader(this->m_d3dHandler->GetDevice());
	this->m_gridEnabled = false;
	return 0;
}

int GraphicsHandler::RenderGrid(int &align, float &scale) //will render the grid from said variables every frame, there will be a updategrid function for this instead later
{
		m_d3dHandler->SetRasterizerState(D3D11_FILL_WIREFRAME);
		this->m_deferredSH->DrawGrid(m_modelsPtr[NULL]);
		m_d3dHandler->SetRasterizerState(D3D11_FILL_SOLID);

	return 0;
}

int GraphicsHandler::RenderFromEditor(Resources::Model* model,GraphicsComponent* component)
{
	m_shaderControl->ClearFrame();

	ConstantBufferHandler::ConstantBuffer::camera::cbData cam;
	this->m_camera->GetCameraPos(cam.cPos);
	this->m_camera->GetViewMatrix(cam.cView);
	cam.cProjection = DirectX::XMLoadFloat4x4(this->m_camera->GetProjectionMatrix());
	ConstantBufferHandler::GetInstance()->camera.UpdateBuffer(&cam);
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);

	m_d3dHandler->SetRasterizerState(D3D11_FILL_WIREFRAME);
	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	((DeferredShader*)m_shaderControl->m_shaders[0])->DrawFromEditor(model);

	m_d3dHandler->SetRasterizerState(D3D11_FILL_SOLID);


	m_shaderControl->DrawFinal();

	/*TEMP CBUFFER STUFF*/

	/*TEMP CBUFFER STUFF*/


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
		this->m_deferredSH->Release();
		delete this->m_deferredSH;
		this->m_deferredSH = nullptr;
	}
	if (this->m_finalSH)
	{
		this->m_finalSH->Release();
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
	if (this->m_shaderControl)
	{
		m_shaderControl->Release();
		delete m_shaderControl;
		this->m_shaderControl = nullptr;
	}
	if (this->m_windowHandle)
	{
		this->m_windowHandle = nullptr;
	}
	for (int i = 0; i < this->m_maxGraphicsComponents; i++)
	{
		if (this->m_graphicsComponents[i])
		{
			delete this->m_graphicsComponents[i];
			this->m_graphicsComponents[i] = nullptr;
		}
	}
	delete[] this->m_modelsPtr;
	delete[] this->m_graphicsComponents;
#ifdef _DEBUG
	m_debugRender.Release();
#endif // _DEBUG

}

int GraphicsHandler::SetComponentArraySize(int newSize)
{
	if (this->m_maxGraphicsComponents < newSize)
	{
		this->IncreaseArraySize(newSize);
	}
	else if (this->m_maxGraphicsComponents > newSize)
	{
		this->DecreaseArraySize(newSize);
	}

	return 0;
}

GraphicsComponent * GraphicsHandler::GetNextAvailableComponent()
{
	if (this->m_nrOfGraphicsComponents < this->m_maxGraphicsComponents)
	{
		this->m_nrOfGraphicsComponents++;
		return this->m_graphicsComponents[this->m_nrOfGraphicsComponents-1];
	}
	else
	{
		this->IncreaseArraySize();
		return this->GetNextAvailableComponent();
	}

	return nullptr;
}

int GraphicsHandler::UpdateComponentList()
{
	int result = 0;

	for (int i = 0; i < m_nrOfGraphicsComponents - 1; i++)
	{
		if (!this->m_graphicsComponents[i]->active)
		{
			GraphicsComponent* tempComponentPtr = this->m_graphicsComponents[this->m_nrOfGraphicsComponents - 1];
			this->m_graphicsComponents[this->m_nrOfGraphicsComponents - 1] = this->m_graphicsComponents[i];
			this->m_graphicsComponents[i] = tempComponentPtr;
			this->m_nrOfGraphicsComponents--;
			i--;
			result++;
		}
	}
	if (!this->m_graphicsComponents[this->m_nrOfGraphicsComponents - 1]->active)
	{
		this->m_nrOfGraphicsComponents--;
		result++;
	}

	return result;
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
