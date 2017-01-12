#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <d3d11.h>
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
	float scale = 1.f;

	void UpdateClicked(DirectX::XMFLOAT2 mousePos)
	{
		if ((mousePos.x > this->position.x - this->size.x && mousePos.x < this->position.x + this->size.x)
			&& (mousePos.y > this->position.y - this->size.y && mousePos.y < this->position.y + this->size.y)) 
		{
			this->wasClicked = true;
		}
	}
	void UpdateClicked(DirectX::XMFLOAT2 mousePos, DirectX::XMFLOAT2 windowSize)
	{
		float mouseX = mousePos.x - (GetSystemMetrics(SM_CXSCREEN) - windowSize.x) / 2;
		float mouseY = mousePos.y - (GetSystemMetrics(SM_CYSCREEN) - windowSize.y) / 2;

		if ((mouseX > this->position.x - this->size.x && mouseX < this->position.x + this->size.x) 
			&& (mouseY > this->position.y - this->size.y && mouseY < this->position.y + this->size.y)) 
		{
			this->wasClicked = true;
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
#endif