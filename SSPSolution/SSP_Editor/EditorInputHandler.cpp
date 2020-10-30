#include "EditorInputHandler.h"


EditorInputHandler::EditorInputHandler(
	HINSTANCE handleInstance,
	HWND handle,
	Camera* camera,
	int w,
	int h)
{
	this->setObjectName("EditorInputHandler");
	setEnabled(true);
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->m_Width = w;
	this->m_Height = h;
	this->resize(w,h);

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
	//this->m_hwnd	    = handle;
	this->m_Camera	    = camera;
	
	this->m_PreviousPos = camera->GetCameraPos();
	for (size_t i = 0; i < NUMBOOLS; i++)
	{
		this->m_KeysHeld[i] = false;
	}
	DIMouse->Acquire();

	this->installEventFilter(this);
}

EditorInputHandler::~EditorInputHandler()
{
}

void EditorInputHandler::KeyboardMovement(double dT)
{
	float speed = 4.0f * dT;
	float translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;
	if (m_KeysHeld[Bools::SHIFT] == true)
	{
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
	
			MouseZoom(dT);
	}
	else {
	if (this->m_KeysHeld[Bools::SPACE] == true)
		translateCameraY += speed;
	}
	if (this->m_KeysHeld[ALT] == true)
		MouseMovement(dT);

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
		this->m_Camera->UpdateView();
	}
}

void EditorInputHandler::MouseMovement(double dT)
{
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	float pitch = 0;
	float yaw = 0;
	float rotationAmount = (DirectX::XM_PI / 8) / 2;

	

	if (this->m_mouse.leftHeld)//m_mouse.currentState.rgbButtons[0])
	{	
			pitch += m_mouse.currentState.lX * 0.01f;
			yaw += m_mouse.currentState.lY * 0.01f;
	}

	if (pitch || yaw) //Rotation
	{
		DirectX::XMFLOAT4 camUpFloat;
		DirectX::XMFLOAT3 camPosFloat;
		DirectX::XMFLOAT3 camTargetFloat;
		this->m_Camera->GetCameraUp(camUpFloat);
		camPosFloat = this->m_Camera->GetCameraPos();
		camTargetFloat = this->m_Camera->GetLookAt();

		DirectX::XMVECTOR rotationVector;

		DirectX::XMVECTOR camUpVec = { 0.0f, 1.0f,0.0f,0.0f };//DirectX::XMLoadFloat4(&camUpFloat);
		DirectX::XMVECTOR camPosVec = DirectX::XMLoadFloat3(&camPosFloat);
		DirectX::XMVECTOR camTargetVec = DirectX::XMLoadFloat3(&camTargetFloat);

		DirectX::XMVECTOR camDir = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(camTargetVec, camPosVec));

		DirectX::XMVECTOR camRight = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(camDir, camUpVec));

		camRight.m128_f32[3] = rotationAmount * -yaw;
		camUpVec.m128_f32[3] = rotationAmount * pitch;
		
		this->m_Camera->RotateCamera(camRight);
		this->m_Camera->RotateCamera(camUpVec);
	};

	if (this->m_mouse.wheelHeld)
	{
		static float lastX = 0.0f;
		static float lastY = 0.0f;

		float x  = lastX - m_mouse.x; // m_mouse.currentState.lX -m_mouse.lastState.lX ;
		float y =  lastY - m_mouse.y;//m_mouse.currentState.lY-  m_mouse.lastState.lY;

		lastX = m_mouse.x;
		lastY = m_mouse.y;

		QVector2D dir(x,-y);
		if(dir.length()> 0.0f){
			

		dir.normalize();
	
			this->m_Camera->ApplyLocalTranslation(
				dir.x()*1.0f,
				dir.y()*1.0f,
				0.0f
			);
			}
		
	}


	this->m_Camera->UpdateView();

	if ((m_mouse.currentState.lX != m_mouse.lastState.lX) || (m_mouse.currentState.lY != m_mouse.lastState.lY))
	{


		// Ensure the mouse location doesn't exceed the screen width or height.
		if (this->m_mouse.x < 0) { this->m_mouse.x = 0; }
		if (this->m_mouse.y < 0) { this->m_mouse.y = 0; }

		if (this->m_mouse.x > width()) { this->m_mouse.x = width(); }
		if (this->m_mouse.y > height()) { this->m_mouse.y = height(); }

		m_mouse.lastState = m_mouse.currentState;
	}
}


void EditorInputHandler::MouseZoom(double dT)
{

	
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	
	float speedrot = 2.5f * dT;
	float translateCameraZ = dT;//0;

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
		this->m_Camera->UpdateView();
	}
}

void EditorInputHandler::CameraReset()
{
		this->m_Camera->Initialize(this->width() / this->height());
		this->m_Camera->SetLookAt(DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f, 1.0f });
		this->m_Camera->SetCameraPos(DirectX::XMVECTOR{ 0.0f, 0.0f, -1.0f, 1.0f });
		this->m_Camera->UpdateView();
}

void EditorInputHandler::UpdateMouse()
{
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouse.currentState);
	this->m_mouse.x = this->m_point.x();
	this->m_mouse.y = this->m_point.y();
	if (this->m_mouse.x > this->width())
		this->m_mouse.x = this->width();
	else if (this->m_mouse.x < 0)
		this->m_mouse.x = 0;
	if (this->m_mouse.y > this->height())
		this->m_mouse.y = this->height();
	else if (this->m_mouse.y < 0)
		this->m_mouse.y = 0;


	 
	if (this->m_mouse.leftHeld)
	{
		SelectionHandler::GetInstance()->ProjectRay(m_mouse.x, m_mouse.y);
		SelectionHandler::GetInstance()->MoveObject(m_KeysHeld[Bools::CONTROL]);
	}


	if (this->m_mouse.wheelHeld && this->m_KeysHeld[ALT]) {
			
	/*	DirectX::XMFLOAT3 posTranslation =
			DirectX::XMFLOAT3(
				m_mouse.x,
				m_mouse.y,
				0.0f
			);

		this->m_PreviousPos = this->m_Camera->GetCameraPos();
		this->m_Camera->ApplyLocalTranslation(
			m_mouse.x*0.2,
			m_mouse.y*0.2,
			0.0f
		);
		this->m_Camera->UpdateView();*/
	}
}

void EditorInputHandler::mouseButtonDown(QMouseEvent * evt)
{
	this->m_mouse.rightHeld = (evt->button() == Qt::RightButton);
	this->m_mouse.leftHeld  = (evt->button() == Qt::LeftButton);
	this->m_mouse.wheelHeld = (evt->button() == Qt::MiddleButton);

	if (this->m_mouse.leftHeld && !this->m_KeysHeld[ALT])
	{
		////*MOUSEPICKING*//
		SelectionHandler::GetInstance()->ProjectRay(m_mouse.x, m_mouse.y);

		bool object = false;
		bool widget = false;

		
		if (SelectionHandler::GetInstance()->HasSelection() )
		{
			widget = SelectionHandler::GetInstance()->PickTransformWidget();
		}
		if (!widget)
			object = SelectionHandler::GetInstance()->PickObjectSelection();

		if (object || widget)
		{
			SelectionHandler::GetInstance()->SetSelection(true);
		}
		else if(!this->m_KeysHeld[ALT])
		{
			SelectionHandler::GetInstance()->SetSelection(false);
		}

		//this->m_mouse.leftHeld = true;
	}
	/*if(evt->button() ==Qt::MiddleButton)
		m_mouse.wheelHeld= true;*/
}

void EditorInputHandler::mouseButtonRelease(QMouseEvent * evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		this->m_mouse.leftHeld = false;
		SelectionHandler::GetInstance()->SetActiveAxis(TransformWidget::NONE);
	}
	if (evt->button() == Qt::MiddleButton)
		m_mouse.wheelHeld = false;
	if (evt->button() == Qt::RightButton)
		m_mouse.rightHeld = false;
}

bool EditorInputHandler::eventFilter(QObject * watched, QEvent * event)
{
	return false;
}


void EditorInputHandler::keyReleased(QKeyEvent * evt)
{
	
		switch (evt->key())
		{
		case Qt::Key_Shift:
			m_KeysHeld[Bools::SHIFT] = false;
			break;
		case Qt::Key_Alt:
			m_KeysHeld[Bools::ALT] = false;
			break;
		case Qt::Key_Control:
			m_KeysHeld[Bools::CONTROL] = false;
			m_ableToDuplicate = true;
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
		case Qt::Key_F:
			m_KeysHeld[Bools::F] = false;
			break;
		case Qt::Key_Space:
			m_KeysHeld[Bools::SPACE] = false;
			break;
		default:
			break;
		}
}


void EditorInputHandler::ViewPortChanged(float height, float width)
{

	this->resize(width,height);
	SelectionHandler::GetInstance()->updateWindowSize(height, width);
}

void EditorInputHandler::deleteModel()
{
	if (SelectionHandler::GetInstance()->HasSelection())
	{
		LevelHandler::GetInstance()->GetCurrentLevel()->RemoveModel
			(SelectionHandler::GetInstance()->GetModelID(), SelectionHandler::GetInstance()->GetInstanceID());
		SelectionHandler::GetInstance()->SetSelection(false);
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
			m_KeysHeld[Bools::W] = true;
			break;
		case Qt::Key_A:
				m_KeysHeld[Bools::A] = true;
			break;
		case Qt::Key_S:
				m_KeysHeld[Bools::S] = true;
			break;
		case Qt::Key_D:
			if (m_ableToDuplicate)
			{
				if (this->m_KeysHeld[Bools::CONTROL] == true)
				{
					
						if (SelectionHandler::GetInstance()->HasSelection())
						{
							Container* temp = SelectionHandler::GetInstance()->GetSelected();
							Container* newEntity = nullptr;
							Resources::Status st = LevelHandler::GetInstance()->GetCurrentLevel()->DuplicateEntity(temp, newEntity);
							if (st == Resources::Status::ST_OK)
								SelectionHandler::GetInstance()->SetSelectedContainer(newEntity);
						}
						m_ableToDuplicate = false;
				}
			}
			m_KeysHeld[Bools::D] = true;
			break;
			
		case Qt::Key_C:
				m_KeysHeld[Bools::C] = true;
			break;
		case Qt::Key_F:
			m_KeysHeld[Bools::F] = true;
			{
				Container* temp = SelectionHandler::GetInstance()->GetSelected();
				if (temp != nullptr) {
					DirectX::XMVECTOR offset = {0.0f,0.0f,0.0f};
					m_Camera->SetCameraPivot(&temp->position, offset,1);
					m_Camera->UpdateView();
				}
			}
			break;
		case Qt::Key_Space:
				m_KeysHeld[Bools::SPACE] = true;
			break;
		case Qt::Key_R:
			CameraReset();
			break;
		case Qt::Key_Delete:
			deleteModel();
			break;
		case (Qt::Key_0):
		case (Qt::Key_Up) :
		case (Qt::Key_Down) :
		case (Qt::Key_Left) :
		case (Qt::Key_Right) :
			SelectionHandler::GetInstance()->RotateObject(evt->key());
			break;

		default:
			break;
		}

	
}

void EditorInputHandler::keyPressEvent(QKeyEvent * evt)
{

	detectInput(0, evt);
}

void EditorInputHandler::keyReleaseEvent(QKeyEvent * evt)
{
	//EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	//EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	//EditorInputHptr->detectInput(this->m_frameTime, evt);
	keyReleased(evt);
}

void EditorInputHandler::mousePressEvent(QMouseEvent * evt)
{
	//EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	//EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	mouseButtonDown(evt);
}

void EditorInputHandler::mouseReleaseEvent(QMouseEvent * evt)
{
	//EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	//EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	mouseButtonRelease(evt);
}

void EditorInputHandler::wheelEvent(QWheelEvent * evt)
{
	const float speed = 1;
	//std::cout << evt->angleDelta().x()<<std::endl;
	//std::cout << evt->angleDelta().y()<< std::endl;
	////m_Camera->IncreaseDistance(();
	//std::cout << m_Camera->GetCameraDistance() << " | " <<m_Camera->GetCameraMaxDistance() <<std::endl;

	DirectX::XMFLOAT3 applyValue;
	applyValue.x += (evt->angleDelta().y() / 120) *speed;
	applyValue.y += (evt->angleDelta().y() / 120) *speed;
	applyValue.z += (evt->angleDelta().y() / 120) *speed;
	//m_Camera->AddToLookAt(applyValue);
	MouseZoom(applyValue.x);
}
