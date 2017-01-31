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
#include "UIHandler.h"

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

const int ARRAY_INC = 5;

class GraphicsHandler
{

#ifdef _DEBUG
private:
	bool editorMode = false;
	enum BoundingTypes {
		T_OBB,
		T_AABB,
		T_PLANE,
		T_SPHERE,
		T_WAYPOINT,
		T_NUM_TYPES
	};
	DebugRenderer m_debugRender;
	std::vector<OBB  *>   obbBoxes;
	std::vector<AABB *>  aabbBoxes;
	std::vector<Plane*> planes;
	std::vector<Sphere*> spheres;

	std::vector<int> numWaypoints;
	std::vector<DirectX::XMVECTOR*> positions[T_NUM_TYPES];
	std::vector<DirectX::XMVECTOR>  colors[T_NUM_TYPES];
	
	
	ID3D11DepthStencilView* dsv;
public:
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,OBB& box,     DirectX::XMVECTOR color = { 1.0f,0.0f,0.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,AABB& box,    DirectX::XMVECTOR color = { 0.0f,1.0f,0.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,Plane& plane, DirectX::XMVECTOR color = { 0.0f,0.0f,1.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos, Sphere& sphere, DirectX::XMVECTOR color = { 0.0f,0.0f,1.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR * wayPoints, int numWaypoints, DirectX::XMVECTOR color = { 0.0f,1.0f,0.0f });
private:
	void RenderBoundingBoxes(bool noClip = true);
#endif // _DEBUG

private:
	Direct3DHandler*		m_d3dHandler;
	ConstantBufferHandler * m_constantBufferHandler;
	DeferredShader*			m_deferredSH;
	FinalShader*			m_finalSH;
	ShaderControl*			m_shaderControl;
	UIHandler*				m_uiHandler;
	HWND* m_windowHandle;
	bool postProcessing = false;

	GraphicsAnimationComponent** m_animGraphicsComponents = nullptr;
	GraphicsComponent** m_graphicsComponents;
	int m_nrOfGraphicsComponents;
	int m_maxGraphicsComponents;
	int m_nrOfGraphicsAnimationComponents;
	int m_maxGraphicsAnimationComponents;
	//New way of saving graphics components
	std::vector<GraphicsComponent*> m_staticGraphicsComponents;
	std::vector<GraphicsComponent*> m_dynamicGraphicsComponents;
	std::vector<GraphicsAnimationComponent*> m_animationGraphicsComponents;


	//temp
	Camera* m_camera;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	//Helper functions
	int IncreaseArraySize();
	int IncreaseArraySize(int increaseTo);
	int DecreaseArraySize();
	int DecreaseArraySize(int decreaseTo);

	int IncreaseArraySizeAnim();
	int IncreaseArraySizeAnim(int increaseTo);
	int DecreaseArraySizeAnim();
	int DecreaseArraySizeAnim(int decreaseTo);

	//[0] = x, [1] = y, [2] = z
	int m_octreeSize[3];
	int m_maxDepth;			//The max depth of a node that contains components
	int m_minDepth;			//The minimum amount of depth for a node that contains components
	int m_minSize;			//The minimum size of a node for which it is still worth splitting
	int m_minContainment;	//The minimum amount contained components for which it is still wroth splitting after minDepth is reached
	//We want to keep splittin the nodes until the time it takes to check if the nodes that contain the components becomes larger than the time gained by culling away the contained components
	struct OctreeNode {
		OctreeNode* extensions[8];
		std::vector<unsigned int> containedEntities;
	};
	OctreeNode m_octreeRoot;
public:
	GRAPHICSDLL_API GraphicsHandler();
	GRAPHICSDLL_API ~GraphicsHandler();

	GRAPHICSDLL_API int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution, bool editorMode = false);
	GRAPHICSDLL_API ID3D11Device* GetDevice() { return this->m_d3dHandler->GetDevice(); };
	GRAPHICSDLL_API Camera* SetCamera(Camera* newCamera);
	GRAPHICSDLL_API int Render(float deltaTime);

	GRAPHICSDLL_API int SetComponentArraySize(int newSize);
	GRAPHICSDLL_API int SetAnimComponentArraySize(int newSize);
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableComponent();
	GRAPHICSDLL_API GraphicsAnimationComponent* GetNextAvailableAnimationComponent();
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableStaticComponent();
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableDynamicComponent();
	GRAPHICSDLL_API int UpdateComponentList();
	GRAPHICSDLL_API int UpdateAnimComponentList();

	GRAPHICSDLL_API UIComponent* GetNextAvailableUIComponent();
	GRAPHICSDLL_API void UpdateUIComponents(DirectX::XMFLOAT2 mousePos);

	GRAPHICSDLL_API TextComponent* GetNextAvailableTextComponent();
	
	GRAPHICSDLL_API int InitializeGrid();
	GRAPHICSDLL_API int RenderGrid(Resources::Model* model, GraphicsComponent* component);
	GRAPHICSDLL_API int RenderFromEditor(Resources::Model* model, GraphicsComponent* component);
	GRAPHICSDLL_API int renderFinalEditor();
	GRAPHICSDLL_API int clearEditor();
	GRAPHICSDLL_API void Shutdown();

	GRAPHICSDLL_API GraphicsAnimationComponent** GetGraphicsAnimationComponents() { return m_animGraphicsComponents; };
	GRAPHICSDLL_API int * GetAmountOfGraphicAnimationComponents() { return &m_nrOfGraphicsAnimationComponents; };

	//Culling functions
	//Function generates an internal datastructure for accelerated rendering through culling techniques. Return: 0 if no components elegible for accelerated datastructure inclusion. 1 if there were comopnents elegible. -1 if the accelerated datastructure could not be created.
	GRAPHICSDLL_API int GenerateOctree();
	//Increase the capacity of the container to a value that's greater or equal to new_cap. If new_cap is greater than the current capacity(), new storage is allocated, otherwise the method does nothing.
	GRAPHICSDLL_API int ReserveDynamicComponents(size_t new_cap);
	GRAPHICSDLL_API int ReserveStaticComponents(size_t new_cap);
	GRAPHICSDLL_API int ReserveAnimationComponents(size_t new_cap);
	//Does the same as above but adds the containers capacity to the new cap thus inreasing the capacity by 'addition'
	GRAPHICSDLL_API int ReserveAdditionalDynamicComponents(size_t addition);
	GRAPHICSDLL_API int ReserveAdditionalStaticComponents(size_t addition);
	GRAPHICSDLL_API int ReserveAdditionalAnimationComponents(size_t addition);

	

	//TEMP STUFF
public:
	GRAPHICSDLL_API void SetTempAnimComponent(void*);
	GRAPHICSDLL_API GraphicsComponent* getComponent(int index);
	GRAPHICSDLL_API GraphicsAnimationComponent* getAnimComponent(int index);
private:
	void m_CreateTempsTestComponents();
};

#endif