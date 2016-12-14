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
		this->m_Communicator->m_EditorInputHandler->detectInput(this->m_frameTime);
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
					this->m_Communicator->m_GraphicsHandler->RenderFromEditor(
						modelPtr->at(i),
						&InstancePtr->at(j).component
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

void D3DRenderWidget::Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter)
{
	InitDosConsole();
	this->m_Communicator = new Communicator();
	this->m_hwnd = (HWND)parent->winId();
	this->m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	Resources::Status st;

	st = this->m_Communicator->Initialize(this->m_hwnd, this->m_hInstance, parent->width(), parent->height(), isPreview, fileImporter->get_M_models());
	this->m_Device = this->m_Communicator->GetDevice();
	this->m_fileImporter = fileImporter;
	this->m_fileImporter->setDevice(this->m_Device);
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent, FileImporter* fileImporter)
	: QWidget(parent) {

	//COMMENT THESE OUT TO ENABLE USE OF 2 RENDER WIDGETS
	setAttribute(Qt::WA_DontShowOnScreen, true);
	parent->update();
	// ***

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	if (parent->width() == 161)
	{
		Initialize(parent, true, fileImporter);
	}
	else
	{
		Initialize(parent, false, fileImporter);
	}
}

void D3DRenderWidget::resizeEvent(QResizeEvent * evt)
{
	//releaseBuffers();
	//swapChain_->ResizeBuffers(1, width(), height(), swapChainDesc_.BufferDesc.Format, 0);
	//swapChain_->GetDesc(&swapChainDesc_);
	//viewport_.Width = width();
	//viewport_.Height = height();
	//createBuffers();
	//this->m_Width = width();
	//this->m_Height = height();

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