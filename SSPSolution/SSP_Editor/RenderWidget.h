#pragma once
#include "ui_SSP_Editor.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "EditorCommunicator.h"
#include "FileImporter.h"

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
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Communicator* m_Communicator;
	FileImporter* m_fileImporter;
	void Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter);

	
private: //for deltaTime
	double m_countsPerSecond = 0.0;
	__int64 m_counterStart = 0;

	int m_frameCount = 0;
	int m_fps = 0;

	__int64 m_frameTimeOld = 0;
	double m_frameTime;
public:
	Communicator* getCommunicator() { return this->m_Communicator; };
private:
	double getFrameTime();
	void startTimer();
	double getTime();
};
