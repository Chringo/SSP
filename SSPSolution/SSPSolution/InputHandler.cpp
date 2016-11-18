#include "InputHandler.h"

InputHandler::InputHandler()
{
	this->m_DIKeyboard = nullptr;
	this->m_DIMouse = nullptr;
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	this->m_screenWidth = 0;
	this->m_screenHeight = 0;
	this->m_lastKeyPressed = -1;

	for (int i = 0; i < 256; i++) {
		this->m_keyboardState[i] = this->m_oldKeyboardState[i] = false;
	}

}

InputHandler::~InputHandler()
{
	this->Shutdown();
}

void InputHandler::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT hr;
	//Mouse initial starting position set to the midle of the screen
	//SetPhysicalCursorPos((GetSystemMetrics(SM_CXSCREEN)) / 2, (GetSystemMetrics(SM_CYSCREEN)) / 2);
	//this->m_mouseX = (GetSystemMetrics(SM_CXSCREEN)) / 2;
	//this->m_mouseY = (GetSystemMetrics(SM_CYSCREEN)) / 2;
	this->m_mouseX = 0;
	this->m_mouseY = 0;
	//Save the resolution for future use
	this->m_screenWidth = screenWidth;
	this->m_screenHeight = screenHeight;

	//Initialize the DirectInput Object
	hr = DirectInput8Create(
		hInstance,					// Instance handle to the application
		DIRECTINPUT_VERSION,		//	dwVersion
		IID_IDirectInput8,			//	RefiID, Unique identifier of the desired interface
		(void**)&this->m_directInput,	//	The pointer who will recive the object
		NULL);						//	LPUNKNOWN punkOuter

									//Check if the object has been created
	if (FAILED(hr)) {
		MessageBox(hwnd, L"DirectInput8Create", L"Error", MB_OK);
	}

	//Create the keyboard device
	hr = this->m_directInput->CreateDevice(
		GUID_SysKeyboard,					// What type of device
		&this->m_DIKeyboard,					// Pointer who will recive the object
		NULL);								//	LPUNKNOWN punkOuter

											//Check if the keyboard object has been created
	if (SUCCEEDED(hr)) {

		//Set the options for the keyboard
		hr = this->m_DIKeyboard->SetDataFormat(&c_dfDIKeyboard);	// Data format of the device

		if (FAILED(hr)) {
			MessageBox(hwnd, L"SetDataFormat_Keyboard", L"Error", MB_OK);
		}

		hr = this->m_DIKeyboard->SetCooperativeLevel(
			hwnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
			);

		if (FAILED(hr)) {
			MessageBox(hwnd, L"SetCooperativeLevel_Keyboard", L"Error", MB_OK);
		}

		hr = this->m_DIKeyboard->Acquire();	//Aquire the keyboard

		if (FAILED(hr)) {
			MessageBox(hwnd, L"Acquire_Keyboard", L"Error", MB_OK);
		}

	}
	else {
		MessageBox(hwnd, L"CreateDevice_Keyboard", L"Error", MB_OK);
	}

	//Create the Mouse mouse device
	hr = this->m_directInput->CreateDevice(
		GUID_SysMouse,
		&this->m_DIMouse,
		NULL);

	//Check if the mouse object has been created
	if (SUCCEEDED(hr)) {

		//Set the options for the mouse
		hr = this->m_DIMouse->SetDataFormat(&c_dfDIMouse);	// Data format of the device

		if (FAILED(hr)) {
			MessageBox(hwnd, L"SetDataFormat_Mouse", L"Error", MB_OK);
		}

		hr = this->m_DIMouse->SetCooperativeLevel(
			hwnd,
			DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND
			);

		if (FAILED(hr)) {
			MessageBox(hwnd, L"SetCooperativeLevel_Mouse", L"Error", MB_OK);
		}

		hr = this->m_DIMouse->Acquire();	//Aquire the mouse

		if (FAILED(hr)) {
			MessageBox(hwnd, L"Acquire_Mouse", L"Error", MB_OK);
		}

	}
	else {
		MessageBox(hwnd, L"CreateDevice_Mouse", L"Error", MB_OK);
	}

	return;
}

void InputHandler::Shutdown()
{
	if (this->m_DIKeyboard) {
		this->m_DIKeyboard->Unacquire();
		this->m_DIKeyboard->Release();
		this->m_DIKeyboard = nullptr;
	}

	if (this->m_DIMouse) {
		this->m_DIMouse->Unacquire();
		this->m_DIMouse->Release();
		this->m_DIMouse = nullptr;
	}

	if (this->m_directInput) {
		this->m_directInput->Release();
	}
}

void InputHandler::Update()
{

	//Check if we can read the devices. 
	//If we cant, the old data will be used

	this->ReadKeyboard();
	this->ReadMouse();

	this->ProcessInput();

}

void InputHandler::ReadKeyboard()
{
	HRESULT hr;
	//Copy the old data
	for (int i = 0; i < 256; i++) {
		this->m_oldKeyboardState[i] = this->m_keyboardState[i];
	}
	//Read the keyboard device
	hr = this->m_DIKeyboard->GetDeviceState(
		sizeof(this->m_keyboardState),
		(LPVOID)&this->m_keyboardState);

	if (FAILED(hr)) {

		//If the keyboard lost focus or was not acquired, try to get control back
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			this->m_DIKeyboard->Acquire();
		}

	}

	return;
}

void InputHandler::ReadMouse()
{
	HRESULT hr;
	this->m_oldDIMouseState = this->m_DIMouseState;

	//Read the mouse device
	hr = this->m_DIMouse->GetDeviceState(
		sizeof(DIMOUSESTATE),
		&this->m_DIMouseState);

	if (FAILED(hr)) {

		//If the mouse lost focus or was not acquired, try to get control back
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED) {
			this->m_DIMouse->Acquire();
		}

	}


	return;
}

void InputHandler::ProcessInput()
{

	this->m_mouseX += this->m_DIMouseState.lX;
	this->m_mouseY += this->m_DIMouseState.lY;

	//Update the mouse position (not physicly)

	//if (this->m_mouseX < 0) {
	//	this->m_mouseX = 0;
	//}

	//if (this->m_mouseX > this->m_screenWidth) {
	//	this->m_mouseX = this->m_screenWidth;
	//}

	//if (this->m_mouseY < 0) {
	//	this->m_mouseY = 0;
	//}

	//if (this->m_mouseY > this->m_screenHeight) {
	//	this->m_mouseY = this->m_screenHeight;
	//}
}

DirectX::XMVECTOR InputHandler::GetMouseDeltaPos()
{
	return DirectX::XMVectorSet(float(this->m_DIMouseState.lX), float(this->m_DIMouseState.lY), 0, 0);	//z,y is not used so set to 0
}

void InputHandler::KeyDown(unsigned int key)
{
	this->m_keyboardState[key] = true;

	return;
}

void InputHandler::KeyUp(unsigned int key)
{
	this->m_keyboardState[key] = false;
	this->m_lastKeyPressed = key;

	return;
}

bool InputHandler::IsKeyDown(unsigned int key)
{
	if (this->m_keyboardState[key]) {
		return true;
	}

	return false;
}

bool InputHandler::IsKeyPressed(unsigned int key)
{
	if (!this->m_oldKeyboardState[key] && this->m_keyboardState[key]) {
		return true;
	}

	return false;
}

bool InputHandler::IsKeyReleased(unsigned int key)
{
	if (this->m_oldKeyboardState[key] && !this->m_keyboardState[key]) {
		return true;
	}

	return false;
}

bool InputHandler::IsMouseKeyPressed(unsigned int key)
{
	if (!this->m_oldDIMouseState.rgbButtons[key] && m_DIMouseState.rgbButtons[key]) {
		return true;
	}

	return false;
}

bool InputHandler::IsMouseKeyDown(unsigned int key)
{
	if (this->m_DIMouseState.rgbButtons[key]) {
		return true;
	}

	return false;
}

bool InputHandler::IsMouseKeyReleased(unsigned int key)
{
	if (this->m_oldDIMouseState.rgbButtons[key] && !this->m_DIMouseState.rgbButtons[key]) {
		return true;
	}

	return false;
}

void InputHandler::SetMousePos(int x, int y)
{
	this->m_mouseX = x;
	this->m_mouseY = y;
}

DirectX::XMFLOAT2 InputHandler::GetMousePos()
{
	POINT tempP;
	GetCursorPos(&tempP);

	return DirectX::XMFLOAT2(float(tempP.x), float(tempP.y));;
}

DirectX::XMFLOAT2 InputHandler::GetMousePosInWindow()
{
	DirectX::XMFLOAT2 mousePos = this->GetMousePos();
	//Move the cords to the window
	float m_mouseX = mousePos.x - (GetSystemMetrics(SM_CXSCREEN) - m_screenWidth) / 2;
	float m_mouseY = mousePos.y - (GetSystemMetrics(SM_CYSCREEN) - m_screenHeight) / 2;

	return DirectX::XMFLOAT2(m_mouseX, m_mouseY);
}
