#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <DirectXMath.h>

struct GraphicsComponent
{
	int active = 0;
	int modelID = -1;
	DirectX::XMFLOAT4X4 worldMatrix;
};

#endif