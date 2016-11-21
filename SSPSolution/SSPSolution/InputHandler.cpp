#include "InputHandler.h"

InputHandler::InputHandler()
{
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	this->m_screenWidth = 0;
	this->m_screenHeight = 0;
	this->m_lastKeyPressed = -1;
}

InputHandler::~InputHandler()
{
	this->Shutdown();
}

void InputHandler::Initialize(int screenWidth, int screenHeight)
{
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	//Save the resolution for future use
	this->m_screenWidth = screenWidth;
	this->m_screenHeight = screenHeight;
}

void InputHandler::Shutdown()
{
}

void InputHandler::Update()
{

	//Check if we can read the devices. 
	//If we cant, the old data will be used

	this->ReadKeyboard();
	this->ReadMouse();

	this->ProcessInput();

}

void InputHandler::SetKeyState(int key, bool state)
{
	this->m_keyboardState[key] = state;
}

void InputHandler::ReadKeyboard()
{
	//Copy the old data
	this->m_oldKeyboardState = this->m_keyboardState;
	return;
}

void InputHandler::ReadMouse()
{
	this->m_oldMouseButtonState = this->m_mouseButtonState;

	this->m_mouseButtonState.right = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	this->m_mouseButtonState.left = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	this->m_mouseButtonState.middle = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	return;
}

void InputHandler::ProcessInput()
{
	return;
}

DirectX::XMVECTOR InputHandler::GetMouseDeltaPos()
{
	return DirectX::XMVectorSet(float(this->m_mouseX), float(this->m_mouseY), 0, 0);	//z,y is not used so set to 0
}

bool InputHandler::IsKeyDown(unsigned int key)
{
	if (key > 0 && key < SDL_NUM_SCANCODES)
	{
		if (this->m_keyboardState[key]) {
			return true;
		}
	}
	return false;
}

bool InputHandler::IsKeyPressed(unsigned int key)
{
	if (key > 0 && key < SDL_NUM_SCANCODES)
	{
		if (!this->m_oldKeyboardState[key] && this->m_keyboardState[key]) {
			return true;
		}
	}

	return false;
}

bool InputHandler::IsKeyReleased(unsigned int key)
{
	if (key > 0 && key < SDL_NUM_SCANCODES)
	{
		if (this->m_oldKeyboardState[key] && !this->m_keyboardState[key]) {
			return true;
		}
	}

	return false;
}

bool InputHandler::IsMouseKeyPressed(unsigned int key)
{
	/*if (!this->m_oldDIMouseState.rgbButtons[key] && m_DIMouseState.rgbButtons[key]) {
		return true;
	}*/

	return false;
}

bool InputHandler::IsMouseKeyDown(unsigned int key)
{
	/*if (this->m_DIMouseState.rgbButtons[key]) {
		return true;
	}*/

	return false;
}

bool InputHandler::IsMouseKeyReleased(unsigned int key)
{
	/*if (this->m_oldDIMouseState.rgbButtons[key] && !this->m_DIMouseState.rgbButtons[key]) {
		return true;
	}*/

	return false;
}

void InputHandler::SetMousePos(int x, int y)
{
	this->m_mouseX = x;
	this->m_mouseY = y;
}

DirectX::XMFLOAT2 InputHandler::GetMousePos()
{

	return DirectX::XMFLOAT2(float(0), float(0));;
}

DirectX::XMFLOAT2 InputHandler::GetMousePosInWindow()
{
	DirectX::XMFLOAT2 mousePos = this->GetMousePos();
	//Move the cords to the window

	return DirectX::XMFLOAT2(m_mouseX, m_mouseY);
}
