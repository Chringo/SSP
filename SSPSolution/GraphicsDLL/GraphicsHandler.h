#ifndef GRAPHICSDLL_GRAPHICSHANDLER
#define GRAPHICSDLL_GRAPHICSHANDLER

#include "Direct3DHandler.h"
#include "DeferredShaderHandler.h"
#include "FinalShaderHandler.h"
#include "Camera.h"
#include "GraphicsComponent.h"
#include "ConstantBufferHandler.h"

//#define GRAPHICSDLL_EXPORTS
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

const int ARRAY_INC = 5;

class GRAPHICSDLL_API GraphicsHandler
{
private:
	Direct3DHandler* m_d3dHandler;
	ConstantBufferHandler * m_constantBufferHandler;
	DeferredShaderHandler* m_deferredSH;
	FinalShaderHandler* m_finalSH;

	HWND* m_windowHandle;

	Resources::Model** modelsPtr;

	GraphicsComponent** m_graphicsComponents;
	int m_nrOfGraphicsComponents;
	int m_maxGraphicsComponents;

	//temp
	Camera* m_camera;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	//Helper functions
	int IncreaseArraySize();
	int DecreaseArraySize();

	bool m_gridEnabled;
public:
	GraphicsHandler();
	~GraphicsHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution);
	Camera* SetCamera(Camera* newCamera);
	int Render();

	int InitializeGrid();
	int RenderGrid(int& align, float& scale);
	void Shutdown();
	
	//temp
	int CreateTriangle();
	int SetTriangle();
};

#endif