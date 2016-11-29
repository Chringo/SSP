#include "RenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	this->m_GraphicsHandler->Shutdown();
	delete this->m_GraphicsHandler;
	delete this->m_camera;
}
void D3DRenderWidget::paintEvent(QPaintEvent * evt)
{
	//render
	//send a signal to render here, qt qill keep signals coming calling the render func or whichever
	this->m_camera->Update();
	this->m_GraphicsHandler->Render();

}
void D3DRenderWidget::Initialize(QWidget* parent)
{
	this->m_GraphicsHandler = new GraphicsHandler();
	//parent->createWinId();
	//WId test;
	//test = parent->winId();
	this->m_hwnd = (HWND)parent->winId();
	this->m_GraphicsHandler->Initialize(&this->m_hwnd, DirectX::XMINT2(parent->width(), parent->height()));
	this->m_camera = new Camera();
	this->m_camera->Initialize();
	this->m_GraphicsHandler->SetCamera(this->m_camera);
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent)
	: QWidget(parent) {
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	this->m_Width = parent->width();
	this->m_Height = parent->height();
	this->m_x = parent->x();
	this->m_y = parent->y();
	parent->pos();
	// Create Device
	Initialize(parent);
	//createDevice();
	//properly create info that graphics handler need to make a swapchain etc here, parent IS where we want the info
	//that means width height and more can be accessed here
}