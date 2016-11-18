#ifndef GRAPHICSDLL_GRAPHICSHANDLER
#define GRAPHICSDLL_GRAPHICSHANDLER

#include "Direct3DHandler.h"
#include "DeferredShaderHandler.h"
#include "LightShaderHandler.h"

class GraphicsHandler
{
private:
	Direct3DHandler* d3dHandler;

	DeferredShaderHandler* deferredSH;
	LightShaderHandler* lightSH;

	HWND* windowHandle;
public:
	GraphicsHandler();
	~GraphicsHandler();

	int Initialize(HWND* windowHandle);
	int Render();
	void Shutdown();
};

#endif