#include "Camera.h"



Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	this->m_cameraPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_lookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_cameraUp = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

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

	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	//The three vectors that defines the new coordinate system
	this->m_cameraPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_lookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	this->m_cameraUp = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	this->m_roll = 0.0f;
	this->m_pitch = 0.0f;
	this->m_yaw = 0.0f;

	//Define the basic view matrix used in rendering the second stage of deferred rendering.
	DirectX::XMVECTOR camPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&this->m_baseViewMatrix, DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp));

	//Define a transformation matrix based on the three rotations a 3D object is capable of
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_pitch, this->m_yaw, this->m_roll);
	//Transform the three components of the view matrix based on the rotations
	camPos = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraPos), camRotationMatrix);
	lookAt = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_lookAt), camRotationMatrix);
	camUp = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraUp), camRotationMatrix);
	//Define the view matrix based on the transformed positions
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp));

	return result;
}

int Camera::Update()
{
	int result = 0;

	//Define a transformation matrix based on the three rotations a 3D object is capable of
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->m_pitch, this->m_yaw, this->m_roll);
	//Transform the three components of the view matrix based on the rotations
	DirectX::XMVECTOR camPos = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraPos), camRotationMatrix);
	DirectX::XMVECTOR lookAt = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_lookAt), camRotationMatrix);
	DirectX::XMVECTOR camUp = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraUp), camRotationMatrix);
	DirectX::XMStoreFloat4(&this->m_cameraPos, camPos);
	DirectX::XMStoreFloat4(&this->m_lookAt, lookAt);
	DirectX::XMStoreFloat4(&this->m_cameraUp, camUp);
	//Define the view matrix based on the transformed positions
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp));
	return result;
}

#pragma region
void Camera::GetViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMLoadFloat4x4(&this->m_viewMatrix);
	return;
}

void Camera::GetBaseViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMLoadFloat4x4(&this->m_baseViewMatrix);
	return;
}

void Camera::GetCameraPos(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMLoadFloat4(&this->m_cameraPos);
	return;
}

void Camera::GetCameraPos(DirectX::XMFLOAT4 & storeIn)
{
	storeIn = DirectX::XMFLOAT4(this->m_cameraPos);
	return;
}

void Camera::GetCameraPos(DirectX::XMFLOAT3 & storeIn)
{
	storeIn = DirectX::XMFLOAT3(this->m_cameraPos.x, this->m_cameraPos.y, this->m_cameraPos.z);
	return;
}

void Camera::GetLookAt(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMLoadFloat4(&this->m_lookAt);
	return;
}

void Camera::GetLookAt(DirectX::XMFLOAT4 & storeIn)
{
	storeIn = DirectX::XMFLOAT4(this->m_lookAt);
	return;
}

void Camera::GetLookAt(DirectX::XMFLOAT3 & storeIn)
{
	storeIn = DirectX::XMFLOAT3(this->m_lookAt.x, this->m_lookAt.y, this->m_lookAt.z);
	return;
}

void Camera::GetCameraUp(DirectX::XMVECTOR & storeIn)
{
	storeIn = DirectX::XMLoadFloat4(&this->m_cameraUp);
	return;
}

void Camera::GetCameraUp(DirectX::XMFLOAT4 & storeIn)
{
	storeIn = DirectX::XMFLOAT4(this->m_cameraUp);
}

void Camera::GetCameraUp(DirectX::XMFLOAT3 & storeIn)
{
	storeIn = DirectX::XMFLOAT3(this->m_cameraUp.x, this->m_cameraUp.y, this->m_cameraUp.z);
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


void Camera::SetCameraPos(DirectX::XMFLOAT4 newCamPos)
{
	this->m_cameraPos = DirectX::XMFLOAT4(newCamPos);
	return;
}

void Camera::SetCameraPos(DirectX::XMVECTOR newCamPos)
{
	DirectX::XMStoreFloat4(&this->m_cameraPos, newCamPos);
	return;
}

void Camera::SetLookAt(DirectX::XMFLOAT4 newLookAt)
{
	this->m_lookAt = DirectX::XMFLOAT4(newLookAt);
	return;
}


void Camera::SetLookAt(DirectX::XMVECTOR newLookAt)
{
	DirectX::XMStoreFloat4(&this->m_lookAt, newLookAt);
	return;
}

void Camera::SetCameraUp(DirectX::XMFLOAT4 newCamUp)
{
	this->m_lookAt = DirectX::XMFLOAT4(newCamUp);
	return;
}


void Camera::SetCameraUp(DirectX::XMVECTOR newCamUp)
{
	DirectX::XMStoreFloat4(&this->m_cameraUp, newCamUp);
	return;
}

void Camera::AddToCameraPos(DirectX::XMFLOAT3 applyValue)
{
	this->m_cameraPos = DirectX::XMFLOAT4(this->m_cameraPos.x + applyValue.x, this->m_cameraPos.y + applyValue.y, this->m_cameraPos.z + applyValue.z, 1.0f);
	return;
}

void Camera::AddToLookAt(DirectX::XMFLOAT3 applyValue)
{
	this->m_lookAt = DirectX::XMFLOAT4(this->m_lookAt.x + applyValue.x, this->m_lookAt.y + applyValue.y, this->m_lookAt.z + applyValue.z, 1.0f);
	return;
}

void Camera::AddToCameraUp(DirectX::XMFLOAT3 applyValue)
{
	this->m_cameraUp = DirectX::XMFLOAT4(this->m_cameraUp.x + applyValue.x, this->m_cameraUp.y + applyValue.y, this->m_cameraUp.z + applyValue.z, 1.0f);
	return;
}

void Camera::MultiplyCameraPos(DirectX::XMFLOAT3 multiplyValue)
{
	this->m_cameraPos = DirectX::XMFLOAT4(this->m_cameraPos.x * multiplyValue.x, this->m_cameraPos.y * multiplyValue.y, this->m_cameraPos.z * multiplyValue.z, 1.0f);
	return;
}

void Camera::MultiplyLookAt(DirectX::XMFLOAT3 multiplyValue)
{
	this->m_lookAt = DirectX::XMFLOAT4(this->m_lookAt.x * multiplyValue.x, this->m_lookAt.y * multiplyValue.y, this->m_lookAt.z * multiplyValue.z, 1.0f);
	return;
}

void Camera::MultiplyCameraUp(DirectX::XMFLOAT3 multiplyValue)
{
	this->m_cameraUp = DirectX::XMFLOAT4(this->m_cameraUp.x * multiplyValue.x, this->m_cameraUp.y * multiplyValue.y, this->m_cameraUp.z * multiplyValue.z, 1.0f);
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