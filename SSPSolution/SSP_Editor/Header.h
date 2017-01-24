#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../AIDLL/AIComponent.h"
#include "LevelHeaders.h"

enum ContainerType
{
	MODEL = 0,
	BUTTON,
	LEVER,
	WHEEL,
	DOOR,
	MAGNET,
	PRESSUREPLATE,
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

struct Button : Container
{
	Button() : Container()
	{
		this->type = BUTTON;
	}
	float interactionDistance;
	float resetTime; // Seconds
};

struct Lever : Container
{
	Lever() : Container() {
		this->type = LEVER;
	}
	float interactionDistance;
};
struct Wheel : Container
{
	Wheel() : Container()
	{
		this->type = WHEEL;
	}
	float interactionDistance;
	float minRotation;
	float maxRotation;
	float rotateTime;
};
struct Door : Container
{
	Door() : Container()
	{
		this->type = DOOR;
	}
	float rotateTime;
	unsigned int numTriggers = 0;
	unsigned int triggerEntityIds[10];

	void AddTrigger(unsigned int entityId)
	{
		for (size_t i = 0; i < numTriggers; i++)
		{
			if (numTriggers == entityId)
				return;
		}
		if (numTriggers < 10) {
			this->triggerEntityIds[numTriggers] = entityId;
			numTriggers += 1;
		}
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
			triggerEntityIds[j] = triggerEntityIds[j + 1];
		}
		numTriggers -= 1;
	}
};
struct CheckpointContainer : Container
{
	unsigned int internalID;
	OBB obb;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;


	LevelData::CheckpointHeader checkpointHeader;
	CheckpointContainer()
	{
		internalID = UINT_MAX;
		position = { 0.0,0.0,0.0 };
		rotation = { 0.0,0.0,0.0 };
		scale = { 1.0, 1.0, 1.0 };

		obb.ort.r[0] = { 1.0f,0.0f,0.0f };
		obb.ort.r[1] = { 0.0f,1.0f,0.0f };
		obb.ort.r[2] = { 0.0f,0.0f,1.0f };
		obb.ext[0] = 0.5f;
		obb.ext[1] = 0.5f;
		obb.ext[2] = 0.5f;
	}
};

struct SelectionLists
{
	std::vector<Resources::Model*>* modelPtr;
	std::vector<CheckpointContainer*>* checkpointPtr;
	//std::vector<Lights*>* lightsPtr;
};
