#include "InputHandler.h"

InputHandler::InputHandler()
{
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	int m_mouseDX = 0;
	int m_mouseDY = 0;
	this->m_screenWidth = 0;
	this->m_screenHeight = 0;
	this->m_mouseButtonState.left = 0;
	this->m_mouseButtonState.right = 0;
	this->m_mouseButtonState.middle = 0;
	this->m_oldMouseButtonState = this->m_mouseButtonState;
}

InputHandler::~InputHandler()
{
	this->Shutdown();
}

void InputHandler::Initialize(int screenWidth, int screenHeight)
{
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	int m_mouseDX = 0;
	int m_mouseDY = 0;
	this->m_mouseButtonState.left = 0;
	this->m_mouseButtonState.right = 0;
	this->m_mouseButtonState.middle = 0;
	this->m_oldMouseButtonState = this->m_mouseButtonState;
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
	//Copy the old data
	this->m_oldMouseButtonState = this->m_mouseButtonState;
	//Read the new data
	this->m_mouseButtonState.right = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
	this->m_mouseButtonState.left = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
	this->m_mouseButtonState.middle = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);

	int xPos = 0, yPos = 0;
	SDL_GetMouseState(&xPos, &yPos);
	this->m_mouseDX = xPos - this->m_mouseX;
	this->m_mouseDY = yPos - this->m_mouseY;
	this->m_mouseX = xPos;
	this->m_mouseY = yPos;
	return;
}

void InputHandler::ProcessInput()
{

	return;
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
	bool result = false;
	switch (key)
	{
	case 0: result = this->m_mouseButtonState.left && !this->m_oldMouseButtonState.left;
		break;
	case 1:	result = this->m_mouseButtonState.right && !this->m_oldMouseButtonState.right;
		break;
	case 2:	result = this->m_mouseButtonState.middle && !this->m_oldMouseButtonState.middle;
		break;
	default:
		break;
	}
	return result;
}

bool InputHandler::IsMouseKeyDown(unsigned int key)
{
	bool result = false;
	switch (key)
	{
	case 0: result = this->m_mouseButtonState.left;
		break;
	case 1:	result = this->m_mouseButtonState.right;
		break;
	case 2:	result = this->m_mouseButtonState.middle;
		break;
	default:
		break;
	}
	return result;
}

bool InputHandler::IsMouseKeyReleased(unsigned int key)
{
	bool result = false;
	switch (key)
	{
	case 0: result = !this->m_mouseButtonState.left && this->m_oldMouseButtonState.left;
		break;
	case 1:	result = !this->m_mouseButtonState.right && this->m_oldMouseButtonState.right;
		break;
	case 2:	result = !this->m_mouseButtonState.middle && this->m_oldMouseButtonState.middle;
		break;
	default:
		break;
	}
	return result;
}

void InputHandler::SetMousePos(int x, int y)
{
	this->m_mouseX = x;
	this->m_mouseY = y;
}

DirectX::XMFLOAT2 InputHandler::GetMousePos()
{
	return DirectX::XMFLOAT2(m_mouseX, m_mouseY);
}

DirectX::XMFLOAT2 InputHandler::GetMouseDelta()
{
	return DirectX::XMFLOAT2(this->m_mouseDX, this->m_mouseDY);
}
