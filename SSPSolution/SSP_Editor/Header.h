#pragma once
#include <DirectXMath.h>
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../AIDLL/AIComponent.h"
#include "LevelHeaders.h"
struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total värde. 
	DirectX::XMVECTOR rotation; // Total värde. 
	GraphicsComponent component;
	AIComponent* aiComponent = nullptr;
	bool isDirty  = false;
	bool isStatic = true;
};

struct AiContainer 
{
	OBB obb;
	
	AIComponent aiComponent;
	AiContainer()
	{
		obb.ort.r[0] = { 1.0f,0.0f,0.0f};
		obb.ort.r[1] = { 0.0f,1.0f,0.0f };
		obb.ort.r[2] = { 0.0f,0.0f,1.0f };
		obb.ext[0] = 0.2f;
		obb.ext[1] = 0.2f;
		obb.ext[2] = 0.2f;
	}
};

struct CheckpointContainer
{
	OBB obb;
	DirectX::XMVECTOR position;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR scale;


	LevelData::CheckpointHeader checkpointHeader;
	CheckpointContainer()
	{
		position = { 0.0,0.0,0.0 };
		rotation = { 0.0,0.0,0.0 };
		scale = { 1.0, 1.0, 1.0 };

		obb.ort.r[0] = { 1.0f,0.0f,0.0f };
		obb.ort.r[1] = { 0.0f,1.0f,0.0f };
		obb.ort.r[2] = { 0.0f,0.0f,1.0f };
		obb.ext[0] = 0.2f;
		obb.ext[1] = 0.2f;
		obb.ext[2] = 0.2f;
	}
};