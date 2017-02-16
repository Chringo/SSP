#include "Level.h"
#include "AIController.h"


Level::Level()
{
	m_ModelMap.reserve(50);

		m_SpawnPoints[0].internalID = 0;
		m_SpawnPoints[0].position   = { 1.0f, 0.0, 0.0f };
		m_SpawnPoints[0].rotation   = { 0.0f, 0.0f, 0.0f };
		m_SpawnPoints[0].isDirty    = true;
		m_SpawnPoints[0].component.worldMatrix  = DirectX::XMMatrixTranslationFromVector(m_SpawnPoints[0].position);
		m_SpawnPoints[0].component.modelID		= PLAYER1;
		m_SpawnPoints[0].component.modelPtr		= DataHandler::GetInstance()->GetModel(m_SpawnPoints[0].component.modelID);
	
		m_SpawnPoints[1].internalID = 1;
		m_SpawnPoints[1].position   = { -1.0f, 0.0, 0.0f };
		m_SpawnPoints[1].rotation   = { 0.0f, 0.0f, 0.0f };
		m_SpawnPoints[1].isDirty    = true;
		m_SpawnPoints[1].component.worldMatrix = DirectX::XMMatrixTranslationFromVector(m_SpawnPoints[1].position);
		m_SpawnPoints[1].component.modelID     = PLAYER2;
		m_SpawnPoints[1].component.modelPtr	    = DataHandler::GetInstance()->GetModel(m_SpawnPoints[1].component.modelID);
		
		for (size_t i = 0; i < NUM_PUZZLE_ELEMENTS; i++)
		{
			this->m_puzzleElements.push_back(std::vector<Container*>());
		}
		
	
}


Level::~Level()
{
	this->Destroy();
}

std::unordered_map<unsigned int, std::vector<Container>>* Level::GetModelEntities()
{
	return &m_ModelMap;
}

std::vector<Light*>* Level::GetLights()
{
	return LightController::GetInstance()->GetLights();
}

std::vector<CheckpointContainer*>* Level::GetCheckpoints()
{
	return m_checkpointHandler.GetAllCheckpoints();
}

Container * Level::GetInstanceEntity(unsigned int entityID)
{
	for (auto iterator = m_ModelMap.begin(); iterator != m_ModelMap.end(); ++iterator)
	{
		std::vector<Container> * cont = &iterator->second;
		for (size_t i = 0; i < cont->size(); i++)
		{
			if (cont->at(i).internalID == entityID)
			{
				return &cont->at(i);
			}
		}
	}

	for (size_t i = 0; i < m_puzzleElements.size(); i++)
	{
		for each (Container* container in m_puzzleElements.at(i)) {
			if (container->internalID == entityID)
			{
				return container;
			}
		}
	}
	return nullptr;
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

Resources::Status Level::AddModelEntity(unsigned int modelID,DirectX::XMVECTOR position, DirectX::XMVECTOR rotation) // Author : Johan Ganeteg
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
	newComponent.internalID = GlobalIDHandler::GetInstance()->GetNewId();
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

Resources::Status Level::AddModelEntityFromLevelFile(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation)
{
	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;

	Container newComponent;

	newComponent.component.modelID = modelID;
	newComponent.position = position;
	newComponent.rotation = rotation;
	newComponent.component.modelPtr = DataHandler::GetInstance()->GetModel(modelID);
	if (newComponent.component.modelPtr == nullptr) {
		unsigned int idTemp = 2307979665;
		newComponent.component.modelPtr = DataHandler::GetInstance()->GetModel(idTemp);
	}
	DirectX::XMMATRIX containerMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
	containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
	newComponent.component.worldMatrix = containerMatrix;
	newComponent.internalID = GlobalIDHandler::GetInstance()->AddExistingID(instanceID);
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

Resources::Status Level::AddCheckpointEntity()
{
	CheckpointContainer * container = new CheckpointContainer();

	this->m_checkpointHandler.GetAllCheckpoints()->push_back(container);
	this->m_checkpointHandler.GetAllCheckpoints()->back()->internalID = GlobalIDHandler::GetInstance()->GetNewId();
	this->m_checkpointHandler.GetAllCheckpoints()->back()->checkpointNumber = 0;
	return Resources::Status::ST_OK;
}

Resources::Status Level::AddPuzzleElement(ContainerType type, void * element)
{

	
	AddModelEntityFromLevelFile(((Container*)element)->component.modelID, ((Container*)element)->internalID, ((Container*)element)->position, ((Button*)element)->rotation);
	this->RemoveModel(((Container*)element)->component.modelID, ((Container*)element)->internalID);
	switch (type)
	{
	case BUTTON:
		m_puzzleElements.at(BUTTON).push_back(((Button*)element));
		break;
	case LEVER:
		m_puzzleElements.at(LEVER).push_back(((Button*)element));
		break;
	case WHEEL:
		m_puzzleElements.at(WHEEL).push_back(((Button*)element));
		break;
	case DOOR:
		m_puzzleElements.at(DOOR).push_back(((Door*)element));
		break;
	case MAGNET:
		break;
	case PRESSUREPLATE:
		break;
	case AI:
		m_LevelAi.GetAllPathComponents()->push_back(((AiContainer*)element));
		break;
	default:
		break;
	}


	return Resources::Status::ST_OK;
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
			if (modelPtr->at(i).internalID == instanceID)
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
		for (size_t i = 0; i < m_puzzleElements.size(); i++)
		{
			for each (Container* container in m_puzzleElements.at(i)) {
				if (container->internalID == instanceID)
				{
					container->position = position;
					container->rotation = rotation;
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
					container->component.worldMatrix = containerMatrix;
					container->isDirty = false;
					return Resources::Status::ST_OK;

				}

			}

		}
		m_LevelAi.UpdatePathComponent(instanceID,position,rotation);

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

Resources::Status Level::UpdateCheckpoint(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation, DirectX::XMVECTOR scale)
{
	CheckpointContainer * container = m_checkpointHandler.GetCheckpoint(instanceID);
	DirectX::XMMATRIX checkOrt = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.m128_f32[0]));
	DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.m128_f32[1]));
	DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.m128_f32[2]));

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
	rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);

	checkOrt = DirectX::XMMatrixMultiply(checkOrt, rotationMatrix);
	checkOrt = DirectX::XMMatrixMultiply(checkOrt, DirectX::XMMatrixTranslationFromVector(position));

	container->obb.ext[0] = scale.m128_f32[0];
	container->obb.ext[1] = scale.m128_f32[1];
	container->obb.ext[2] = scale.m128_f32[2];

	container->obb.ort = checkOrt;

	return Resources::Status::ST_OK;
}

Resources::Status Level::AddPointLight()
{

	LightController::GetInstance()->AddLight(LIGHTING::LT_POINT);
	return Resources::Status::ST_OK;
}



Resources::Status Level::RemoveModel(unsigned int modelID, unsigned int instanceID) // Author : Johan Ganeteg
{
	if (modelID == PLAYER1 || modelID == PLAYER2)
		return Resources::Status::ST_OK;

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(modelID);
	std::vector<Container>* modelPtr;
	if (got == m_ModelMap.end()) { // if  does not exists in memory
		for (size_t i = 0; i < m_checkpointHandler.GetAllCheckpoints()->size(); i++)
		{
			if (m_checkpointHandler.GetAllCheckpoints()->at(i)->internalID == instanceID)
			{
				m_checkpointHandler.GetAllCheckpoints()->erase(m_checkpointHandler.GetAllCheckpoints()->begin() + i);
				return  Resources::Status::ST_OK;
			}
		}
		for (size_t i = 0; i < LightController::GetInstance()->GetLights()->size(); i++)
		{
			if (LightController::GetInstance()->GetLights()->at(i)->internalID == instanceID)
			{
				LightController::GetInstance()->RemoveLight(i, LIGHTING::LT_POINT);
			}
		}

	}
	else {
		modelPtr = &got->second;
		//modelPtr->erase(modelPtr->begin() + instanceID);
		for (size_t i = 0; i < modelPtr->size(); i++)
		{
			if (instanceID == modelPtr->at(i).internalID)
			{
					modelPtr->erase(modelPtr->begin() + i);
					return Resources::Status::ST_OK;
			}
		}
	}
		for (size_t i = 0; i < m_puzzleElements.size(); i++)
		{
			for (size_t j = 0; j < m_puzzleElements.at(i).size(); j++)
			{
				if (m_puzzleElements.at(i).at(j)->internalID == instanceID)
				{
					m_puzzleElements.at(i).erase(m_puzzleElements.at(i).begin() + j);
					return Resources::Status::ST_OK;
				}

			}
		}
		m_LevelAi.DeletePathComponent(instanceID);
		
	return Resources::Status::ST_OK;
}

Resources::Status Level::DuplicateEntity( Container *& source, Container*& destination)
{

	std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(source->component.modelID);
	std::vector<Container>* modelPtr;

	if (got == m_ModelMap.end()) { // if  does not exists in container memory

		for (size_t i = 0; i < m_puzzleElements.size(); i++) // check puzzle elements
		{
			for (size_t j = 0; j < m_puzzleElements.at(i).size(); j++)
			{
				if (m_puzzleElements.at(i).at(j)->component.modelID == source->component.modelID)
				{
					

					Container temp = *source;
					temp.component.modelPtr = source->component.modelPtr;
					modelPtr = &got->second;
					temp.internalID = GlobalIDHandler::GetInstance()->GetNewId();
					destination = &modelPtr->back();
					//SelectionHandler::GetInstance()->SetSelectedContainer()
					this->m_ModelMap[source->component.modelID].push_back(temp);

					return Resources::Status::ST_OK;
				}

			}
		}
		std::vector<Light*> *lvec = LightController::GetInstance()->GetLights();
		for each (Light* light in *lvec)
		{
			if (source->internalID == light->internalID)
			{
				Point * point = new Point;
				point->pickSphere = ((Point *)light)->pickSphere;
				point->rangeSphere = ((Point *)light)->rangeSphere;
				point->position = ((Point *)light)->data->position;
				point->type = LIGHT;
				//point->internalID = GlobalIDHandler::GetInstance()->GetNewId();

				LIGHTING::Point * data = ((Point*)source)->data;

				LightController::GetInstance()->AddLight(point, data, LIGHTING::LT_POINT);
				LightController::GetInstance()->GetLights()->back()->position = light->position;
				LightController::GetInstance()->GetLights()->back()->type = LIGHT;

				destination = LightController::GetInstance()->GetLights()->back();
				destination->type = LIGHT;
			}
			return Resources::Status::ST_OK;
		}

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		Container temp = *source;
		temp.component.modelPtr = source->component.modelPtr;
		modelPtr = &got->second;
		temp.internalID = GlobalIDHandler::GetInstance()->GetNewId();
		modelPtr->push_back(temp);
		destination = &modelPtr->back();
		//SelectionHandler::GetInstance()->SetSelectedContainer()
		return Resources::Status::ST_OK;
	}
}

bool Level::isEmpty()
{
	if (this->GetNumEntities() == 0 && this->GetNumPuzzleElements() == 0 && this->GetNumLights() == 0) {
		
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

unsigned int Level::GetNumPuzzleElements()
{
	unsigned int amount = 0;
	for (size_t i = 0; i < m_puzzleElements.size(); i++)
	{
		amount += (unsigned int)m_puzzleElements.at(i).size();
	}
	return amount;
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
	levelName = "untitled_level";

	m_SpawnPoints[0].position = { 1.0f, 0.0, 0.0f };
	m_SpawnPoints[0].rotation = { 0.0f, 0.0f, 0.0f };
	m_SpawnPoints[0].isDirty = true;

	m_SpawnPoints[1].position = { -1.0f, 0.0, 0.0f };
	m_SpawnPoints[1].rotation = { 0.0f, 0.0f, 0.0f };
	m_SpawnPoints[1].isDirty = true;

	m_LevelAi.Destroy();
	GlobalIDHandler::GetInstance()->ResetIDs();
	//Ui::UiControlHandler::GetInstance()->GetAttributesHandler()->Deselect();
	for (size_t i = 0; i < m_puzzleElements.size(); i++)
	{
		for (size_t j = 0; j < m_puzzleElements.at(i).size(); j++)
		{
			delete m_puzzleElements.at(i).at(j);
		}
		m_puzzleElements.at(i).clear();
	}

	//for each (std::vector<Container*> elementContainer in m_puzzleElements){ //Remove all puzzle elements
	//	for (size_t i = 0; i < elementContainer.size(); i++)
	//	{
	//		delete elementContainer.at(i);
	//	}
	//	elementContainer.clear();
	//}
	for each (CheckpointContainer* container in *this->GetCheckpoints())
	{
		delete container;
	}
	this->GetCheckpoints()->clear();


	LightController::GetInstance()->Destroy();
}

void Level::SetSpawnPoint(LevelData::SpawnHeader data, int index)
{

	if (index != 0 && index != 1)
		return;

	m_SpawnPoints[index].position = DirectX::XMVectorSet(data.position[0], data.position[1], data.position[2], 0.0);
	m_SpawnPoints[index].rotation = DirectX::XMVectorSet(data.rotation[0], data.rotation[1], data.rotation[2], 0.0);
	m_SpawnPoints[index].isDirty = true;
}

const std::vector<Container*>* Level::GetPuzzleElements(ContainerType type)
{
	if (type >= ContainerType::NUM_PUZZLE_ELEMENTS)
		return nullptr;
	
	return &this->m_puzzleElements.at(type);
}

Button * Level::ConvertToButton(Container*& object)
{

	if (object->type != ContainerType::MODEL)
		this->ConvertToContainer(object);
	Container* entity = this->GetInstanceEntity(object->internalID);

	if (entity != nullptr)
	{
		// Create a new button,
		// transfer the entity information
		// Remove the old container
		// put the button into the button vector

		Button* newButton = new Button(*entity); // copy the container
		this->RemoveModel(entity->component.modelID, entity->internalID); // remove the old one
		this->m_puzzleElements.at(BUTTON).push_back(newButton); // add to button array
		object = newButton; //set the obj to the new button as well. Incase the programmer tries to use the obj afterwards. This avoids crashes
		return newButton; //Return new button

	}

	return nullptr;
}

Door * Level::ConvertToDoor(Container *& object)
{

	if (object->type != ContainerType::MODEL)
		this->ConvertToContainer(object);

	Container* entity = this->GetInstanceEntity(object->internalID);
	if (entity != nullptr)
	{
		// Create a new door,
		// transfer the entity information
		// Remove the old container
		// put the door into the door vector

		Door* newDoor = new Door(*entity); // copy the container
		this->RemoveModel(entity->component.modelID, entity->internalID); // remove the old one
		this->m_puzzleElements.at(DOOR).push_back(newDoor); // add to door array
		object = newDoor; //set the object to the new door as well. In case the programmer tries to use the object afterwards. This avoids crashes
		return newDoor; //Return new button
		
	}

	return nullptr;
}



Container * Level::ConvertToContainer(Container *& object)
{
	if (object->type >= ContainerType::NUM_PUZZLE_ELEMENTS)
		return nullptr;

	ContainerType type = object->type;
	//Create new container.
	//fill it with data.
	//put it into the corresponding array
	//Remove from the puzzle array it came from

	for (size_t i = 0; i < m_puzzleElements.at(type).size(); i++)
	{
		if (m_puzzleElements.at(type).at(i)->internalID == object->internalID) // puzzleElement Found
		{
			std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap.find(m_puzzleElements.at(type).at(i)->component.modelID); //find the vector that holds this type of model
			std::vector<Container>* modelPtr;

			Container newComponent((Container)*m_puzzleElements.at(type).at(i));		// Create new container component
			modelPtr = &got->second;													// The vector that holds the the corresponding model
			modelPtr->push_back(newComponent);											// Push back the newly created container
			m_puzzleElements.at(type).erase(m_puzzleElements.at(type).begin() + i);	    // Erase the puzzle element
			object = &modelPtr->back();
			return &modelPtr->back(); //Return the model
		}

	}

	


	return nullptr;
}

AiContainer * Level::ConvertToAI(Container *& object)
{
	if (object->type != ContainerType::MODEL)
		this->ConvertToContainer(object);

	Container* entity = this->GetInstanceEntity(object->internalID);
	if (entity != nullptr)
	{
		// Create a new door,
		// transfer the entity information
		// Remove the old container
		// put the door into the door vector


		AiContainer* newAI = this->m_LevelAi.NewPathComponent(); // get new ai component

		newAI->ConvertFromContainer(entity); //Get the data from the  container

		this->RemoveModel(entity->component.modelID, entity->internalID); // remove the old one
		object = newAI; //set the object to the new AI as well. In case the programmer tries to use the object afterwards. This avoids crashes
		return newAI; //Return new ai
	}


	return nullptr;
}

Wheel * Level::ConvertToWheel(Container *& object)
{
	if (object->type != ContainerType::MODEL)
		this->ConvertToContainer(object);
	Container* entity = this->GetInstanceEntity(object->internalID);

	if (entity != nullptr)
	{
		// Create a new wheel,
		// transfer the entity information
		// Remove the old container
		// put the wheel into the wheel vector

		Wheel* newWheel = new Wheel(*entity); // copy the container
		this->RemoveModel(entity->component.modelID, entity->internalID); // remove the old one
		this->m_puzzleElements.at(WHEEL).push_back(newWheel); // add to wheel array
		object = newWheel; //set the obj to the new wheel as well. Incase the programmer tries to use the obj afterwards. This avoids crashes
		return newWheel; //Return new Wheel

	}
	return nullptr;
}

Lever * Level::ConvertToLever(Container *& object)
{
	
	if (object->type != ContainerType::MODEL)
		this->ConvertToContainer(object);
	Container* entity = this->GetInstanceEntity(object->internalID);

	if (entity != nullptr)
	{
		// Create a new lever,
		// transfer the entity information
		// Remove the old container
		// put the wheel into the wheel vector

		Lever* newLever = new Lever(*entity); // copy the container
		this->RemoveModel(entity->component.modelID, entity->internalID); // remove the old one
		this->m_puzzleElements.at(LEVER).push_back(newLever); // add to Lever array
		object = newLever; //set the obj to the new Lever as well. Incase the programmer tries to use the obj afterwards. This avoids crashes
		return newLever; //Return new Lever

	}
	return nullptr;
}
