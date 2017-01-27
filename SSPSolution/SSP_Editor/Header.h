#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../AIDLL/AIComponent.h"
#include "LevelHeaders.h"
#include "../SSPSolution/ComponentStructs.h"

enum ContainerType
{
	MODEL = 0,
	BUTTON,
	LEVER,
	WHEEL,
	DOOR,
	MAGNET,
	PRESSUREPLATE,
	NUM_PUZZLE_ELEMENTS,
	LIGHT,
	AIWAYPOINT,
	CHECKPOINT,

	NUM_TYPES
};
struct Container
{
	unsigned int	  internalID;
	DirectX::XMVECTOR position;		// Total värde. 
	DirectX::XMVECTOR rotation;		// Total värde. 
	GraphicsComponent component;
	AIComponent*	  aiComponent = nullptr;
	bool			  isDirty      = false;
	bool			  isStatic     = true;

	ContainerType type;
	Container() {
		type = MODEL;
	};
	Container(const Container &obj) {  // copy constructor
	
		this->internalID	= obj.internalID	;
		this->position		= obj.position		;
		this->rotation		= obj.rotation		;
		this->component		= obj.component		;
		this->aiComponent	= obj.aiComponent	;
		this->isDirty		= obj.isDirty		;
		this->isStatic		= obj.isStatic		;
		this->type = MODEL;
	}
	Container& operator=(Container const& obj)
	{
	
		this->internalID  = obj.internalID;
		this->position	  = obj.position;
		this->rotation	  = obj.rotation;
		this->component   = obj.component;
		this->aiComponent = obj.aiComponent;
		this->isDirty	  = obj.isDirty;
		this->isStatic	  = obj.isStatic;
		this->type = MODEL;
		return *this;
	}

	};

struct ListenerContainer : Container
{
	unsigned int numTriggers = 0;
	EVENT		 listenEvent[20];
	unsigned int triggerEntityIds[20];
	Container*	 triggerContainers[20]; //pointers to the triggers
	ListenerContainer() : Container() {
		memset(triggerEntityIds, 0, sizeof(UINT) * 20);

	};
	ListenerContainer(const Container &obj) : Container(obj) //copy constructor to convert from a container type to Button
	{
	}
	bool AddTrigger(Container* trigger, EVENT listenEvent)
	{
		for (size_t i = 0; i < numTriggers; i++)
		{
			if (triggerEntityIds[i] == trigger->internalID) //check if it already exists
				return false;
		}
		if (numTriggers < 20) {
			this->triggerEntityIds[numTriggers] = trigger->internalID;
			this->triggerContainers[numTriggers] = trigger;
			this->listenEvent[numTriggers] = listenEvent;
			numTriggers += 1;
		}
		else
			return false;
	}

	bool UpdateTriggerEvent(Container* trigger, EVENT newEvent) {
		for (size_t i = 0; i < numTriggers; i++)
		{
			if (triggerEntityIds[i] == trigger->internalID) //check if it already exists
			{
				listenEvent[i] = newEvent;
				return true;
			}
		}

		return false;
	}
	void DeleteTrigger(unsigned int entityId)
	{
		int index = 0;
		bool triggerExist = false;
		for (size_t i = 0; i < numTriggers; i++) //find the index of the trigger to be deleted
		{
			if (triggerEntityIds[i] == entityId) {
				index = i;
				triggerExist = true;
				break;
			}
		}
		if (!triggerExist) // if the trigger does not exist
			return;

		for (size_t j = index; j < numTriggers - 1; j++) //move the data to the left in the array
		{
			triggerEntityIds[j]		= triggerEntityIds[j + 1];
			triggerContainers[j]    = triggerContainers[j + 1];
			listenEvent[j]		    = listenEvent[j + 1];
		}
		numTriggers -= 1;
	}

};

struct Button : ListenerContainer
{
	Button() : ListenerContainer()
	{
		this->type = BUTTON;
	}
	Button(const Container &obj) : ListenerContainer(obj) //copy constructor to convert from a container type to Button
	{
		this->type = BUTTON;
	}
	float interactionDistance = 1.0f;
	float resetTime = 0.0f; // Seconds
};

struct Lever : ListenerContainer
{
	Lever() : ListenerContainer() {
		this->type = LEVER;
	}
	float interactionDistance;
};
struct Wheel : ListenerContainer
{
	Wheel() : ListenerContainer()
	{
		this->type = WHEEL;
	}
	float interactionDistance;
	float minRotation;
	float maxRotation;
	float rotateTime;

	float timeToReset;	  //Sekunder
	float resetTime;	  //Sekunder
};
struct Door : ListenerContainer
{
	Door() : ListenerContainer()
	{
		this->type = DOOR;
	}
	Door(const Container &obj) : ListenerContainer(obj) //copy constructor to convert from a container type to Door
	{
		this->type = DOOR;
	}
	float rotateTime;
	
};
struct CheckpointContainer : Container
{
	unsigned int internalID;
	unsigned int checkpointNumber;
	OBB obb;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR color;
private:
	LevelData::CheckpointHeader checkpointHeader;
public:
	CheckpointContainer()
	{
		type = CHECKPOINT;
		internalID = UINT_MAX;
		position = { 0.0,0.0,0.0 };
		rotation = { 0.0,0.0,0.0 };
		scale = { 1.0, 1.0, 1.0 };

		obb.ort.r[0] = { 1.0f,0.0f,0.0f };
		obb.ort.r[1] = { 0.0f,1.0f,0.0f };
		obb.ort.r[2] = { 0.0f,0.0f,1.0f };
		obb.ext[0] = this->scale.m128_f32[0];
		obb.ext[1] = this->scale.m128_f32[1];
		obb.ext[2] = this->scale.m128_f32[2];
	}
	void Update()
	{
		obb.ext[0] = this->scale.m128_f32[0];
		obb.ext[1] = this->scale.m128_f32[1];
		obb.ext[2] = this->scale.m128_f32[2];


		DirectX::XMMATRIX containerMatrix = this->obb.ort;

		DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rotation.m128_f32[0]));
		DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.m128_f32[1]));
		DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rotation.m128_f32[2]));
		//Create the rotation matrix
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrixZ, rotationMatrixX);
		rotationMatrix = DirectX::XMMatrixMultiply(rotationMatrix, rotationMatrixY);

		//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);
		//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
		//containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, rotationMatrix);
		//containerMatrix = DirectX::XMMatrixMultiply(containerMatrix, DirectX::XMMatrixTranslationFromVector(position));
 
		this->obb.ort = rotationMatrix;
		this->isDirty = false;
	}

	LevelData::CheckpointHeader * GetDataPtr()
	{
		this->checkpointHeader.entityID = this->internalID;
		this->checkpointHeader.checkpointNumber = this->checkpointNumber;
		this->checkpointHeader.position[0] = this->position.m128_f32[0];
		this->checkpointHeader.position[1] = this->position.m128_f32[1];
		this->checkpointHeader.position[2] = this->position.m128_f32[2];
		memcpy(this->checkpointHeader.ext, this->obb.ext, sizeof(float) * 3);
		memcpy(this->checkpointHeader.ort, &this->obb.ort, sizeof(float) * 16);


		return &this->checkpointHeader;
	}

	CheckpointContainer(LevelData::CheckpointHeader data)
	{
		this->internalID = data.entityID;
		this->type = CHECKPOINT;
		this->checkpointNumber = data.checkpointNumber;
		memcpy(&this->obb.ext[0], data.ext, sizeof(float) * 3);
		memcpy(&this->obb.ort, data.ort, sizeof(float) * 16);

		float rotX = atan2(this->obb.ort.r[2].m128_f32[1], this->obb.ort.r[2].m128_f32[2]);
		float rotY = atan2(-this->obb.ort.r[2].m128_f32[0], sqrt(pow(this->obb.ort.r[2].m128_f32[1], 2) + pow(this->obb.ort.r[2].m128_f32[2], 2)));
		float rotZ = atan2(this->obb.ort.r[1].m128_f32[0], this->obb.ort.r[0].m128_f32[0]);

		this->rotation = { rotX, rotY, rotZ };
		this->position = { data.position[0], data.position[1],data.position[2] };
		this->scale = { this->obb.ext[0], this->obb.ext[1], this->obb.ext[2] };

		this->Update();

	}

};

struct SelectionLists
{
	std::vector<Resources::Model*>* modelPtr;
	std::vector<CheckpointContainer*>* checkpointPtr;
	//std::vector<Lights*>* lightsPtr;
};
