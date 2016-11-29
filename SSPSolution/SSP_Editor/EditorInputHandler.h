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
	struct DIMOUSESTATES
	{
		LONG IX;
		LONG IY;
		LONG IZ;
		BYTE rgbButtons[4];
	};
	int m_Width;
	int m_Height;
	DirectX::XMFLOAT3 m_PreviousPos;
	HWND m_hwnd;
	Camera* m_Camera;
	DIMOUSESTATES		 m_mouseLastState;
	LPDIRECTINPUT8		 m_directInput;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
public:
	EditorInputHandler(HINSTANCE handleInstance, HWND handle, Camera* camera, int, int);
	~EditorInputHandler();
	void detectInput(double dT);
};