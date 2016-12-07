#include "EditorInputHandler.h"

EditorInputHandler::EditorInputHandler(HINSTANCE handleInstance, HWND handle, Camera* camera, int w, int h, GraphicsHandler* graphicshandler)
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
	this->m_hwnd = handle;
	this->m_Camera = camera;
	this->m_PreviousPos = camera->GetCameraPos();
}

EditorInputHandler::~EditorInputHandler()
{
}

void EditorInputHandler::detectInput(double dT)
{

	DIMOUSESTATE mouseCurrentState;

	BYTE keyBoardState[256]; // the amount of buttons a char array of 256.

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrentState);

	DIKeyboard->GetDeviceState(sizeof(keyBoardState), (LPVOID)&keyBoardState);

	float speed = 5.0f * dT;
	float speedrot = 2.5f * dT;
	int result = 1;
	float translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;
	float yaw = 0, pitch = 0;

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(this->m_hwnd, &mousePos);

#pragma region SHIFT + ALT +

#pragma endregion

#pragma region CONTROL +
	if (keyBoardState[DIK_LCONTROL] & 0x80)
	{
		if (mouseCurrentState.rgbButtons[0])
		{
			DirectX::XMVECTOR rayOrigin, rayDirection;
			int m_MouseX = mousePos.x;
			int m_MouseY = mousePos.y;
			if (m_MouseX > m_Width)
				m_MouseX = m_Width;
			else if (m_MouseX < 0)
				m_MouseX = 0;
			if (m_MouseY > m_Height)
				m_MouseY = m_Height;
			else if (m_MouseY < 0)
				m_MouseY = 0;

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
			X = (((2.0f * m_MouseX) / m_Width) - 1) / camProjection._11;
			Y = -(((2.0f * m_MouseY) / m_Height) - 1) / camProjection._22;
			Z = 1.0f;    //View space's Z direction ranges from 0 to 1, so we set 1 since the ray goes "into" the screen

			localRayDirection = DirectX::XMVectorSet(X, Y, Z, 0.0f);

			DirectX::XMMATRIX inverseCamView;
			DirectX::XMVECTOR det = { 1,1,1,1 };
			DirectX::XMMATRIX temp;
			this->m_Camera->GetBaseViewMatrix(temp);

			//inverseCamView = DirectX::XMMatrixInverse(&det, temp);

			//rayOrigin = XMVector3TransformCoord(LocalRayOrigin, inverseCamView);
			//rayDirection = DirectX::XMVector3TransformNormal(localRayDirection, inverseCamView);
			//rayDirection = DirectX::XMVector3Normalize(rayDirection);

			DirectX::XMFLOAT4 test;
			DirectX::XMStoreFloat4(&test, localRayDirection);
			this->m_Camera->SetLookAt(test);
		}
	}
#pragma endregion

#pragma region SHIFT +
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
		if (keyBoardState[DIK_A] & 0x80)
		{
			translateCameraX -= speed;
		}
		if (keyBoardState[DIK_D] & 0x80)
		{
			translateCameraX += speed;
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
			if (mouseCurrentState.lY < 0)
			{
				translateCameraZ += speedrot;
			}
			if (mouseCurrentState.lY > 0)
			{
				translateCameraZ -= speedrot;
			}

		}


	}
#pragma endregion

#pragma region ALT +
	if (keyBoardState[DIK_LALT] & 0x80)
	{
		if (mouseCurrentState.rgbButtons[0])
		{
			if ((mouseCurrentState.lX!= m_mouseLastState.lX) || (mouseCurrentState.lY != m_mouseLastState.lY))
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
	}
#pragma endregion
	
#pragma region ONLY MOUSE
	if (mouseCurrentState.rgbButtons[0] &&
		!keyBoardState[DIK_LALT] & 0x80 ||
		!keyBoardState[DIK_LSHIFT] & 0x80)
	{
		DirectX::XMVECTOR rayOrigin, rayDirection;
		int m_MouseX = mousePos.x;
		int m_MouseY = mousePos.y;
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
		DirectX::XMVECTOR LocalRayOrigin = 
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
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
		X = (((2.0f * m_MouseX) / m_Width) - 1) / camProjection._11;
		Y = -(((2.0f * m_MouseY) / m_Height) - 1) / camProjection._22;
		Z = 1.0f;
		localRayDirection = DirectX::XMVectorSet(X, Y, Z, 0.0f);

		DirectX::XMMATRIX inverseCamView;
		DirectX::XMVECTOR det = { 1,1,1,1 };
		DirectX::XMMATRIX temp;
		this->m_Camera->GetBaseViewMatrix(temp);

		inverseCamView = DirectX::XMMatrixInverse(&det, temp);

		rayOrigin = XMVector3TransformCoord(LocalRayOrigin, inverseCamView);
		rayDirection = DirectX::XMVector3TransformNormal(localRayDirection, inverseCamView);
		rayDirection = DirectX::XMVector3Normalize(rayDirection);
		
		//checks if we picked on a model by iterating triangles;
		float result = Intersection(rayOrigin,rayDirection);
		if (result != FLT_MAX)
		{
			this->m_Picked;
		}

		//DO PICKING TEST TO FIND MESH IN SCENE
		//GET POINTER?
		//STORE AS LAST PICK
		//NEED A GET FUNC SO OTHER CLASSES CAN GET CURRENT PICK
		//HIGHLIGHT?


		//DirectX::XMFLOAT4 test;
		//DirectX::XMStoreFloat4(&test, localRayDirection);
		//this->m_Camera->SetLookAt(test);
	}
#pragma endregion

	if (keyBoardState[DIK_F] & 0x80)
	{
		this->m_Camera->Initialize();
		this->m_Camera->Update();
	}



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
float EditorInputHandler::Intersection(DirectX::XMVECTOR rayOrigin, DirectX::XMVECTOR rayDirection)
{
	//for (int i = 0; i <  this->m_Communicator/ 3; i++)
	//{
	//
	//	//Triangle's vertices, V1, V2, V3
	//	DirectX::XMVECTOR tri1V1 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	DirectX::XMVECTOR tri1V2 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	DirectX::XMVECTOR tri1V3 = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//
	//
	//	//Temporary 3d floats for each vertex.
	//	DirectX::XMFLOAT3 _tV1, _tV2, _tV3;
	//
	//	_tV1.x = obj.finalVector[i * 3 + 0].x;
	//	_tV1.y = obj.finalVector[i * 3 + 0].y;
	//	_tV1.z = obj.finalVector[i * 3 + 0].z;
	//
	//	_tV2.x = obj.finalVector[i * 3 + 1].x;
	//	_tV2.y = obj.finalVector[i * 3 + 1].y;
	//	_tV2.z = obj.finalVector[i * 3 + 1].z;
	//
	//	_tV3.x = obj.finalVector[i * 3 + 2].x;
	//	_tV3.y = obj.finalVector[i * 3 + 2].y;
	//	_tV3.z = obj.finalVector[i * 3 + 2].z;
	//
	//
	//	tri1V1 = DirectX::XMVectorSet(_tV1.x, _tV1.y, _tV1.z, 1.0f);
	//	tri1V2 = DirectX::XMVectorSet(_tV2.x, _tV2.y, _tV2.z, 1.0f);
	//	tri1V3 = DirectX::XMVectorSet(_tV3.x, _tV3.y, _tV3.z, 1.0f);
	//
	//	tri1V1 = DirectX::XMVector3TransformCoord(tri1V1, matrices.World);
	//	tri1V2 = DirectX::XMVector3TransformCoord(tri1V2, matrices.World);
	//	tri1V3 = DirectX::XMVector3TransformCoord(tri1V3, matrices.World);
	//
	//
	//	DirectX::XMVECTOR U = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//	DirectX::XMVECTOR V = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//
	//	DirectX::XMVECTOR faceNormal = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//
	//	U = DirectX::XMVectorSubtract(tri1V2, tri1V1);
	//	V = DirectX::XMVectorSubtract(tri1V3, tri1V1);
	//
	//	faceNormal = DirectX::XMVector3Cross(U, V);
	//	faceNormal = DirectX::XMVector3Normalize(faceNormal);
	//
	//	DirectX::XMVECTOR triPoint = tri1V1;
	//
	//	//plane equation
	//	float tri1A = DirectX::XMVectorGetX(faceNormal);
	//	float tri1B = DirectX::XMVectorGetY(faceNormal);
	//	float tri1C = DirectX::XMVectorGetZ(faceNormal);
	//	float tri1D = (-tri1A*DirectX::XMVectorGetX(triPoint) - tri1B*DirectX::XMVectorGetY(triPoint) - tri1C*DirectX::XMVectorGetZ(triPoint));
	//
	//	//now we find wher the ray intersects with the triangles plane,
	//
	//	float ep1, ep2, t = 0.0f;
	//
	//	float planeIntersectX, planeIntersectY, planeIntersectz = 0.0f;
	//
	//
	//	DirectX::XMVECTOR pointInPlane = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//
	//	ep1 = (DirectX::XMVectorGetX(rayOrigin)      *   tri1A) + (DirectX::XMVectorGetY(rayOrigin)       *   tri1B) + (DirectX::XMVectorGetZ(rayOrigin)       *   tri1C);
	//	ep2 = (DirectX::XMVectorGetX(rayDirection)   *   tri1A) + (DirectX::XMVectorGetY(rayDirection)    *   tri1B) + (DirectX::XMVectorGetZ(rayDirection)    *   tri1C);
	//
	//	if (ep2 != 0.0f)
	//	{
	//		t = -(ep1 + tri1D) / (ep2);
	//	}
	//	if (t > 0.0f)
	//	{
	//		planeIntersectX = DirectX::XMVectorGetX(rayOrigin) + DirectX::XMVectorGetX(rayDirection) * t;
	//		planeIntersectY = DirectX::XMVectorGetY(rayOrigin) + DirectX::XMVectorGetY(rayDirection) * t;
	//		planeIntersectz = DirectX::XMVectorGetZ(rayOrigin) + DirectX::XMVectorGetZ(rayDirection) * t;
	//
	//		pointInPlane = DirectX::XMVectorSet(planeIntersectX, planeIntersectY, planeIntersectz, 0.0f);
	//
	//		if (checkPointInTriangle(tri1V1, tri1V2, tri1V3, pointInPlane))
	//		{
	//			return t / 2.0f;
	//		}
	//	}
	//
	//
	//}
	return FLT_MAX;
}
bool EditorInputHandler::checkPointInTriangle(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& triangleV1, const DirectX::XMVECTOR& triangleV2, const DirectX::XMVECTOR& triangleV3)
{
	//Denna funktion kolla om en point är i en triangle genom att
	DirectX::XMVECTOR collisionPoint1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV3, triangleV2), DirectX::XMVectorSubtract(point, triangleV2));

	DirectX::XMVECTOR collisionPoint2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV3, triangleV2), DirectX::XMVectorSubtract(triangleV1, triangleV2));
	if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(collisionPoint1, collisionPoint2)) >= 0)
	{
		collisionPoint1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV3, triangleV1), DirectX::XMVectorSubtract(point, triangleV1));
		collisionPoint2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV3, triangleV1), DirectX::XMVectorSubtract(triangleV2, triangleV1));
		if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(collisionPoint1, collisionPoint2)) >= 0)
		{
			collisionPoint1 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV2, triangleV1), DirectX::XMVectorSubtract(point, triangleV1));
			collisionPoint2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(triangleV2, triangleV1), DirectX::XMVectorSubtract(triangleV3, triangleV1));
			if (DirectX::XMVectorGetX(DirectX::XMVector3Dot(collisionPoint1, collisionPoint2)) >= 0)
			{
				return true;
			}
		}
	}
	return false;
}