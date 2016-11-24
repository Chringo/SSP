#ifndef GRAPHICSDLL_GRAPHICSHANDLER
#define GRAPHICSDLL_GRAPHICSHANDLER

#include "Direct3DHandler.h"
#include "DeferredShaderHandler.h"
#include "LightShaderHandler.h"
#include "Camera.h"

//#define GRAPHICSDLL_EXPORTS
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

class GRAPHICSDLL_API GraphicsHandler
{
private:
	Direct3DHandler* m_d3dHandler;

	DeferredShaderHandler* m_deferredSH;
	LightShaderHandler* m_lightSH;

	HWND* m_windowHandle;

	Camera* m_camera;

	DirectX::XMMATRIX m_projectionMatrix;

	//temp
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
public:
	GraphicsHandler();
	~GraphicsHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution);
	int SetCamera(Camera* newCamera);
	int Render();
	void Shutdown();

	//temp
	int CreateTriangle();
	int SetTriangle();
};

#endif