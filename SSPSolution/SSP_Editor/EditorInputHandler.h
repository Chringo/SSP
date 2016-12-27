#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <windows.h>
#include <Wincodec.h>
#include <qevent.h>
#include "Level.h"
#include "../GraphicsDLL/Camera.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../ResourceLib/ResourceHandler.h"
#include "Header.h"
#include "ui_SSP_Editor.h"

#pragma comment (lib,"GraphicsDLL-d.lib")
#pragma comment (lib,"ResourceLib.lib")
#pragma comment (lib,"physicsDLL.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

struct HasPicked
{
	int ID = 0;
	int listInstance = 0;
};

struct Mouse
{
	DIMOUSESTATE currentState;
	DIMOUSESTATE lastState;
	int x;
	int y;
	int lastX;
	int lastY;

	bool rightHeld = false;
	bool leftHeld = false;
};

struct TransformWidget
{
public:
	enum AXIS
	{
		NONE = -1,
		X,
		Y,
		Z,

		NUM_AXIS
	};
private:
	bool m_active = false;
	DirectX::XMVECTOR m_colors[4];
public:
	OBB selectedObjectOBB;
	OBB axisOBB[NUM_AXIS];
	DirectX::XMVECTOR * axisColors[NUM_AXIS];
	int SelectedModelIndex = NONE;
	unsigned int selectedAxis = NONE;
	

public:
	bool IsActive()
	{
		return m_active;
	};
	void setActive(bool active)
	{
		this->m_active = active;
	};
	void SelectObb(OBB &selectedObject)
	{
		selectedObjectOBB = selectedObject;

		

		for (int i = 0; i < NUM_AXIS; i++)
		{
			axisOBB[i].ort = selectedObject.ort;
			axisOBB[i].pos = selectedObject.pos;
			//relative to origin
			axisOBB[i].pos.m128_f32[i] += 1.f;

			//relative to object
			//axisOBB[i].pos.m128_f32[i] += selectedObject.ort.r[i].m128_f32[i] + .1f;
		}
	};

	void SelectAxis(int i)
	{
		axisColors[X] = &m_colors[X];
		axisColors[Y] = &m_colors[Y];
		axisColors[Z] = &m_colors[Z];

		if (i >= X && Z >= i)
		{
			selectedAxis = i;
			axisColors[i] = &m_colors[NUM_AXIS];
		}
		else
		{
			selectedAxis = NONE;
		}
	}

	TransformWidget()
	{
		for (int i = 0; i < NUM_AXIS; i++)
		{
			for (int j = 0; j < NUM_AXIS; j++)
			{
				axisOBB[i].ext[j] = 0.15;
			}
		}

		m_colors[X] = { 1.0, 0.0, 0.0, 0.0 };
		m_colors[Y] = { 0.0, 1.0, 0.0, 0.0 };
		m_colors[Z] = { 0.0, 0.0, 1.0, 0.0 };
		m_colors[NUM_AXIS] = { 1.0, 1.0, 1.0, 0.0 };
		
		axisColors[X] = &m_colors[X];
		axisColors[Y] = &m_colors[Y];
		axisColors[Z] = &m_colors[Z];

	};
};

struct PickRay
{
	DirectX::XMVECTOR origin, direction, localOrigin;
};

enum Bools {
	SHIFT = 0,
	ALT = 1,
	CONTROL = 2,
	W = 3,
	A = 4,
	S = 5,
	D = 6,
	C = 7,
	SPACE = 8,
	F = 9,
	NUMBOOLS
};


class EditorInputHandler
{
private:
	Mouse m_mouse;
	PickRay m_ray;
	int m_Width;
	int m_Height;

	bool m_KeysHeld[Bools::NUMBOOLS];
	
	QPoint m_point;
	Level* m_currentLevel;

	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	GraphicsHandler* m_GraphicsHandler;
	PhysicsHandler* m_PhysicsHandler;
	std::vector<Resources::Model*>* modelPtr;
	Camera* m_Camera;

	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIMouse;
private:
	void m_ProjectRay(int X, int Y);
	bool m_PickTransformWidget();
	bool m_PickObjectSelection();
public:
	//OBB m_Axis[3];
	TransformWidget transformWidget = TransformWidget();
	
	HasPicked m_Picked;
	HasPicked m_LastPicked;
	void detectInput(double dT, QKeyEvent* key);
	void SetMousePos(QPoint point) { this->m_point = point; };
	void KeyboardMovement(double dT);
	void MouseMovement(double dT);
	void MouseZoom(double dT);
	void CameraReset();
	void MousePicking();
	void keyReleased(QKeyEvent* evt);
	void UpdatePos(int index);
	void UpdateMouse();
	void ViewPortChanged(float height, float width);
	void mouseButtonDown(QMouseEvent* evt);
	void mouseButtonRelease(QMouseEvent * evt);
	void RotateObject(int direction);
	void MoveObject();



	EditorInputHandler(
		HINSTANCE handleInstance,
		HWND handle,
		Camera* camera,
		int,
		int,
		GraphicsHandler* graphicshandler,
		Level* m_currentLevel,
		std::vector<Resources::Model*>* modelPtr
	);
	~EditorInputHandler();
};