#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <DirectXMath.h>

struct GraphicsComponent
{
	int active = 0;
	int modelID = -1;
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

struct UIComponent
{
	int active = 0;
	int spriteID = -1;
	bool wasClicked = false;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(10.0f, 10.0f);
};
#endif