#include "SelectionHandler.h"

SelectionHandler::SelectionHandler()
{
}

SelectionHandler::~SelectionHandler()
{
}

void SelectionHandler::Initialize(Camera * camera,
	int winWidth, 
	int winHeight,
	Level * currentLevel,
	std::vector<Resources::Model*>* modelPtr)
{
	this->m_Camera = camera;
	this->m_Width = winWidth;
	this->m_Height = winHeight;
	this->m_currentLevel = currentLevel;
	this->m_modelPtr = modelPtr;
	this->m_attributesHandler = Ui::UiControlHandler::GetInstance()->GetAttributesHandler();
	this->m_ray.direction = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_ray.origin = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_ray.localOrigin = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_checkpointPtr = currentLevel->GetCheckpoints();
	this->m_lightPtr = currentLevel->GetLights();

}

void SelectionHandler::updateWindowSize(int winHeight, int winWidth)
{
	this->m_Height = winHeight;
	this->m_Width = winWidth;
}

SelectionHandler * SelectionHandler::GetInstance()
{
	static SelectionHandler selectionHandler;

	return &selectionHandler;
}

void SelectionHandler::Update()
{
	m_transformWidget.UpdateOBB();
	m_IsDirty = false;
	
}

bool SelectionHandler::NeedsUpdate()
{
	return m_IsDirty;
}


Container * SelectionHandler::GetSelected()
{
	return this->m_transformWidget.GetContainer();
}

bool SelectionHandler::HasSelection()
{
	return m_transformWidget.IsActive();
}

void SelectionHandler::SetSelection(bool selection)
{
	this->m_transformWidget.setActive(selection);
	if (selection == false)
		m_attributesHandler->Deselect();
}

void SelectionHandler::SetSelectedContainer(Container *& selection)
{
	OBB box = this->m_ConvertOBB(selection->component.modelPtr->GetOBBData(), selection);
	
	this->m_transformWidget.Select(box, selection, selection->internalID, selection->component.modelID);
	m_attributesHandler->SetSelection(selection);
	//m_transformWidget.Select()
}

void SelectionHandler::SetActiveAxis(int axis)
{
	this->m_transformWidget.SelectAxis(axis);
}


const unsigned int SelectionHandler::GetModelID()
{
	return m_transformWidget.GetModelID();
}

const unsigned int SelectionHandler::GetInstanceID()
{
	return m_transformWidget.GetInstanceID();
}

void SelectionHandler::GetSelectionRenderComponents(
	OBB*& axisOBBs,
	DirectX::XMVECTOR*& axisOBBpos,
	DirectX::XMVECTOR**& axisColors,
	OBB*& objectOBB,
	/*DirectX::XMVECTOR*& objectOBBpos,*/
	DirectX::XMVECTOR*& objectColor) 
{
	axisOBBs = this->m_transformWidget.GetAxisOBBs();
	axisColors = this->m_transformWidget.GetAxisColors();
	axisOBBpos = this->m_transformWidget.GetAxisOBBpositons();

	objectOBB = this->m_transformWidget.GetSelectedObjectOBB();
	objectColor = this->m_transformWidget.GetSelectedObjectOBBColor();
}

void SelectionHandler::ProjectRay(int X, int Y)
{
	DirectX::XMMATRIX inverseCamView;
	this->m_Camera->GetViewMatrix(inverseCamView);
	inverseCamView = DirectX::XMMatrixInverse(nullptr, inverseCamView);

	this->m_ray.direction = DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(
		DirectX::XMVectorSet(
		(((2.0f * X) / m_Width) - 1) / this->m_Camera->GetProjectionMatrix()->_11,
			-(((2.0f * Y) / m_Height) - 1) / this->m_Camera->GetProjectionMatrix()->_22,
			1.0f,
			0.0f),
		inverseCamView));

	this->m_ray.localOrigin = DirectX::XMLoadFloat3(&this->m_Camera->GetCameraPos());

	this->m_ray.origin = DirectX::XMVector3TransformCoord(this->m_ray.localOrigin, inverseCamView);
}

bool SelectionHandler::PickTransformWidget()
{
	bool result;

	for (int i = 0; i < TransformWidget::NUM_AXIS; i++)
	{
		result = this->m_PhysicsHandler->IntersectRayOBB(
			this->m_ray.localOrigin, this->m_ray.direction,
			this->m_transformWidget.GetAxisOBBs()[i], this->m_transformWidget.GetAxisOBBpositons()[i]);
		if (result)
		{
			m_transformWidget.SelectAxis(i);
			break;
		}
		else
		{
			m_transformWidget.SelectAxis(TransformWidget::NONE);
		}
	}

	return result;
}

bool SelectionHandler::PickObjectSelection()
{
	bool gotHit = false;
	float hitDistance = FLT_MAX;
	float minHitDistance = FLT_MAX;



	//checks if we picked on a model by iterating
	std::unordered_map<unsigned int, std::vector<Container>>* m_Map = m_currentLevel->GetModelEntities();
	if (!m_Map->empty())
	{
		Resources::Status st;
		std::vector<Container>* InstancePtr = nullptr;
		for (size_t i = 0; i < m_modelPtr->size(); i++)
		{
			std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map->find(m_modelPtr->at(i)->GetId());

			if (got == m_Map->end()) { // if  does not exists in memory
				continue;
			}
			else {
				InstancePtr = &got->second;
				for (size_t j = 0; j < InstancePtr->size(); j++)
				{
					OBB obj = m_ConvertOBB(m_modelPtr->at(i)->GetOBBData(), &InstancePtr->at(j));

					bool result = false;
					
					/*PICKING HERE NEEDS DISTANCE CHECK*/
					//result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, InstancePtr->at(j).position, hitDistance);

					DirectX::XMVECTOR OBBPosition;
					OBBPosition.m128_f32[0] = m_modelPtr->at(i)->GetOBBData().position.x;
					OBBPosition.m128_f32[1] = m_modelPtr->at(i)->GetOBBData().position.y;
					OBBPosition.m128_f32[2] = m_modelPtr->at(i)->GetOBBData().position.z;

					DirectX::XMMATRIX tempOBBPos = DirectX::XMMatrixTranslationFromVector(OBBPosition);

					tempOBBPos = tempOBBPos * InstancePtr->at(j).component.worldMatrix;

					//result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, *this->m_transformWidget.GetOBBCenterPostition(), hitDistance);
					result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, tempOBBPos.r[3], hitDistance);
					//transformWidget.setActive(result);
					if (result && hitDistance < minHitDistance)
					{
						if (HasSelection())
						{
							if (this->m_transformWidget.GetModelID() != InstancePtr->at(j).component.modelID)
							{
								this->m_transformWidget.SetOBBCenterPosition(DirectX::XMVECTOR{
									m_modelPtr->at(i)->GetOBBData().position.x, m_modelPtr->at(i)->GetOBBData().position.y,
									m_modelPtr->at(i)->GetOBBData().position.z });
							}
						}
						else
						{
							this->m_transformWidget.SetOBBCenterPosition(DirectX::XMVECTOR{
								m_modelPtr->at(i)->GetOBBData().position.x, m_modelPtr->at(i)->GetOBBData().position.y,
								m_modelPtr->at(i)->GetOBBData().position.z });
						}

						minHitDistance = hitDistance;
						//update widget with the intersected obb
						Container* cont = &InstancePtr->at(j);
						this->m_transformWidget.Select(obj, cont, InstancePtr->at(j).internalID, m_modelPtr->at(i)->GetId());
						Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(cont);

						gotHit = result;
					}

				}
				if (!gotHit)
				{

					//this->transformWidget.DeSelect();
				}
			}
		}
	}

	for each (Point* light in *m_lightPtr)
	{
		bool result = false;
		//result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, container->obb, container->position, hitDistance);
		result = this->m_PhysicsHandler->IntersectRaySphere(m_ray.localOrigin, this->m_ray.direction, light->pickSphere, light->position, hitDistance);
		if (result && hitDistance < minHitDistance)
		{
			minHitDistance = hitDistance;
			//update widget with the intersected obb
			this->m_transformWidget.Select(light->pickSphere, light); //OVERLOAD AND HANLDE THIS
			Container* cont = (Container*)light;
			Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(cont);

			gotHit = result;
		}
	}

	//check the checkpoints
	for each (CheckpointContainer* container in *m_checkpointPtr)
	{

		bool result = false;
		result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, container->obb, container->position, hitDistance);
		if (result && hitDistance < minHitDistance)
		{
			minHitDistance = hitDistance;
			//update widget with the intersected obb
			this->m_transformWidget.Select(container->obb, container); //OVERLOAD AND HANLDE THIS
			Container* cont = (Container*)container;
			Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(cont);

			gotHit = result;
		}
	}

	//check the puzzle elements
	for (size_t i = 0; i < ContainerType::NUM_PUZZLE_ELEMENTS; i++)
	{
		const std::vector<Container*>* cont = m_currentLevel->GetPuzzleElements(ContainerType(i));
		for (size_t j = 0; j < cont->size(); j++)
		{
			OBB obj = m_ConvertOBB(cont->at(j)->component.modelPtr->GetOBBData(), cont->at(j));
			bool result = false;
			result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, cont->at(j)->position, hitDistance);
			if (result && hitDistance < minHitDistance)
			{
				minHitDistance = hitDistance;
				//update widget with the intersected obb
				Container* ptr = (Container*)cont->at(j);
				
				this->m_transformWidget.Select(obj, ptr, i, cont->at(j)->component.modelPtr->GetId());
				Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(ptr);

				gotHit = result;
			}
			
		}

	}
	

	//check the spawnPoints
	for (size_t i = 0; i < 2; i++)
	{
		Container* spawn = m_currentLevel->GetSpawnPoint(i);
		OBB obj = m_ConvertOBB(spawn->component.modelPtr->GetOBBData(), spawn);

		bool result = false;
		result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, spawn->position, hitDistance);
		//transformWidget.setActive(result);
		if (result && hitDistance < minHitDistance)
		{
			
			minHitDistance = hitDistance;
			//update widget with the intersected obb
			this->m_transformWidget.Select(obj, spawn, i, spawn->component.modelPtr->GetId());
			Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(spawn);

			gotHit = result;
		}
	}
	std::vector<AiContainer*>* container = m_currentLevel->GetAiHandler()->GetAllPathComponents();
	for (size_t i = 0; i < container->size(); i++)
	{
		Container* ai = (Container*) container->at(i);
		OBB obj = m_ConvertOBB(ai->component.modelPtr->GetOBBData(), ai);

		bool result = false;
		result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, ai->position, hitDistance);
		//transformWidget.setActive(result);
		if (result && hitDistance < minHitDistance)
		{
			minHitDistance = hitDistance;
			//update widget with the intersected obb
			this->m_transformWidget.Select(obj, ai, i, ai->component.modelPtr->GetId());
			Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->SetSelection(ai);

			gotHit = result;
		}

	}

	return gotHit;

	//return true;
}

void SelectionHandler::MoveObject(bool noSnap)
{
	if (m_transformWidget.IsActive() && m_transformWidget.GetSelectedAxis() != TransformWidget::NONE)
	{
		Container * instance = m_transformWidget.GetContainer();
		static const DirectX::XMVECTOR normals[TransformWidget::NUM_AXIS] = { { 1.0f,0.0f,0.0f },{ 0.0f,1.0f,0.0f }, { 0.0f,0.0f,1.0f } };
		DirectX::XMVECTOR planes[TransformWidget::NUM_AXIS];
		float t[TransformWidget::NUM_AXIS];
		DirectX::XMVECTOR P[TransformWidget::NUM_AXIS];

		for (int i = 0; i < TransformWidget::NUM_AXIS; i++)
		{
			planes[i] = DirectX::XMPlaneFromPointNormal(m_transformWidget.GetAxisOBBpositons()[m_transformWidget.GetSelectedAxis()], normals[i]);
			t[i] = -((DirectX::XMVector3Dot(m_ray.localOrigin, normals[i]).m128_f32[0] + planes[i].m128_f32[3]) / DirectX::XMVector3Dot(m_ray.direction, normals[i]).m128_f32[0]);
			P[i] = DirectX::XMVectorAdd(m_ray.localOrigin, DirectX::XMVectorScale(m_ray.direction, t[i]));
		}

		
		float d = 0.0f;
		int planeAxis;
		for (int i = 0; i < TransformWidget::NUM_AXIS; i++)
		{
			if (i != m_transformWidget.GetSelectedAxis())
			{
				float dot = abs(DirectX::XMPlaneDotNormal(planes[i], m_ray.direction).m128_f32[0]);

				if (d < dot && i)
				{
					d = dot;
					planeAxis = i;
				}
			}
		}


		DirectX::XMVECTOR Diff = DirectX::XMVectorSubtract(P[planeAxis], m_transformWidget.GetAxisOBBpositons()[m_transformWidget.GetSelectedAxis()]);


		
		if (noSnap)//snap
		{
			instance->position.m128_f32[m_transformWidget.GetSelectedAxis()] =
				DirectX::XMVectorAdd(instance->position, Diff).m128_f32[m_transformWidget.GetSelectedAxis()];
			
			/*this->m_transformWidget.GetOBBCenterPostition()->m128_f32[m_transformWidget.GetSelectedAxis()] =
				DirectX::XMVectorAdd(*m_transformWidget.GetOBBCenterPostition(), Diff).m128_f32[m_transformWidget.GetSelectedAxis()];*/
		}
		else//snap
		{
			instance->position.m128_f32[m_transformWidget.GetSelectedAxis()] = (int)instance->position.m128_f32[m_transformWidget.GetSelectedAxis()];

			if (Diff.m128_f32[m_transformWidget.GetSelectedAxis()] > 1.0)
			{
				instance->position.m128_f32[m_transformWidget.GetSelectedAxis()] += 1.0f;
				/*m_transformWidget.GetOBBCenterPostition()->m128_f32[m_transformWidget.GetSelectedAxis()] += 1.0f;*/
			}
			else if (Diff.m128_f32[m_transformWidget.GetSelectedAxis()] < -1.0)
			{
				instance->position.m128_f32[m_transformWidget.GetSelectedAxis()] -= 1.0f;
				/*m_transformWidget.GetOBBCenterPostition()->m128_f32[m_transformWidget.GetSelectedAxis()] -= 1.0f;*/
			}
		}

		//this->m_transformWidget.SetOBBCenterPosition(DirectX::XMVectorAdd(*this->m_transformWidget.GetOBBCenterPostition()));
		//flag instance for update
		m_IsDirty = true;
		instance->isDirty = true;
		Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->UpdateSelection();
		
	}

}

void SelectionHandler::RotateObject(int direction)
{
	if (m_transformWidget.IsActive())
	{
		Container * instance = m_transformWidget.GetContainer();


		DirectX::XMVECTOR rotation;
		float angle = 45.f;//DirectX::XMConvertToRadians(45.f);


		switch (direction)
		{
		case (Key_Up):
			//rotation = DirectX::XMQuaternionRotationNormal({ 1.0f,0.0f,0.0f }, angle);
			instance->rotation.m128_f32[0] += angle;
			break;
		case (Key_Down):
			//rotation = DirectX::XMQuaternionRotationNormal({ 1.0f,0.0f,0.0f }, -angle);
			instance->rotation.m128_f32[0] -= angle;
			break;
		case (Key_Left):
			//rotation = DirectX::XMQuaternionRotationNormal({ 0.0f,1.0f,0.0f }, angle);
			instance->rotation.m128_f32[1] += angle;
			break;
		case (Key_Right):
			//rotation = DirectX::XMQuaternionRotationNormal({ 0.0f,1.0f,0.0f }, -angle);
			instance->rotation.m128_f32[1] -= angle;
			break;
		case (Key_0):
			instance->rotation = DirectX::XMQuaternionIdentity();
			m_IsDirty = true;
			instance->isDirty = true;
			return;
		default:
			break;
		}

		//if (DirectX::XMVector3Length(instance->rotation).m128_f32[0] < 0.01f)
		//	instance->rotation = rotation;
		//else
			//instance->rotation = rotation; //DirectX::XMQuaternionMultiply(instance->rotation, rotation);

		m_IsDirty = true;
		instance->isDirty = true;
	}

}

OBB SelectionHandler::m_ConvertOBB(BoundingBoxHeader & boundingBox, Container * instancePtr)
{
	OBB obj;

	obj.ext[0] = boundingBox.extension[0];
	obj.ext[1] = boundingBox.extension[1];
	obj.ext[2] = boundingBox.extension[2];

	//obj.pos = DirectX::XMVectorSet(
	//	instancePtr->position.m128_f32[0],
	//	instancePtr->position.m128_f32[1],
	//	instancePtr->position.m128_f32[2],
	//	1.0f);
	

	/*DirectX::XMMATRIX tempPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{
		boundingBox.position.x, boundingBox.position.y,
		boundingBox.position.z });

	tempPos = tempPos * instancePtr->component.worldMatrix;
	this->m_transformWidget.SetOBBCenterPosition(tempPos.r[3]);*/

	/*DirectX::XMMATRIX tempPos = DirectX::XMMatrixTranslationFromVector(DirectX::XMVECTOR{
		boundingBox.position.x, boundingBox.position.y,
		boundingBox.position.z });*/

	/*tempPos = tempPos * instancePtr->component.worldMatrix;
	this->m_transformWidget.SetOBBCenterPosition(tempPos.r[3]);*/
	/*this->m_transformWidget.SetOBBCenterPosition(DirectX::XMVECTOR{
		boundingBox.position.x, boundingBox.position.y, boundingBox.position.z });*/
	/*this->m_transformWidget.SetOBBCenterPosition(DirectX::XMVectorAdd(DirectX::XMVECTOR{
		boundingBox.position.x, boundingBox.position.y, boundingBox.position.z }, instancePtr->position));*/



	DirectX::XMMATRIX extensionMatrix;
	extensionMatrix = DirectX::XMMatrixSet(
		boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
		boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
		boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	obj.ort = extensionMatrix;

	//into worldspace we go!
	DirectX::XMMATRIX tempWorld = instancePtr->component.worldMatrix;
	DirectX::XMMATRIX finalOrt = DirectX::XMMatrixMultiply(extensionMatrix, tempWorld);
	obj.ort = finalOrt;

	return obj;
}