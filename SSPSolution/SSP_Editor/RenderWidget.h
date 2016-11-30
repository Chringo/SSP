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
	void Initialize(QWidget* parent);



	double countsPerSecond = 0.0;
	__int64 counterStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 frameTimeOld = 0;
	double frameTime;



	void startTimer()
	{
		LARGE_INTEGER frequencycount;

		QueryPerformanceFrequency(&frequencycount);
		countsPerSecond = double(frequencycount.QuadPart);

		QueryPerformanceCounter(&frequencycount);
		counterStart = frequencycount.QuadPart;
	}

	double getTime()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return double(currentTime.QuadPart - counterStart) / countsPerSecond;
	}

	double getFrameTime()
	{
		LARGE_INTEGER currentTime;
		__int64 tickCount;
		QueryPerformanceCounter(&currentTime);

		tickCount = currentTime.QuadPart - frameTimeOld;
		frameTimeOld = currentTime.QuadPart;

		if (tickCount < 0.0f)
		{
			tickCount = 0.0f;
		}

		return float(tickCount) / countsPerSecond;
	}
};
