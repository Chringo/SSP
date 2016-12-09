#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>

//#define GRAPHICSDLL_EXPORTS
#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif

class GRAPHICSDLL_API Camera
{
private:
	//The second matrix used in deferred rendering
	DirectX::XMFLOAT4X4 m_viewMatrix;
	//The base view matrix were created with the following values
	//Pos:		0, 0, 0
	//LookAt:	0, 0, 1
	//CamUp:	0, 1, 0
	DirectX::XMFLOAT4X4 m_baseViewMatrix;
	//The three vectors that defines the new coordinate system
	DirectX::XMFLOAT4 m_cameraPos;
	DirectX::XMFLOAT4 m_lookAt;
	DirectX::XMFLOAT4 m_cameraUp;
	//The 4 values of a quaternion  
	DirectX::XMFLOAT4 m_rotation;
	DirectX::XMFLOAT4 m_rotateAroundPos;
public:
	Camera();
	virtual ~Camera();

	//Creates the base camera views
	int Initialize(float screenAspect = 1280.0f / 720, float fieldOfView = (float)DirectX::XM_PI / 4.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
	//Create a new camera view matrix based on the 6 comtained values available through the setters.
	//Also updates the cameraPos, lookAt and cameraUp values with the rotations in roll, pitch and yaw.
	int Update();
#pragma region
	void GetViewMatrix(DirectX::XMMATRIX& storeIn);
	DirectX::XMFLOAT4X4 * GetViewMatrix();
	void GetBaseViewMatrix(DirectX::XMMATRIX& storeIn);
	void GetCameraPos(DirectX::XMVECTOR& storeIn);
	void GetCameraPos(DirectX::XMFLOAT4& storeIn);
	void GetCameraPos(DirectX::XMFLOAT3& storeIn);
	DirectX::XMFLOAT3 GetCameraPos();
	void GetLookAt(DirectX::XMVECTOR& storeIn);
	void GetLookAt(DirectX::XMFLOAT4& storeIn);
	void GetLookAt(DirectX::XMFLOAT3& storeIn);
	DirectX::XMFLOAT3 GetLookAt();
	void GetCameraUp(DirectX::XMVECTOR& storeIn);
	void GetCameraUp(DirectX::XMFLOAT4& storeIn);
	void GetCameraUp(DirectX::XMFLOAT3& storeIn);

#pragma endregion getters
#pragma region
	void SetCameraPos(DirectX::XMFLOAT4 newCamPos);
	void SetCameraPos(DirectX::XMVECTOR newCamPos);
	void SetLookAt(DirectX::XMFLOAT4 newLookAt);
	void SetLookAt(DirectX::XMVECTOR newLookAt);
	void SetCameraUp(DirectX::XMFLOAT4 newCamUp);
	void SetCameraUp(DirectX::XMVECTOR newCamUp);
	void SetRotation(DirectX::XMFLOAT4 newRotation);
	//Additional value control methods
	void AddToCameraPos(DirectX::XMFLOAT3 applyValue);
	void AddToLookAt(DirectX::XMFLOAT3 applyValue);
	void AddToCameraUp(DirectX::XMFLOAT3 applyValue);
	void MultiplyCameraPos(DirectX::XMFLOAT3 multiplyValue);
	void MultiplyLookAt(DirectX::XMFLOAT3 multiplyValue);
	void MultiplyCameraUp(DirectX::XMFLOAT3 multiplyValue);
	//Warning: As the camera uses quaternion rotations and only stores these in one position this functions can only be called once after setting a rotation and before updating.
	void ApplyRotation(DirectX::XMFLOAT4 rotationAddition);
	//Sets the camera position along the rotated coordinate system of the camera
	void SetLocalTranslation(float x, float y, float z);
	//Translates the camera along the rotated coordinate system of the camera
	void ApplyLocalTranslation(float x, float y, float z);
	//Calls the ApplyLocalTranslation(float x, float y, float z) with the values in translation
	void ApplyLocalTranslation(DirectX::XMFLOAT3 translation);
	//
	void SetRotationAroundPosOffset(float x, float y, float z);
	void SetRotationAroundPos(float x, float y, float z);
	
#pragma endregion setters
private:
};

#endif