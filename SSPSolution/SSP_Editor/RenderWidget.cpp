#include "RenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	Resources::Status status;
	status = this->m_Communicator->Release();
	if (status == Resources::ST_OK)
	{
		delete this->m_Communicator;
	}
}

void D3DRenderWidget::paintEvent(QPaintEvent * evt)
{
	QPoint local = this->mapFromGlobal(QCursor::pos());
	this->m_Communicator->m_EditorInputHandler->SetMousePos(local);

	this->m_frameCount++;
	if (getTime() > 1.0f)
	{
		this->m_fps = this->m_frameCount;
		this->m_frameCount = 0;
		startTimer();
	}
	this->m_frameTime = getFrameTime();

	if (!this->m_Communicator->m_IsPreview)
	{
		this->m_Communicator->m_EditorInputHandler->KeyboardMovement(this->m_frameTime);
		this->m_Communicator->m_EditorInputHandler->MousePicking();
	}

	if (!this->m_Communicator->m_Map.empty())
	{
	Resources::Status st;
	std::vector<Container>* InstancePtr = nullptr;
	std::vector<Resources::Model*>* modelPtr = this->m_fileImporter->get_M_models();
	this->m_Communicator->m_GraphicsHandler->clearEditor();

	for (size_t i = 0; i < modelPtr->size(); i++)
		{
			std::unordered_map<unsigned int, std::vector<Container>>::iterator got = this->m_Communicator->m_Map.find(modelPtr->at(i)->GetId());
			
			if (got == this->m_Communicator->m_Map.end()) { // if  does not exists in memory

			}
			else {
				InstancePtr = &got->second;
				for (size_t j = 0; j < InstancePtr->size(); j++)
				{
					BoundingBoxHeader boundingBox = modelPtr->at(i)->GetOBBData();
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


					this->m_Communicator->m_GraphicsHandler->RenderFromEditor(
						modelPtr->at(i),
						&InstancePtr->at(j).component
					);
					this->m_Communicator->m_GraphicsHandler->RenderBoundingVolume(
						InstancePtr->at(j).position,
						obj
					);
				}

			}

		}
		this->m_Communicator->m_GraphicsHandler->renderFinalEditor();
	}
	else
	{
		this->m_Communicator->m_GraphicsHandler->Render();
	}
	//std::cout << "FPS: " << this->m_fps << std::endl;
	this->update();
}

void D3DRenderWidget::keyPressEvent(QKeyEvent * evt)
{
	this->m_Communicator->m_EditorInputHandler->detectInput(this->m_frameTime, evt);
}

void D3DRenderWidget::keyReleaseEvent(QKeyEvent * evt)
{
	this->m_Communicator->m_EditorInputHandler->keyReleased(evt);
}

void D3DRenderWidget::Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter)
{
	Resources::Status st;
	InitDosConsole();

	this->m_Communicator = new Communicator();
	this->m_hwnd = (HWND)parent->winId();
	this->m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);

	st = this->m_Communicator->Initialize(
		this->m_hwnd,
		this->m_hInstance,
		parent->width(),
		parent->height(),
		isPreview,
		fileImporter->get_M_models()
	);
	
	this->m_Device = this->m_Communicator->GetDevice();
	this->m_fileImporter = fileImporter;
	this->m_fileImporter->setDevice(this->m_Device);
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent, FileImporter* fileImporter)
	: QWidget(parent) {

	setAttribute(Qt::WA_DontShowOnScreen, true);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);

	Initialize(parent, false, fileImporter);
	setFocusPolicy(Qt::StrongFocus);
}

void D3DRenderWidget::startTimer()
{
	LARGE_INTEGER frequencycount;

	QueryPerformanceFrequency(&frequencycount);
	m_countsPerSecond = double(frequencycount.QuadPart);

	QueryPerformanceCounter(&frequencycount);
	m_counterStart = frequencycount.QuadPart;
}

double D3DRenderWidget::getTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - m_counterStart) / m_countsPerSecond;
}

double D3DRenderWidget::getFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - this->m_frameTimeOld;
	this->m_frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / this->m_countsPerSecond;
}