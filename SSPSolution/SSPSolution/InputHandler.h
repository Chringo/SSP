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
	DIMOUSESTATE m_oldDIMouseState;
	unsigned char m_keyboardState[256];
	unsigned char m_oldKeyboardState[256];

	int m_mouseX;
	int m_mouseY;
	int m_screenWidth;
	int m_screenHeight;
	unsigned int m_lastKeyPressed;

public:
	InputHandler();
	~InputHandler();
	void Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	void Update();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyPressed(unsigned int key);
	bool IsKeyDown(unsigned int key);
	bool IsKeyReleased(unsigned int key);

	//0 = left, 1 = right, 2 = scroll click, 3 = "Down button" on mouse
	bool IsMouseKeyPressed(unsigned int key);
	bool IsMouseKeyDown(unsigned int key);
	bool IsMouseKeyReleased(unsigned int key);

	void SetMousePos(int x, int y);

	DirectX::XMFLOAT2 GetMousePos();
	DirectX::XMFLOAT2 GetMousePosInWindow();

private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();
	DirectX::XMVECTOR GetMouseDeltaPos();

};

#endif