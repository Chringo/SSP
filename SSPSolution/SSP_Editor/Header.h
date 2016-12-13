#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total värde. 
	float rotation; //Total värde. 
	GraphicsComponent component;
};
