#include "Level.h"



Level::Level()
{
	m_ModelMap.reserve(50);

	Container player1;
	Container player2;
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got;



		player1.internalID = 0;
		player1.position = { 1.0f, 0.0, 0.0f };
		player1.rotation = { 0.0f, 0.0f, 0.0f };
		player1.isDirty = true;
		player1.component.worldMatrix = DirectX::XMMatrixIdentity();
		player1.component.modelID = PLAYER1;
		this->m_ModelMap[PLAYER1].push_back(player1);
		this->m_uniqueModels.push_back(PLAYER1);
	
		player2.internalID = 1;
		player2.position = { -1.0f, 0.0, 0.0f };
		player2.rotation = { 0.0f, 0.0f, 0.0f };
		player2.isDirty = true;
		player2.component.worldMatrix = DirectX::XMMatrixIdentity();
		player2.component.modelID = PLAYER2;
		this->m_ModelMap[PLAYER2].push_back(player2);
		this->m_uniqueModels.push_back(PLAYER2);
	
}


Level::~Level()
{
}

std::unordered_map<unsigned int, std::vector<Container>>* Level::GetModelEntities()
{
	return &m_ModelMap;
}

std::unordered_map<unsigned int, std::vector<Container>>* Level::GetLights()
{
	return &m_LightMap;
}

Resources::Status Level::GetModelEntity(unsigned int modelID, unsigned int instanceID, Container & container)
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;


	if (got == m_ModelMap.end()) { // if  does not exists in memory
		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;

		for (size_t i = 0; i < modelPtr->size(); i++)
		{
			if (modelPtr->at(i).internalID == instanceID)
			{
				container = modelPtr->at(i);
				return Resources::Status::ST_OK;
			}
		}
		return Resources::Status::ST_RES_MISSING;
	}
}

Resources::Status Level::AddModelEntity(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation) // Author : Johan Ganeteg
{
	if (modelID == PLAYER1 || modelID == PLAYER2)
	{
		switch (modelID)
		{
		case PLAYER1:
			this->m_ModelMap[modelID].at(0).position = { 1.0f, 0.0f, 0.0f };
			this->m_ModelMap[modelID].at(0).rotation = { 0.0f, 0.0f, 0.0f };
			this->m_ModelMap[modelID].at(0).isDirty = true;
			break;
		case PLAYER2:
			this->m_ModelMap[modelID].at(0).position = { -1.0f, 0.0f, 0.0f };
			this->m_ModelMap[modelID].at(0).rotation = { 0.0f, 0.0f, 0.0f };
			this->m_ModelMap[modelID].at(0).isDirty = true;
			break;
		default:
			break;
		}
		//SelectionHandler::GetInstance()->SetSelection(false);
		return Resources::Status::ST_OK;
	}
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;



	Container newComponent;

	newComponent.component.modelID = modelID;
	newComponent.position = position;
	newComponent.rotation = rotation;
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newComponent.component.worldMatrix = containerMatrix;
	newComponent.internalID = instanceID;
	newComponent.isDirty = false;



	if (got == m_ModelMap.end()) { // if  does not exists in memory
		this->m_ModelMap[modelID].push_back(newComponent);
		this->m_uniqueModels.push_back(modelID);
		return Resources::Status::ST_OK;
	}
	else {
		modelPtr = &got->second;
		modelPtr->push_back(newComponent);
		return Resources::Status::ST_OK;
	}
	
}

Resources::Status Level::UpdateModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation) // Author : Johan Ganeteg
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;

	if (got == m_ModelMap.end()) { // if  does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;

		for (size_t i = 0; i < modelPtr->size(); i++)
		{
			if (i == instanceID)
			{
				modelPtr->at(i).position = position;
				modelPtr->at(i).rotation = rotation;
				DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();
				DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);
				//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
				containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
				containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
				modelPtr->at(i).component.worldMatrix = containerMatrix;
				modelPtr->at(i).isDirty = false;
				return Resources::Status::ST_OK;
			}

		}
		return Resources::Status::ST_RES_MISSING;
	}
}

Resources::Status Level::RemoveModel(unsigned int modelID, unsigned int instanceID) // Author : Johan Ganeteg
{
	if (modelID == PLAYER1 || modelID == PLAYER2)
		return Resources::Status::ST_OK;

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;

	if (got == m_ModelMap.end()) { // if  does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		modelPtr = &got->second;
		modelPtr->erase(modelPtr->begin() + instanceID);
		return Resources::Status::ST_OK;
	}
}

bool Level::isEmpty()
{
	if (this->GetNumEntities() == 0) {
		return true;
	}
	return false;
}

unsigned int Level::GetNumEntities()
{
	unsigned int num = 0;

	for (auto iterator = m_ModelMap.begin(); iterator != m_ModelMap.end(); ++iterator)
	{
		std::vector<Container> * vector = &iterator->second;
		num += vector->size();
	}
	return num;
}

unsigned int Level::GetNumLights()
{
	return 0;
}

void Level::Destroy()
{
	m_uniqueModels.clear();
	m_ModelMap.clear();
	m_LightMap.clear();
	levelName = "untitled_level";
}
