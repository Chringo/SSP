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
	enum DIRECTION
	{
		Key_Left	= 0x01000012,
		Key_Up		= 0x01000013,
		Key_Right	= 0x01000014,
		Key_Down	= 0x01000015,
		Key_0		= 0x30,
	};
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
	void Update();
	bool NeedsUpdate();

	Container * GetSelected();
	bool HasSelection();
	void SetSelection(bool selection);
	void SetSelectedContainer(Container* selection);
	void SetActiveAxis(int axis);

	const unsigned int GetModelID();
	const unsigned int GetInstanceID();
	void GetSelectionRenderComponents(
		OBB*& axisOBBs,
		DirectX::XMVECTOR*& axisOBBpos,
		DirectX::XMVECTOR**& axisColors,
		OBB*& objectOBB,
		DirectX::XMVECTOR*& objectColor);
	

	void ProjectRay(int X, int Y);
	void RotateObject(int direction);
	void MoveObject();
	bool PickTransformWidget();
	bool PickObjectSelection();

private:
	PhysicsHandler* m_PhysicsHandler;
	std::vector<Resources::Model*>* m_modelPtr;

	bool m_IsDirty = true;
	PickRay m_ray;
	Camera * m_Camera;
	TransformWidget m_transformWidget = TransformWidget();
	int m_Width;
	int m_Height;
	Level * m_currentLevel;
};



#endif