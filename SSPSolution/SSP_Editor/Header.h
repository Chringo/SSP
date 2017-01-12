#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total värde. 
	DirectX::XMVECTOR rotation; //Total värde. 
	GraphicsComponent component;
	bool isDirty  = false;
	bool isStatic = true;
};

