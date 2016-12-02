#pragma once
#include <dinput.h>
#include <dinputd.h>
#include <windows.h>
#include <Wincodec.h>
#include "../GraphicsDLL/Camera.h"


#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class EditorInputHandler
{
private:
	int m_Width;
	int m_Height;
	int m_MouseX;
	int m_MouseY;

	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	Camera* m_Camera;
	DIMOUSESTATE		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
public:
	EditorInputHandler(HINSTANCE handleInstance, HWND handle, Camera* camera, int, int);
	~EditorInputHandler();
	void detectInput(double dT);
};