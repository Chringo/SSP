#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <DirectXMath.h>
#include "../ResourceLib/Model.h"

struct GraphicsComponent
{
	int active = 0;
	unsigned int modelID = 0;
	Resources::Model* modelPtr;
	DirectX::XMMATRIX worldMatrix;
};

struct penis //john....
{
	int active = 0;
	int modelID = -1;
	int joints = 0;
	DirectX::XMMATRIX worldMatrix;

	DirectX::XMMATRIX finalTransforms[32];
};
#endif