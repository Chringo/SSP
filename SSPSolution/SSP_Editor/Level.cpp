#include "Level.h"
#include "AIController.h"


Level::Level()
{
	m_ModelMap.reserve(50);

		m_SpawnPoints[0].internalID = 0;
		m_SpawnPoints[0].position   = { 1.0f, 0.0, 0.0f };
		m_SpawnPoints[0].rotation   = { 0.0f, 0.0f, 0.0f };
		m_SpawnPoints[0].isDirty    = true;
		m_SpawnPoints[0].component.worldMatrix = DirectX::XMMatrixTranslationFromVector(m_SpawnPoints[0].position);
		m_SpawnPoints[0].component.modelID  = PLAYER1;
		m_SpawnPoints[0].component.modelPtr = DataHandler::GetInstance()->GetModel(m_SpawnPoints[0].component.modelID);
	
		m_SpawnPoints[1].internalID = 1;
		m_SpawnPoints[1].position   = { -1.0f, 0.0, 0.0f };
		m_SpawnPoints[1].rotation   = { 0.0f, 0.0f, 0.0f };
		m_SpawnPoints[1].isDirty    = true;
		m_SpawnPoints[1].component.worldMatrix = DirectX::XMMatrixTranslationFromVector(m_SpawnPoints[1].position);
		m_SpawnPoints[1].component.modelID     = PLAYER2;
		m_SpawnPoints[1].component.modelPtr = DataHandler::GetInstance()->GetModel(m_SpawnPoints[1].component.modelID);
		
		
	//AIController cont(m_LevelAi.NewPathComponent());			   //TEMP
	//cont.AddWaypoint( { 1.0f,0.0f,0.0f });						   //TEMP
	//cont.AddWaypoint({ 5.0f,0.0f,0.0f });						   //TEMP
	//cont.AddWaypoint({ 5.0f,5.0f,0.0f });						   //TEMP
	//cont.AddWaypoint({ 5.0f,5.0f,5.0f });						   //TEMP
	//cont.AddWaypoint({ 5.0f,0.0f,5.0f });						   //TEMP
	//cont.AddWaypoint({ 3.0f,0.0f,5.0f });						   //TEMP
	//cont.AddWaypoint({ 1.0f,5.0f,5.0f });						   //TEMP
	//cont.AddWaypoint({ 0.0f,-5.0f,0.0f });
	
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

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;

	Container newComponent;

	newComponent.component.modelID  = modelID;
	newComponent.position		    = position;
	newComponent.rotation		    = rotation;
	newComponent.component.modelPtr = DataHandler::GetInstance()->GetModel(modelID);
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newComponent.component.worldMatrix = containerMatrix;
	newComponent.internalID = instanceID;
	newComponent.isDirty = true;

	if (got == m_ModelMap.end()) { // if does not exists in memory
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

				DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.m128_f32[0]));
				DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.m128_f32[1]));
				DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.m128_f32[2]));
				//Create the rotation matrix
				DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
				rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);

				//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);
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

Resources::Status Level::UpdateSpawnPoint(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{

	if (instanceID != 0 && instanceID != 1)
		return Resources::Status::ST_RES_MISSING;

	m_SpawnPoints[instanceID].position = position;
	m_SpawnPoints[instanceID].rotation = rotation;
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.m128_f32[0]));
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.m128_f32[1]));
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.m128_f32[2]));
	//Create the rotation matrix
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
	rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);

	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	m_SpawnPoints[instanceID].component.worldMatrix = containerMatrix;
	m_SpawnPoints[instanceID].isDirty = false;
	return Resources::Status::ST_OK;


	return Resources::Status::ST_OK;
}

Resources::Status Level::RemoveModel(unsigned int modelID, unsigned int instanceID) // Author : Johan Ganeteg
{
	//if (modelID == PLAYER1 || modelID == PLAYER2)
	//	return Resources::Status::ST_OK;

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

Resources::Status Level::DuplicateEntity( Container *& source, Container*& destination)
{

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(source->component.modelID);
	std::vector<Container>* modelPtr;

	if (got == m_ModelMap.end()) { // if  does not exists in memory
		return Resources::Status::ST_RES_MISSING;
	}
	else {
		Container temp = *source;
		temp.component.modelPtr = source->component.modelPtr;
		modelPtr = &got->second;
		temp.internalID = modelPtr->size();
		modelPtr->push_back(temp);
		destination = &modelPtr->back();
		//SelectionHandler::GetInstance()->SetSelectedContainer()
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

Container * Level::GetSpawnPoint(int index)
{

	for (size_t i = 0; i < 2; i++)
	{
		if (m_SpawnPoints[i].component.modelPtr == nullptr) // This fixes the error that the spawnpoints did not get their right models
			m_SpawnPoints[i].component.modelPtr = DataHandler::GetInstance()->GetModel(m_SpawnPoints[i].component.modelID);
	}

	if     (index > 1)
		return &m_SpawnPoints[1];
	else if( index < 0)
		return &m_SpawnPoints[0];
	else
		return &m_SpawnPoints[index];
	
}

void Level::Destroy()
{
	m_uniqueModels.clear();
	m_ModelMap.clear();
	m_LightMap.clear();
	levelName = "untitled_level";

	m_SpawnPoints[0].position = { 1.0f, 0.0, 0.0f };
	m_SpawnPoints[0].rotation = { 0.0f, 0.0f, 0.0f };
	m_SpawnPoints[0].isDirty = true;

	m_SpawnPoints[1].position = { -1.0f, 0.0, 0.0f };
	m_SpawnPoints[1].rotation = { 0.0f, 0.0f, 0.0f };
	m_SpawnPoints[1].isDirty = true;

	m_LevelAi.Destroy();
	//Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->Deselect();
}

void Level::SetSpawnPoint(LevelData::SpawnHeader data, int index)
{

	if (index != 0 && index != 1)
		return;

	m_SpawnPoints[index].position = DirectX::XMVectorSet(data.position[0], data.position[1], data.position[2], 0.0);
	m_SpawnPoints[index].rotation = DirectX::XMVectorSet(data.rotation[0], data.rotation[1], data.rotation[2], 0.0);
	m_SpawnPoints[index].isDirty = true;
}
