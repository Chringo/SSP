#include "RenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	this->m_GraphicsHandler->Shutdown();
	delete this->m_GraphicsHandler;
	delete this->m_EditorInputHandler;
}
void D3DRenderWidget::paintEvent(QPaintEvent * evt)
{
	//render
	//send a signal to render here, qt qill keep signals coming calling the render func or whichever
	this->frameCount++;
	if (getTime() > 1.0f)
	{
		this->fps = frameCount;
		this->frameCount = 0;
		startTimer();
	}
	frameTime = getFrameTime();


	this->m_EditorInputHandler->detectInput(frameTime);
	this->m_GraphicsHandler->Render();
	this->update();
}
void D3DRenderWidget::Initialize(QWidget* parent)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = (HWND)parent->winId();
	this->m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	this->m_GraphicsHandler->Initialize(&this->m_hwnd, DirectX::XMINT2(parent->width(), parent->height()));
	this->m_Camera = new Camera();
	this->m_Camera->Initialize();
	Camera* oldCam = this->m_GraphicsHandler->SetCamera(this->m_Camera);
	delete oldCam;
	oldCam = nullptr;
	this->m_EditorInputHandler = new EditorInputHandler(this->m_hInstance,this->m_hwnd,this->m_Camera, this->m_Width, this->m_Height);
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent)
	: QWidget(parent) {
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	Initialize(parent);
	// Create Device
	//createDevice();
	//properly create info that graphics handler need to make a swapchain etc here, parent IS where we want the info
	//that means width height and more can be accessed here
	//this->m_timer = new QTimer(this);
	//connect(this->m_timer, SIGNAL(timeout()), this, SLOT(update()));
	//this->m_timer->start(16);

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