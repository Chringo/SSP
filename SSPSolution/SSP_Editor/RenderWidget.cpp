#include "RenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	this->m_GraphicsHandler->Shutdown();
	delete this->m_GraphicsHandler;
	if (this->m_test == 1)
	{
		delete this->m_EditorInputHandler;
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

	if (m_test == 1)
	{
		this->m_EditorInputHandler->detectInput(this->m_frameTime);
	}
	this->m_GraphicsHandler->Render();
	this->update();
}

void D3DRenderWidget::Initialize(QWidget* parent, bool isPreview)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = (HWND)parent->winId();
	if (!isPreview)
	{
		this->m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);

	}
	this->m_GraphicsHandler->Initialize(&this->m_hwnd, DirectX::XMINT2(parent->width(), parent->height()));
	this->m_Camera = new Camera();
	this->m_Camera->Initialize();
	Camera* oldCam = this->m_GraphicsHandler->SetCamera(this->m_Camera);
	delete oldCam;
	oldCam = nullptr;
	this->m_test = 0;
	if (!isPreview)
	{
		this->m_EditorInputHandler = new EditorInputHandler(this->m_hInstance,this->m_hwnd,this->m_Camera, this->m_Width, this->m_Height);
		this->m_test = 1;
	}
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent)
	: QWidget(parent) {

	//COMMENT THESE OUT TO ENABLE USE OF 2 RENDER WIDGETS
	setAttribute(Qt::WA_DontShowOnScreen, true);
	parent->update();
	// ***

	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	if (parent->width() == 256)
	{
		Initialize(parent, true);
	}
	else
	{
		Initialize(parent, false);
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