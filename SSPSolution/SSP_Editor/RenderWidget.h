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

class D3DRenderWidget : public QWidget {
	Q_OBJECT
		Q_DISABLE_COPY(D3DRenderWidget)
public:
	D3DRenderWidget(QWidget* parent = NULL, FileImporter* fileImporter = NULL);
	virtual ~D3DRenderWidget();
	virtual QPaintEngine* paintEngine() const { return NULL; }
	virtual void keyPressEvent(QKeyEvent * evt);
	virtual void keyReleaseEvent(QKeyEvent * evt);
protected:
	virtual void paintEvent(QPaintEvent* evt);
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Communicator* m_Communicator;
	FileImporter* m_fileImporter;
	ID3D11Device* m_Device;
	void Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter);

	
private: //for deltaTime
	double m_countsPerSecond = 0.0;
	__int64 m_counterStart = 0;

	int m_frameCount = 0;
	int m_fps = 0;

	__int64 m_frameTimeOld = 0;
	double m_frameTime;
public:
	ID3D11Device* getDevice() { return this->m_Device; };
	Communicator* getCommunicator() { return this->m_Communicator; };
private:
	double getFrameTime();
	void   startTimer();
	double getTime();
	void InitDosConsole() {
		AllocConsole();
		freopen("CONIN$", "rb" , stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
};
