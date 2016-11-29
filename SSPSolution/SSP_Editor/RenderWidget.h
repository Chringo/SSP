#pragma once
#include "ui_SSP_Editor.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"

class D3DRenderWidget : public QWidget {
	Q_OBJECT
		Q_DISABLE_COPY(D3DRenderWidget)
public:
	D3DRenderWidget(QWidget* parent = NULL);
	virtual ~D3DRenderWidget();
	virtual QPaintEngine* paintEngine() const { return NULL; }
protected:
	virtual void paintEvent(QPaintEvent* evt);
	virtual void resizeEvent(QResizeEvent* evt);
	int m_Width;
	int m_Height;
	int m_x;
	int m_y;
private:
	QTimer* m_timer;
	HWND m_hwnd;
	Camera* m_camera;
	GraphicsHandler* m_GraphicsHandler;
	void Initialize(QWidget* parent);
};
