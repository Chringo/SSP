#pragma once
#ifndef SSPEDITOR_SELECTIONHANDLER_SELECTIONHANDLER_H
#define SSPEDITOR_SELECTIONHANDLER_SELECTIONHANDLER_H
#include "Header.h"
#include "TransformWidget.h"
#include "../physicsDLL/PhysicsHandler.h"

class SelectionHandler
{
public:
	struct PickRay
	{
		DirectX::XMVECTOR origin, direction, localOrigin;
	};

private:



public:
	SelectionHandler();
	~SelectionHandler();

	void Initialize(Camera * camera, int winWidth, int winHeight, PhysicsHandler* physicsHandler);
	static SelectionHandler* GetInstance();
	void SelectObject(Container * container, unsigned int modelID, OBB obb);
	void UpdateSelected(
		DirectX::XMVECTOR position = { 0.0 },
		DirectX::XMVECTOR rotation = { 0.0 },
		DirectX::XMVECTOR scale = { 0.0 });
	Container * GetSelected();
	TransformWidget * GetTransformWidget();

	void ProjectRay(int X, int Y);
	void RotateObject(int direction);
	void MoveObject();
	bool PickTransformWidget();
	bool PickObjectSelection();

private:
	PhysicsHandler* m_PhysicsHandler;
	std::vector<Resources::Model*>* m_modelPtr;

	PickRay m_ray;
	Camera * m_Camera;
	TransformWidget transformWidget;
	int m_Width;
	int m_Height;

};



#endif