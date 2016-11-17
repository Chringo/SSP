#ifndef SSPAPPLICATION_CORE_INPUTHANDLER_H
#define SSPAPPLICATION_CORE_INPUTHANDLER_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>

class InputHandler
{
private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_DIKeyboard;
	IDirectInputDevice8* m_DIMouse;
	DIMOUSESTATE m_DIMouseState;
	DIMOUSESTATE m_OldDIMouseState;
	unsigned char m_KeyboarState[256];
	unsigned char m_OldKeyboardState[256];

	int m_mouseX;
	int m_mouseY;
	int m_screenWidth;
	int m_screenHeight;
	unsigned int m_lastKeyPressed;
	bool wasAnyKeyPressed;

public:
	InputHandler();
	~InputHandler();


private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();
	DirectX::XMVECTOR GetMouseDeltaPos();

};

#endif