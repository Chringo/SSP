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

	for (size_t i = 0; i < m_animGraphicsComponents[0]->jointCount; i++)
	{
		Sphere sp;
		sp.radius = 0.2f;


		DirectX::XMMATRIX tpose = DirectX::XMMATRIX(m_animGraphicsComponents[0]->modelPtr->GetSkeleton()->GetSkeletonData()->joints[i].invBindPose);
		DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(tpose);
		tpose = DirectX::XMMatrixInverse(&det, tpose);

		DirectX::XMVECTOR zero = { 0,0,0,0 };
		DirectX::XMMATRIX world = m_animGraphicsComponents[0]->worldMatrix;
		DirectX::XMMATRIX joint = m_animGraphicsComponents[0]->finalJointTransforms[i];

		DirectX::XMVECTOR pos = m_animGraphicsComponents[0]->finalJointTransforms[i].r[3];
		joint = DirectX::XMMatrixMultiply(tpose, joint);
		zero = DirectX::XMVector3TransformCoord(zero, joint);
		zero = DirectX::XMVector3TransformCoord(zero, world);

		m_debugRender.Render(zero, sp);
	}

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
	
	

	//for (size_t i = 0; i < m_animGraphicsComponents[0]->jointCount; i++)
	//{
	//	Sphere sp;
	//	sp.radius = 0.2f;


	//	DirectX::XMMATRIX tpose = DirectX::XMMATRIX(m_animGraphicsComponents[0]->modelPtr->GetSkeleton()->GetSkeletonData()->joints[i].invBindPose);
	//	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(tpose);
	//	tpose = DirectX::XMMatrixInverse(&det, tpose);

	//	DirectX::XMVECTOR zero ={ 0,0,0,0 };
	//	DirectX::XMMATRIX world = m_animGraphicsComponents[0]->worldMatrix;
	//	DirectX::XMMATRIX joint = m_animGraphicsComponents[0]->finalJointTransforms[i];

	//	DirectX::XMVECTOR pos   = m_animGraphicsComponents[0]->finalJointTransforms[i].r[3];
	//	joint = DirectX::XMMatrixMultiply(tpose,joint);
	//	zero = DirectX::XMVector3TransformCoord(zero, joint);
	//	zero = DirectX::XMVector3TransformCoord(zero, world);
	//	
	//	m_debugRender.Render(zero, sp);
	//}
	

	positions[T_WAYPOINT].clear();
	colors[T_WAYPOINT].clear();
	numWaypoints.clear();
	


	planes.clear();
	obbBoxes.clear();
	aabbBoxes.clear();
	spheres.clear();

}


int GraphicsHandler::RenderOctree(OctreeNode * curNode, Camera::ViewFrustrum * cullingFrustrum)
{
	int result = 0;
	//Enum
	enum { MAX_BRANCHING = 8 };
	//Safety check
	if (curNode != nullptr)
	{

		AABB myAABB = { curNode->ext.x , curNode->ext.y , curNode->ext.z };
		result += 1;
		DirectX::XMVECTOR renderColor = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		//Branch
		//If I am culled
		Camera::C_AABB branchBounds;
		branchBounds.pos = curNode->pos;
		branchBounds.ext = curNode->ext;
		CullingResult cullingResult = cullingFrustrum->TestAgainstAABB(branchBounds);
		if (cullingResult != CullingResult::FRUSTRUM_OUTSIDE)
		{
		/*float distance = -1.0;
		Camera::C_Ray ray = this->m_camera->CastRay();
		bool intersectsRay = this->RayVSAABB(ray, branchBounds, distance);
		bool originInNode = this->PointVSAABB(ray.origin, branchBounds);
		if (originInNode || (intersectsRay && distance < 1.5f))
		{*/
			renderColor = DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
			/*myAABB.ext[0] *= 1.0f;
			myAABB.ext[1] *= 1.0f;
			myAABB.ext[2] *= 1.0f;*/
			this->m_debugRender.Render(DirectX::XMLoadFloat3(&curNode->pos), myAABB, renderColor);
		}

		//Test against all internal components if it is the root
		if (curNode == &this->m_octreeRoot)
		{
			AABB containedAABB = { curNode->ext.x , curNode->ext.y , curNode->ext.z };
			for (OctreeBV* contained : curNode->containedComponents)
			{
				renderColor = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				containedAABB.ext[0] = contained->ext.x;
				containedAABB.ext[1] = contained->ext.y;
				containedAABB.ext[2] = contained->ext.z;
				this->m_debugRender.Render(DirectX::XMLoadFloat3(&contained->pos), containedAABB, renderColor);
			}
		}
		for (int i = 0; i < 8; i++)
		{
			//For all non-culled branches
			if (curNode->branches[i] != nullptr)
			{
				////Do the check to see if the branch is within the view frustrum
				//Camera::C_AABB branchBounds;
				//branchBounds.pos = curNode->pos;
				//branchBounds.ext = curNode->ext;
				//CullingResult cullingResult = cullingFrustrum->TestAgainstAABB(branchBounds);
				//if (cullingResult != CullingResult::FRUSTRUM_OUTSIDE)
				//{
				//	renderColor = DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
				//	myAABB.ext[0] *= 0.9999f;
				//	myAABB.ext[1] *= 0.9999f;
				//	myAABB.ext[2] *= 0.9999f;
				//}

				//Enter your branch
				result += RenderOctree(curNode->branches[i], cullingFrustrum);
			}
		}

	}
	return result;
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
	this->m_maxDepth = OCTREE_NODE_MAX_DEPTH;
	this->m_minDepth = OCTREE_NODE_MIN_DEPTH;
	this->m_minContainment = OCTREE_NODE_MIN_CONTAINMENT;
	this->m_minSize = OCTREE_NODE_MIN_SIZE;
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
	m_shaderControl->Initialize(this->m_d3dHandler->GetDevice(), this->m_d3dHandler->GetDeviceContext(), this->m_d3dHandler->GetViewPort());
	m_shaderControl->SetBackBuffer(m_d3dHandler->GetBackbufferRTV(), m_d3dHandler->GetBackbufferSRV());

	this->m_overviewCamera.Initialize(500.0f);
	DirectX::XMVECTOR camPos = DirectX::XMVectorSet(20.f, 1.f, -10.f, 0.f);
	DirectX::XMVECTOR camOffset = DirectX::XMVectorSet(0.f, 50.f, 0.f, 0.f);
	this->m_overviewCamera.SetCameraPivot(&camPos, camOffset, 2.0f);
	this->m_overviewCamera.RotateCameraPivot(0.f, -1.0f);
	/*this->m_overviewCamera.SetCameraPos(camPos);
	camPos = DirectX::XMVectorSet(10.f, 0.f, 0.f, 0.f);
	this->m_overviewCamera.SetLookAt(camPos);*/
	this->m_overviewCamera.Update();
	this->m_useOverview = false;

	this->m_activeLightIndices.reserve(MAX_ACTIVE_LIGHTS);

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
	int result = 0;
	ConstantBufferHandler::GetInstance()->ResetConstantBuffers();

	this->m_d3dHandler->ClearBlendState();
	m_shaderControl->ClearFrame();
	static float elapsedTime = 0.0f;
	elapsedTime += deltaTime / 1000000;

#pragma region CameraIntersectListCreation
	m_camera->ClearIntersectList();
	Camera::C_Ray ray = this->m_camera->CastRayFromMaxDistance();
	for (size_t i = 0; i < 8; i++)
	{
		this->TraverseOctreeRay(this->m_octreeRoot.branches[i], ray, false);
	}

	for (OctreeBV* i : this->m_octreeRoot.containedComponents)
	{
		if (i->isInRay)
		{
			result++;
			
			/*DirectX::XMMATRIX ortm;
			DirectX::XMFLOAT4X4 ort;
			memcpy(&ortm.r[0], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[0], sizeof(float) * 3);
			memcpy(&ortm.r[1], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[1], sizeof(float) * 3);
			memcpy(&ortm.r[2], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[2], sizeof(float) * 3);

			DirectX::XMStoreFloat4x4(&ort, ortm);
			this->m_camera->AddToIntersectCheck(
				ort,
				DirectX::XMFLOAT3(m_ConvertOBB(this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData()).ext),
				i->pos
			);*/

			this->m_camera->AddToIntersectCheck(i->pos, i->ext);

			i->isInRay = false;
			//This component needs to be checked against the ray for camera intersection
		}
	}
#pragma endregion

	/*TEMP CBUFFER STUFF*/
	ConstantBufferHandler::ConstantBuffer::frame::cbData frame;
	if (this->m_useOverview)
	{
		this->m_overviewCamera.GetCameraPos(frame.cPos);
		this->m_overviewCamera.GetViewMatrix(frame.cView);
		frame.cProjection = DirectX::XMLoadFloat4x4(this->m_overviewCamera.GetProjectionMatrix());
	}
	else
	{
		this->m_camera->GetCameraPos(frame.cPos);
		this->m_camera->GetViewMatrix(frame.cView);
		frame.cProjection = DirectX::XMLoadFloat4x4(this->m_camera->GetProjectionMatrix());
	}
	
	frame.cTimer = elapsedTime;


	/********************/

	ConstantBufferHandler::GetInstance()->frame.UpdateBuffer(&frame);

	//Use the root node in the octree to create arrays of things to render, one array for each model id

	std::vector<InstanceData> instancedRenderingList;
	std::vector<size_t> staticNormalRenderingList;
	//Go through all components dynamic components
	size_t renderCap = this->m_dynamicGraphicsComponents.size();
#pragma omp parallel num_threads(2)
	{
		int myThreadID = omp_get_thread_num();
		//int amountOfThreads = omp_get_num_threads();
		//printf("My ID: %d Out of: %d\n", myThreadID, amountOfThreads);
		if (myThreadID == 0)
		{
			int amountOfModelsToRender = 0;
			int componentsInTree = this->m_octreeRoot.containedComponents.size();

#pragma region
			unsigned int firstRenderedModelID = UINT_MAX;
			unsigned int firstRenderedInstancedModelID = 0;
			unsigned int lastModelID = 0;
			Resources::Model* lastModelPtr = nullptr;
			Resources::Model* firstRenderedModelPtr = nullptr;
			Resources::Model* firstRenderedInstancedModelPtr = nullptr;
			//Find the first model to be rendered and use that ones ModelID to prepare the loop after this one
			for (OctreeBV* i : this->m_octreeRoot.containedComponents)
			{
				if (i->isRendered)
				{
					if (i->modelID == lastModelID)
					{
						firstRenderedInstancedModelID = i->modelID;
						firstRenderedInstancedModelPtr = i->modelPtr;
						break;
					}
					lastModelPtr = i->modelPtr;
					lastModelID = i->modelID;
					if (firstRenderedModelID == UINT_MAX)
					{
						firstRenderedModelID = i->modelID;
						firstRenderedModelPtr = i->modelPtr;
					}
				}
			}

			//m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
			//m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);

			int amountOfModelOccurrencees = 0;
			unsigned int lastComponentIndex = 0;
			lastModelID = firstRenderedModelID;
			lastModelPtr = firstRenderedModelPtr;
			OctreeBV* lastRenderedComponent = nullptr;
			//m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal); // render shadows
			for (OctreeBV* i : this->m_octreeRoot.containedComponents)
			{
				//If the component is to be rendered, increase the counter
				if (i->isRendered)
				{
					//Because we know that the list is sorted, when the ID changes we can create an array with the amounf of last model ID occurrencees
					if (lastModelID != i->modelID || amountOfModelOccurrencees >= this->m_deferredSH->MAX_INSTANCED_GEOMETRY)
					{
						if (amountOfModelOccurrencees > 1)
						{
							//Create the array
							InstanceData instanceData;
							instanceData.modelID = lastModelID;
							instanceData.modelPtr = lastModelPtr;
							instanceData.amountOfInstances = amountOfModelOccurrencees;
							//instanceData.componentSpecific = new DirectX::XMFLOAT4X4[amountOfModelOccurrencees];
							instancedRenderingList.push_back(instanceData);
							amountOfModelOccurrencees = 0;

						}
						else
						{
							//m_shaderControl->Draw(this->m_staticGraphicsComponents[lastComponentIndex].modelPtr, &this->m_staticGraphicsComponents[lastComponentIndex]);
							staticNormalRenderingList.push_back(lastComponentIndex);
							lastRenderedComponent->isRendered = false;
							amountOfModelOccurrencees = 0;
						}
					}
					//Prepare the data for the next model ID
					lastModelPtr = i->modelPtr;
					lastModelID = i->modelID;
					lastComponentIndex = i->componentIndex;
					lastRenderedComponent = i;
					++amountOfModelOccurrencees;
				}
			}
			if (componentsInTree > 0 && lastRenderedComponent != nullptr)
			{
				if (amountOfModelOccurrencees > 1)
				{
					InstanceData instanceData;
					instanceData.modelID = lastModelID;
					instanceData.modelPtr = lastModelPtr;
					instanceData.amountOfInstances = amountOfModelOccurrencees;
					//instanceData.componentSpecific = new DirectX::XMFLOAT4X4[amountOfModelOccurrencees];
					instancedRenderingList.push_back(instanceData);
				}
				else
				{

					//m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal); 

					//m_shaderControl->Draw(this->m_staticGraphicsComponents[lastComponentIndex].modelPtr, &this->m_staticGraphicsComponents[lastComponentIndex]);
					staticNormalRenderingList.push_back(lastComponentIndex);

					lastRenderedComponent->isRendered = false;
					amountOfModelOccurrencees = -1;
				}
			}

			//Fill the array with valuable data
			int instancedRenderingIndex = 0;
			int instancedModelCount = 0;
			lastModelID = firstRenderedInstancedModelID;
			lastModelPtr = firstRenderedInstancedModelPtr;
			for (OctreeBV* i : this->m_octreeRoot.containedComponents)
			{
				//reset the 'isRendered' bool
				if (i->isRendered)
				{
					//If it is time to change 
					if (i->modelID != instancedRenderingList[instancedRenderingIndex].modelID || instancedModelCount >= this->m_deferredSH->MAX_INSTANCED_GEOMETRY)
					{
						instancedRenderingIndex++;
						instancedModelCount = 0;
					}
					//Get the data
					DirectX::XMMATRIX worldMatrix = this->m_staticGraphicsComponents[i->componentIndex].worldMatrix;
					worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
					//Store the data
					DirectX::XMStoreFloat4x4(&instancedRenderingList[instancedRenderingIndex].componentSpecific[instancedModelCount++], worldMatrix);
					i->isRendered = false;
				}
			}
#pragma endregion Octree stuff

		}
		else if (myThreadID == 1)
		{
			//Check lights against frustrum
#pragma region
			//Get the data and convert it into the specialized data we need
			LIGHTING::LightHandler::LightArray* lightArrayPtr = this->m_LightHandler->Get_Light_List();
			LIGHTING::Point* specializedData = lightArrayPtr->dataPtr;
			//Loop the lights
			Camera::ViewFrustrum frustrum;
			this->m_camera->GetViewFrustrum(frustrum);
			int lightsInFrustrum = 0;
			for (int lightIndex = 0; lightIndex < lightArrayPtr->numItems; lightIndex++)
			{
				if (frustrum.TestAgainstSphere(specializedData[lightIndex].position, specializedData[lightIndex].radius) > 0)
				{
					this->m_activeLightIndices.push_back(lightIndex);
				}
				//specializedData[lightIndex].isActive = frustrum.TestAgainstSphere(specializedData[lightIndex].position, specializedData[lightIndex].radius) > 0;
				//lightsInFrustrum += specializedData[lightIndex].isActive;
			}
			//printf("Lights in frustrum %d\n", lightsInFrustrum);
			//Update light buffer
			//this->m_LightHandler->UpdateStructuredBuffer(LIGHTING::LIGHT_TYPE::LT_POINT);
			this->m_LightHandler->UpdateActiveLightsToGPU(&m_activeLightIndices);
			this->m_activeLightIndices.clear();
#pragma endregion LightCulling


			m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);

#pragma region 

#pragma region Render shadows for normal (non instanced) geometry

			m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Shadow); // render shadows
			for (size_t i = 0; i < renderCap; i++) //FOR EACH NORMAL GEOMETRY
			{
				if (this->m_dynamicGraphicsComponents[i]->active)
				{
					m_shaderControl->Draw(this->m_dynamicGraphicsComponents[i]->modelPtr, this->m_dynamicGraphicsComponents[i]);
				}

			}


			for (size_t i = 0; i < m_persistantGraphicsComponents.size(); i++) //FOR EACH NORMAL G
			{
				if (this->m_persistantGraphicsComponents[i]->active)
				{
					m_shaderControl->Draw(this->m_persistantGraphicsComponents[i]->modelPtr, this->m_persistantGraphicsComponents[i]);
				}

			}
#pragma endregion
#pragma region Render shadows for animated geometry
			m_shaderControl->SetVariation(ShaderLib::ShaderVariations::AnimatedShadow);
			for (int i = 0; i < this->m_nrOfGraphicsAnimationComponents; i++) //FOR EACH ANIMATED
			{
				if (this->m_animGraphicsComponents[i]->active == false)
					continue;
				m_shaderControl->Draw(m_animGraphicsComponents[i]->modelPtr, m_animGraphicsComponents[i]);

			}
#pragma endregion
#pragma endregion Shadow pass


#pragma region
			m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal); //render
			for (size_t i = 0; i < (size_t)renderCap; i++) //FOR EACH NORMAL GEOMETRY
			{
				if (this->m_dynamicGraphicsComponents[i]->active)
				{
					m_shaderControl->Draw(this->m_dynamicGraphicsComponents[i]->modelPtr, this->m_dynamicGraphicsComponents[i]);
				}

			}

			renderCap = this->m_persistantGraphicsComponents.size();
			for (size_t i = 0; i < (size_t)renderCap; i++) //FOR EACH NORMAL GEOMETRY
			{
				if (this->m_persistantGraphicsComponents[i]->active)
				{
					m_shaderControl->Draw(this->m_persistantGraphicsComponents[i]->modelPtr, this->m_persistantGraphicsComponents[i]);
				}

			}
#pragma endregion Render non-instanced geometry

#pragma region
			m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Animated);
			for (int i = 0; i < this->m_nrOfGraphicsAnimationComponents; i++) //FOR EACH ANIMATED
			{
				if (this->m_animGraphicsComponents[i]->active == false)
					continue;
				m_shaderControl->Draw(m_animGraphicsComponents[i]->modelPtr, m_animGraphicsComponents[i]);


			}
#pragma endregion Render animated objects

		}
	}
	//m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Normal);
	//Render non-instanced static geometry
	for(size_t lastComponentIndex : staticNormalRenderingList)
		m_shaderControl->Draw(this->m_staticGraphicsComponents[lastComponentIndex].modelPtr, &this->m_staticGraphicsComponents[lastComponentIndex]);

#pragma region
	m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Instanced); //render instanced
	for (size_t i = 0; i < instancedRenderingList.size(); i++)
	{
		m_shaderControl->DrawInstanced(&instancedRenderingList.at(i));
	}

#pragma endregion Render Instanced objects

	//render joints

	//for (int a = 0; a < 21; a++)
	//{
	//	Sphere sphere;
	//	sphere.radius = 0.25;
	//	DirectX::XMMATRIX* inverseBindPose = &static_cast<DirectX::XMMATRIX>(m_animGraphicsComponents[0]->modelPtr->GetSkeleton()->GetSkeletonData()->joints[a].invBindPose);

	//	DirectX::XMVECTOR pos = m_animGraphicsComponents[0]->worldMatrix.r[3];
	//	DirectX::XMVECTOR offSet = DirectX::XMMatrixInverse(nullptr, *inverseBindPose).r[3];

	//	pos = DirectX::XMVectorAdd(pos, offSet);
	//	pos = DirectX::XMVectorSetW(pos, 1);

	//	this->RenderBoundingVolume(offSet, sphere);
	//}



	m_LightHandler->SetBufferAsActive();
	m_LightHandler->SetStaticShadowsToGPU();
	m_shaderControl->DrawFinal();

#pragma region
	if (postProcessing)
	{
		ID3D11ShaderResourceView* srv = m_d3dHandler->GetBackbufferSRV();
		ID3D11DeviceContext* context  = m_d3dHandler->GetDeviceContext();
		ID3D11RenderTargetView* temp  = nullptr;
		context->OMSetRenderTargets(1, &temp, NULL);
		context->PSSetShaderResources(6,1,&srv);

		m_shaderControl->PostProcess();
		ID3D11ShaderResourceView * tab[1];
		tab[0] = NULL;
		context->PSSetShaderResources(6, 1, tab);
	}
#pragma endregion Post Processing

#pragma region
#ifdef _DEBUG
	Camera::ViewFrustrum renderTest;
	this->m_camera->GetViewFrustrum(renderTest);

	ID3D11RenderTargetView* temp = m_d3dHandler->GetBackbufferRTV();
	ID3D11DeviceContext* context = m_d3dHandler->GetDeviceContext();
	context->OMSetRenderTargets(1, &temp, this->dsv);
	m_debugRender.SetActive();

	//this->RenderOctree(&this->m_octreeRoot, &renderTest);
	RenderBoundingBoxes(false);

	int modelQueries = Resources::ResourceHandler::GetInstance()->GetQueryCounter();
	assert(modelQueries == 0); // If this triggers, The resource lib has been accessed somewhere outside of level loading.
	Resources::ResourceHandler::GetInstance()->ResetQueryCounter();
#endif // _DEBUG
#pragma endregion Debug rendering


	this->m_uiHandler->DrawUI();
	this->m_d3dHandler->PresentScene();
	
	return result;
}


 int GraphicsHandler::RenderStaticObjectShadows()
{

	 m_shaderControl->SetActive(ShaderControl::Shaders::DEFERRED);
	 m_shaderControl->SetVariation(ShaderLib::ShaderVariations::Shadow);

	for (GraphicsComponent comp : m_staticGraphicsComponents)
		 m_shaderControl->Draw(comp.modelPtr, &comp);



	 
	 return  1;
}

 int GraphicsHandler::Update(float deltaTime)
 {
	 int result = 0;
	 //Convert deltaTime into seconds for easier timer management
	 deltaTime = deltaTime / 1000000.0f;
	 result = this->m_LightHandler->Update(deltaTime, this->m_camera->GetFocusPoint());

	 return result;
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
	m_LightHandler->SetBufferAsActive();
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
	/*for (size_t i = 0; i < this->m_staticGraphicsComponents.size(); i++)
	{
		if (this->m_staticGraphicsComponents[i] != nullptr)
		{
			delete this->m_staticGraphicsComponents[i];
			this->m_staticGraphicsComponents[i] = nullptr;
		}
	}*/
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
	for (size_t i = 0; i < this->m_persistantGraphicsComponents.size(); i++)
	{
		if (this->m_persistantGraphicsComponents[i] != nullptr)
		{
			delete this->m_persistantGraphicsComponents[i];
			this->m_persistantGraphicsComponents[i] = nullptr;
		}
	}
	this->m_animationGraphicsComponents.clear();
	this->m_staticGraphicsComponents.clear();
	this->m_dynamicGraphicsComponents.clear();
	this->m_persistantGraphicsComponents.clear();

	//Clear the Octree of contained components
	for (size_t i = 0; i < this->m_octreeRoot.containedComponents.size(); i++)
	{
		if (this->m_octreeRoot.containedComponents[i] != nullptr)
		{
			delete this->m_octreeRoot.containedComponents[i];
			this->m_octreeRoot.containedComponents[i] = nullptr;
		}
	}
	//Delete the octree branches
	this->DeleteOctree(&this->m_octreeRoot);

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
	enum{CORNER_MAX = 8};
	size_t componentCount = this->m_staticGraphicsComponents.size();


	//Create the BoundingVolume we cull against
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[0];
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[1];
	//this->m_staticGraphicsComponents[0]->modelPtr->GetOBBData().extension[2];

	float minX, maxX, minY, maxY, minZ, maxZ;
	minX = minY = minZ = D3D11_FLOAT32_MAX;
	maxX = maxY = maxZ = -D3D11_FLOAT32_MAX;

	std::vector<OctreeBV> listOfComponentBV;
	this->m_octreeRoot.containedComponents.resize(componentCount);
	//Fill the Octree with empty structs
	for (size_t i = 0; i < componentCount; i++)
	{
		this->m_octreeRoot.containedComponents[i] = new OctreeBV();
	}
	//Fill the octree with the data
	size_t i = 0;
	for ( i = 0; i < componentCount; i++)
	{
		//Fill the component with data
		this->m_octreeRoot.containedComponents[i]->pos = this->m_staticGraphicsComponents[i].pos;
		this->m_octreeRoot.containedComponents[i]->modelID = this->m_staticGraphicsComponents[i].modelID;
		this->m_octreeRoot.containedComponents[i]->componentIndex = i;
		this->m_octreeRoot.containedComponents[i]->ext.x = this->m_staticGraphicsComponents[i].modelPtr->GetOBBData().extension[0];
		this->m_octreeRoot.containedComponents[i]->ext.y = this->m_staticGraphicsComponents[i].modelPtr->GetOBBData().extension[1];
		this->m_octreeRoot.containedComponents[i]->ext.z = this->m_staticGraphicsComponents[i].modelPtr->GetOBBData().extension[2];
		Resources::ResourceHandler::GetInstance()->GetModel(this->m_octreeRoot.containedComponents[i]->modelID, this->m_octreeRoot.containedComponents[i]->modelPtr);
		
		//If the rotation isn't 0 create a bigger AABB
#pragma region
		if (!DirectX::XMMatrixIsIdentity(this->m_staticGraphicsComponents[i].ort))
		//if (this->m_octreeRoot.containedComponents[i]->ext.x != this->m_staticGraphicsComponents[i]->extensions.x || this->m_octreeRoot.containedComponents[i]->ext.y != this->m_staticGraphicsComponents[i]->extensions.y || this->m_octreeRoot.containedComponents[i]->ext.z != this->m_staticGraphicsComponents[i]->extensions.z)
		//if (this->m_staticGraphicsComponents[i]->rotation.x != 0 || this->m_staticGraphicsComponents[i]->rotation.y != 0 || this->m_staticGraphicsComponents[i]->rotation.z)
		{
			DirectX::XMVECTOR quaternion;
			DirectX::XMVECTOR translation;
			DirectX::XMVECTOR scale;
			DirectX::XMMatrixDecompose(&scale, &quaternion, &translation, this->m_staticGraphicsComponents[i].worldMatrix);
			DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(quaternion);
			DirectX::XMVECTOR corners[8];
			//Define the 8 AABB corners (if we exclude the center)
			corners[0] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ -this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ -this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ -this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[1] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ -this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ -this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ +this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[2] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ +this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ -this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ +this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[3] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ +this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ -this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ -this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[4] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ -this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ +this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ -this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[5] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ -this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ +this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ +this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[6] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ +this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ +this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ +this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			corners[7] = DirectX::XMVectorSet(/*this->m_octreeRoot.containedComponents[i]->pos.x*/ +this->m_octreeRoot.containedComponents[i]->ext.x, /*this->m_octreeRoot.containedComponents[i]->pos.y*/ +this->m_octreeRoot.containedComponents[i]->ext.y, /*this->m_octreeRoot.containedComponents[i]->pos.z*/ -this->m_octreeRoot.containedComponents[i]->ext.z, 1.0f);
			//Transform the 8 corners to the OBB corners
			for (int cornerIndex = 0; cornerIndex < CORNER_MAX; cornerIndex++)
			{
				//corners[i] = DirectX::XMVector3Rotate(corners[cornerIndex], quaternion);
				//Store the old pos
				DirectX::XMVECTOR oldPos = corners[cornerIndex];
				//Calculate the new pos
				corners[cornerIndex] = DirectX::XMVector4Transform(corners[cornerIndex], rotationMatrix);
			}
			//For the 8 OBB corners, calculate the largest extensions along each axis
			DirectX::XMFLOAT3 absExt;
			absExt.x = corners[0].m128_f32[0];
			absExt.y = corners[0].m128_f32[1];
			absExt.z = corners[0].m128_f32[2];
			for (int cornerIndex = 1; cornerIndex < 8; cornerIndex++)
			{
				if (abs(corners[cornerIndex].m128_f32[0]) > absExt.x)
					absExt.x = abs(corners[cornerIndex].m128_f32[0]);
				if (abs(corners[cornerIndex].m128_f32[1]) > absExt.y)
					absExt.y = abs(corners[cornerIndex].m128_f32[1]);
				if (abs(corners[cornerIndex].m128_f32[2]) > absExt.z)
					absExt.z = abs(corners[cornerIndex].m128_f32[2]);
			}
			this->m_octreeRoot.containedComponents[i]->ext.x = absExt.x;
			this->m_octreeRoot.containedComponents[i]->ext.y = absExt.y;
			this->m_octreeRoot.containedComponents[i]->ext.z = absExt.z;
		}
#pragma endregion Generate an AABB that contains the OBB

		//Check for the lowest and highest values
		if (this->m_octreeRoot.containedComponents[i]->pos.x - this->m_octreeRoot.containedComponents[i]->ext.x < minX)
			minX = this->m_octreeRoot.containedComponents[i]->pos.x - this->m_octreeRoot.containedComponents[i]->ext.x;
		if (this->m_octreeRoot.containedComponents[i]->pos.x + this->m_octreeRoot.containedComponents[i]->ext.x > maxX)
			maxX = this->m_octreeRoot.containedComponents[i]->pos.x + this->m_octreeRoot.containedComponents[i]->ext.x;
		if (this->m_octreeRoot.containedComponents[i]->pos.y - this->m_octreeRoot.containedComponents[i]->ext.y < minY)
			minY = this->m_octreeRoot.containedComponents[i]->pos.y - this->m_octreeRoot.containedComponents[i]->ext.y;
		if (this->m_octreeRoot.containedComponents[i]->pos.y + this->m_octreeRoot.containedComponents[i]->ext.y > maxY)
			maxY = this->m_octreeRoot.containedComponents[i]->pos.y + this->m_octreeRoot.containedComponents[i]->ext.y;
		if (this->m_octreeRoot.containedComponents[i]->pos.z - this->m_octreeRoot.containedComponents[i]->ext.z < minZ)
			minZ = this->m_octreeRoot.containedComponents[i]->pos.z - this->m_octreeRoot.containedComponents[i]->ext.z;
		if (this->m_octreeRoot.containedComponents[i]->pos.z + this->m_octreeRoot.containedComponents[i]->ext.z > maxZ)
			maxZ = this->m_octreeRoot.containedComponents[i]->pos.z + this->m_octreeRoot.containedComponents[i]->ext.z;
	}
	//After having finished filling the octree with data, sort it
	std::sort(this->m_octreeRoot.containedComponents.begin(), this->m_octreeRoot.containedComponents.end(), Sorting_on_modelID());
	
	//Could we calculate the average distribution along each axis?
	/*We have the amount of components, maybe we could take the most interesting axis of division when it comes to component clustering?
	A forest with a single telephone pole would be very uninteresting in the y-dimension, lending more value to optimizing the x-z plane division.
	Should we thus have a min-size for each axis of division? Though we should know the result of min size on max depth. If the deltaX of the world (DX)
	is 64m and we have a max of 5 division we would, with no min size, reach 2m along the x-axis.
	*/

	//We should start with optimizing for the largest delta
	float largestSize = max(max(maxX - minX, maxY - minY), maxZ - minZ);
	//Determine the max division according to the largest size divided by the min size. Roof the value.
	int size = int(largestSize + 0.5f);
	bool reachedMaxDepth = false;
	this->m_maxDepth = 0;
	while (!reachedMaxDepth)
	{
		this->m_maxDepth++;
		size = size / 2;
		reachedMaxDepth = !(size > this->m_minSize);
	}
	//this->m_maxDepth = int((largestSize / this->m_minSize) + 0.5f);
	//this->m_maxDepth = OCTREE_NODE_MAX_DEPTH;
	//Initialize the octree root
	for (i = 0; i < 8; i++)
	{
		this->m_octreeRoot.branches[i] = nullptr;
	}
	int option = 1;
	switch (option)
	{
	case 0:
		//This is the ordinary cubetree
		this->m_octreeRoot.ext = DirectX::XMFLOAT3((maxX - minX) / 2.0f, (maxY - minY) / 2.0f, (maxZ - minZ) / 2.0f);
		this->m_octreeRoot.pos = DirectX::XMFLOAT3(minX + this->m_octreeRoot.ext.x, minY + this->m_octreeRoot.ext.y, minZ + this->m_octreeRoot.ext.z);
		break;
	case 1:
		//This version does not center the cubed octree
		this->m_octreeRoot.ext = DirectX::XMFLOAT3((largestSize) / 2.0f, (largestSize) / 2.0f, (largestSize) / 2.0f);
		this->m_octreeRoot.pos = DirectX::XMFLOAT3(minX + this->m_octreeRoot.ext.x, minY + this->m_octreeRoot.ext.y, minZ + this->m_octreeRoot.ext.z);
		break;
	case 2:
		//This centers the cubed octree
		this->m_octreeRoot.ext = DirectX::XMFLOAT3((maxX - minX) / 2.0f, (maxY - minY) / 2.0f, (maxZ - minZ) / 2.0f);
		this->m_octreeRoot.pos = DirectX::XMFLOAT3(minX + this->m_octreeRoot.ext.x, minY + this->m_octreeRoot.ext.y, minZ + this->m_octreeRoot.ext.z);
		this->m_octreeRoot.ext = DirectX::XMFLOAT3((largestSize) / 2.0f, (largestSize) / 2.0f, (largestSize) / 2.0f);
		break;
	default:
		//This is the ordinary cubetree
		this->m_octreeRoot.ext = DirectX::XMFLOAT3((maxX - minX) / 2.0f, (maxY - minY) / 2.0f, (maxZ - minZ) / 2.0f);
		this->m_octreeRoot.pos = DirectX::XMFLOAT3(minX + this->m_octreeRoot.ext.x, minY + this->m_octreeRoot.ext.y, minZ + this->m_octreeRoot.ext.z);
		break;
	}

	//Build the tree
	this->OctreeExtend(&this->m_octreeRoot, 0);


	return result;
}
int GraphicsHandler::FrustrumCullOctreeLeft()
{
	int result = 0;
	enum { MAX_BRANCHES = 8 };
	Camera::ViewFrustrum currentFrustrum;
	this->m_camera->GetViewFrustrum(currentFrustrum);
	for (int i = 0; i < MAX_BRANCHES; i += 2)
	{
		if (this->m_octreeRoot.branches[i] != nullptr)
		{
			this->TraverseOctree(this->m_octreeRoot.branches[i], &currentFrustrum);
		}
	}

	/*int cap = this->m_octreeRoot.containedComponents.size();
	for (int i = 0; i < cap; i++)
	{
		if (this->m_octreeRoot.containedComponents[i]->isRendered)
		{
			result++;
		}
	}*/
	return result;
}
int GraphicsHandler::FrustrumCullOctreeRight()
{
	int result = 0;
	enum { MAX_BRANCHES = 8 };
	Camera::ViewFrustrum currentFrustrum;
	this->m_camera->GetViewFrustrum(currentFrustrum);
	for (int i = 1; i < MAX_BRANCHES; i += 2)
	{
		if (this->m_octreeRoot.branches[i] != nullptr)
		{
			this->TraverseOctree(this->m_octreeRoot.branches[i], &currentFrustrum);
		}
	}

	/*int cap = this->m_octreeRoot.containedComponents.size();
	for (int i = 0; i < cap; i++)
	{
		if (this->m_octreeRoot.containedComponents[i]->isRendered)
		{
			result++;
		}
	}*/
	return result;
}

int GraphicsHandler::FrustrumCullOctreeNodeThreaded(int threadCount)
{
	int result = 0;
	enum { MAX_BRANCHES = 8 };
	Camera::ViewFrustrum currentFrustrum;
	this->m_camera->GetViewFrustrum(currentFrustrum);
#pragma omp parallel num_threads(threadCount)
	{
		int i;
#pragma omp for private(i)
		for (i = 0; i < MAX_BRANCHES; i++)
		{
			if (this->m_octreeRoot.branches[i] != nullptr)
			{
				this->TraverseOctree(this->m_octreeRoot.branches[i], &currentFrustrum);
			}
		}
	}

	int cap = this->m_octreeRoot.containedComponents.size();
	for (int i = 0; i < cap; i++)
	{
		//this->m_octreeRoot.containedComponents[i]->isRendered = true;
		if (this->m_octreeRoot.containedComponents[i]->isRendered)
		{
			result++;
		}
	}
	return result;
}
int GraphicsHandler::FrustrumCullOctreeNode()
{
	int result = 0;
	enum {MAX_BRANCHES = 8};
	Camera::ViewFrustrum currentFrustrum;
	this->m_camera->GetViewFrustrum(currentFrustrum);
	for (int i = 0; i < MAX_BRANCHES; i++)
	{
		if (this->m_octreeRoot.branches[i] != nullptr)
		{
			this->TraverseOctree(this->m_octreeRoot.branches[i], &currentFrustrum);
		}
	}

	int cap = this->m_octreeRoot.containedComponents.size();
	for (int i = 0; i < cap; i++)
	{
		//this->m_octreeRoot.containedComponents[i]->isRendered = true;
		if (this->m_octreeRoot.containedComponents[i]->isRendered)
		{
			result++;
		}
	}
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
	//std::transform(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), this->m_staticGraphicsComponents.begin(), GraphicsComponent_Remove_All_Unary());

	this->m_staticGraphicsComponents.clear();

	//Set size for the vector
	this->m_staticGraphicsComponents.resize(new_cap);
	//Go through vector and make pointers point to a structure
	size_t amountOfComponents = new_cap;
	for (size_t i = 0; i < amountOfComponents; i++)
	{
		this->m_staticGraphicsComponents[i].active = false;
	}

	//Clear the Octree of contained components
	for (size_t i = 0; i < this->m_octreeRoot.containedComponents.size(); i++)
	{
		if (this->m_octreeRoot.containedComponents[i] != nullptr)
		{
			delete this->m_octreeRoot.containedComponents[i];
			this->m_octreeRoot.containedComponents[i] = nullptr;
		}
	}
	this->m_octreeRoot.containedComponents.clear();
	//Delete the octree branches
	this->DeleteOctree(&this->m_octreeRoot);

	return  result;
}

int GraphicsHandler::ResizeAnimationComponents(size_t new_cap)
{
	int result = 0;
	this->m_animationGraphicsComponents.resize(new_cap);
	return  result;
}

int GraphicsHandler::ResizePersistentComponents(size_t new_cap)
{
	int result = 1;
	result = this->m_persistantGraphicsComponents.size();
	//Delete all old components
	//std::remove_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	//std::remove(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), GraphicsComponent_Remove_All_Predicate());
	std::transform(this->m_persistantGraphicsComponents.begin(), this->m_persistantGraphicsComponents.end(), this->m_persistantGraphicsComponents.begin(), GraphicsComponent_Remove_All_Unary());

	this->m_persistantGraphicsComponents.clear();

	//Set size for the vector
	this->m_persistantGraphicsComponents.resize(new_cap, nullptr);
	//Go through vector and make pointers point to a structure
	size_t amountOfComponents = new_cap;
	for (size_t i = 0; i < amountOfComponents; i++)
	{
		this->m_persistantGraphicsComponents[i] = new GraphicsComponent();
	}
	return  result;
}

 int GraphicsHandler::GenerateStaticSceneShadows()
{
	 //For each light

		// Set the light as active for shadow rendering
		
		// Render the static objects in the scene

		// Save the textures to the light, (or to file)
	 
	 //
#ifdef _DEBUG
	 Resources::ResourceHandler::GetInstance()->ResetQueryCounter();
#endif // _DEBUG


//	Render(0.0f);


	LIGHTING::LightHandler::LightArray* lights =  m_LightHandler->Get_Light_List();
	
	ID3D11DeviceContext * context = this->m_d3dHandler->GetDeviceContext();
	ID3D11Device* device		  = this->m_d3dHandler->GetDevice();

	lights->ReleaseShadowMaps(); //release the textures if there are any
	if (lights->numShadowLights <= 0)
		return 1;

#pragma region Create the textureCubeArray
	D3D11_TEXTURE2D_DESC ShadowTexDesc;

	ShadowTexDesc.Width				 = (UINT)STATIC_SHADOWMAP_RESOLUTION;	
	ShadowTexDesc.Height			 = (UINT)STATIC_SHADOWMAP_RESOLUTION;	
	ShadowTexDesc.MipLevels			 = 1;
	ShadowTexDesc.ArraySize			 = 6 * lights->numShadowLights;	//one for each axis * number of lights
	ShadowTexDesc.Format			 = DXGI_FORMAT_R32_TYPELESS;
	ShadowTexDesc.SampleDesc.Count   = 1;
	ShadowTexDesc.SampleDesc.Quality = 0;
	ShadowTexDesc.Usage				 = D3D11_USAGE_DEFAULT;
	ShadowTexDesc.BindFlags			 = D3D11_BIND_SHADER_RESOURCE;
	ShadowTexDesc.CPUAccessFlags	 = D3D11_CPU_ACCESS_WRITE;
	ShadowTexDesc.MiscFlags			 = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//Create the render target Texture
	ID3D11Texture2D* tempTexture;
	HRESULT hResult = device->CreateTexture2D(&ShadowTexDesc, NULL, &tempTexture);
	if (FAILED(hResult))
	{
		return 1;
	}
 //Set up the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewShadowDesc;
	resourceViewShadowDesc.Format		 = DXGI_FORMAT_R32_FLOAT;
	resourceViewShadowDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
	resourceViewShadowDesc.TextureCubeArray.NumCubes		 = lights->numShadowLights;
	resourceViewShadowDesc.TextureCubeArray.First2DArrayFace = 0;
	resourceViewShadowDesc.TextureCubeArray.MostDetailedMip  = 0;
	resourceViewShadowDesc.TextureCubeArray.MipLevels		 = -1;



	//Create the resourceView;

	hResult = device->CreateShaderResourceView(tempTexture, &resourceViewShadowDesc, &lights->shadowMaps);
	if (FAILED(hResult))
		return 1;
#pragma endregion


	ConstantBufferHandler::GetInstance()->ResetConstantBuffers();

	for (size_t i = 0; i < lights->numShadowLights; i++) //for each light
	{

		m_shaderControl->ClearFrame();

		m_LightHandler->SetShadowCastingLight(lights->shadowLightIndex[i]);
		this->RenderStaticObjectShadows();						   //render statics
	
		
	
		for (size_t j = 0; j < 6; j++) //for each side of the cube map
		{
			context->CopySubresourceRegion(tempTexture, j + (6 * i), 0, 0, 0, m_shaderControl->GetShadowTexture(), j, NULL); //copy the  jth texture in the cubeMap

		}
	}

	m_LightHandler->SetStaticShadowsToGPU();
	tempTexture->Release();	

	 return  1;
}

int GraphicsHandler::ResetAnimationComponents()
{
	for (int i = 0; i < this->m_maxGraphicsAnimationComponents; i++) {
		if (this->m_animGraphicsComponents[i])
		{
			delete this->m_animGraphicsComponents[i];
		}
		this->m_animGraphicsComponents[i] = new GraphicsAnimationComponent();
	}
	this->m_nrOfGraphicsAnimationComponents = 0;
	return 0;
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
	if (this->m_staticGraphicsComponents.size() > 0)
	{
		for (int i = 0; i < this->m_staticGraphicsComponents.size(); i++)
		{
			if (this->m_staticGraphicsComponents[i].active == 0)
			{
				result = &this->m_staticGraphicsComponents[i];
				break;
			}
		}
		/*for (GraphicsComponent i : this->m_staticGraphicsComponents)
		{
			if (i.active == 0)
			{
				result = &i;
				break;
			}
		}*/

		/*result = std::find_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(),
		[](GraphicsComponent comp) { return (comp.active == 0); });*/
		/*if (std::find_if(this->m_staticGraphicsComponents.begin(), this->m_staticGraphicsComponents.end(), [&new_id](const entry &arg) {
		return arg.first == new_id; }) != ...)*/
		/*if (result != nullptr && result->active)
		{
			result = nullptr;
		}*/
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
	if (this->m_dynamicGraphicsComponents.size() > 0)
	{
		//If the list is empty it crashed, have fun
		//result = *std::find(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), Find_Available_gComponent());
		result = *std::find_if(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(),
			[](GraphicsComponent* comp) { return (comp->active == 0); });
		if (result != nullptr && result->active)
		{
			result = nullptr;
		}
	}
	return result;
}

GraphicsComponent * GraphicsHandler::GetNextAvailablePersistentComponent()
{
	GraphicsComponent* result = nullptr;
	if (this->m_persistantGraphicsComponents.size() > 0)
	{
		//If the list is empty it crashed, have fun
		//result = *std::find(this->m_dynamicGraphicsComponents.begin(), this->m_dynamicGraphicsComponents.end(), Find_Available_gComponent());
		result = *std::find_if(this->m_persistantGraphicsComponents.begin(), this->m_persistantGraphicsComponents.end(),
			[](GraphicsComponent* comp) { return (comp->active == 0); });
		if (result != nullptr && result->active)
		{
			result = nullptr;
		}
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

int GraphicsHandler::RemoveUIComponentFromPtr(UIComponent * ptr)
{
	return this->m_uiHandler->RemoveUIComponent(ptr);
}

int GraphicsHandler::RemoveLastUIComponent()
{
	return this->m_uiHandler->RemoveLastUIComponent();
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

void GraphicsHandler::ToggleOverviewCamera()
{
	this->m_useOverview = !this->m_useOverview;
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
	int containedCount = curNode->containedComponents.size();
	int shouldBranch = 0;
	//Determin if we should branch this node
	//Big enough to branch
	shouldBranch += (curNode->ext.x < this->m_minSize && curNode->ext.y < this->m_minSize && curNode->ext.z < this->m_minSize);
	//Contains enough for branching to be worth it
	shouldBranch += !(containedCount > this->m_minContainment);
	//Has not yet reached the maximum allowed depth
	shouldBranch += !(depth < this->m_maxDepth);
	//If should branch equals zero
	if (shouldBranch == 0 || (depth < this->m_minDepth))
	{
		//Create the branches, we cull them later if not needed
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
		//MAX	MIN		MIN											  
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
		//MAX	MAX		MIN											  
		curNode->branches[7]->pos.x = curNode->pos.x + curNode->ext.x / 2;
		curNode->branches[7]->pos.y = curNode->pos.y + curNode->ext.y / 2;
		curNode->branches[7]->pos.z = curNode->pos.z - curNode->ext.z / 2;
		curNode->branches[7]->ext.x = curNode->ext.x / 2;
		curNode->branches[7]->ext.y = curNode->ext.y / 2;
		curNode->branches[7]->ext.z = curNode->ext.z / 2;

#pragma endregion Creating the branches

		//Fill the branches
		for (int index = 0; index < containedCount; index++)
		{
			//For all 8 branches: check if they intersect with the entity
			for (int j = 0; j < 8; j++)
			{
				int withinCount = 0;
				DirectX::XMFLOAT3 compExtension = curNode->containedComponents[index]->ext;

				if (AABBvsAABBIntersectionTest(curNode->branches[j]->pos, curNode->branches[j]->ext, curNode->containedComponents[index]->pos, curNode->containedComponents[index]->ext))
				{
					//The component is within the branch
					curNode->branches[j]->containedComponents.push_back(curNode->containedComponents[index]);
					++withinCount;
				}
			}
		}

		//If we are not in the root which should contain the master list of components
		if (depth > 0)
		{
			//Empty this branch because we gave the brances our components
			curNode->containedComponents.clear();
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
					branchBounds.pos = curNode->branches[i]->pos;
					branchBounds.ext = curNode->branches[i]->ext;
					
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

void GraphicsHandler::TraverseOctreeRay(OctreeNode * curNode, Camera::C_Ray ray, bool pingRay)
{
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
					Camera::C_AABB branchBounds;
					branchBounds.pos = curNode->branches[i]->pos;
					branchBounds.ext = curNode->branches[i]->ext;
					//Check the ray origin vs octree
					bool originInNode = this->PointVSAABB(ray.origin, branchBounds);
					
					if (originInNode)
					{
						TraverseOctreeRay(curNode->branches[i], ray, pingRay);
					}
					//Check the ray vs octree
					else 
					{
						float distance = -1.0;
						bool intersectsRay = this->RayVSAABB(ray, branchBounds, distance);
						if (intersectsRay)
						{
							if (pingRay == false)
							{
								if (distance < 1.5f)
								{
									TraverseOctreeRay(curNode->branches[i], ray, pingRay);
								}
							}
							else 
							{
								if (distance < 100.f)
								{
									TraverseOctreeRay(curNode->branches[i], ray, pingRay);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			//Leaf
			for each (OctreeBV* entityComponent in curNode->containedComponents)
			{
				if (!pingRay)
				{
					entityComponent->isInRay = true;
				}
				else
				{
					entityComponent->isInPingRay = true;
				}
			}
		}
	}
}

bool GraphicsHandler::RayVSAABB(Camera::C_Ray ray, Camera::C_AABB bb, float& distance)
{
#pragma region
	////double tx1 = (b.min.x - r.x0.x)*r.n_inv.x;
	////double tx2 = (b.max.x - r.x0.x)*r.n_inv.x;
	//double tx1 = ((bb.pos.x - bb.ext.x) - ray.origin.x) * (1.f / ray.dir.x);
	//double tx2 = ((bb.pos.x + bb.ext.x) - ray.origin.x) * (1.f / ray.dir.x);

	//double tmin = min(tx1, tx2);
	//double tmax = max(tx1, tx2);

	////double ty1 = (b.min.y - r.x0.y)*r.n_inv.y;
	////double ty2 = (b.max.y - r.x0.y)*r.n_inv.y;
	//double ty1 = ((bb.pos.y - bb.ext.y) - ray.origin.y) * (1.f / ray.dir.y);
	//double ty2 = ((bb.pos.y + bb.ext.y) - ray.origin.y) * (1.f / ray.dir.y);

	//tmin = max(tmin, min(ty1, ty2));
	//tmax = min(tmax, max(ty1, ty2));

	//double tz1 = ((bb.pos.z - bb.ext.z) - ray.origin.z) * (1.f / ray.dir.z);
	//double tz2 = ((bb.pos.z + bb.ext.z) - ray.origin.z) * (1.f / ray.dir.z);

	//tmin = max(tmin, min(tz1, tz2));
	//tmax = min(tmax, max(tz1, tz2));

	//distance = tmax - tmin;
	//return tmax >= tmin;
#pragma endregion v1

#pragma region
	////Other implementation
	//float tmin = FLT_MIN;
	//float tmax = FLT_MAX;

	////For x axis
	//float invDir = 1.f / ray.dir.x;
	//float min = bb.pos.x - bb.ext.x;
	//float max = bb.pos.x + bb.ext.x;
	//float t0 = (min - ray.origin.x) * invDir;
	//float t1 = (max - ray.origin.x) * invDir;
	//if (t0 > t1)
	//{
	//	std::swap(t0, t1);
	//}
	//tmin = t0 > tmin ? t0 : tmin;
	//tmax = t1 < tmax ? t1 : tmax;
	//if (tmax <= tmin)
	//{
	//	return false;
	//}

	////For y axis
	//invDir = 1.f / ray.dir.y;
	//min = bb.pos.y - bb.ext.y;
	//max = bb.pos.y + bb.ext.y;
	//t0 = (min - ray.origin.y) * invDir;
	//t1 = (max - ray.origin.y) * invDir;
	//if (t0 > t1)
	//{
	//	std::swap(t0, t1);
	//}
	//tmin = t0 > tmin ? t0 : tmin;
	//tmax = t1 < tmax ? t1 : tmax;
	//if (tmax <= tmin)
	//{
	//	return false;
	//}

	////For z axis
	//invDir = 1.f / ray.dir.z;
	//min = bb.pos.z - bb.ext.z;
	//max = bb.pos.z + bb.ext.z;
	//t0 = (min - ray.origin.z) * invDir;
	//t1 = (max - ray.origin.z) * invDir;
	//if (t0 > t1)
	//{
	//	std::swap(t0, t1);
	//}
	//tmin = t0 > tmin ? t0 : tmin;
	//tmax = t1 < tmax ? t1 : tmax;
	//if (tmax <= tmin)
	//{
	//	return false;
	//}
	//distance = tmax - tmin;
	//return true;
#pragma endregion v2

#pragma region
	//Implementation from 3dProject
	DirectX::XMFLOAT3 invDir = DirectX::XMFLOAT3(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
	
	float t1 = ((bb.pos.x - bb.ext.x) - ray.origin.x) * invDir.x;
	float t2 = ((bb.pos.x + bb.ext.x) - ray.origin.x) * invDir.x;
	float t3 = ((bb.pos.y - bb.ext.y) - ray.origin.y) * invDir.y;
	float t4 = ((bb.pos.y + bb.ext.y) - ray.origin.y) * invDir.y;
	float t5 = ((bb.pos.z - bb.ext.z) - ray.origin.z) * invDir.z;
	float t6 = ((bb.pos.z + bb.ext.z) - ray.origin.z) * invDir.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	//Ray is intersecting AABB, but whole AABB is behind us
	if (tmax < 0)
	{
		return false;
	}

	//Ray doesn't intersect AABB
	if (tmin > tmax)
	{
		return false;
	}

	//Return intersection true and distance to model
	distance = tmin;
	return true;
#pragma endregion v3
}

bool GraphicsHandler::PointVSAABB(DirectX::XMFLOAT3 pos, Camera::C_AABB bb)
{
	if (pos.x < bb.pos.x - bb.ext.x)
		return false;
	if (pos.x > bb.pos.x + bb.ext.x)
		return false;
	if (pos.y < bb.pos.y - bb.ext.y)
		return false;
	if (pos.y > bb.pos.y + bb.ext.y)
		return false;
	if (pos.z < bb.pos.z - bb.ext.z)
		return false;
	if (pos.z > bb.pos.z + bb.ext.z)
		return false;

	return true;
}

void GraphicsHandler::DeleteOctree(OctreeNode * curNode)
{
	for (int i = 0; i < 8; i++)
	{
		if (curNode->branches[i] != nullptr)
		{
			this->DeleteOctree(curNode->branches[i]);
			delete curNode->branches[i];
			curNode->branches[i] = nullptr;
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

inline OBB GraphicsHandler::m_ConvertOBB(BoundingBoxHeader & boundingBox) //Convert from BBheader to OBB struct										
{
	OBB obj;

	obj.ext[0] = boundingBox.extension[0];
	obj.ext[1] = boundingBox.extension[1];
	obj.ext[2] = boundingBox.extension[2];


	DirectX::XMMATRIX extensionMatrix;
	extensionMatrix = DirectX::XMMatrixSet(
		boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
		boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
		boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	obj.ort = extensionMatrix;


	return obj;
}

float GraphicsHandler::Ping_GetDistanceToClosestOBB(float maxDistance)
{
	std::vector<Camera::C_OBB> OBBs;

	Camera::C_Ray ray = this->m_camera->CastRayFromMaxDistance();

	//Change dir of the ray
	//ray.dir = DirectX::XMFLOAT3(ray.dir.x * -1, ray.dir.y * -1, ray.dir.z * -1);
	
	//Cast a ray that sets hited OctreeBV isInPingRay to true
	for (size_t i = 0; i < 8; i++)
	{
		this->TraverseOctreeRay(this->m_octreeRoot.branches[i], ray, true);
	}

	//Search for the results
	for (OctreeBV* i : this->m_octreeRoot.containedComponents)
	{
		if (i->isInPingRay)
		{
			//Create the OBB
			DirectX::XMMATRIX ortm;
			DirectX::XMFLOAT4X4 ort;
			memcpy(&ortm.r[0], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[0], sizeof(float) * 3);
			memcpy(&ortm.r[1], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[1], sizeof(float) * 3);
			memcpy(&ortm.r[2], &this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData().extensionDir[2], sizeof(float) * 3);

			DirectX::XMStoreFloat4x4(&ort, ortm);

			Camera::C_OBB obb;
			obb.ort = ort;
			obb.ext = DirectX::XMFLOAT3(m_ConvertOBB(this->m_staticGraphicsComponents[i->componentIndex].modelPtr->GetOBBData()).ext);
			obb.pos = i->pos;

			OBBs.push_back(obb);	//Push to the list of OBBs
			i->isInPingRay = false;
		}
	}

	//Check distance to all marked OBBs
	const float EPSILON = 1e-5f;
	float targetDistance = maxDistance;	//Max distance
	float intersectDistance = maxDistance + 0.3f;	//Closest distance
	float hitDistance = maxDistance;	//Current hit distance

	for (Camera::C_OBB i : OBBs)
	{
		OBB obb;
		obb.ext[0] = i.ext.x;
		obb.ext[1] = i.ext.y;
		obb.ext[2] = i.ext.z;

		obb.ort = DirectX::XMLoadFloat4x4(&i.ort);

		if (this->m_camera->m_IntersectRayOBB(this->m_camera->GetMaxDistanceCamPos(), this->m_camera->GetDirection(), obb, DirectX::XMLoadFloat3(&i.pos), hitDistance))
		{
			if (hitDistance < intersectDistance && fabs(hitDistance - targetDistance) > EPSILON)
			{
				intersectDistance = hitDistance;
			}
		}
	}


	return intersectDistance;
}
