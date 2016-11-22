#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX baseViewMatrix;
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAt;
	DirectX::XMVECTOR cameraUp;

	float roll;
	float pitch;
	float yaw;
public:
	Camera();
	virtual ~Camera();

	int Initialize();
	int Update();
#pragma region
	void GetViewMatrix(DirectX::XMMATRIX& storeIn);
	void GetBaseViewMatrix(DirectX::XMMATRIX& storeIn);
	DirectX::XMFLOAT3 GetCameraPos();
	DirectX::XMFLOAT3 GetLookAt();
	DirectX::XMFLOAT3 GetCameraUp();
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