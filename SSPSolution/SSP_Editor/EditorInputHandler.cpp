#include "EditorInputHandler.h"


EditorInputHandler::EditorInputHandler(
	HINSTANCE handleInstance,
	HWND handle,
	Camera* camera,
	int w,
	int h,
	GraphicsHandler* graphicshandler,
	Level* m_currentLevel,
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

	//hr = m_directInput->CreateDevice(GUID_SysKeyboard,
	//	&DIKeyboard,
	//	NULL);

	hr = m_directInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	//hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	//hr = DIKeyboard->SetCooperativeLevel(handle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(handle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	this->m_currentLevel  = m_currentLevel;
	this->modelPtr      = modelPtr;
	this->m_hwnd	    = handle;
	this->m_Camera	    = camera;
	this->m_PreviousPos = camera->GetCameraPos();
	for (size_t i = 0; i < NUMBOOLS; i++)
	{
		this->m_KeysHeld[i] = false;
	}
	DIMouse->Acquire();
	this->m_LastPicked.listInstance = NULL;
	this->m_ray.direction = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_ray.origin = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);
	this->m_ray.localOrigin = DirectX::XMVectorSet(0.0, 0.0, 0.0, 0.0);

	
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
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	float pitch = 0;
	float yaw = 0;

		if (m_mouse.currentState.rgbButtons[0])
		{
			if ((m_mouse.currentState.lX != m_mouse.lastState.lX) || (m_mouse.currentState.lY != m_mouse.lastState.lY))
			{
				pitch += m_mouse.currentState.lX * 0.01f;
	
				yaw += m_mouse.currentState.lY * 0.01f;
	
				// Ensure the mouse location doesn't exceed the screen width or height.
				if (this->m_mouse.x < 0) { this->m_mouse.x = 0; }
				if (this->m_mouse.y < 0) { this->m_mouse.y = 0; }
	
				if (this->m_mouse.x > m_Width) { this->m_mouse.x = m_Width; }
				if (this->m_mouse.y > m_Height) { this->m_mouse.y = m_Height; }
	
				m_mouse.lastState = m_mouse.currentState;
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

	
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	
	float speedrot = 2.5f * dT;
	float translateCameraZ = 0;

	if (m_mouse.currentState.rgbButtons[0])
	{
		if (m_mouse.currentState.lY < 0)
		{
			translateCameraZ += speedrot;
		}
		if (m_mouse.currentState.lY > 0)
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

void EditorInputHandler::UpdateMouse()
{
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	this->m_mouse.x = this->m_point.x();
	this->m_mouse.y = this->m_point.y();
	if (this->m_mouse.x > this->m_Width)
		this->m_mouse.x = this->m_Width;
	else if (this->m_mouse.x < 0)
		this->m_mouse.x = 0;
	if (this->m_mouse.y > this->m_Height)
		this->m_mouse.y = this->m_Height;
	else if (this->m_mouse.y < 0)
		this->m_mouse.y = 0;
}

void EditorInputHandler::mouseButtonDown(QMouseEvent * evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		//*MOUSEPICKING*//
		if (transformWidget.IsActive())
		{
			m_PickTransformWidget();
		}
		
		m_PickObjectSelection();

		this->m_mouse.leftHeld = true;

	}
}

void EditorInputHandler::mouseButtonRelease(QMouseEvent * evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		this->m_mouse.leftHeld = false;

	}
}

void EditorInputHandler::m_ProjectRay(int X, int Y)
{
	DirectX::XMMATRIX inverseCamView;
	this->m_Camera->GetViewMatrix(inverseCamView);
	inverseCamView = DirectX::XMMatrixInverse(nullptr, inverseCamView);

	this->m_ray.direction = DirectX::XMVector3Normalize(DirectX::XMVector3TransformNormal(
		DirectX::XMVectorSet(
			(((2.0f * X) / m_Width) - 1) / this->m_Camera->GetProjectionMatrix()->_11,
			-(((2.0f * Y) / m_Height) - 1) / this->m_Camera->GetProjectionMatrix()->_22,
			1.0f,
			0.0f),
		inverseCamView));

	this->m_ray.localOrigin = DirectX::XMLoadFloat3(&this->m_Camera->GetCameraPos());

	this->m_ray.origin = DirectX::XMVector3TransformCoord(this->m_ray.localOrigin, inverseCamView);
}

bool EditorInputHandler::m_PickTransformWidget()
{
	bool result;

	for (int i = 0; i < TransformWidget::NUM_AXIS; i++)
	{
		result = this->m_PhysicsHandler->IntersectRayOBB(
			this->m_ray.localOrigin, this->m_ray.direction,
			this->transformWidget.axisOBB[i], this->transformWidget.axisOBB[i].pos);
		if (result)
		{
			transformWidget.selectedAxis = i;
			break;
		}
		else
		{
			transformWidget.selectedAxis = TransformWidget::NONE;
		}
	}

	return result;
}

bool EditorInputHandler::m_PickObjectSelection()
{
	////checks if we picked on a model by iterating
	//std::unordered_map<unsigned int, std::vector<Container>>* m_Map = m_currentLevel->GetModelEntities();
	//if (!m_Map->empty())
	//{
	//	BoundingBoxHeader boundingBox;
	//	Resources::Status st;
	//	std::vector<Container>* InstancePtr = nullptr;
	//	bool result;
	//	for (size_t i = 0; i < modelPtr->size(); i++)
	//	{
	//		std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map->find(modelPtr->at(i)->GetId());

	//		if (got == m_Map->end()) { // if  does not exists in memory
	//			continue;
	//		}
	//		else {
	//			InstancePtr = &got->second;
	//			for (size_t j = 0; j < InstancePtr->size(); j++)
	//			{
	//				boundingBox = modelPtr->at(i)->GetOBBData();
	//				OBB obj;
	//				obj.ext[0] = boundingBox.extension[0];
	//				obj.ext[1] = boundingBox.extension[1];
	//				obj.ext[2] = boundingBox.extension[2];

	//				DirectX::XMFLOAT3 temp;
	//				temp.x = InstancePtr->at(j).position.m128_f32[0];
	//				temp.y = InstancePtr->at(j).position.m128_f32[1];
	//				temp.z = InstancePtr->at(j).position.m128_f32[2];
	//				obj.pos = DirectX::XMLoadFloat3(&temp);


	//				boundingBox.extensionDir;
	//				DirectX::XMMATRIX temp2;
	//				temp2 = DirectX::XMMatrixSet(
	//					boundingBox.extensionDir[0].x, boundingBox.extensionDir[0].y, boundingBox.extensionDir[0].z, 0.0f,
	//					boundingBox.extensionDir[1].x, boundingBox.extensionDir[1].y, boundingBox.extensionDir[1].z, 0.0f,
	//					boundingBox.extensionDir[2].x, boundingBox.extensionDir[2].y, boundingBox.extensionDir[2].z, 0.0f,
	//					0.0f, 0.0f, 0.0f, 1.0f
	//				);

	//				obj.ort = temp2;

	//				//OBB obj = *(OBB*)&boundingBox;
	//				//DONT FORGET TO MULTIPLY MATRIX
	//				DirectX::XMMATRIX temp4 = InstancePtr->at(j).component.worldMatrix;
	//				DirectX::XMMATRIX temp3 = DirectX::XMMatrixMultiply(temp2, temp4);
	//				obj.ort = temp3;

	//				result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, InstancePtr->at(j).position);

	//				if (result)
	//				{

	//					this->m_Picked.ID = modelPtr->at(i)->GetId();
	//					this->m_Picked.listInstance = j;

	//					//update widget with the intersected obb
	//					this->transformWidget.Update(obj);

	//					if (this->m_Picked.listInstance != this->m_LastPicked.listInstance)
	//					{
	//						this->m_LastPicked = m_Picked;

	//					}
	//					else if (this->m_LastPicked.listInstance == NULL)
	//					{
	//						this->m_LastPicked = this->m_Picked;
	//					}
	//				}

	return false;
}

void EditorInputHandler::MousePicking()
{

	if (m_mouse.currentState.rgbButtons[0] && !this->m_KeysHeld[SHIFT])
	{
		this->m_ProjectRay(m_mouse.x, m_mouse.y);

		//checks if we picked on a model by iterating
		std::unordered_map<unsigned int, std::vector<Container>>* m_Map = m_currentLevel->GetModelEntities();
		if (!m_Map->empty())
		{
			BoundingBoxHeader boundingBox;
			Resources::Status st;
			std::vector<Container>* InstancePtr = nullptr;
			bool result;
			for (size_t i = 0; i < modelPtr->size(); i++)
			{
				std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_Map->find(modelPtr->at(i)->GetId());

				if (got == m_Map->end()) { // if  does not exists in memory
					continue;
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

						result = this->m_PhysicsHandler->IntersectRayOBB(m_ray.localOrigin, this->m_ray.direction, obj, InstancePtr->at(j).position);

						if (result)
						{
							
							this->m_Picked.ID = modelPtr->at(i)->GetId();
							this->m_Picked.listInstance = j;

							//update widget with the intersected obb
							this->transformWidget.Update(obj);

							if (this->m_Picked.listInstance != this->m_LastPicked.listInstance)
							{
								this->m_LastPicked = m_Picked;

							}
							else if (this->m_LastPicked.listInstance == NULL)
							{
								this->m_LastPicked = this->m_Picked;
							}
						}

						int index = -1;
						for (int k = 0; k < TransformWidget::NUM_AXIS; k++)
						{
							result = this->m_PhysicsHandler->IntersectRayOBB(
								this->m_ray.localOrigin, this->m_ray.direction, 
								this->transformWidget.axisOBB[k], this->transformWidget.axisOBB[k].pos);
							if (result)
							{
								index = k;
								break;
							}
						}
						if (index != -1)
						{
							UpdatePos(index);
						}
					}
				}
			}
		}
	}
	this->m_mouse.lastX = this->m_mouse.x;
	this->m_mouse.lastY = this->m_mouse.y;
}

void EditorInputHandler::keyReleased(QKeyEvent * evt)
{
	
		switch (evt->key())
		{
		case Qt::Key_Shift:
			m_KeysHeld[Bools::SHIFT] = false;
			m_KeysHeld[Bools::W]	 = false;
			m_KeysHeld[Bools::A]	 = false;
			m_KeysHeld[Bools::S]	 = false;
			m_KeysHeld[Bools::D]	 = false;
			m_KeysHeld[Bools::C]	 = false;
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

void EditorInputHandler::UpdatePos(int index)
{
	float temp1, temp2;
	std::unordered_map<unsigned int, std::vector<Container>>* m_Map = m_currentLevel->GetModelEntities();
	if (this->m_mouse.lastX != this->m_mouse.x)
	{
		temp1 = this->m_mouse.lastX - this->m_mouse.x;
		temp2 = this->m_mouse.lastY - this->m_mouse.y;
		if (index == 0)
		{
			if (temp1 > 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[0] += -1;
			}
			if (temp1 < 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[0] += 1;
			}
			//this->m_Map->at(m_LastPicked.ID).at(m_LastPicked.listInstance).rotation += 45.0f;
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).isDirty = true;

		}
		if (index == 1)
		{
			if (temp2 > 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[1] += 1;
			}
			if (temp2 < 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[1] += -1;
			}
			//this->m_Map->at(m_LastPicked.ID).at(m_LastPicked.listInstance).rotation += 45.0f;
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).isDirty = true;

		}
		if (index == 2)
		{
			if (temp1 > 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[2] += -1;
			}
			if (temp1 < 0)
			{
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).position.m128_f32[2] += 1;
			}
			//this->m_Map->at(m_LastPicked.ID).at(m_LastPicked.listInstance).rotation += 45.0f;
				m_Map->at(m_Picked.ID).at(m_Picked.listInstance).isDirty = true;

		}
		for (size_t i = 0; i < TransformWidget::NUM_AXIS; i++)
		{
			if (index == 0)
			{
				if (temp1 > 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[0] += -1;
				}
				if (temp1 < 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[0] += 1;
				}
			}
			if (index == 1)
			{
				if (temp2 > 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[1] += 1;
				}
				if (temp2 < 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[1] += -1;
				}
			}
			if (index == 2)
			{
				if (temp1 > 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[2] += -1;
				}
				if (temp1 < 0)
				{
					this->transformWidget.axisOBB[i].pos.m128_f32[2] += 1;
				}
			}
		}
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