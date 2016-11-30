#include "EditorInputHandler.h"

EditorInputHandler::EditorInputHandler(HINSTANCE handleInstance, HWND handle, Camera* camera, int w, int h)
{
	this->m_Width = w;
	this->m_Height = h;
	HRESULT hr = DirectInput8Create(
		handleInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput,
		NULL);

	hr = m_directInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	hr = m_directInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(handle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	this->m_hwnd = handle;
	this->m_Camera = camera;
	this->m_PreviousPos = camera->GetCameraPos();
}

EditorInputHandler::~EditorInputHandler()
{
}

void EditorInputHandler::detectInput(double dT)
{

	DIMOUSESTATES mouseCurrentState;

	BYTE keyBoardState[256]; // the amount of buttons a char array of 256.

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATES), &mouseCurrentState);

	DIKeyboard->GetDeviceState(sizeof(keyBoardState), (LPVOID)&keyBoardState);

	float speed = 15.0f * dT;
	int result = 1;
	float translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;
	float yaw = 0, pitch = 0;

	if (keyBoardState[DIK_LSHIFT] & 0x80)
	{
		if (keyBoardState[DIK_W] & 0x80)
		{
			translateCameraZ += speed;
		}
		if (keyBoardState[DIK_S] & 0x80)
		{
			translateCameraZ -= speed;
		}
		if (keyBoardState[DIK_D] & 0x80)
		{
			translateCameraX += speed;
		}
		if (keyBoardState[DIK_A] & 0x80)
		{
			translateCameraX -= speed;
		}
		if (keyBoardState[DIK_C] & 0x80)
		{
			translateCameraY -= speed;
		}
		if (keyBoardState[DIK_SPACE] & 0x80)
		{
			translateCameraY += speed;
		}
		if (mouseCurrentState.rgbButtons[0])
		{
			DirectX::XMVECTOR rayOrigin, rayDirection;
			POINT mousePos;
			GetCursorPos(&mousePos);
			ScreenToClient(this->m_hwnd, &mousePos);
			int mouseX = mousePos.x;
			int mouseY = mousePos.y;
			if (mouseX > m_Width)
				mouseX = m_Width;
			else if (mouseX < 0)
				mouseX = 0;
			if (mouseY > m_Height)
				mouseY = m_Height;
			else if (mouseY < 0)
				mouseY = 0;

			DirectX::XMVECTOR localRayDirection = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMVECTOR LocalRayOrigin = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			float X, Y, Z;
			DirectX::XMFLOAT4X4 camProjection;
			float fieldOfView = (float)DirectX::XM_PI / 4.0f;
			float screenAspect = (float)m_Width / (float)m_Height;
			
			DirectX::XMStoreFloat4x4(
				&camProjection,
				DirectX::XMMatrixPerspectiveFovLH(
					fieldOfView,
					screenAspect,
					0.1f,
					1000.0f)
			);
			
			//Transform 2D pick position on screen space to 3D ray in View space
			X = (((2.0f * mouseX) / m_Width) - 1) / camProjection._11;
			Y = -(((2.0f * mouseY) / m_Height) - 1) / camProjection._22;
			Z = 1.0f;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

			localRayDirection = DirectX::XMVectorSet(X, Y, Z, 0.0f);

			DirectX::XMMATRIX inverseCamView;
			DirectX::XMVECTOR det = { 1,1,1,1 };
			DirectX::XMMATRIX temp;
			this->m_Camera->GetBaseViewMatrix(temp);

			inverseCamView = DirectX::XMMatrixInverse(&det, temp);

			rayOrigin = XMVector3TransformCoord(LocalRayOrigin, inverseCamView);
			rayDirection = DirectX::XMVector3TransformNormal(localRayDirection, inverseCamView);
			rayDirection = DirectX::XMVector3Normalize(rayDirection);

			DirectX::XMFLOAT4 test;
			DirectX::XMStoreFloat4(&test, rayOrigin);
			this->m_Camera->SetLookAt(test);




			//if ((mouseCurrentState.IX != m_mouseLastState.IX) || (mouseCurrentState.IY != m_mouseLastState.IY))
			//{
			//	yaw += m_mouseLastState.IX * 0.001f * dT;
			//
			//	pitch += mouseCurrentState.IY * 0.001f * dT;
			//
			//	m_mouseLastState = mouseCurrentState;
			//}
		}
		if ((translateCameraY || translateCameraZ || translateCameraX || yaw || pitch))
		{
			DirectX::XMFLOAT3 posTranslation = 
				DirectX::XMFLOAT3(
					float(translateCameraX),
					float(translateCameraY),
					float(translateCameraZ)
				);

			this->m_Camera->AddToCameraPos(posTranslation);
			this->m_Camera->AddToLookAt(posTranslation);

				//float rotationAmount = DirectX::XM_PI / 8;
				//DirectX::XMFLOAT4 newRotation = 
				//	DirectX::XMFLOAT4(
				//		yaw,
				//		pitch,
				//		0.0f,
				//		DirectX::XMScalarCos(rotationAmount / 2.0f)
				//	);
				//
				//this->m_Camera->SetRotation(newRotation);
			this->m_Camera->Update();
			this->m_PreviousPos = this->m_Camera->GetCameraPos();
		}

	}
}
