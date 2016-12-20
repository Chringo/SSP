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

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

struct HasPicked
{
	int ID = 0;
	int listInstance = 0;
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
	int m_Width;
	int m_Height;
	int m_MouseX;
	int m_MouseY;
	int m_LastMouseX;
	int m_LastMouseY;
	bool m_KeysHeld[Bools::NUMBOOLS];
	QPoint m_point;
	Level* currentLevel;

	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	GraphicsHandler* m_GraphicsHandler;
	PhysicsHandler* m_PhysicsHandler;
	std::vector<Resources::Model*>* modelPtr;
	Camera* m_Camera;

	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIMouse;

public:
	OBB m_Axis[3];
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




	EditorInputHandler(
		HINSTANCE handleInstance,
		HWND handle,
		Camera* camera,
		int,
		int,
		GraphicsHandler* graphicshandler,
		Level* currentLevel,
		std::vector<Resources::Model*>* modelPtr
	);
	~EditorInputHandler();
};