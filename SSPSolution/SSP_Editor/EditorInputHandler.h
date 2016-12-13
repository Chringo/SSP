#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <windows.h>
#include <Wincodec.h>
#include "../GraphicsDLL/Camera.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../physicsDLL/PhysicsHandler.h"
#include "../ResourceLib/ResourceHandler.h"
#include "Header.h"
#pragma comment (lib,"../Debug/ResourceLib")

#pragma comment (lib,"../Debug/physicsDLL.lib")

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

struct HasPicked
{
	int ID = 0;
	int listInstance = 0;
};
class EditorInputHandler
{
private:
	int m_Width;
	int m_Height;
	int m_MouseX;
	int m_MouseY;
	bool isHeld;


	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	GraphicsHandler* m_GraphicsHandler;
	PhysicsHandler* m_PhysicsHandler;
	std::unordered_map<unsigned int, std::vector<Container>>* m_Map;
	std::vector<Resources::Model*>* modelPtr;
	HasPicked m_Picked;
	Camera* m_Camera;
	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
public:
	EditorInputHandler(
		HINSTANCE handleInstance,
		HWND handle,
		Camera* camera,
		int,
		int,
		GraphicsHandler* graphicshandler,
		std::unordered_map<unsigned int, std::vector<Container>>* map,
		std::vector<Resources::Model*>* modelPtr
	);
	~EditorInputHandler();
	void detectInput(double dT);
};