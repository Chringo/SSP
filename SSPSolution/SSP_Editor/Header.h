#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total v�rde. 
	float rotation; //Total v�rde. 
	GraphicsComponent component;
};
