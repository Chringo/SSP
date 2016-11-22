#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_baseViewMatrix;
	DirectX::XMVECTOR m_cameraPos;
	DirectX::XMVECTOR m_lookAt;
	DirectX::XMVECTOR m_cameraUp;

	float m_roll;
	float m_pitch;
	float m_yaw;
public:
	Camera();
	virtual ~Camera();

	int Initialize();
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