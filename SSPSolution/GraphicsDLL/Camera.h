#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>
#include "../physicsDLL/PhysicsHandler.h" 

//#define GRAPHICSDLL_EXPORTS
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

enum CullingResult {
	FRUSTRUM_OUTSIDE = 0,
	FRUSTRUM_INTERSECT,
	FRUSTRUM_INSIDE
};

struct cameraFrameData
{
	DirectX::XMMATRIX pView;
	DirectX::XMMATRIX pProjection;
	DirectX::XMVECTOR pPos;
};

class Camera
{
private:
	//The second matrix used in deferred rendering
	DirectX::XMFLOAT4X4 m_viewMatrix;
	//The base view matrix were created with the following values
	//Pos:		0, 0, 0
	//LookAt:	0, 0, 1
	//CamUp:	0, 1, 0
	DirectX::XMFLOAT4X4 m_baseViewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
	//The three vectors used to define the new coordinate system
	DirectX::XMFLOAT4 m_cameraPos;
	DirectX::XMFLOAT4 m_lookAt;
	DirectX::XMFLOAT4 m_cameraUp;

	DirectX::XMVECTOR * m_focusPoint;
	DirectX::XMVECTOR m_focusPointOffset;
	DirectX::XMVECTOR m_cameraMaxDistancePos;
	//DirectX::XMVECTOR m_targetCameraPos;
	//DirectX::XMVECTOR m_targetCameraRot;
	//DirectX::XMVECTOR m_targetCameraUp;
	DirectX::XMVECTOR m_camRightvector;
	DirectX::XMVECTOR m_camDirvector;
	float m_distance;
	float m_maxDistance;
	float m_yaw;
	float m_pitch;
	//The values for the projection matrix
	float m_screenAspect;
	float m_fieldOfView;
	float m_deltaTime;
	float m_targetDistance;

public:
	struct Plane {
		DirectX::XMFLOAT4 normal;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
	struct C_AABB {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 ext;
		DirectX::XMVECTOR GetPositiveVertex(const DirectX::XMVECTOR &normal);
		DirectX::XMVECTOR GetNegativeVertex(const DirectX::XMVECTOR &normal);
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
	struct C_OBB {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 ext;
		DirectX::XMFLOAT4X4 ort;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
	struct C_Ray {
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT3 dir;
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
	struct C_BOX {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 min;
		DirectX::XMFLOAT3 max;
	};

	struct ViewFrustrum {
		//Left, Right, Bottom, Top, Near, Far
		Plane myPlanes[6];
		//0 = outside. 1 = intersects frustrum. 2 = inside frustrum.
		CullingResult TestAgainstAABB(C_AABB box);
		CullingResult TestAgainstBox(C_BOX box);
		//An conservative test is fast but may not cull all things that could be culled
		CullingResult TestAgainstOBBConservative(C_OBB box);
		//An exact test will always cull all things perfectly but is slow
		CullingResult TestAgainstOBBExact(C_OBB box);
		void* operator new(size_t i) { return _aligned_malloc(i, 16); };
		void operator delete(void* p) { _aligned_free(p); };
	};
public:
	GRAPHICSDLL_API Camera();
	GRAPHICSDLL_API virtual ~Camera();
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
	//Creates the base camera views
	GRAPHICSDLL_API int Initialize(float screenAspect = 1280.f / 720.f, float fieldOfView = ((float)DirectX::XM_PI*5)/12.0f, float nearPlane = 0.1f, float farPlane = 200.0f);
	//Create a new camera view matrix based on the 6 comtained values available through the setters.
	//Also updates the cameraPos, lookAt and cameraUp values with the rotations in roll, pitch and yaw.
	GRAPHICSDLL_API int Update();
	GRAPHICSDLL_API int UpdateDeltaTime(float dt);
	GRAPHICSDLL_API int UpdateView();
	GRAPHICSDLL_API int UpdateProjection();
	GRAPHICSDLL_API int UpdateProjection(float screenAspect, float fieldOfView = (float)DirectX::XM_PI / 4.0f, float nearPlane = 0.1f, float farPlane = 200.0f);
	//	0/1 = failed(succeeded to create the view frustrum.
	GRAPHICSDLL_API int GetViewFrustrum(ViewFrustrum& storeIn);
	GRAPHICSDLL_API int Reset();
	GRAPHICSDLL_API C_Ray CastRay();
	GRAPHICSDLL_API C_Ray CastRayFromMaxDistance();
	GRAPHICSDLL_API int AddToIntersectCheck(DirectX::XMFLOAT4X4 ort, DirectX::XMFLOAT3 ext, DirectX::XMFLOAT3 pos);
	GRAPHICSDLL_API int ClearIntersectList();

#pragma region
	GRAPHICSDLL_API void GetViewMatrix(DirectX::XMMATRIX& storeIn);
	GRAPHICSDLL_API DirectX::XMFLOAT4X4 * GetViewMatrix();
	GRAPHICSDLL_API void GetBaseViewMatrix(DirectX::XMMATRIX& storeIn);
	GRAPHICSDLL_API DirectX::XMFLOAT4X4 * GetProjectionMatrix();
	GRAPHICSDLL_API void GetCameraPos(DirectX::XMVECTOR& storeIn);
	GRAPHICSDLL_API void GetCameraPos(DirectX::XMFLOAT4& storeIn);
	GRAPHICSDLL_API void GetCameraPos(DirectX::XMFLOAT3& storeIn);
	GRAPHICSDLL_API DirectX::XMFLOAT3 GetCameraPos();
	GRAPHICSDLL_API void GetLookAt(DirectX::XMVECTOR& storeIn);
	GRAPHICSDLL_API void GetLookAt(DirectX::XMFLOAT4& storeIn);
	GRAPHICSDLL_API void GetLookAt(DirectX::XMFLOAT3& storeIn);
	GRAPHICSDLL_API DirectX::XMFLOAT3 GetLookAt();
	GRAPHICSDLL_API void GetCameraUp(DirectX::XMVECTOR& storeIn);
	GRAPHICSDLL_API void GetCameraUp(DirectX::XMFLOAT4& storeIn);
	GRAPHICSDLL_API void GetCameraUp(DirectX::XMFLOAT3& storeIn);
	GRAPHICSDLL_API DirectX::XMVECTOR GetCameraPivot();
	GRAPHICSDLL_API float GetCameraDistance();
	GRAPHICSDLL_API float GetCameraMaxDistance();
	GRAPHICSDLL_API DirectX::XMVECTOR GetMaxDistanceCamPos() { return m_cameraMaxDistancePos; };

	GRAPHICSDLL_API void GetCameraFrameData(cameraFrameData& storeIn);
	GRAPHICSDLL_API cameraFrameData GetCameraFrameData();
	GRAPHICSDLL_API DirectX::XMVECTOR GetDirection();

#pragma endregion getters
#pragma region
	GRAPHICSDLL_API void SetCameraPos(DirectX::XMFLOAT4 newCamPos);
	GRAPHICSDLL_API void SetCameraPos(DirectX::XMVECTOR newCamPos);
	GRAPHICSDLL_API void SetCameraPivot(DirectX::XMVECTOR *lockTarget, DirectX::XMVECTOR targetOffset, float distance);
	GRAPHICSDLL_API void SetCameraPivotOffset(DirectX::XMVECTOR targetOffset, float distance);
	GRAPHICSDLL_API void SetLookAt(DirectX::XMFLOAT4 newLookAt);
	GRAPHICSDLL_API void SetLookAt(DirectX::XMVECTOR newLookAt);
	GRAPHICSDLL_API void SetCameraUp(DirectX::XMFLOAT4 newCamUp);
	GRAPHICSDLL_API void SetCameraUp(DirectX::XMVECTOR newCamUp);
	//GRAPHICSDLL_API void SetRotation(DirectX::XMFLOAT4 newRotation);
	//Additional value control methods
	GRAPHICSDLL_API void AddToCameraPos(DirectX::XMFLOAT3 applyValue);
	GRAPHICSDLL_API void AddToLookAt(DirectX::XMFLOAT3 applyValue);
	GRAPHICSDLL_API void AddToCameraUp(DirectX::XMFLOAT3 applyValue);
	GRAPHICSDLL_API void MultiplyCameraPos(DirectX::XMFLOAT3 multiplyValue);
	GRAPHICSDLL_API void MultiplyLookAt(DirectX::XMFLOAT3 multiplyValue);
	GRAPHICSDLL_API void MultiplyCameraUp(DirectX::XMFLOAT3 multiplyValue);
	//Warning: As the camera uses quaternion rotations and only stores these in one position this functions can only be called once after setting a rotation and before updating.
	//GRAPHICSDLL_API void ApplyRotation(DirectX::XMFLOAT4 rotationAddition);
	GRAPHICSDLL_API void RotateCameraPivot(float pitch, float yaw);
	GRAPHICSDLL_API void RotateCamera(double x, double y, double z, double angle);
	GRAPHICSDLL_API void RotateCamera(DirectX::XMFLOAT4 rotation);
	GRAPHICSDLL_API void RotateCamera(DirectX::XMVECTOR rotation);
	//Sets the camera position along the rotated coordinate system of the camera
	GRAPHICSDLL_API void SetLocalTranslation(float x, float y, float z);
	//Translates the camera along the rotated coordinate system of the camera
	GRAPHICSDLL_API void ApplyLocalTranslation(float x, float y, float z);
	//Calls the ApplyLocalTranslation(float x, float y, float z) with the values in translation
	GRAPHICSDLL_API void ApplyLocalTranslation(DirectX::XMFLOAT3 translation);
	GRAPHICSDLL_API void SetDistance(float newDistance);
	GRAPHICSDLL_API void DecreaseDistance(float amount);
	GRAPHICSDLL_API void IncreaseDistance(float amount);

	GRAPHICSDLL_API DirectX::XMVECTOR GetRight();
#pragma endregion setters
private:
	DirectX::XMVECTOR Conjugate(DirectX::XMVECTOR quat);
	DirectX::XMVECTOR Mult(DirectX::XMVECTOR a, DirectX::XMVECTOR b);
	DirectX::XMVECTOR m_Dir();
	DirectX::XMVECTOR m_Right();
	void m_updatePos();
	void m_calcDistance();
	bool m_IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos, float &distanceToOBB);
	std::vector<C_OBB> m_intersectionOBBs;
};

#endif