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
#include "SelectionHandler.h"

#pragma comment (lib,"GraphicsDLL-d.lib")
#pragma comment (lib,"ResourceLib.lib")
#pragma comment (lib,"physicsDLL.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


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
	int m_Width;
	int m_Height;

	bool m_KeysHeld[Bools::NUMBOOLS];
	
	QPoint m_point;
	Level* m_currentLevel;

	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	Camera* m_Camera;

	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIMouse;

public:


	void detectInput(double dT, QKeyEvent* key);
	void SetMousePos(QPoint point) { this->m_point = point; };
	void KeyboardMovement(double dT);
	void MouseMovement(double dT);
	void MouseZoom(double dT);
	void CameraReset();
	void keyReleased(QKeyEvent* evt);
	void UpdateMouse();
	void ViewPortChanged(float height, float width);
	void mouseButtonDown(QMouseEvent* evt);
	void mouseButtonRelease(QMouseEvent * evt);



	EditorInputHandler(
		HINSTANCE handleInstance,
		HWND handle,
		Camera* camera,
		int,
		int
	);
	~EditorInputHandler();
};