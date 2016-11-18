#ifndef SSPAPPLICATION_CORE_INPUTHANDLER_H
#define SSPAPPLICATION_CORE_INPUTHANDLER_H

#include <SDL.h>
#include <DirectXMath.h>
class InputHandler
{
private:
	//DIMOUSESTATE m_DIMouseState;
	//DIMOUSESTATE m_oldDIMouseState;
	unsigned char m_keyboardState[256];
	unsigned char m_oldKeyboardState[256];
	Uint8* m_keyboardState;
	Uint8* m_oldKeyboardState;

	int m_mouseX;
	int m_mouseY;
	int m_screenWidth;
	int m_screenHeight;
	unsigned int m_lastKeyPressed;

public:
	InputHandler();
	~InputHandler();
	void Initialize(int screenWidth, int screenHeight);
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