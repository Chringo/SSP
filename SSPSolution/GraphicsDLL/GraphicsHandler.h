#ifndef GRAPHICSDLL_GRAPHICSHANDLER
#define GRAPHICSDLL_GRAPHICSHANDLER

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

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

	int Initialize(HWND* windowHandle, const DirectX::XMFLOAT2& resolution);
	int Render(const DirectX::XMMATRIX& viewMatrix, const DirectX::XMFLOAT3& cameraPos);
	void Shutdown();
};

#endif