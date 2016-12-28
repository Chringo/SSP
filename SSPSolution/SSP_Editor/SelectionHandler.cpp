#include "SelectionHandler.h"

SelectionHandler::SelectionHandler()
{
}

SelectionHandler::~SelectionHandler()
{
}

SelectionHandler * SelectionHandler::GetInstance()
{
	static SelectionHandler selectionHandler;

	return &selectionHandler;
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
			this->transformWidget.axisOBB[i], this->transformWidget.axisOBB[i].pos);
		if (result)
		{
			transformWidget.SelectAxis(i);
			break;
		}
		else
		{
			transformWidget.SelectAxis(TransformWidget::NONE);
		}
	}

	return result;
}

bool SelectionHandler::PickObjectSelection()
{
	bool gotHit = false;

	//checks if we picked on a model by iterating
	std::unordered_map<unsigned int, std::vector<Container>>* m_Map = m_currentLevel->GetModelEntities();
	if (!m_Map->empty())
	{
		BoundingBoxHeader boundingBox;
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
					boundingBox = m_modelPtr->at(i)->GetOBBData();
					OBB obj;
					obj.ext[0] = boundingBox.extension[0];
					obj.ext[1] = boundingBox.extension[1];
					obj.ext[2] = boundingBox.extension[2];

					obj.pos = DirectX::XMVectorSet(
						InstancePtr->at(j).position.m128_f32[0],
						InstancePtr->at(j).position.m128_f32[1],
						InstancePtr->at(j).position.m128_f32[2],
						1.0f);


					DirectX::XMMATRIX extensionMatrix;
					extensionMatrix = DirectX::XMMatrixSet(
						boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
						boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
						boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);

					obj.ort = extensionMatrix;

					//OBB obj = *(OBB*)&boundingBox;
					//DONT FORGET TO MULTIPLY MATRIX
					DirectX::XMMATRIX tempWorld = InstancePtr->at(j).component.worldMatrix;
					DirectX::XMMATRIX finalOrt = DirectX::XMMatrixMultiply(extensionMatrix, tempWorld);
					obj.ort = finalOrt;

					bool result = false;
					/*PICKING HERE NEEDS DISTANCE CHECK*/
					result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, InstancePtr->at(j).position);
					//transformWidget.setActive(result);
					if (result)
					{
						


						//update widget with the intersected obb
						this->transformWidget.Select(obj, &InstancePtr->at(j), i, j, m_modelPtr->at(i)->GetId());


						gotHit = result;
					}

				}
				if (!gotHit)
				{
					this->transformWidget.DeSelect();
				}
			}
		}
	}
	return gotHit;

	//return true;
}