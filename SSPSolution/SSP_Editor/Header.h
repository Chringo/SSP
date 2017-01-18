#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../AIDLL/AIComponent.h"
struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total v�rde. 
	DirectX::XMVECTOR rotation; //Total v�rde. 
	GraphicsComponent component;
	bool isDirty  = false;
	bool isStatic = true;
};

struct AiContainer : public Container
{
	AIComponent    aiComponent;
};

