#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../AIDLL/AIComponent.h"
struct Container
{
	unsigned int	  internalID;
	DirectX::XMVECTOR position;		// Total v�rde. 
	DirectX::XMVECTOR rotation;		// Total v�rde. 
	GraphicsComponent component;
	AIComponent*	  aiComponent = nullptr;
	bool			  isDirty      = false;
	bool			  isStatic     = true;
	Container() {}
	Container(const Container &obj) {  // copy constructor
	
		this->internalID	= obj.internalID	;
		this->position		= obj.position		;
		this->rotation		= obj.rotation		;
		this->component		= obj.component		;
		this->aiComponent	= obj.aiComponent	;
		this->isDirty		= obj.isDirty		;
		this->isStatic		= obj.isStatic		;
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

		return *this;
	}

	};

struct AiContainer 
{
	OBB obb;
	
	AIComponent aiComponent;
	AiContainer()
	{
		obb.ort.r[0] = { 1.0f,0.0f,0.0f};
		obb.ort.r[1] = { 0.0f,1.0f,0.0f };
		obb.ort.r[2] = { 0.0f,0.0f,1.0f };
		obb.ext[0] = 0.2f;
		obb.ext[1] = 0.2f;
		obb.ext[2] = 0.2f;
	}
};

