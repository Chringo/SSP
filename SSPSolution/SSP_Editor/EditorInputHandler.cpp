#include "EditorInputHandler.h"


EditorInputHandler::EditorInputHandler(
	HINSTANCE handleInstance,
	HWND handle,
	Camera* camera,
	int w,
	int h,
	GraphicsHandler* graphicshandler,
	std::unordered_map<unsigned int, std::vector<Container>>* map,
	std::vector<Resources::Model*>* modelPtr)
{
	this->m_Width = w;
	this->m_Height = h;
	this->m_GraphicsHandler = graphicshandler;
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
	this->m_Map = map;
	this->modelPtr = modelPtr;
	this->m_hwnd = handle;
	this->m_Camera = camera;
	this->m_PreviousPos = camera->GetCameraPos();
	for (size_t i = 0; i < NUMBOOLS; i++)
	{
		this->m_KeysHeld[i] = false;
	}
	DIMouse->Acquire();
}

EditorInputHandler::~EditorInputHandler()
{
}

void EditorInputHandler::KeyboardMovement(double dT)
{
	float speed = 2.0f * dT;
	float translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;

	if (this->m_KeysHeld[Bools::W] == true)
		translateCameraZ += speed;

	if (this->m_KeysHeld[Bools::A] == true)
		translateCameraX -= speed;
	if (this->m_KeysHeld[Bools::S] == true)
		translateCameraZ -= speed;
	if (this->m_KeysHeld[Bools::D] == true)
		translateCameraX += speed;
	if (this->m_KeysHeld[Bools::C] == true)
		translateCameraY -= speed;
	if (this->m_KeysHeld[Bools::SPACE] == true)
		translateCameraY += speed;
	if (this->m_KeysHeld[ALT] == true)
		MouseMovement(dT);
	if (this->m_KeysHeld[SHIFT] == true)
		MouseZoom(dT);

	if ((translateCameraY || translateCameraZ || translateCameraX))
	{
		DirectX::XMFLOAT3 posTranslation =
			DirectX::XMFLOAT3(
				float(translateCameraX),
				float(translateCameraY),
				float(translateCameraZ)
			);

		this->m_PreviousPos = this->m_Camera->GetCameraPos();
		this->m_Camera->ApplyLocalTranslation(
			float(translateCameraX),
			float(translateCameraY),
			float(translateCameraZ)
		);
		this->m_Camera->Update();
	}
}

void EditorInputHandler::MouseMovement(double dT)
{
	DIMOUSESTATE mouseCurrentState;

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);
	float pitch = 0;
	float yaw = 0;

		if (mouseCurrentState.rgbButtons[0])
		{
			if ((mouseCurrentState.lX != m_mouseLastState.lX) || (mouseCurrentState.lY != m_mouseLastState.lY))
			{
				pitch += mouseCurrentState.lX * 0.01f;
	
				yaw += mouseCurrentState.lY * 0.01f;
	
				// Ensure the mouse location doesn't exceed the screen width or height.
				if (m_MouseX < 0) { m_MouseX = 0; }
				if (m_MouseY < 0) { m_MouseY = 0; }
	
				if (m_MouseX > m_Width) { m_MouseX = m_Width; }
				if (m_MouseY > m_Height) { m_MouseY = m_Height; }
	
				m_mouseLastState = mouseCurrentState;
			}
		}

	if ((yaw || pitch))
	{
		float rotationAmount = DirectX::XM_PI / 8;
	
		DirectX::XMFLOAT4 newRotation =
			DirectX::XMFLOAT4(
				yaw * DirectX::XMScalarSin(rotationAmount / 2.0f),
				pitch * DirectX::XMScalarSin(rotationAmount / 2.0f),
				0.0f,
				DirectX::XMScalarCos(rotationAmount / 2.0f)
			);
	
		this->m_Camera->SetRotation(newRotation);
		this->m_Camera->Update();
	}

}

void EditorInputHandler::MouseZoom(double dT)
{
	DIMOUSESTATE mouseCurrentState;
	
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);

	float speedrot = 2.5f * dT;
	float translateCameraZ = 0;

	if (mouseCurrentState.rgbButtons[0])
	{
		if (mouseCurrentState.lY < 0)
		{
			translateCameraZ += speedrot;
		}
		if (mouseCurrentState.lY > 0)
		{
			translateCameraZ -= speedrot;
		}
	
	}
	if (translateCameraZ)
	{
		DirectX::XMFLOAT3 posTranslation =
			DirectX::XMFLOAT3(
				0.0f,
				0.0f,
				float(translateCameraZ)
			);

		this->m_PreviousPos = this->m_Camera->GetCameraPos();
		this->m_Camera->ApplyLocalTranslation(
			0.0f,
			0.0f,
			float(translateCameraZ)
		);
		this->m_Camera->Update();
	}
}

void EditorInputHandler::CameraReset()
{
		this->m_Camera->Initialize(this->m_Width / this->m_Height);
		this->m_Camera->Update();
}

void EditorInputHandler::MousePicking()
{
	DIMOUSESTATE mouseCurrentState;

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);

	if (mouseCurrentState.rgbButtons[0])
	{
		DirectX::XMVECTOR rayOrigin, rayDirection;
		int m_MouseX = m_point.x();
		int m_MouseY = m_point.y();
		if (m_MouseX > m_Width)
			m_MouseX = m_Width;
		else if (m_MouseX < 0)
			m_MouseX = 0;
		if (m_MouseY > m_Height)
			m_MouseY = m_Height;
		else if (m_MouseY < 0)
			m_MouseY = 0;

		DirectX::XMVECTOR localRayDirection =
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR LocalRayOrigin = DirectX::XMLoadFloat3(&this->m_Camera->GetCameraPos());
		//DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		float X, Y, Z;
		DirectX::XMFLOAT4X4* camProjectionPtr = this->m_Camera->GetProjectionMatrix();
		DirectX::XMFLOAT4X4 camProjection = *(DirectX::XMFLOAT4X4*)camProjectionPtr;
		//float fieldOfView = (float)DirectX::XM_PI / 4.0f;
		//float screenAspect = (float)m_Width / (float)m_Height;



		X = (((2.0f * m_MouseX) / m_Width) - 1) / camProjection._11;
		Y = -(((2.0f * m_MouseY) / m_Height) - 1) / camProjection._22;
		Z = 1.0f;

		localRayDirection = DirectX::XMVectorSet(X, Y, Z, 0.0f);
		DirectX::XMMATRIX inverseCamView;
		DirectX::XMVECTOR det = { 1,1,1,1 };
		DirectX::XMMATRIX temp1;
		this->m_Camera->GetBaseViewMatrix(temp1);

		inverseCamView = DirectX::XMMatrixInverse(&det, temp1);

		//rayOrigin = DirectX::XMVector3TransformCoord(LocalRayOrigin, inverseCamView);
		rayDirection = DirectX::XMVector3TransformNormal(localRayDirection, inverseCamView);
		rayDirection = DirectX::XMVector3Normalize(rayDirection);

		//checks if we picked on a model by iterating

		if (!this->m_Map->empty())
		{
			BoundingBoxHeader boundingBox;
			Resources::Status st;
			std::vector<Container>* InstancePtr = nullptr;
			bool result;
			for (size_t i = 0; i < modelPtr->size(); i++)
			{
				std::unordered_map<unsigned int, std::vector<Container>>::iterator got = this->m_Map->find(modelPtr->at(i)->GetId());

				if (got == this->m_Map->end()) { // if  does not exists in memory

				}
				else {
					InstancePtr = &got->second;
					for (size_t j = 0; j < InstancePtr->size(); j++)
					{
						boundingBox = modelPtr->at(i)->GetOBBData();
						OBB obj;
						obj.ext[0] = boundingBox.extension[0];
						obj.ext[1] = boundingBox.extension[1];
						obj.ext[2] = boundingBox.extension[2];

						DirectX::XMFLOAT3 temp;
						temp.x = InstancePtr->at(j).position.m128_f32[0];
						temp.y = InstancePtr->at(j).position.m128_f32[1];
						temp.z = InstancePtr->at(j).position.m128_f32[2];
						obj.pos = DirectX::XMLoadFloat3(&temp);

						obj.ort;
						boundingBox.extensionDir;
						DirectX::XMMATRIX temp2;
						temp2 = DirectX::XMMatrixSet(
							boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
							boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
							boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f
						);

						obj.ort = temp2;

						//OBB obj = *(OBB*)&boundingBox;
						//DONT FORGET TO MULTIPLY MATRIX
						DirectX::XMMATRIX temp4 = InstancePtr->at(j).component.worldMatrix;
						DirectX::XMMATRIX temp3 = DirectX::XMMatrixMultiply(temp2, temp4);
						obj.ort = temp3;

						result = this->m_PhysicsHandler->IntersectRayOBB(LocalRayOrigin, rayDirection, obj, InstancePtr->at(j).position);
						if (result)
						{
							this->m_Picked.ID = modelPtr->at(i)->GetId();
							this->m_Picked.listInstance = j;
							std::cout << "true" << std::endl;
							break;
						}
						else
							std::cout << "false" << std::endl;
					}

				}

			}
			//this->m_Communicator->m_GraphicsHandler->renderFinalEditor();
		}
		else
		{
			//this->m_Communicator->m_GraphicsHandler->Render();
		}

	}
}






//DIMOUSESTATE mouseCurrentState;
//
//BYTE keyBoardState[256]; // the amount of buttons a char array of 256.
//
//DIKeyboard->Acquire();
//DIMouse->Acquire();
//
//DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);
//
//DIKeyboard->GetDeviceState(sizeof(keyBoardState), (LPVOID)&keyBoardState);
//
//float speed = 5.0f * dT;
//float speedrot = 2.5f * dT;
//int result = 1;
//float translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;
//float yaw = 0, pitch = 0;
//
//POINT mousePos;
//GetCursorPos(&mousePos);
//ScreenToClient(this->m_hwnd, &mousePos);



void EditorInputHandler::keyReleased(QKeyEvent * evt)
{
		switch (evt->key())
		{
		case Qt::Key_Shift:
			m_KeysHeld[Bools::SHIFT] = false;
			m_KeysHeld[Bools::W] = false;
			m_KeysHeld[Bools::A] = false;
			m_KeysHeld[Bools::S] = false;
			m_KeysHeld[Bools::D] = false;
			m_KeysHeld[Bools::C] = false;
			m_KeysHeld[Bools::SPACE] = false;
			break;
		case Qt::Key_Alt:
			m_KeysHeld[Bools::ALT] = false;
			break;
		case Qt::Key_Control:
			m_KeysHeld[Bools::CONTROL] = false;
			break;
		case Qt::Key_W:
			m_KeysHeld[Bools::W] = false;
			break;
		case Qt::Key_A:
			m_KeysHeld[Bools::A] = false;
			break;
		case Qt::Key_S:
			m_KeysHeld[Bools::S] = false;
			break;
		case Qt::Key_D:
			m_KeysHeld[Bools::D] = false;
			break;
		case Qt::Key_C:
			m_KeysHeld[Bools::C] = false;
			break;
		case Qt::Key_Space:
			m_KeysHeld[Bools::SPACE] = false;
			break;
		default:
			break;
		}
}

void EditorInputHandler::detectInput(double dT, QKeyEvent* evt)
{
		switch (evt->key())
		{
		case Qt::Key_Shift:
			m_KeysHeld[Bools::SHIFT] = true;
			break;
		case Qt::Key_Alt:
			m_KeysHeld[Bools::ALT] = true;
			break;
		case Qt::Key_Control:
			m_KeysHeld[Bools::CONTROL] = true;
			break;
		case Qt::Key_W:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::W] = true;
			}
			break;
		case Qt::Key_A:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::A] = true;
			}
			break;
		case Qt::Key_S:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::S] = true;
			}
			break;
		case Qt::Key_D:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::D] = true;
			}
			break;
		case Qt::Key_C:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::C] = true;
			}
			break;
		case Qt::Key_Space:
			if (m_KeysHeld[Bools::SHIFT] == true)
			{
				m_KeysHeld[Bools::SPACE] = true;
			}
			break;
		default:
			break;
		}

		switch (evt->key())
		{
		case Qt::Key_R:
			CameraReset();
			break;
		default:
			break;
		}
}