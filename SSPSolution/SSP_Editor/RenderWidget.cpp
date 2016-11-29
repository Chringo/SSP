#include "RenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	this->m_GraphicsHandler->Shutdown();
	delete this->m_GraphicsHandler;
}
void D3DRenderWidget::paintEvent(QPaintEvent * evt)
{
	//render
	//send a signal to render here, qt qill keep signals coming calling the render func or whichever
	this->m_GraphicsHandler->Render();
}
void D3DRenderWidget::Initialize(QWidget* parent)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	this->m_hwnd = (HWND)parent->winId();
	this->m_GraphicsHandler->Initialize(&this->m_hwnd, DirectX::XMINT2(parent->width(), parent->height()));
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent)
	: QWidget(parent) {
	Initialize(parent);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	// Create Device
	//createDevice();
	//properly create info that graphics handler need to make a swapchain etc here, parent IS where we want the info
	//that means width height and more can be accessed here
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