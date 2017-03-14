#ifndef GRAPHICSDLL_GRAPHICSCOMPONENT_H
#define GRAPHICSDLL_GRAPHICSCOMPONENT_H

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <string>
#include "../ResourceLib/Model.h"

struct GraphicsComponent
{
	int active = 0;
	unsigned int modelID = 0;
	Resources::Model* modelPtr = nullptr;
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX ort;
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 extensions;
	GraphicsComponent() {}
	GraphicsComponent(const GraphicsComponent& a) {

		this->active	  = a.active;
		this->modelID     = a.modelID;
		this->modelPtr    = a.modelPtr;
		this->worldMatrix = a.worldMatrix;
		this->ort		  = a.ort;
		this->pos		  = a.pos;
		this->extensions  = a.extensions;
	} // user-defined copy ctor
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

};

struct GraphicsAnimationComponent : GraphicsComponent
{
	int jointCount = 0;
	DirectX::XMMATRIX finalJointTransforms[22];

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) {	_aligned_free(p); };
};

struct UIComponent
{
	int active = 0;
	int spriteID = -1;
	bool wasClicked = false;
	bool isHovered = false;
	DirectX::XMFLOAT2 position = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 origin = DirectX::XMFLOAT2(0.0f, 0.0f);
	DirectX::XMFLOAT2 size = DirectX::XMFLOAT2(10.0f, 10.0f);
	float scale = 1.f;
	float rotation = 0.f;
	float layerDepth = 1.f;

	void ResetValuesToDefault()
	{
		this->active = 0;
		this->spriteID = -1;
		this->wasClicked = false;
		this->isHovered = false;
		this->position = DirectX::XMFLOAT2(0.0f, 0.0f);
		this->size = DirectX::XMFLOAT2(10.0f, 10.0f);
		this->scale = 1.f;
		this->rotation = 0.f;
		this->layerDepth = 1.f;
	}
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
	DirectX::XMFLOAT2 origin = DirectX::XMFLOAT2(0.f, 0.f);
	float rotation = 0.f;
	float layerDepth = 0.f;
	bool useBlackText = false;

	void ResetValuesToDefault()
	{
		this->active = 0;
		this->text = L"";
		this->position = DirectX::XMFLOAT2(0.f, 0.f);
		this->scale = DirectX::XMFLOAT2(1.f, 1.f);
		this->origin = DirectX::XMFLOAT2(0.f, 0.f);
		this->rotation = 0.f;
		this->layerDepth = 0.f;
		this->useBlackText = false;
	}
};

#endif