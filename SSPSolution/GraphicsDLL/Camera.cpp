#include "Camera.h"



Camera::Camera()
{
	this->m_viewMatrix = DirectX::XMMatrixIdentity();
	this->m_baseViewMatrix = DirectX::XMMatrixIdentity();
	this->m_cameraPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_cameraUp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	this->m_roll = 0.0f;
	this->m_pitch = 0.0f;
	this->m_yaw = 0.0f;
}


Camera::~Camera()
{
}

int Camera::Initialize()
{
	int result = 1;

	this->m_viewMatrix = DirectX::XMMatrixIdentity();
	this->m_cameraPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_cameraUp = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	this->m_roll = 0.0f;
	this->m_pitch = 0.0f;
	this->m_yaw = 0.0f;

	//Define the basic view matrix used in rendering the second stage of deferred rendering.
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR camPos = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 1.0f);
	DirectX::XMVECTOR camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	this->m_baseViewMatrix = DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp);

	//Define a transformation matrix based on the three rotations a 3D object is capable of
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_pitch, this->m_yaw, this->m_roll);
	//Transform the three components of the view matrix based on the rotations
	camPos = DirectX::XMVector3TransformCoord(this->m_cameraPos, camRotationMatrix);
	lookAt = DirectX::XMVector3TransformCoord(this->m_lookAt, camRotationMatrix);
	camUp = DirectX::XMVector3TransformCoord(this->m_cameraUp, camRotationMatrix);
	//Define the view matrix based on the transformed positions and vectors
	this->m_viewMatrix = DirectX::XMMatrixLookAtLH(this->m_cameraPos, this->m_lookAt, this->m_cameraUp);

	return result;
}

int Camera::Update()
{
	int result = 0;



	return result;
}

#pragma region
void Camera::GetViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMMATRIX(this->m_viewMatrix);
	return;
}

void Camera::GetBaseViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMMATRIX(this->m_baseViewMatrix);
	return;
}

void Camera::GetCameraPos(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMVECTOR(this->m_cameraPos);
	return;
}

void Camera::GetCameraPos(DirectX::XMFLOAT3 & storeIn)
{
	DirectX::XMStoreFloat3(&storeIn, this->m_cameraPos);
	return;
}

void Camera::GetLookAt(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMVECTOR(this->m_lookAt);
	return;
}

void Camera::GetLookAt(DirectX::XMFLOAT3 & storeIn)
{
	DirectX::XMStoreFloat3(&storeIn, this->m_lookAt);
	return;
}

void Camera::GetCameraUp(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMVECTOR(this->m_cameraUp);
	return;
}

void Camera::GetCameraUp(DirectX::XMFLOAT3 & storeIn)
{
	DirectX::XMStoreFloat3(&storeIn, this->m_cameraUp);
	return;
}

float Camera::GetRoll()
{
	return this->m_roll;
}

float Camera::GetPitch()
{
	return this->m_pitch;
}

float Camera::GetYaw()
{
	return this->m_yaw;
}
#pragma endregion getters
#pragma region
void Camera::SetCameraPos(DirectX::XMFLOAT3 newCamPos)
{
	this->m_cameraPos = DirectX::XMLoadFloat3(&newCamPos);
	return;
}

void Camera::SetCameraPos(DirectX::XMVECTOR newCamPos)
{
	this->m_cameraPos = newCamPos;
	return;
}

void Camera::SetLookAt(DirectX::XMFLOAT3 newLookAt)
{
	this->m_lookAt = DirectX::XMLoadFloat3(&newLookAt);
	return;
}

void Camera::SetLookAt(DirectX::XMVECTOR newLookAt)
{
	this->m_lookAt = newLookAt;
	return;
}

void Camera::SetCameraUp(DirectX::XMFLOAT3 newCamUp)
{
	this->m_cameraUp = DirectX::XMLoadFloat3(&newCamUp);
	return;
}

void Camera::SetCameraUp(DirectX::XMVECTOR newCamUp)
{
	this->m_cameraUp = newCamUp;
	return;
}

void Camera::SetRoll(float roll)
{
	this->m_roll = roll;
	return;
}

void Camera::ApplyRoll(float rollIncrease)
{
	this->m_roll += rollIncrease;
	return;
}

void Camera::SetPitch(float pitch)
{
	this->m_pitch = pitch;
	return;
}

void Camera::ApplyPitch(float pitchIncrease)
{
	this->m_pitch += pitchIncrease;
	return;
}

void Camera::SetYaw(float yaw)
{
	this->m_yaw = yaw;
	return;
}

void Camera::ApplyYaw(float yawIncrease)
{
	this->m_yaw += yawIncrease;
	return;
}
#pragma endregion setters