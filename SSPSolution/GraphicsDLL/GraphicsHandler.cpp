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

	//Use the root node in the octree to create arrays of things to render, one array for each model id

	int amountOfModelsToRender = 0;
	int componentsInTree = this->m_octreeRoot.containedComponents.size();
	struct InstanceData {
		int modelID;
		int amountOfInstances;
		DirectX::XMFLOAT4X4* componentSpecific;
	};
	std::vector<InstanceData> instancedRenderingList;
	unsigned int lastModelID = 0;
	if (componentsInTree > 0)
		lastModelID = this->m_octreeRoot.containedComponents[0]->modelID;
	int amountOfModelOccurrencees = 0;
	//for (int i = 0; i < componentsInTree; i++)
	//{
	//	//If the component is to be rendered, increase the counter
	//	if (this->m_octreeRoot.containedComponents[i]->isRendered)
	//	{
	//		//Because we know that the list is sorted, when the ID changes we can create an array with the amounf of last model ID occurrencees
	//		if (lastModelID != this->m_octreeRoot.containedComponents[i]->modelID)
	//		{
	//			//Create the array
	//			InstanceData instanceData;
	//			instanceData.modelID = lastModelID;
	//			instanceData.amountOfInstances = amountOfModelOccurrencees;
	//			instanceData.componentSpecific = new DirectX::XMFLOAT4X4[amountOfModelOccurrencees];

	//			amountOfModelOccurrencees = 0;
	//		}
	//		++amountOfModelOccurrencees;
	//	}
	//}
	//Fill the array with valuable data
	int instancedRenderingIndex = 0;
	int instancedModelCount = 0;
	//for (int i = 0; i < componentsInTree; i++)
	//{
	//	//reset the 'isRendered' bool
	//	if (this->m_octreeRoot.containedComponents[i]->isRendered != false)
	//	{
	//		//If it is time to change 
	//		if (this->m_octreeRoot.containedComponents[i]->modelID != lastModelID)
	//		{
	//			instancedRenderingIndex++;
	//			instancedModelCount = 0;
	//		}
	//		//Store the data
	//		DirectX::XMStoreFloat4x4(&instancedRenderingList[instancedRenderingIndex].componentSpecific[instancedModelCount++], this->m_staticGraphicsComponents[i]->worldMatrix);
	//		
	//		this->m_octreeRoot.containedComponents[i]->isRendered = false;
	//	}
	//}
	//By all means it should be done by now

	m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);
	//Go through all components in the root node and render the ones that should be rendered
	for (int i = 0; i < componentsInTree; i++) //FOR EACH NORMAL GEOMETRY
	{
		if (this->m_octreeRoot.containedComponents[i]->isRendered)
		{
			if (this->m_staticGraphicsComponents[this->m_octreeRoot.containedComponents[i]->componentIndex]->active)
			{
				m_shaderControl->Draw(this->m_staticGraphicsComponents[this->m_octreeRoot.containedComponents[i]->componentIndex]->modelPtr, this->m_staticGraphicsComponents[this->m_octreeRoot.containedComponents[i]->componentIndex]);
			}
		}
		/*if (this->m_staticGraphicsComponents[i]->active == false)
		continue;
		m_shaderControl->Draw(m_staticGraphicsComponents[i]->modelPtr, m_staticGraphicsComponents[i]);*/
	}
	for (int i = 0; i < this->m_dynamicGraphicsComponents.size(); i++) //FOR EACH NORMAL GEOMETRY
	{
		if (this->m_dynamicGraphicsComponents[i]->active)
		{
			m_shaderControl->Draw(this->m_dynamicGraphicsComponents[i]->modelPtr, this->m_dynamicGraphicsComponents[i]);
		}
		
		/*if (this->m_staticGraphicsComponents[i]->active == false)
		continue;
		m_shaderControl->Draw(m_staticGraphicsComponents[i]->modelPtr, m_staticGraphicsComponents[i]);*/
	}

	//for (int i = 0; i < this->m_nrOfGraphicsComponents; i++) //FOR EACH NORMAL GEOMETRY
	//{
	//	if (this->m_staticGraphicsComponents[i]->active == false)
	//		continue;
	//	m_shaderControl->Draw(m_staticGraphicsComponents[i]->modelPtr, m_staticGraphicsComponents[i]);
	//}

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
	//Clear the memory for the components
	for (size_t i = 0; i < this->m_staticGraphicsComponents.size(); i++)
	{
		if (this->m_staticGraphicsComponents[i] != nullptr)
		{
			delete this->m_staticGraphicsComponents[i];
			this->m_staticGraphicsComponents[i] = nullptr;
		}
	}
	for (size_t i = 0; i < this->m_dynamicGraphicsComponents.size(); i++)
	{
		if (this->m_dynamicGraphicsComponents[i] != nullptr)
		{
			delete this->m_dynamicGraphicsComponents[i];
			this->m_dynamicGraphicsComponents[i] = nullptr;
		}
	}
	for (size_t i = 0; i < this->m_animationGraphicsComponents.size(); i++)
	{
		if (this->m_animationGraphicsComponents[i] != nullptr)
		{
			delete this->m_animationGraphicsComponents[i];
			this->m_animationGraphicsComponents[i] = nullptr;
		}
	}
	this->m_animationGraphicsComponents.clear();
	this->m_staticGraphicsComponents.clear();
	this->m_dynamicGraphicsComponents.clear();


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

int GraphicsHandler::GenerateOctree()
{
	int result = 0;
	//Check amount of components to be included into the octree
	int componentCount = this->m_nrOfGraphicsComponents;


	//Create the BoundingVolume we cull against
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[0];
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[1];
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[2];

	float minX, maxX, minY, maxY, minZ, maxZ;
	minX = minY = minZ = D3D11_FLOAT32_MAX;
	maxX = maxY = maxZ = -D3D11_FLOAT32_MAX;

	std::vector<OctreeBV> listOfComponentBV;
	this->m_octreeRoot.containedComponents.resize(componentCount);
	//Fill the octree with the data
	for (size_t i = 0; i < componentCount; i++)
	{
		this->m_octreeRoot.containedComponents[i]->ext.x = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().extension[0];
		this->m_octreeRoot.containedComponents[i]->ext.y = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().extension[1];
		this->m_octreeRoot.containedComponents[i]->ext.z = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().extension[2];
		this->m_octreeRoot.containedComponents[i]->pos.x = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().position.x;
		this->m_octreeRoot.containedComponents[i]->pos.y = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().position.y;
		this->m_octreeRoot.containedComponents[i]->pos.z = this->m_staticGraphicsComponents[i]->modelPtr->GetOBBData().position.z;
		this->m_octreeRoot.containedComponents[i]->modelID = this->m_staticGraphicsComponents[i]->modelID;

		//Check for the lowest and highest values
		if (this->m_octreeRoot.containedComponents[i]->ext.x < minX)
			minX = this->m_octreeRoot.containedComponents[i]->ext.x;
		else if (this->m_octreeRoot.containedComponents[i]->ext.x > maxX)
			maxX = this->m_octreeRoot.containedComponents[i]->ext.x;
		if (this->m_octreeRoot.containedComponents[i]->ext.y < minY)
			minY = this->m_octreeRoot.containedComponents[i]->ext.y;
		else if (this->m_octreeRoot.containedComponents[i]->ext.y > maxY)
			maxY = this->m_octreeRoot.containedComponents[i]->ext.y;
		if (this->m_octreeRoot.containedComponents[i]->ext.z < minZ)
			minZ = this->m_octreeRoot.containedComponents[i]->ext.z;
		else if (this->m_octreeRoot.containedComponents[i]->ext.z > maxZ)
			maxZ = this->m_octreeRoot.containedComponents[i]->ext.z;
	}
	//After having finished filling the octree with data, sort it
	std::sort(this->m_octreeRoot.containedComponents.begin(), this->m_octreeRoot.containedComponents.end(), Sorting_on_modelID());
	
	//Initialize the octree root
	for (int i = 0; i < 8; i++)
	{
		this->m_octreeRoot.branches[i] = nullptr;
	}
	this->m_octreeRoot.ext = DirectX::XMFLOAT3((maxX - minX) / 2.0f, (maxY - minY) / 2.0f, (maxZ - minZ) / 2.0f);
	this->m_octreeRoot.pos = DirectX::XMFLOAT3(minX + this->m_octreeRoot.ext.x, minY + this->m_octreeRoot.ext.y, minZ + this->m_octreeRoot.ext.z);
	

	//Build the tree
	this->OctreeExtend(&this->m_octreeRoot, 0);

	return result;
}

GRAPHICSDLL_API int GraphicsHandler::FrustrumCullOctreeNode()
{
	int result = 0;
	Camera::ViewFrustrum currentFrustrum;
	this->m_camera->GetViewFrustrum(currentFrustrum);
	this->TraverseOctree(&this->m_octreeRoot, &currentFrustrum);

	return result;
}

int GraphicsHandler::ResizeDynamicComponents(size_t new_cap)
{
	int result = 1;
	result = this->m_dynamicGraphicsComponents.size();
	//Delete all old components
	//std::remove_if(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	//std::remove(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	std::transform(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), this->m_dynamicGraphicsComponents.begin(), GraphicsComponent_Remove_All_Unary());

	this->m_dynamicGraphicsComponents.clear();

	//Set size for the vector
	this->m_dynamicGraphicsComponents.resize(new_cap, nullptr);
	//Go through vector and make pointers point to a structure
	size_t amountOfComponents = new_cap;
	for (size_t i = 0; i < amountOfComponents; i++)
	{
		this->m_dynamicGraphicsComponents[i] = new GraphicsComponent();
	}
	return  result;
}

int GraphicsHandler::ResizeStaticComponents(size_t new_cap)
{
	int result = 1;
	result = this->m_staticGraphicsComponents.size();
	//Delete all old components
	//std::remove_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	//std::remove(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	std::transform(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), this->m_staticGraphicsComponents.begin(), GraphicsComponent_Remove_All_Unary());

	this->m_staticGraphicsComponents.clear();

	//Set size for the vector
	this->m_staticGraphicsComponents.resize(new_cap, nullptr);
	//Go through vector and make pointers point to a structure
	size_t amountOfComponents = new_cap;
	for (size_t i = 0; i < amountOfComponents; i++)
	{
		this->m_staticGraphicsComponents[i] = new GraphicsComponent();
	}
	return  result;
}

int GraphicsHandler::ResizeAnimationComponents(size_t new_cap)
{
	int result = 0;
	this->m_animationGraphicsComponents.resize(new_cap);
	return  result;
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


GraphicsComponent * GraphicsHandler::GetNextAvailableStaticComponent()
{
	GraphicsComponent* result = nullptr;
	//Yea
	//result = *std::find_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), Find_Available_Component);
	result = *std::find_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(),
		[](GraphicsComponent* comp) { return (comp->active == 0); });
	
	/*if (std::find_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), [&new_id](const entry &arg) {
		return arg.first == new_id; }) != ...)*/
	if (result != nullptr && result->active)
	{
		result = nullptr;
	}
	return result;
	//Yea that happened
	/*if ((result = *std::find(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), Find_Available_gComponent()))->active == true)
		return result;
	else
		return nullptr;*/
}

GraphicsComponent * GraphicsHandler::GetNextAvailableDynamicComponent()
{
	GraphicsComponent* result = nullptr;
	//result = *std::find(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), Find_Available_gComponent());
	result = *std::find_if(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(),
		[](GraphicsComponent* comp) { return (comp->active == 0); });
	if (result != nullptr && result->active)
	{
		result = nullptr;
	}
	return result;
}

//GraphicsAnimationComponent * GraphicsHandler::GetNextAvailableAnimationComponent()
//{
//	GraphicsAnimationComponent* newComponent = new GraphicsAnimationComponent();
//	this->m_animationGraphicsComponents.push_back(newComponent);
//	return newComponent;
//}
	
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

void GraphicsHandler::OctreeExtend(OctreeNode* curNode, int depth)
{
	//Check if this node we are currently working on can split
	if (depth < this->m_maxDepth)
	{
		int containedCount = curNode->containedComponents.size();
		if (containedCount> 0)
		{
			if (curNode->ext.x > this->m_minSize && curNode->ext.y > this->m_minSize && curNode->ext.z > this->m_minSize)
			{
				if (depth < this->m_minDepth)
				{
					if (containedCount > this->m_minContainment)
					{
						//Create the extensions, we cull them if not needed later
						for (int i = 0; i < 8; i++)
						{
							curNode->branches[i] = new OctreeNode();
						}
						//For the 8 new branches
#pragma region
						//MIN	MIN		MIN
						curNode->branches[0]->pos.x = curNode->pos.x - curNode->ext.x / 2;
						curNode->branches[0]->pos.y = curNode->pos.y - curNode->ext.y / 2;
						curNode->branches[0]->pos.z = curNode->pos.z - curNode->ext.z / 2;
						curNode->branches[0]->ext.x = curNode->ext.x / 2;
						curNode->branches[0]->ext.y = curNode->ext.y / 2;
						curNode->branches[0]->ext.z = curNode->ext.z / 2;
						//MIN	MIN		MAX											 
						curNode->branches[1]->pos.x = curNode->pos.x - curNode->ext.x / 2;
						curNode->branches[1]->pos.y = curNode->pos.y - curNode->ext.y / 2;
						curNode->branches[1]->pos.z = curNode->pos.z + curNode->ext.z / 2;
						curNode->branches[1]->ext.x = curNode->ext.x / 2;
						curNode->branches[1]->ext.y = curNode->ext.y / 2;
						curNode->branches[1]->ext.z = curNode->ext.z / 2;
						//MAX	MIN		MAX											  
						curNode->branches[2]->pos.x = curNode->pos.x + curNode->ext.x / 2;
						curNode->branches[2]->pos.y = curNode->pos.y - curNode->ext.y / 2;
						curNode->branches[2]->pos.z = curNode->pos.z + curNode->ext.z / 2;
						curNode->branches[2]->ext.x = curNode->ext.x / 2;
						curNode->branches[2]->ext.y = curNode->ext.y / 2;
						curNode->branches[2]->ext.z = curNode->ext.z / 2;
						//MIN	MIN		MAX											  
						curNode->branches[3]->pos.x = curNode->pos.x + curNode->ext.x / 2;
						curNode->branches[3]->pos.y = curNode->pos.y - curNode->ext.y / 2;
						curNode->branches[3]->pos.z = curNode->pos.z - curNode->ext.z / 2;
						curNode->branches[3]->ext.x = curNode->ext.x / 2;
						curNode->branches[3]->ext.y = curNode->ext.y / 2;
						curNode->branches[3]->ext.z = curNode->ext.z / 2;

						//MIN	MAX		MIN											  
						curNode->branches[4]->pos.x = curNode->pos.x - curNode->ext.x / 2;
						curNode->branches[4]->pos.y = curNode->pos.y + curNode->ext.y / 2;
						curNode->branches[4]->pos.z = curNode->pos.z - curNode->ext.z / 2;
						curNode->branches[4]->ext.x = curNode->ext.x / 2;
						curNode->branches[4]->ext.y = curNode->ext.y / 2;
						curNode->branches[4]->ext.z = curNode->ext.z / 2;
						//MIN	MAX		MAX											  
						curNode->branches[5]->pos.x = curNode->pos.x - curNode->ext.x / 2;
						curNode->branches[5]->pos.y = curNode->pos.y + curNode->ext.y / 2;
						curNode->branches[5]->pos.z = curNode->pos.z + curNode->ext.z / 2;
						curNode->branches[5]->ext.x = curNode->ext.x / 2;
						curNode->branches[5]->ext.y = curNode->ext.y / 2;
						curNode->branches[5]->ext.z = curNode->ext.z / 2;
						//MAX	MAX		MAX											  
						curNode->branches[6]->pos.x = curNode->pos.x + curNode->ext.x / 2;
						curNode->branches[6]->pos.y = curNode->pos.y + curNode->ext.y / 2;
						curNode->branches[6]->pos.z = curNode->pos.z + curNode->ext.z / 2;
						curNode->branches[6]->ext.x = curNode->ext.x / 2;
						curNode->branches[6]->ext.y = curNode->ext.y / 2;
						curNode->branches[6]->ext.z = curNode->ext.z / 2;
						//MIN	MAX		MAX											  
						curNode->branches[7]->pos.x = curNode->pos.x + curNode->ext.x / 2;
						curNode->branches[7]->pos.y = curNode->pos.y + curNode->ext.y / 2;
						curNode->branches[7]->pos.z = curNode->pos.z - curNode->ext.z / 2;
						curNode->branches[7]->ext.x = curNode->ext.x / 2;
						curNode->branches[7]->ext.y = curNode->ext.y / 2;
						curNode->branches[7]->ext.z = curNode->ext.z / 2;

#pragma endregion Creating the branches


						//Fill the branches with the components
						//Split this node
						int xSplit = -1, ySplit = -1, zSplit = -1;
						//For every contained component
						for (int index = 0; index < containedCount; index++)
						{
							/*float distance = curNode->containedComponents[index].pos.x - curNode->pos.x;
							if (abs(distance) < curNode->containedComponents[index].ext.x)
								xSplit = 0;
							else
								xSplit += (distance > 0) * 2;

							distance = curNode->containedComponents[index].pos.y - curNode->pos.y;
							if (abs(distance) < curNode->containedComponents[index].ext.y)
								ySplit = 0;
							else
								ySplit += (distance > 0) * 2;

							distance = curNode->containedComponents[index].pos.z - curNode->pos.z;
							if (abs(distance) < curNode->containedComponents[index].ext.z)
								zSplit = 0;
							else
								zSplit += (distance > 0) * 2;*/


							
							//For all 8 branches: check if they intersect with the entity
							for (int j = 0; j < 8; j++)
							{
								if (AABBvsAABBIntersectionTest(curNode->branches[j]->pos, curNode->branches[j]->ext, curNode->containedComponents[index]->pos, curNode->containedComponents[index]->ext))
								{
									//The component is within the branch
									curNode->branches[j]->containedComponents.push_back(curNode->containedComponents[index]);
								}
							}

						}
						//After having pushed the components into the child remove them from this branch unless this is the root
						if (depth > 0)
						{
							curNode->containedComponents.clear();
						}
					}
				}
			}
			//Cull the branches without components
			for (int i = 0; i < 8; i++)
			{
				if (curNode->branches[i]->containedComponents.size() == 0)
				{
					delete curNode->branches[i];
					curNode->branches[i] = nullptr;
				}
				else
				{
					//Do the same algorithm for the child branches that are not culled
					this->OctreeExtend(curNode->branches[i], depth + 1);
				}
			}
		}
	}
	

}

void GraphicsHandler::TraverseOctree(OctreeNode * curNode, Camera::ViewFrustrum * cullingFrustrum)
{
	//Enum
	enum {MAX_BRANCHING = 8};
	//Safety check
	if (curNode != nullptr)
	{
		if (curNode->containedComponents.size() <= 0)
		{
			//Branch
			for (int i = 0; i < 8; i++)
			{
				//For all non-culled branches
				if (curNode->branches[i] != nullptr)
				{
					//Do the check to see if the branch is within the view frustrum
					Camera::C_AABB branchBounds;
					branchBounds.pos = curNode->pos;
					branchBounds.ext = curNode->ext;
					CullingResult cullingResult = cullingFrustrum->TestAgainstAABB(branchBounds);
					if (cullingResult != CullingResult::FRUSTRUM_OUTSIDE)
					{
						TraverseOctree(curNode->branches[i], cullingFrustrum);
					}
				}
			}
		}
		else
		{
			//Leaf
			for each (OctreeBV* entityComponent in curNode->containedComponents)
			{
				entityComponent->isRendered = true;
			}
		}
	}
}

int GraphicsHandler::AABBvsAABBIntersectionTest(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 ext1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 ext2)
{
	//If the distance between the centers is more than the combined exstensions, the box is not colliding on that axis
	if (abs(pos1.x - pos2.x) > ext1.x + ext2.x)
		return 0;
	if (abs(pos1.y - pos2.y) > ext1.y + ext2.y)
		return 0;
	if (abs(pos1.z - pos2.z) > ext1.z + ext2.z)
		return 0;
	//A one line version that does not use branch prediction
	//return abs(pos1.x - pos2.x) > ext1.x + ext2.x * abs(pos1.y - pos2.y) > ext1.y + ext2.y * abs(pos1.z - pos2.z) > ext1.z + ext2.z;
	return 1;
}
