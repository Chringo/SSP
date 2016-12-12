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
	Resources::ResourceHandler::GetInstance()->LoadLevel(UINT(1337)); //placeholder id

	this->m_shaderControl = new ShaderControl;
	m_shaderControl->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext(), resolution);
	m_shaderControl->SetBackBufferRTV(m_d3dHandler->GetBackbufferRTV());

	ConstantBufferHandler::GetInstance()->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext());

	this->m_camera = new Camera;
	this->m_camera->Initialize();

	this->m_CreateTempsTestComponents();
	


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
		m_shaderControl->Draw(m_modelsPtr[0], this->m_graphicsComponents[i]);
	}
	//for (int i = 0; i < 0; i++) //FOR EACH "OTHER TYPE OF GEOMETRY" ETC...
	//{
	//}
	//m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Animated);
	//m_shaderControl->Draw(m_modelsPtr[1], this->m_animGraphicsComponents[0]);


	m_shaderControl->DrawFinal();

	/*TEMP CBUFFER STUFF*/

	/*TEMP CBUFFER STUFF*/


	this->m_d3dHandler->PresentScene();
	return 0;
}

int GraphicsHandler::InitializeGrid()
{
	//Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), m_modelsPtr[0]);
	m_d3dHandler->InitializeGridRasterizer();
	m_deferredSH->InitializeGridShader(this->m_d3dHandler->GetDevice());
	return 0;
}

int GraphicsHandler::RenderGrid(Resources::Model* model, GraphicsComponent* component) //will render the grid from said variables every frame, there will be a updategrid function for this instead later
{
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Wireframe);

	m_d3dHandler->SetRasterizerState(D3D11_FILL_WIREFRAME);
	m_shaderControl->Draw(model, component);
	m_d3dHandler->SetRasterizerState(D3D11_FILL_SOLID);

	return 0;
}

int GraphicsHandler::RenderFromEditor(Resources::Model* model,GraphicsComponent* component)
{

	ConstantBufferHandler::ConstantBuffer::camera::cbData cam;
	this->m_camera->GetCameraPos(cam.cPos);
	this->m_camera->GetViewMatrix(cam.cView);
	cam.cProjection = DirectX::XMLoadFloat4x4(this->m_camera->GetProjectionMatrix());
	ConstantBufferHandler::GetInstance()->camera.UpdateBuffer(&cam);
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);

	//ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	this->m_shaderControl->Draw(model, component);
	//((DeferredShader*)m_shaderControl->m_shaders[0])->DrawFromEditor(model);

	return 0;
}

int GraphicsHandler::renderFinalEditor()
{
	m_shaderControl->DrawFinal();
	this->m_d3dHandler->PresentScene();
	return 0;
}

int GraphicsHandler::clearEditor()
{
	m_shaderControl->ClearFrame();
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

	for (int i = 1; i < 2; i++)
	{
		delete this->m_animGraphicsComponents[i];
		this->m_animGraphicsComponents[i] = nullptr;
	}

	delete[] this->m_modelsPtr;
	delete[] this->m_graphicsComponents;
	delete[] this->m_animGraphicsComponents;
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

void GraphicsHandler::SetTempAnimComponent(void * component)
{
	m_animGraphicsComponents[0] = (penis*)component;
}

void GraphicsHandler::m_CreateTempsTestComponents()
{
	this->m_modelsPtr = new Resources::Model*[2];

	this->m_graphicsComponents = new GraphicsComponent*[this->m_maxGraphicsComponents];
	for (int i = 0; i < this->m_maxGraphicsComponents; i++) {
		this->m_graphicsComponents[i] = nullptr;
	}

	DirectX::XMMATRIX tempWorld = DirectX::XMMatrixIdentity();

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

	
	this->m_animGraphicsComponents = new penis*[2];
	for (int i = 0; i < 2; i++) {
		this->m_animGraphicsComponents[i] = nullptr;
	}

	this->m_animGraphicsComponents[1] = new penis;

	this->m_animGraphicsComponents[1]->worldMatrix = DirectX::XMMatrixIdentity();
	for (int j = 0; j < 32; j++)
	{
		this->m_animGraphicsComponents[1]->finalTransforms[j] = DirectX::XMMatrixIdentity();
	}
	
	/*TEMP MODELS*/
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(13337), m_modelsPtr[0]);
	Resources::ResourceHandler::GetInstance()->GetModel(UINT(1337), m_modelsPtr[1]);
	

}
