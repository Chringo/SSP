#pragma once
#ifndef SSPEDITOR_SELECTIONHANDLER_SELECTIONHANDLER_H
#define SSPEDITOR_SELECTIONHANDLER_SELECTIONHANDLER_H
#include "Header.h"
#include "TransformWidget.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "Level.h"

class SelectionHandler
{
public:
	struct PickRay
	{
		DirectX::XMVECTOR origin, direction, localOrigin;
	};

private:
	OBB m_ConvertOBB(BoundingBoxHeader & boundingBox, Container * instancePtr);


public:
	SelectionHandler();
	~SelectionHandler();

	void Initialize(Camera * camera,
		int winWidth,
		int winHeight,
		Level * currentLevel,
		std::vector<Resources::Model*>* modelPtr);
	void updateWindowSize(int winHeight, int winWidth);
	static SelectionHandler* GetInstance();

	Container * GetSelected();
	TransformWidget * GetTransformWidget();

	const unsigned int GetModelID();
	const unsigned int GetInstanceID();

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
	TransformWidget transformWidget = TransformWidget();
	int m_Width;
	int m_Height;
	Level * m_currentLevel;
};



#endif