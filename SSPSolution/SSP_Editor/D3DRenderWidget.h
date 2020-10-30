#pragma once
#include "ui_SSP_Editor.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "EditorCommunicator.h"
#include "FileImporter.h"
#include "DataHandler.h"
#include <stdio.h>
#include "Windows.h"
#include "Wincon.h"
#include <QTimer>
#define DRAW_FRAMERATE 60
class D3DRenderWidget : public QWidget {
	Q_OBJECT
		Q_DISABLE_COPY(D3DRenderWidget)
public:
	D3DRenderWidget(QWidget* parent = NULL, FileImporter* fileImporter = NULL);
	virtual ~D3DRenderWidget();
	virtual QPaintEngine* paintEngine() const { return NULL; }
	virtual void keyPressEvent(QKeyEvent * evt);
	virtual void keyReleaseEvent(QKeyEvent * evt);
	virtual void mousePressEvent(QMouseEvent * evt);
	virtual void mouseReleaseEvent(QMouseEvent * evt);
	virtual void resizeEvent(QResizeEvent *event);

protected:
	virtual void paintEvent(QPaintEvent* evt) override;
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Communicator* m_Communicator;
	FileImporter* m_fileImporter;
	ID3D11Device* m_Device;
	QTimer *drawLoopTimer;
	void Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter);
	QWidget* parent;
	
private: //for deltaTime
	double m_countsPerSecond = 0.0;
	__int64 m_counterStart = 0;

	int m_frameCount = 0;
	int m_fps = 0;

	__int64 m_frameTimeOld = 0;
	double m_frameTime;
public:
	//void RenderScene();
	ID3D11Device* getDevice() { return this->m_Device; };
	Communicator* getCommunicator() { return this->m_Communicator; };
	const int GetFps() const {return m_fps;}
public slots:
	void RenderScene();
private:
	double getTime();
	double getFrameTime();
	void   startTimer();
	void InitDosConsole() {
		AllocConsole();
		freopen("CONIN$", "rb" , stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
};
