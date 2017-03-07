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
#include "LightHandler.h"
#include <algorithm>
#include <omp.h>
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

const int ARRAY_INC = 5;
const int OCTREE_NODE_MIN_CONTAINMENT = 1;
const int OCTREE_NODE_MAX_DEPTH = 5;
const int OCTREE_NODE_MIN_DEPTH = 1;
const float OCTREE_NODE_MIN_SIZE = 2.0f;
const unsigned int STATIC_SHADOWMAP_RESOLUTION = 512; // ratio always 1:1
const unsigned int CUBE_MAP_RESOLUTION = 128; //ratio always 1:1
const size_t MAX_ACTIVE_LIGHTS = 40;
					  
	
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
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,OBB& box,        DirectX::XMVECTOR color = { 1.0f,0.0f,0.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,AABB& box,       DirectX::XMVECTOR color = { 0.0f,1.0f,0.0f });
	GRAPHICSDLL_API void RenderBoundingVolume(DirectX::XMVECTOR& pos,Plane& plane,    DirectX::XMVECTOR color = { 0.0f,0.0f,1.0f });
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
	LIGHTING::LightHandler*			m_LightHandler;
	HWND* m_windowHandle;
	bool postProcessing = false;

	GraphicsAnimationComponent** m_animGraphicsComponents = nullptr;
	GraphicsComponent** m_graphicsComponents;
	int m_nrOfGraphicsComponents;
	int m_maxGraphicsComponents;
	int m_nrOfGraphicsAnimationComponents;
	int m_maxGraphicsAnimationComponents;
	//New way of saving graphics components which is not used yet
	//std::vector<GraphicsComponent*> m_staticGraphicsComponents;
	std::vector<GraphicsComponent> m_staticGraphicsComponents;
	std::vector<GraphicsComponent*> m_dynamicGraphicsComponents;
	std::vector<GraphicsComponent*> m_persistantGraphicsComponents;
	std::vector<GraphicsAnimationComponent*> m_animationGraphicsComponents;

	std::vector<int> m_activeLightIndices;

	ID3D11ShaderResourceView* m_sceneCubeMap = nullptr;
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
	float m_minSize;		//The minimum size of a nodes sides for which it is still worth splitting

	int m_minContainment;	//The minimum amount contained components for which it is still wroth splitting after minDepth is reached
	//We want to keep splittin the nodes until the time it takes to check if the nodes that contain the components becomes larger than the time gained by culling away the contained components
	struct OctreeBV {
		unsigned int componentIndex;
		unsigned int modelID;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 ext;
		bool isRendered;
		bool isInRay;
		Resources::Model* modelPtr;
		bool isInPingRay;
		
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	}; 
	struct OctreeNode {
		OctreeNode* branches[8] = { nullptr };
		std::vector<OctreeBV*> containedComponents;
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 ext;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
	OctreeNode m_octreeRoot;
	Camera m_overviewCamera;
	bool m_useOverview;
	
#ifdef _DEBUG
	int RenderOctree(OctreeNode * curNode, Camera::ViewFrustrum * cullingFrustrum);
#endif
	struct Sorting_on_modelID
	{
		inline bool operator() (const OctreeBV* first, const OctreeBV* second)
		{
			bool statementTrue = first->modelID < second->modelID;
			return statementTrue;
		}
	};

	//INACTIVE
	struct Find_Available_gComponent
	{
		inline bool operator() (const GraphicsComponent* comp)
		{
			return comp->active == 0;
		}
	};

	//INACTIVE
	struct GraphicsComponent_Remove_Inactive_Predicate {
		inline bool operator()(GraphicsComponent* component) {
			bool result = false;
			if (component != nullptr)
			{
				if (component->active == false)
				{
					delete component;
					component = nullptr;
					result = true;
				}
			}
			else
				result = true;
			return result;
		}
	};

	//USE AT CREATION OF A NEW LEVEL TO DELETE OLD COMPONENTS
	struct GraphicsComponent_Remove_All_Predicate {
		inline void operator()(GraphicsComponent* component) {
			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}
	};
	struct GraphicsComponent_Remove_All_Unary {
		inline GraphicsComponent* operator()(GraphicsComponent*& component) {
			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
			return component;
		}
	};

public:
	GRAPHICSDLL_API GraphicsHandler();
	GRAPHICSDLL_API ~GraphicsHandler();

	GRAPHICSDLL_API int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution, bool editorMode = false);
	GRAPHICSDLL_API ID3D11Device* GetDevice() { return this->m_d3dHandler->GetDevice(); };
	GRAPHICSDLL_API Camera* SetCamera(Camera* newCamera);
	GRAPHICSDLL_API int Render(float deltaTime);
	GRAPHICSDLL_API int RenderStaticObjectShadows();
	GRAPHICSDLL_API int RenderStaticScene();
	GRAPHICSDLL_API int Update(float deltaTime);


	GRAPHICSDLL_API int SetComponentArraySize(int newSize);
	GRAPHICSDLL_API int SetAnimComponentArraySize(int newSize);
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableComponent();
	GRAPHICSDLL_API GraphicsAnimationComponent* GetNextAvailableAnimationComponent();
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableStaticComponent();
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailableDynamicComponent();
	GRAPHICSDLL_API GraphicsComponent* GetNextAvailablePersistentComponent();
	GRAPHICSDLL_API int UpdateComponentList();
	GRAPHICSDLL_API int UpdateAnimComponentList();

	GRAPHICSDLL_API UIComponent* GetNextAvailableUIComponent();
	GRAPHICSDLL_API void UpdateUIComponents(DirectX::XMFLOAT2 mousePos);
	GRAPHICSDLL_API int RemoveUIComponentFromPtr(UIComponent* ptr);
	GRAPHICSDLL_API int RemoveLastUIComponent();

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
	GRAPHICSDLL_API int FrustrumCullOctreeLeft();
	GRAPHICSDLL_API int FrustrumCullOctreeRight();
	GRAPHICSDLL_API int FrustrumCullOctreeNodeThreaded(int threadCount);
	GRAPHICSDLL_API int FrustrumCullOctreeNode();
	//Deletes all data and creates a new vector of pointers to new empty datastructures for your "GetComponent" pleasures~
	GRAPHICSDLL_API int ResizeDynamicComponents(size_t new_cap);
	GRAPHICSDLL_API int ResizeStaticComponents(size_t new_cap);
	GRAPHICSDLL_API int ResizeAnimationComponents(size_t new_cap);
	GRAPHICSDLL_API int ResizePersistentComponents(size_t new_cap);
	GRAPHICSDLL_API int ResetAnimationComponents();


	GRAPHICSDLL_API int GenerateStaticSceneShadows();
	GRAPHICSDLL_API int GenerateSceneCubeMap();

	//TEMP STUFF
public:
	GRAPHICSDLL_API void SetTempAnimComponent(void*);
	GRAPHICSDLL_API GraphicsComponent* getComponent(int index);
	GRAPHICSDLL_API GraphicsAnimationComponent* getAnimComponent(int index);
	GRAPHICSDLL_API void ToggleOverviewCamera();
	GRAPHICSDLL_API float Ping_GetDistanceToClosestOBB(int maxDistance);	//Used for the Ping
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
private:
	void m_CreateTempsTestComponents();

	void OctreeExtend(OctreeNode* curNode, int depth);
	void TraverseOctree(OctreeNode* curNode, Camera::ViewFrustrum* cullingFrustrum);
	void TraverseOctreeRay(OctreeNode* curNode, Camera::C_Ray ray, bool pingRay);
	bool RayVSAABB(Camera::C_Ray ray, Camera::C_AABB bb, float& distance);
	bool PointVSAABB(DirectX::XMFLOAT3 pos, Camera::C_AABB bb);
	void DeleteOctree(OctreeNode* curNode);
	int AABBvsAABBIntersectionTest(DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 ext1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 ext2);
	inline OBB m_ConvertOBB(BoundingBoxHeader & boundingBox);
};

#endif