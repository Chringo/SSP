#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>

//#define GRAPHICSDLL_EXPORTS
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

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
	//DirectX::XMFLOAT4 m_rotation;
	//The values for the projection matrix
	float m_screenAspect;
	float m_fieldOfView;
public:
	GRAPHICSDLL_API Camera();
	GRAPHICSDLL_API virtual ~Camera();

	//Creates the base camera views
	GRAPHICSDLL_API int Initialize(float screenAspect = 1280.f / 720, float fieldOfView = (float)DirectX::XM_PI / 4.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
	//Create a new camera view matrix based on the 6 comtained values available through the setters.
	//Also updates the cameraPos, lookAt and cameraUp values with the rotations in roll, pitch and yaw.
	GRAPHICSDLL_API int Update();
	GRAPHICSDLL_API int UpdateProjection();
	GRAPHICSDLL_API int UpdateProjection(float screenAspect, float fieldOfView = (float)DirectX::XM_PI / 4.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
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

	GRAPHICSDLL_API void GetCameraFrameData(cameraFrameData& storeIn);
	GRAPHICSDLL_API cameraFrameData GetCameraFrameData();

#pragma endregion getters
#pragma region
	GRAPHICSDLL_API void SetCameraPos(DirectX::XMFLOAT4 newCamPos);
	GRAPHICSDLL_API void SetCameraPos(DirectX::XMVECTOR newCamPos);
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
	GRAPHICSDLL_API void RotateCamera(double x, double y, double z, double angle);
	GRAPHICSDLL_API void RotateCamera(DirectX::XMFLOAT4 rotation);
	GRAPHICSDLL_API void RotateCamera(DirectX::XMVECTOR rotation);
	//Sets the camera position along the rotated coordinate system of the camera
	GRAPHICSDLL_API void SetLocalTranslation(float x, float y, float z);
	//Translates the camera along the rotated coordinate system of the camera
	GRAPHICSDLL_API void ApplyLocalTranslation(float x, float y, float z);
	//Calls the ApplyLocalTranslation(float x, float y, float z) with the values in translation
	GRAPHICSDLL_API void ApplyLocalTranslation(DirectX::XMFLOAT3 translation);
	GRAPHICSDLL_API void AlignWithRay(DirectX::XMVECTOR direction);

	
#pragma endregion setters
private:
	DirectX::XMVECTOR conjugate(DirectX::XMVECTOR quat);
	DirectX::XMVECTOR mult(DirectX::XMVECTOR a, DirectX::XMVECTOR b);
};

#endif