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
	DirectX::XMMATRIX m_viewMatrix;
	//The base view matrix were created with the following values
	//Pos:		0, 0, -1
	//LookAt:	0, 0, 0
	//CamUp:	0, 1, 0
	DirectX::XMMATRIX m_baseViewMatrix;
	//The three vectors that defines the new coordinate system
	DirectX::XMVECTOR m_cameraPos;
	DirectX::XMVECTOR m_lookAt;
	DirectX::XMVECTOR m_cameraUp;

	//The three rotational values a 3D object is capable of
	float m_roll;
	float m_pitch;
	float m_yaw;
public:
	Camera();
	virtual ~Camera();

	//Creates the base camera views
	int Initialize();
	//Update the camera view matrix based on the 6 comtained values available through the setters
	int Update();
#pragma region
	void GetViewMatrix(DirectX::XMMATRIX& storeIn);
	void GetBaseViewMatrix(DirectX::XMMATRIX& storeIn);
	void GetCameraPos(DirectX::XMVECTOR& storeIn);
	void GetCameraPos(DirectX::XMFLOAT3& storeIn);
	void GetLookAt(DirectX::XMVECTOR& storeIn);
	void GetLookAt(DirectX::XMFLOAT3& storeIn);
	void GetCameraUp(DirectX::XMVECTOR& storeIn);
	void GetCameraUp(DirectX::XMFLOAT3& storeIn);
	float GetRoll();
	float GetPitch();
	float GetYaw();
#pragma endregion getters
#pragma region
	void SetCameraPos(DirectX::XMFLOAT3 newCamPos);
	void SetCameraPos(DirectX::XMVECTOR newCamPos);
	void SetLookAt(DirectX::XMFLOAT3 newLookAt);
	void SetLookAt(DirectX::XMVECTOR newLookAt);
	void SetCameraUp(DirectX::XMFLOAT3 newCamUp);
	void SetCameraUp(DirectX::XMVECTOR newCamUp);

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