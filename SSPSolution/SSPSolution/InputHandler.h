#ifndef SSPAPPLICATION_CORE_INPUTHANDLER_H
#define SSPAPPLICATION_CORE_INPUTHANDLER_H

#include <SDL.h>
#include <DirectXMath.h>
#include <map>
class InputHandler
{
private:
	//DIMOUSESTATE m_DIMouseState;
	//DIMOUSESTATE m_oldDIMouseState;
	//unsigned char m_keyboardState[256];
	//unsigned char m_oldKeyboardState[256];
	const int KEYBOARD_STATE_NUM = SDL_NUM_SCANCODES;
	std::map<int, bool> m_oldKeyboardState;
	std::map<int, bool> m_keyboardState;
	struct MouseButtonState {
		bool left;
		bool right;
		bool middle;
		bool x1;
		bool x2;
	};
	MouseButtonState m_mouseButtonState;
	MouseButtonState m_oldMouseButtonState;
	int m_mouseX;
	int m_mouseY;
	int m_mouseDX;
	int m_mouseDY;
	int m_screenWidth;
	int m_screenHeight;
	int m_mouseWheelX;
	int m_mouseWheelY;

public:
	InputHandler();
	~InputHandler();
	void Initialize(int screenWidth, int screenHeight);
	void Shutdown();
	void Update();

	void SetMouseState(int button, bool state);
	void SetKeyState(int key, bool state);
	//Use SDL_SCANCODE_[key] where [key] is the key examined
	//Pressed means it is active but wasn't last frame
	bool IsKeyPressed(unsigned int key);
	//Down means it is active regardless of last frame
	bool IsKeyDown(unsigned int key);
	//Released means it isn't active but was last frame
	bool IsKeyReleased(unsigned int key);

	//0 = left, 1 = right, 2 = scroll click
	//Pressed means it is active but wasn't last frame
	bool IsMouseKeyPressed(unsigned int key);
	//Down means it is active regardless of last frame
	bool IsMouseKeyDown(unsigned int key);
	//Released means it isn't active but was last frame
	bool IsMouseKeyReleased(unsigned int key);

	void SetMousePos(int x, int y);
	void SetMouseWheel(int x, int y);
	void ApplyMouseWheel(int x, int y);

	DirectX::XMFLOAT2 GetMousePos();
	DirectX::XMFLOAT2 GetMouseDelta();
	DirectX::XMFLOAT2 GetMouseWheel();

private:
	void ReadKeyboard();
	void ReadMouse();
	void ProcessInput();

};

#endif