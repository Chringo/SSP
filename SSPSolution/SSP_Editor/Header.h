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
	AI,
	NUM_TYPES,
	NONE //important for ui stuff

};
struct Container
{
	unsigned int	  internalID;
	DirectX::XMVECTOR position;		// Total värde. 
	DirectX::XMVECTOR rotation;		// Total värde. 
	GraphicsComponent component;
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
		memset(triggerContainers, NULL, sizeof(Container*));
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
		triggerContainers[numTriggers] = nullptr;
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
private:
	LevelData::ButtonHeader data;
public:
	LevelData::ButtonHeader * GetData()
	{
		//fill entity data
		data.EntityID = this->internalID;
		data.isStatic = this->isStatic;
		//if (this->aiComponent != nullptr)
		//{
		//	data.isStatic = false;
		//	data.hasAi = true;
		//}
		//else {
		//	data.hasAi = false;
		//}
		data.modelID = this->component.modelID;
		data.position[0] = this->position.m128_f32[0]; //pos
		data.position[1] = this->position.m128_f32[1];
		data.position[2] = this->position.m128_f32[2];
		data.rotation[0] = this->rotation.m128_f32[0]; // rot
		data.rotation[1] = this->rotation.m128_f32[1];
		data.rotation[2] = this->rotation.m128_f32[2];

		//fill listener data
		data.Listener.numConnections = this->numTriggers;
		for (int i = 0; i < this->numTriggers; i++)
		{
			data.Listener.Event[i] = this->listenEvent[i];
			data.Listener.SenderID[i] = this->triggerEntityIds[i];
		}

		//fill unique data
		data.resetTime = this->resetTime;
		data.interactionDistance = this->interactionDistance;

		return &data;
	}
	Button(LevelData::ButtonHeader* dataPtr)
	{
		this->type = BUTTON;

		//entity load
		this->internalID = dataPtr->EntityID;
		this->isStatic = dataPtr->isStatic;
		this->component.modelID = dataPtr->modelID;
		this->component.worldMatrix = DirectX::XMMatrixIdentity();
		this->position = { dataPtr->position[0], dataPtr->position[1], dataPtr->position[2] };
		this->rotation = { dataPtr->rotation[0], dataPtr->rotation[1], dataPtr->rotation[2] };


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
		
		this->component.worldMatrix = containerMatrix;

		//listener load
		this->numTriggers = dataPtr->Listener.numConnections;
		for (int i = 0; i < this->numTriggers; i++)
		{
			this->listenEvent[i] = (EVENT)dataPtr->Listener.Event[i];
			this->triggerEntityIds[i] = dataPtr->Listener.SenderID[i];
		}

		//unique data load
		this->resetTime = dataPtr->resetTime;
		this->interactionDistance = dataPtr->interactionDistance;

		this->isDirty = true;
		
	}
};

struct Lever : ListenerContainer
{
	Lever() : ListenerContainer() {
		this->type = LEVER;
	}
	Lever(const Container &obj) : ListenerContainer(obj) //copy constructor to convert from a container type to Lever
	{
		this->type = LEVER;
	}
	float interactionDistance;

private:
	LevelData::LeverHeader data;
public:
	LevelData::LeverHeader * GetData()
	{
		//fill entity data
		data.EntityID = this->internalID;
		data.isStatic = this->isStatic;
		//if (this->aiComponent != nullptr)
		//{
		//	data.isStatic = false;
		//	data.hasAi = true;
		//}
		//else {
		//	data.hasAi = false;
		//}
		data.modelID = this->component.modelID;
		data.position[0] = this->position.m128_f32[0];
		data.position[1] = this->position.m128_f32[1];
		data.position[2] = this->position.m128_f32[2];
		data.rotation[0] = this->rotation.m128_f32[0];
		data.rotation[1] = this->rotation.m128_f32[1];
		data.rotation[2] = this->rotation.m128_f32[2];

		//fill listener data
		data.Listener.numConnections = this->numTriggers;
		for (int i = 0; i < this->numTriggers; i++)
		{
			data.Listener.Event[i] = this->listenEvent[i];
			data.Listener.SenderID[i] = this->triggerEntityIds[i];
		}

		//fill unique data
		data.interactionDistance = this->interactionDistance;

		return &data;
	}
	Lever(LevelData::LeverHeader* dataPtr)
	{
		this->type = LEVER;

		//entity load
		this->internalID = dataPtr->EntityID;
		this->isStatic = dataPtr->isStatic;
		this->component.modelID = dataPtr->modelID;
		this->component.worldMatrix = DirectX::XMMatrixIdentity();
		this->position = { dataPtr->position[0], dataPtr->position[1], dataPtr->position[2] };
		this->rotation = { dataPtr->rotation[0], dataPtr->rotation[1], dataPtr->rotation[2] };


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

		this->component.worldMatrix = containerMatrix;

		//listener load
		this->numTriggers = dataPtr->Listener.numConnections;
		for (int i = 0; i < this->numTriggers; i++)
		{
			this->listenEvent[i] = (EVENT)dataPtr->Listener.Event[i];
			this->triggerEntityIds[i] = dataPtr->Listener.SenderID[i];
		}

		//unique data load
		this->interactionDistance = dataPtr->interactionDistance;

		this->isDirty = true;
	}

};
struct Wheel : ListenerContainer
{
	Wheel() : ListenerContainer()
	{
		this->type = WHEEL;
	}
	Wheel(const Container &obj) : ListenerContainer(obj) //copy constructor to convert from a container type to Wheel
	{
		this->type = WHEEL;
	}
	float interactionDistance = 3.0f;
	float minRotation = 0.0f;
	float maxRotation = 720.0f;
	float rotateTime  = 4.0f;
	float timeToReset = 0.0f;	  //Sekunder
	float resetTime   = 4.0f;	  //Sekunder



private:
	LevelData::WheelHeader data;
public:
	LevelData::WheelHeader * GetData()
	{
		//fill entity data
		data.EntityID = this->internalID;
		data.isStatic = this->isStatic;
		//if (this->aiComponent != nullptr)
		//{
		//	data.isStatic = false;
		//	data.hasAi = true;
		//}
		//else {
		//	data.hasAi = false;
		//}
		data.modelID = this->component.modelID;
		data.position[0] = this->position.m128_f32[0];
		data.position[1] = this->position.m128_f32[1];
		data.position[2] = this->position.m128_f32[2];
		data.rotation[0] = this->rotation.m128_f32[0];
		data.rotation[1] = this->rotation.m128_f32[1];
		data.rotation[2] = this->rotation.m128_f32[2];

		//fill listener data
		data.Listener.numConnections = this->numTriggers;
		for (int i = 0; i < this->numTriggers; i++)
		{
			data.Listener.Event[i] = this->listenEvent[i];
			data.Listener.SenderID[i] = this->triggerEntityIds[i];
		}

		//fill unique data
		data.interactionDistance = this->interactionDistance;
		data.min = this->minRotation / 360.f;
		data.max = this->maxRotation / 360.f;
		data.resetTime = this->resetTime;
		data.resetDelay = this->timeToReset;	  //Sekunder
		data.time = this->rotateTime;	  //Sekunder


		return &data;
	}
	Wheel(LevelData::WheelHeader* dataPtr)
	{
		this->type = WHEEL;

		//entity load
		this->internalID = dataPtr->EntityID;
		this->isStatic = dataPtr->isStatic;
		this->component.modelID = dataPtr->modelID;
		this->component.worldMatrix = DirectX::XMMatrixIdentity();
		this->position = { dataPtr->position[0], dataPtr->position[1], dataPtr->position[2] };
		this->rotation = { dataPtr->rotation[0], dataPtr->rotation[1], dataPtr->rotation[2] };


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

		this->component.worldMatrix = containerMatrix;

		//listener load
		this->numTriggers = dataPtr->Listener.numConnections;
		for (int i = 0; i < this->numTriggers; i++)
		{
			this->listenEvent[i] = (EVENT)dataPtr->Listener.Event[i];
			this->triggerEntityIds[i] = dataPtr->Listener.SenderID[i];
		}

		//unique data load
		this->interactionDistance = dataPtr->interactionDistance;
		this->minRotation		  = dataPtr->min * 360.f;
		this->maxRotation		  = dataPtr->max * 360.f;
		this->resetTime			  = dataPtr->resetTime;
		this->timeToReset		  = dataPtr->resetDelay;
		this->rotateTime		  = dataPtr->time;

		this->isDirty = true;
	}
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

private:
	LevelData::DoorHeader data;
public:
	LevelData::DoorHeader * GetData()
	{
		//fill entity data
		data.EntityID = this->internalID;
		data.isStatic = this->isStatic;
	//if (this->aiComponent != nullptr)
	//{
	//	data.isStatic = false;
	//	data.hasAi = true;
	//}
	//else {
	//	data.hasAi = false;
	//}
		data.modelID = this->component.modelID;
		data.position[0] = this->position.m128_f32[0];
		data.position[1] = this->position.m128_f32[1];
		data.position[2] = this->position.m128_f32[2];
		data.rotation[0] = this->rotation.m128_f32[0];
		data.rotation[1] = this->rotation.m128_f32[1];
		data.rotation[2] = this->rotation.m128_f32[2];

		//fill listener data
		data.Listener.numConnections = this->numTriggers;
		for (int i = 0; i < this->numTriggers; i++)
		{
			data.Listener.Event[i] = this->listenEvent[i];
			data.Listener.SenderID[i] = this->triggerEntityIds[i];
		}

		//fill unique data
		data.rotateTime = this->rotateTime;

		return &data;
	}
	Door(LevelData::DoorHeader* dataPtr)
	{
		this->type = DOOR;

		//entity load
		this->internalID = dataPtr->EntityID;
		this->isStatic = dataPtr->isStatic;
		this->component.modelID = dataPtr->modelID;
		this->component.worldMatrix = DirectX::XMMatrixIdentity();
		this->position = { dataPtr->position[0], dataPtr->position[1], dataPtr->position[2] };
		this->rotation = { dataPtr->rotation[0], dataPtr->rotation[1], dataPtr->rotation[2] };


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

		this->component.worldMatrix = containerMatrix;

		//listener load
		this->numTriggers = dataPtr->Listener.numConnections;
		for (int i = 0; i < this->numTriggers; i++)
		{
			this->listenEvent[i] = (EVENT)dataPtr->Listener.Event[i];
			this->triggerEntityIds[i] = dataPtr->Listener.SenderID[i];
		}

		//unique data load
		this->rotateTime = dataPtr->rotateTime;

		this->isDirty = true;

	}
	
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
	CheckpointContainer() : Container()
	{
		type = CHECKPOINT;
		//internalID = 4;
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

struct AiContainer : ListenerContainer
{
	AiContainer() : ListenerContainer() {
		this->type = AI;
	}

	AiContainer(const Container &obj) : ListenerContainer(obj) //copy constructor to convert from a container type to Ai
	{
		this->type = AI;
	}
	AIComponent	  aiComponent;



	void ConvertFromContainer(Container* obj)
	{
		this->aiComponent.AC_entityID = obj->internalID;
		this->internalID = obj->internalID;
		this->position	 = obj->position;
		this->rotation   = obj->rotation;
		this->component  = obj->component;
		this->isDirty	 = obj->isDirty;
		this->isStatic	 = obj->isStatic;
		this->type		 = AI;

	}
private:
	LevelData::AiHeader data;
public:
	LevelData::AiHeader * GetData()
	{
		//fill entity data
		data.EntityID = this->internalID;
		data.isStatic = this->isStatic;
		//if (this->aiComponent != nullptr)
		//{
		data.isStatic = false;
		data.hasAi = true;
		//}
		//else {
		//	data.hasAi = false;
		//}
		data.modelID = this->component.modelID;
		data.position[0] = this->position.m128_f32[0];
		data.position[1] = this->position.m128_f32[1];
		data.position[2] = this->position.m128_f32[2];
		data.rotation[0] = this->rotation.m128_f32[0];
		data.rotation[1] = this->rotation.m128_f32[1];
		data.rotation[2] = this->rotation.m128_f32[2];

		//fill listener data
		data.Listener.numConnections = this->numTriggers;
		for (int i = 0; i < this->numTriggers; i++)
		{
			data.Listener.Event[i] = this->listenEvent[i];
			data.Listener.SenderID[i] = this->triggerEntityIds[i];
		}

		//fill unique data
		data.time = this->aiComponent.AC_time;
		data.speed = this->aiComponent.AC_speed;
		data.pattern = this->aiComponent.AC_pattern;
		data.nrOfWaypoints = this->aiComponent.AC_nrOfWaypoint;
		
		for (int i = 0; i < data.nrOfWaypoints; i++)
		{
			data.wayPoints[i][0] = this->aiComponent.AC_waypoints[i].m128_f32[0];
			data.wayPoints[i][1] = this->aiComponent.AC_waypoints[i].m128_f32[1];
			data.wayPoints[i][2] = this->aiComponent.AC_waypoints[i].m128_f32[2];
		}

		return &data;
	}

	AiContainer(LevelData::AiHeader* dataPtr)
	{
		this->type = AI;

		//entity load
		this->internalID = dataPtr->EntityID;
		this->isStatic = dataPtr->isStatic;
		this->component.modelID = dataPtr->modelID;
		this->component.worldMatrix = DirectX::XMMatrixIdentity();
		this->position = { dataPtr->position[0], dataPtr->position[1], dataPtr->position[2] };
		this->rotation = { dataPtr->rotation[0], dataPtr->rotation[1], dataPtr->rotation[2] };


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

		this->component.worldMatrix = containerMatrix;

		//listener load
		this->numTriggers = dataPtr->Listener.numConnections;
		for (int i = 0; i < this->numTriggers; i++)
		{
			this->listenEvent[i] = (EVENT)dataPtr->Listener.Event[i];
			this->triggerEntityIds[i] = dataPtr->Listener.SenderID[i];
		}

		//unique data load
		//fill unique data
		this->aiComponent.AC_time =	dataPtr->time;
		this->aiComponent.AC_speed = dataPtr->speed;
		this->aiComponent.AC_pattern = dataPtr->pattern;
		this->aiComponent.AC_nrOfWaypoint = dataPtr->nrOfWaypoints;

		for (int i = 0; i < dataPtr->nrOfWaypoints; i++)
		{
			this->aiComponent.AC_waypoints[i].m128_f32[0] = dataPtr->wayPoints[i][0];
			this->aiComponent.AC_waypoints[i].m128_f32[1] = dataPtr->wayPoints[i][1];
			this->aiComponent.AC_waypoints[i].m128_f32[2] = dataPtr->wayPoints[i][2];
		}

		this->isDirty = true;
	}
};

struct Light : Container
{
	Light() : Container() {};
};

struct Point : Light
{
	Sphere pickSphere;
	Sphere rangeSphere;

	LIGHTING::Point * data;
	
	Point() : Light()
	{
		this->type = LIGHT;
	}

	void Initialize(LIGHTING::Point * data)
	{
		this->data = data;

		this->type = LIGHT;

		this->data->color.r = 0.8f;
		this->data->color.g = 0.6f;
		this->data->color.b = 0.6f;

		this->data->falloff.constant = 1.0f; //0.07f, 0.017f };
		this->data->falloff.linear = 0.07f;
		this->data->falloff.quadratic = 0.017f;
		this->data->intensity = 3.0f;
		this->data->radius = 3.0f;
		this->data->position = { 0.0f,0.0f,0.0f };
		this->data->isActive = true;
		rangeSphere.radius = this->data->radius;

		this->position = this->data->position;
		pickSphere.radius = 0.25f;
	}

	void Update()
	{
		this->data->position = this->position;
		this->rangeSphere.radius = this->data->radius;
		isDirty = false;
	};

	Point& operator=(Point const& obj)
	{
		this->internalID = obj.internalID;
		this->position = obj.position;
		this->rotation = obj.rotation;
		this->component = obj.component;
		this->isDirty = obj.isDirty;
		this->isStatic = obj.isStatic;
		this->type = LIGHT;

		this->rangeSphere = obj.rangeSphere;
		this->pickSphere = obj.rangeSphere;
		return *this;
	}
};

struct Ambient
{
	float r;
	float g;
	float b;
	float intensity;

	Ambient()
	{
		this->r = 1.0f;
		this->g = 1.0f;
		this->b = 1.0f;
		this->intensity = 0.2f;
	}
};