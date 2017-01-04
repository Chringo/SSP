#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <DirectXMath.h>

struct GraphicsComponent
{
	int active = 0;
	unsigned int modelID = 0;
	DirectX::XMMATRIX worldMatrix;
};

struct penis
{
	int active = 0;
	int modelID = -1;
	int joints = 0;
	DirectX::XMMATRIX worldMatrix;

	DirectX::XMMATRIX finalTransforms[32];
};
#endif