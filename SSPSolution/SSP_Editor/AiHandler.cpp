#include "AiHandler.h"



AiHandler::AiHandler()
{
}


AiHandler::~AiHandler()
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		delete m_Components.at(i);
	}
}

AiContainer * AiHandler::NewPathComponent()
{
	m_Components.push_back(new AiContainer());
	m_Components.back()->aiComponent.AC_pattern = Pattern::AI_ONEWAY;
	return m_Components.back();
}

AiContainer * AiHandler::GetPathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->aiComponent.AC_entityID == EntityID) {
			return m_Components.at(EntityID);
		}
	}
	return nullptr;
}

std::vector<AiContainer*>* AiHandler::GetAllPathComponents()
{
	return &m_Components;
}

void AiHandler::DeletePathComponent(int EntityID)
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (m_Components.at(i)->aiComponent.AC_entityID == EntityID) {
			delete m_Components.at(i);
			m_Components.erase(m_Components.begin() + i);
			return;
		}
	}
	

}

void AiHandler::UpdatePathComponent(int entityID , DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{

	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (entityID != m_Components.at(i)->internalID)
			continue;
		m_Components.at(i)->position = position;
		m_Components.at(i)->rotation = rotation;
		DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();

		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.m128_f32[2]));
		//Create the rotation matrix
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);

		containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
		containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
		m_Components.at(i)->component.worldMatrix = containerMatrix;
		m_Components.at(i)->isDirty = false;
		return;
	}

}

void AiHandler::Destroy()
{
	m_Components.clear();
}


