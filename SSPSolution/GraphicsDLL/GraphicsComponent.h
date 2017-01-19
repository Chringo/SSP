#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
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

struct UIComponent
{
	int active = 0;
	int spriteID = -1;
	bool wasClicked = false;
	bool isHovered = false;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(10.0f, 10.0f);
	float scale = 1.f;
	float rotation = 0.f;
	float layerDepth = 1.f;

	void UpdateClicked(DirectX::XMFLOAT2 mousePos)
	{
		if ((mousePos.x > this->position.x && mousePos.x < this->position.x + this->size.x)
			&& (mousePos.y > this->position.y && mousePos.y < this->position.y + this->size.y)) 
		{
			this->wasClicked = true;
		}
	}
	void UpdateClicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize)
	{
		float mouseX = mousePos.x - (GetSystemMetrics(SM_CXSCREEN) - windowSize.x) / 2;
		float mouseY = mousePos.y - (GetSystemMetrics(SM_CYSCREEN) - windowSize.y) / 2;

		if ((mouseX > this->position.x && mouseX < this->position.x + this->size.x) 
			&& (mouseY > this->position.y && mouseY < this->position.y + this->size.y)) 
		{
			this->wasClicked = true;
		}
	}
	void UpdateHover(DirectX::XMFLOAT2 mousePos)
	{
		if ((mousePos.x > this->position.x && mousePos.x < this->position.x + this->size.x)
			&& (mousePos.y > this->position.y && mousePos.y < this->position.y + this->size.y))
		{
			this->isHovered = true;
		}
		else
		{
			this->isHovered = false;
		}
	}
	bool CheckClicked()
	{
		if (this->wasClicked)
		{
			this->wasClicked = false;
			return true;
		}
		else 
		{
			return false;
		}
	}
};

struct TextComponent
{
	int active = 0;
	std::wstring text = L"";
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0.f, 0.f);
	DirectX::XMFLOAT2 scale = DirectX::XMFLOAT2(1.f, 1.f);
	float rotation = 0.f;
	float layerDepth = 0.f;
};
#endif