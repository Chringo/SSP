#ifndef GRAPHICSDLL_GRAPHICSHANDLER
#define GRAPHICSDLL_GRAPHICSHANDLER

#include "Direct3DHandler.h"
#include "DeferredShader.h"
#include "FinalShader.h"
#include "Camera.h"
#include "GraphicsComponent.h"
#include "ConstantBufferHandler.h"
#include "ShaderControl.h"
#include "DebugRenderer.h"

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

const int ARRAY_INC = 5;

class GRAPHICSDLL_API GraphicsHandler
{

#ifdef _DEBUG
private:
	DebugRenderer m_debugRender;
	std::vector<OBB*> obbBoxes;
	std::vector<AABB*> aabbBoxes;
public:
	void RenderBoundingVolume(OBB& box);
	void RenderBoundingVolume(AABB& box);
#endif // _DEBUG

private:
	Direct3DHandler*		m_d3dHandler;
	ConstantBufferHandler * m_constantBufferHandler;
	DeferredShader*			m_deferredSH;
	FinalShader*			m_finalSH;
	ShaderControl*			m_shaderControl;
	HWND* m_windowHandle;
	bool postProcessing = false;
	Resources::Model** m_modelsPtr;

	penis** m_animGraphicsComponents;
	GraphicsComponent** m_graphicsComponents;
	int m_nrOfGraphicsComponents;
	int m_maxGraphicsComponents;

	//temp
	Camera* m_camera;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	//Helper functions
	int IncreaseArraySize();
	int IncreaseArraySize(int increaseTo);
	int DecreaseArraySize();
	int DecreaseArraySize(int decreaseTo);

	bool m_gridEnabled;
public:
	GraphicsHandler();
	~GraphicsHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution);
	ID3D11Device* GetDevice() { return this->m_d3dHandler->GetDevice(); };
	Camera* SetCamera(Camera* newCamera);
	int Render();

	int SetComponentArraySize(int newSize);
	GraphicsComponent* GetNextAvailableComponent();
	int UpdateComponentList();

	int InitializeGrid();
	int RenderGrid(int& align, float& scale);
	int RenderFromEditor(Resources::Model* model, GraphicsComponent* component);
	void Shutdown();

	//TEMP STUFF
public:
	void SetTempAnimComponent(void*);
private:
	void m_CreateTempsTestComponents();
};

#endif