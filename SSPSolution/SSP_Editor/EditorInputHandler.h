#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <windows.h>
#include <Wincodec.h>
#include "../GraphicsDLL/Camera.h"
#include "../GraphicsDLL/GraphicsHandler.h"

#include "../ResourceLib/ResourceHandler.h"
#include "EditorCommunicator.h"
#pragma comment (lib,"../Debug/ResourceLib")

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
struct HasPicked
{
	int ID = 0;
	int listInstance = 0;
	int pickedObject = 0;
};
class EditorInputHandler
{
private:
	int m_Width;
	int m_Height;
	int m_MouseX;
	int m_MouseY;

	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	GraphicsHandler* m_GraphicsHandler;
	Communicator* m_Communicator;
	HasPicked m_Picked;
	Camera* m_Camera;
	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	float Intersection(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection);
	bool checkPointInTriangle(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& triangleV1, const DirectX::XMVECTOR& triangleV2, const DirectX::XMVECTOR& triangleV3);
public:
	EditorInputHandler(HINSTANCE handleInstance, HWND handle, Camera* camera, int, int, GraphicsHandler* graphicshandler, Communicator* com);
	~EditorInputHandler();
	void detectInput(double dT);
};