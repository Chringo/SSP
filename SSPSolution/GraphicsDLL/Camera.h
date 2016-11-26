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
	//Pos:		0, 0, -1
	//LookAt:	0, 0, 0
	//CamUp:	0, 1, 0
	DirectX::XMFLOAT4X4 m_baseViewMatrix;
	//The three vectors that defines the new coordinate system
	DirectX::XMFLOAT4 m_cameraPos;
	DirectX::XMFLOAT4 m_lookAt;
	DirectX::XMFLOAT4 m_cameraUp;

	//The three rotational values a 3D object is capable of
	float m_roll;
	float m_pitch;
	float m_yaw;
public:
	Camera();
	virtual ~Camera();

	//Creates the base camera views
	int Initialize();
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
	void GetLookAt(DirectX::XMVECTOR& storeIn);
	void GetLookAt(DirectX::XMFLOAT4& storeIn);
	void GetLookAt(DirectX::XMFLOAT3& storeIn);
	void GetCameraUp(DirectX::XMVECTOR& storeIn);
	void GetCameraUp(DirectX::XMFLOAT4& storeIn);
	void GetCameraUp(DirectX::XMFLOAT3& storeIn);
	float GetRoll();
	float GetPitch();
	float GetYaw();
#pragma endregion getters
#pragma region
	void SetCameraPos(DirectX::XMFLOAT4 newCamPos);
	void SetCameraPos(DirectX::XMVECTOR newCamPos);
	void SetLookAt(DirectX::XMFLOAT4 newLookAt);
	void SetLookAt(DirectX::XMVECTOR newLookAt);
	void SetCameraUp(DirectX::XMFLOAT4 newCamUp);
	void SetCameraUp(DirectX::XMVECTOR newCamUp);
	//Additional value control methods
	void AddToCameraPos(DirectX::XMFLOAT3 applyValue);
	void AddToLookAt(DirectX::XMFLOAT3 applyValue);
	void AddToCameraUp(DirectX::XMFLOAT3 applyValue);
	void MultiplyCameraPos(DirectX::XMFLOAT3 multiplyValue);
	void MultiplyLookAt(DirectX::XMFLOAT3 multiplyValue);
	void MultiplyCameraUp(DirectX::XMFLOAT3 multiplyValue);

	void SetRoll(float roll);
	void ApplyRoll(float rollIncrease);
	void SetPitch(float pitch);
	void ApplyPitch(float pitchIncrease);
	void SetYaw(float yaw);
	void ApplyYaw(float yawIncrease);
#pragma endregion setters
private:
};

#endif