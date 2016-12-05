#pragma once
#include "ui_SSP_Editor.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "EditorInputHandler.h"

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
	int m_test;
	int m_Width;
	int m_Height;
	int m_x;
	int m_y;
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Camera* m_Camera;
	GraphicsHandler* m_GraphicsHandler;
	EditorInputHandler* m_EditorInputHandler;
	void Initialize(QWidget* parent, bool isPreview);

	
private: //for deltaTime
	double m_countsPerSecond = 0.0;
	__int64 m_counterStart = 0;

	int m_frameCount = 0;
	int m_fps = 0;

	__int64 m_frameTimeOld = 0;
	double m_frameTime;

private:
	double getFrameTime();
	void startTimer();
	double getTime();
};
