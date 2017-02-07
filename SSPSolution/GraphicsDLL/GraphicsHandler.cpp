#include "GraphicsHandler.h"

#ifdef _DEBUG
void GraphicsHandler::RenderBoundingVolume(DirectX::XMVECTOR& pos,OBB & box, DirectX::XMVECTOR color)
{
	obbBoxes.push_back(&box);
	positions[T_OBB].push_back(&pos);
	colors[T_OBB].push_back(color);
}

void GraphicsHandler::RenderBoundingVolume(DirectX::XMVECTOR& pos,AABB & box, DirectX::XMVECTOR color)
{
	aabbBoxes.push_back(&box);
	positions[T_AABB].push_back(&pos);
	colors[T_AABB].push_back(color);
}

void GraphicsHandler::RenderBoundingVolume(DirectX::XMVECTOR & pos, Plane & plane, DirectX::XMVECTOR color)
{
	planes.push_back(&plane);
	positions[T_PLANE].push_back(&pos);
	colors[T_PLANE].push_back(color);
}
void GraphicsHandler::RenderBoundingVolume(DirectX::XMVECTOR & pos, Sphere & sphere, DirectX::XMVECTOR color)
{
	spheres.push_back(&sphere);
	positions[T_SPHERE].push_back(&pos);
	colors[T_SPHERE].push_back(color);
}

void GraphicsHandler::RenderBoundingVolume(DirectX::XMVECTOR * wayPoints, int numWaypoints, DirectX::XMVECTOR color)
{
	positions[T_WAYPOINT].push_back(wayPoints);
	colors[T_WAYPOINT].push_back(color);
	this->numWaypoints.push_back(numWaypoints);
	
	return;
}

void GraphicsHandler::RenderBoundingBoxes(bool noClip)
{
	ID3D11RenderTargetView* temp = m_d3dHandler->GetBackbufferRTV();
	ID3D11DeviceContext* context = m_d3dHandler->GetDeviceContext();
	if(noClip)
		context->OMSetRenderTargets(1, &temp, nullptr);
	else
		context->OMSetRenderTargets(1, &temp, this->dsv);
	m_debugRender.SetActive();
	for (size_t i = 0; i < obbBoxes.size(); i++)
	{
		m_debugRender.Render(*positions[T_OBB].at(i), *obbBoxes.at(i), colors[T_OBB].at(i));
	}
	positions[T_OBB].clear();
	colors[T_OBB].clear();
	for (size_t i = 0; i < aabbBoxes.size(); i++)
	{
		m_debugRender.Render(*positions[T_AABB].at(i), *aabbBoxes.at(i), colors[T_AABB].at(i));
	}
	positions[T_AABB].clear();
	colors[T_AABB].clear();
	for (size_t i = 0; i < planes.size(); i++)
	{
		m_debugRender.Render(*positions[T_PLANE].at(i), *planes.at(i), colors[T_PLANE].at(i));
	}
	positions[T_PLANE].clear();
	colors[T_PLANE].clear();
	for (size_t i = 0; i < spheres.size(); i++)
	{
		m_debugRender.Render(*positions[T_SPHERE].at(i), *spheres.at(i), colors[T_SPHERE].at(i));
	}
	positions[T_SPHERE].clear();
	colors[T_SPHERE].clear();

	for (size_t i = 0; i < positions[T_WAYPOINT].size(); i++)
	{
		m_debugRender.Render(positions[T_WAYPOINT].at(i), numWaypoints.at(i), colors[T_WAYPOINT].at(i));
	}
	positions[T_WAYPOINT].clear();
	colors[T_WAYPOINT].clear();
	numWaypoints.clear();
	


	planes.clear();
	obbBoxes.clear();
	aabbBoxes.clear();
	spheres.clear();

}
#endif // _DEBUG

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
			newArray[i] = new GraphicsComponent();
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

int GraphicsHandler::IncreaseArraySizeAnim()
{
	GraphicsAnimationComponent** newArray = new GraphicsAnimationComponent*[this->m_maxGraphicsAnimationComponents + ARRAY_INC];

	for (int i = 0; i < this->m_maxGraphicsAnimationComponents + ARRAY_INC; i++)
	{
		if (i < this->m_nrOfGraphicsAnimationComponents)
		{
			newArray[i] = this->m_animGraphicsComponents[i];
		}
		else
		{
			newArray[i] = new GraphicsAnimationComponent();
		}
	}
	delete[] this->m_animGraphicsComponents;
	this->m_animGraphicsComponents = newArray;
	this->m_maxGraphicsAnimationComponents += ARRAY_INC;

	return 1;
}

int GraphicsHandler::IncreaseArraySizeAnim(int increaseTo)
{
	GraphicsAnimationComponent** newArray = new GraphicsAnimationComponent*[increaseTo];

	for (int i = 0; i < increaseTo; i++)
	{
		if (i < this->m_nrOfGraphicsAnimationComponents)
		{
			newArray[i] = this->m_animGraphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}
	delete[] this->m_graphicsComponents;
	this->m_animGraphicsComponents = newArray;
	this->m_maxGraphicsAnimationComponents = increaseTo;

	return 1;
}

int GraphicsHandler::DecreaseArraySizeAnim()
{
	this->m_maxGraphicsAnimationComponents -= ARRAY_INC;
	GraphicsAnimationComponent** newArray = new GraphicsAnimationComponent*[this->m_maxGraphicsComponents];

	for (int i = 0; i < this->m_maxGraphicsAnimationComponents; i++)
	{
		if (i < this->m_nrOfGraphicsAnimationComponents)
		{
			newArray[i] = this->m_animGraphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}

	for (int i = this->m_maxGraphicsAnimationComponents; i < this->m_maxGraphicsAnimationComponents + ARRAY_INC; i++)
	{
		if (this->m_animGraphicsComponents[i])
		{
			delete this->m_animGraphicsComponents[i];
		}
	}

	delete[] this->m_animGraphicsComponents;
	this->m_animGraphicsComponents = newArray;

	return 1;
}

int GraphicsHandler::DecreaseArraySizeAnim(int decreaseTo)
{
	GraphicsAnimationComponent** newArray = new GraphicsAnimationComponent*[decreaseTo];

	for (int i = 0; i < decreaseTo; i++)
	{
		if (i < this->m_nrOfGraphicsAnimationComponents)
		{
			newArray[i] = this->m_animGraphicsComponents[i];
		}
		else
		{
			newArray[i] = nullptr;
		}
	}

	for (int i = decreaseTo; i < this->m_maxGraphicsAnimationComponents; i++)
	{
		if (this->m_animGraphicsComponents[i])
		{
			delete this->m_animGraphicsComponents[i];
		}
	}

	delete[] this->m_animGraphicsComponents;
	this->m_animGraphicsComponents = newArray;
	this->m_maxGraphicsAnimationComponents = decreaseTo;


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
	this->m_uiHandler			   = nullptr;
	this->m_LightHandler		   = nullptr;
	this->m_nrOfGraphicsComponents = 0;
	this->m_maxGraphicsComponents  = 5;
	this->m_nrOfGraphicsAnimationComponents = 0;
	this->m_maxGraphicsAnimationComponents = 5;
}


GraphicsHandler::~GraphicsHandler()
{
}

int GraphicsHandler::Initialize(HWND * windowHandle, const DirectX::XMINT2& resolution,  bool editorMode)
{
	this->m_d3dHandler = new Direct3DHandler;
	
	if (this->m_d3dHandler->Initialize(windowHandle, resolution))
	{
		return 1;
	}
#ifdef _DEBUG
	this->editorMode = editorMode;
	if (!editorMode)
#endif //_DEBUG
	{
		this->m_uiHandler = new UIHandler;
		this->m_uiHandler->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext());
		//Resources::ResourceHandler::GetInstance()->LoadLevel(UINT(1337)); //placeholder id
		//this->m_CreateTempsTestComponents();
	}
	ConstantBufferHandler::GetInstance()->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext());
	this->m_graphicsComponents = new GraphicsComponent*[this->m_maxGraphicsComponents];
	for (int i = 0; i < this->m_maxGraphicsComponents; i++) {
		//this->m_graphicsComponents[i] = nullptr;
		this->m_graphicsComponents[i] = new GraphicsComponent();
	}

	this->m_animGraphicsComponents = new GraphicsAnimationComponent*[this->m_maxGraphicsAnimationComponents];
	for (int i = 0; i < this->m_maxGraphicsAnimationComponents; i++) {
		//this->m_graphicsComponents[i] = nullptr;
		this->m_animGraphicsComponents[i] = new GraphicsAnimationComponent();
	}
	this->m_LightHandler = LIGHTING::LightHandler::GetInstance();
	this->m_LightHandler->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext());

	this->m_shaderControl = new ShaderControl;
	m_shaderControl->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext(), resolution);
	m_shaderControl->SetBackBuffer(m_d3dHandler->GetBackbufferRTV(), m_d3dHandler->GetBackbufferSRV());



	//this->m_CreateTempsTestComponents();
	//InitializeGrid();
#ifdef _DEBUG
	 obbBoxes.reserve(20);
	 aabbBoxes.reserve(20);
	 planes.reserve(20);
	 spheres.reserve(20);
	 dsv = m_shaderControl->GetBackBufferDSV();
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

int GraphicsHandler::Render(float deltaTime)
{
	ConstantBufferHandler::GetInstance()->ResetConstantBuffers();

	this->m_d3dHandler->ClearBlendState();
	m_shaderControl->ClearFrame();
	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime / 1000000;


	/*TEMP CBUFFER STUFF*/
	ConstantBufferHandler::ConstantBuffer::frame::cbData frame;
	this->m_camera->GetCameraPos(frame.cPos);
	this->m_camera->GetViewMatrix(frame.cView);
	frame.cProjection = DirectX::XMLoadFloat4x4(m_camera->GetProjectionMatrix());
	frame.cTimer = elapsedTime;
	/********************/

	ConstantBufferHandler::GetInstance()->frame.UpdateBuffer(&frame);

	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);
	for (int i = 0; i < this->m_nrOfGraphicsComponents; i++) //FOR EACH NORMAL GEOMETRY
	{
		if (this->m_graphicsComponents[i]->active == false)
			continue;
		m_shaderControl->Draw(m_graphicsComponents[i]->modelPtr, m_graphicsComponents[i]);
	}

	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Animated);
	for (int i = 0; i < this->m_nrOfGraphicsAnimationComponents; i++) //FOR EACH ANIMATED
	{
		if (this->m_animGraphicsComponents[i]->active == false)
			continue;
		m_shaderControl->Draw(m_animGraphicsComponents[i]->modelPtr, m_animGraphicsComponents[i]);
		
	}
	m_LightHandler->SetBuffersAsActive();
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
	RenderBoundingBoxes(false);
#endif // _DEBUG

	this->m_uiHandler->DrawUI();
	this->m_d3dHandler->PresentScene();
	
	return 0;
}

int GraphicsHandler::InitializeGrid()
{

	m_d3dHandler->InitializeGridRasterizer();

	this->m_shaderControl->InitializeWireframe(this->m_d3dHandler->GetDevice());
	return 0;
}

int GraphicsHandler::RenderGrid(Resources::Model* model, GraphicsComponent* component) //will render the grid from said variables every frame, there will be a updategrid function for this instead later
{

	ConstantBufferHandler::ConstantBuffer::frame::cbData frame;
	this->m_camera->GetCameraPos(frame.cPos);
	this->m_camera->GetViewMatrix(frame.cView);
	frame.cProjection = DirectX::XMLoadFloat4x4(this->m_camera->GetProjectionMatrix());
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	ConstantBufferHandler::GetInstance()->frame.UpdateBuffer(&frame);
	
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Wireframe);

	m_d3dHandler->SetRasterizerState(D3D11_FILL_WIREFRAME);
	m_shaderControl->Draw(model, component);
	m_d3dHandler->SetRasterizerState(D3D11_FILL_SOLID);

	return 0;
}

int GraphicsHandler::RenderFromEditor(Resources::Model* model,GraphicsComponent* component)
{

	ConstantBufferHandler::ConstantBuffer::frame::cbData frame;
	this->m_camera->GetCameraPos(frame.cPos);
	this->m_camera->GetViewMatrix(frame.cView);
	frame.cProjection = DirectX::XMLoadFloat4x4(this->m_camera->GetProjectionMatrix());
	ConstantBufferHandler::GetInstance()->frame.UpdateBuffer(&frame);
	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);

	//ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	this->m_shaderControl->Draw(model, component);
	//((DeferredShader*)m_shaderControl->m_shaders[0])->DrawFromEditor(model);

	return 0;
}

int GraphicsHandler::renderFinalEditor()
{
	m_LightHandler->SetBuffersAsActive();
	m_shaderControl->DrawFinal();
#ifdef _DEBUG
	RenderBoundingBoxes();
#endif // _DEBUG

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
	if (this->m_uiHandler)
	{
		this->m_uiHandler->Shutdown();
		delete this->m_uiHandler;
		this->m_uiHandler = nullptr;
	}
	if (this->m_windowHandle)
	{
		this->m_windowHandle = nullptr;
	}

	//for (int i = 1; i < 2; i++)
	//{
	//	delete this->m_animGraphicsComponents[i];
	//	this->m_animGraphicsComponents[i] = nullptr;
	//}
#ifdef _DEBUG
	if (!editorMode)
	{
		for (int i = 0; i < this->m_maxGraphicsComponents; i++)
		{
			if (this->m_graphicsComponents[i] != nullptr)
			{
				delete this->m_graphicsComponents[i];
				this->m_graphicsComponents[i] = nullptr;
			}
		}

		for (int i = 0; i < this->m_maxGraphicsAnimationComponents; i++)
		{
			if (this->m_animGraphicsComponents[i] != nullptr)
			{
				delete this->m_animGraphicsComponents[i];
				this->m_animGraphicsComponents[i] = nullptr;
			}
		}
	}
#endif // _DEBUG

	
	delete[] this->m_animGraphicsComponents;
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

int GraphicsHandler::SetAnimComponentArraySize(int newSize)
{
	if (this->m_maxGraphicsAnimationComponents < newSize)
	{
		this->IncreaseArraySize(newSize);
	}
	else if (this->m_maxGraphicsAnimationComponents > newSize)
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
GraphicsAnimationComponent* GraphicsHandler::GetNextAvailableAnimationComponent()
{
	if (this->m_nrOfGraphicsAnimationComponents < this->m_maxGraphicsAnimationComponents)
	{
		this->m_nrOfGraphicsAnimationComponents++;
		return this->m_animGraphicsComponents[this->m_nrOfGraphicsAnimationComponents - 1];
	}
	else
	{
		this->IncreaseArraySizeAnim();
		return this->GetNextAvailableAnimationComponent();
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

int GraphicsHandler::UpdateAnimComponentList()
{
	int result = 0;

	for (int i = 0; i < m_nrOfGraphicsAnimationComponents - 1; i++)
	{
		if (!this->m_animGraphicsComponents[i]->active)
		{
			GraphicsAnimationComponent* tempComponentPtr = this->m_animGraphicsComponents[this->m_nrOfGraphicsAnimationComponents - 1];
			this->m_animGraphicsComponents[this->m_nrOfGraphicsAnimationComponents - 1] = this->m_animGraphicsComponents[i];
			this->m_animGraphicsComponents[i] = tempComponentPtr;
			this->m_nrOfGraphicsAnimationComponents--;
			i--;
			result++;
		}
	}
	if (!this->m_animGraphicsComponents[this->m_nrOfGraphicsAnimationComponents - 1]->active)
	{
		this->m_nrOfGraphicsAnimationComponents--;
		result++;
	}

	return result;
}

UIComponent * GraphicsHandler::GetNextAvailableUIComponent()
{
	return this->m_uiHandler->GetNextUIComponent();
}

void GraphicsHandler::UpdateUIComponents(DirectX::XMFLOAT2 mousePos)
{
	this->m_uiHandler->UpdateUIComponentsclicked(mousePos);
}

TextComponent * GraphicsHandler::GetNextAvailableTextComponent()
{
	return this->m_uiHandler->GetNextTextComponent();
}

void GraphicsHandler::SetTempAnimComponent(void * component)
{
	m_animGraphicsComponents[0] = (GraphicsAnimationComponent*)component;
}

GraphicsComponent * GraphicsHandler::getComponent(int index)
{
	return this->m_graphicsComponents[index];
}

GraphicsAnimationComponent * GraphicsHandler::getAnimComponent(int index)
{
	return this->m_animGraphicsComponents[index];
}

void GraphicsHandler::m_CreateTempsTestComponents()
{
	

	this->m_graphicsComponents = new GraphicsComponent*[this->m_maxGraphicsComponents];
	for (int i = 0; i < this->m_maxGraphicsComponents; i++) {
		//this->m_graphicsComponents[i] = nullptr;
		this->m_graphicsComponents[i] = new GraphicsComponent();

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

	
	this->m_animGraphicsComponents = new GraphicsAnimationComponent*[2];
	for (int i = 0; i < 2; i++) {
		this->m_animGraphicsComponents[i] = nullptr;
	}

	this->m_animGraphicsComponents[1] = new GraphicsAnimationComponent;

	this->m_animGraphicsComponents[1]->worldMatrix = DirectX::XMMatrixIdentity();
	for (int j = 0; j < 32; j++)
	{
		this->m_animGraphicsComponents[1]->finalJointTransforms[j] = DirectX::XMMatrixIdentity();
	}
	


}
