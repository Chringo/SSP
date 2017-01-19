#include "Camera.h"



Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_baseViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixIdentity());
	this->m_cameraPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_lookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->m_cameraUp = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//this->m_rotation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//this->m_rotateAroundPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_screenAspect = 0.0f;
	this->m_fieldOfView = 0.0f;
}


Camera::~Camera()
{
}

int Camera::Initialize(float screenAspect, float fieldOfView, float nearPlane, float farPlane)
{
	int result = 1;

	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_baseViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixIdentity());
	//The three vectors that defines the new coordinate system
	this->m_cameraPos = DirectX::XMFLOAT4(0.0f, 2.0f, 7.0f, 1.0f);
	this->m_lookAt = DirectX::XMFLOAT4(0.0f, 2.0f, 8.0f, 1.0f);
	this->m_cameraUp = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	//this->m_rotation = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//this->m_rotateAroundPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	this->m_screenAspect = 0.0f;
	this->m_fieldOfView = 0.0f;

	this->m_yaw = 0.0f;
	this->m_pitch = 0.0f;
	this->m_focusPoint = nullptr;
	this->m_focusPointOffset = { 0.0 };
	this->m_camRightvector = { 0.0 };
	this->m_camDirvector = { 0.0 };
	//Define the basic view matrix used in rendering the second stage of deferred rendering.
	DirectX::XMVECTOR camPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	DirectX::XMVECTOR camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
	DirectX::XMStoreFloat4x4(&this->m_baseViewMatrix, DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp));

	//Define a transformation matrix based on the three rotations a 3D object is capable of
	//DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->m_rotation));
	//Transform the three components of the view matrix based on the rotations
	//camPos = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraPos), camRotationMatrix);
	//lookAt = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_lookAt), camRotationMatrix);
	//camUp = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->m_cameraUp), camRotationMatrix);
	//Define the view matrix based on the transformed positions
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(camPos, lookAt, camUp));
	
	//Create the projection matrix
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, nearPlane, farPlane));
	return result;
}

int Camera::Update(float dt)
{
	int result = 1;
	this->m_updatePos();
	//DirectX::XMVECTOR finalFocus = DirectX::XMVectorAdd(*m_focusPoint, m_focusPointOffset);
	//DirectX::XMStoreFloat4(&this->m_lookAt, finalFocus);

	//DirectX::XMVECTOR camPosVec = DirectX::XMVectorAdd(finalFocus, DirectX::XMVectorScale(DirectX::XMVectorScale(m_Dir(), -1.0), m_distance));
	//

	//DirectX::XMMATRIX hier = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorAdd(*m_focusPoint, m_focusPointOffset));

	//m_focusVec = DirectX::XMVectorSubtract(camPosVec, finalFocus);

	//DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);

	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&this->m_cameraPos), DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraUp)));
	//DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&this->m_viewMatrix);
	//
	//view = DirectX::XMMatrixMultiply(hier, view);
	//DirectX::XMStoreFloat4x4(&this->m_viewMatrix, view);

	return result;
}

int Camera::UpdateView()
{
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&this->m_cameraPos), DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraUp)));
	return 1;
}

int Camera::UpdateProjection()
{
	int result = 1;
	if (this->m_screenAspect <= 0 || this->m_fieldOfView <= 0)
	{
		result = 0;
	}
	else
	{
		//Update the projection matrix
		DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(this->m_fieldOfView, this->m_screenAspect, 0.1f, 1000.0f));
	}

	return result;
}

int Camera::UpdateProjection(float screenAspect , float fieldOfView , float nearPlane, float farPlane ) {

	//Update the projection matrix
	this->m_screenAspect = screenAspect;
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, nearPlane, farPlane));

	return 1;
}

#pragma region
void Camera::GetViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMLoadFloat4x4(&this->m_viewMatrix);
	return;
}

DirectX::XMFLOAT4X4 * Camera::GetViewMatrix()
{
	return &this->m_viewMatrix;
}

void Camera::GetBaseViewMatrix(DirectX::XMMATRIX & storeIn)
{
	storeIn = DirectX::XMLoadFloat4x4(&this->m_baseViewMatrix);
	return;
}

DirectX::XMFLOAT4X4 * Camera::GetProjectionMatrix()
{
	return &this->m_projectionMatrix;
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

DirectX::XMFLOAT3 Camera::GetCameraPos()
{
	return DirectX::XMFLOAT3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z);
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

DirectX::XMFLOAT3 Camera::GetLookAt()
{
	return DirectX::XMFLOAT3(m_lookAt.x, m_lookAt.y, m_lookAt.z);
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
DirectX::XMVECTOR Camera::GetCameraPivot()
{
	return *this->m_focusPoint;
}
void Camera::GetCameraFrameData(cameraFrameData & storeIn)
{
	storeIn.pView = DirectX::XMLoadFloat4x4(&this->m_viewMatrix);
	storeIn.pProjection = DirectX::XMLoadFloat4x4(&this->m_projectionMatrix);
	storeIn.pPos = DirectX::XMLoadFloat4(&this->m_cameraPos);
	return;
}
cameraFrameData Camera::GetCameraFrameData()
{
	cameraFrameData myData;
	myData.pView = DirectX::XMLoadFloat4x4(&this->m_viewMatrix);
	myData.pProjection = DirectX::XMLoadFloat4x4(&this->m_projectionMatrix);
	myData.pPos = DirectX::XMLoadFloat4(&this->m_cameraPos);
	return  myData;
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

void Camera::SetCameraPivot(DirectX::XMVECTOR *lockTarget, DirectX::XMVECTOR targetOffset, float distance)
{
	this->m_focusPoint = lockTarget;
	this->m_distance = distance;
	this->m_focusPointOffset = targetOffset;
	
	DirectX::XMVECTOR camPosVec = *lockTarget;
	camPosVec = DirectX::XMVectorAdd(camPosVec, m_focusPointOffset);
	camPosVec.m128_f32[2] += distance;

	this->m_camDirvector = m_Dir();
	this->m_camRightvector = m_Right();
	DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);

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

//void Camera::SetRotation(DirectX::XMFLOAT4 newRotation)
//{
//	this->m_rotation = newRotation;
//}

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

void Camera::RotateCameraPivot(float pitch, float yaw)
{

	m_pitch += pitch;
	m_yaw -= yaw;

	//1.48352986 is ~85 degrees in radians
	if (m_pitch > 1.48352986f)
		m_pitch = 1.48352986f;
	else if (m_pitch < -1.48352986f)
		m_pitch = -1.48352986f;
	
	this->m_camRightvector = m_Right();
	this->m_camDirvector = m_Dir();

	return;
}
void Camera::RotateCamera(double x, double y, double z, double angle)
{
	//Define the vectors we will use
	DirectX::XMVECTOR temp, quatView, result;
	temp = quatView = result = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//Precalculate the sin of the angle
	float scalarSin = DirectX::XMScalarSin(float(angle)/ 2.0f);
	//Calculate the quaternion rotation
	temp = DirectX::XMVectorSetX(temp, float(x) * scalarSin);
	temp = DirectX::XMVectorSetY(temp, float(y) * scalarSin);
	temp = DirectX::XMVectorSetZ(temp, float(z) * scalarSin);
	temp = DirectX::XMVectorSetW(temp, DirectX::XMScalarCos(float(angle) / 2.0f));
	//Calculate the old lookat vector direction
	quatView = DirectX::XMVectorSetX(quatView, this->m_lookAt.x - this->m_cameraPos.x);
	quatView = DirectX::XMVectorSetY(quatView, this->m_lookAt.y - this->m_cameraPos.y);
	quatView = DirectX::XMVectorSetZ(quatView, this->m_lookAt.z - this->m_cameraPos.z);
	quatView = DirectX::XMVectorSetW(quatView, 0.0f);
	//Rotate the vector and normalize it
	result = DirectX::XMVector3Normalize(mult(DirectX::XMVector3Normalize(mult(temp, quatView)), conjugate(temp)));
	//Move the lookAt vector back to the camera
	this->m_lookAt.x = DirectX::XMVectorGetX(result) + this->m_cameraPos.x;
	this->m_lookAt.y = DirectX::XMVectorGetY(result) + this->m_cameraPos.y;
	this->m_lookAt.z = DirectX::XMVectorGetZ(result) + this->m_cameraPos.z;
	quatView = DirectX::XMLoadFloat4(&this->m_cameraUp);
	result = DirectX::XMVector3Normalize(mult(DirectX::XMVector3Normalize(mult(temp, quatView)), conjugate(temp)));
	DirectX::XMStoreFloat4(&this->m_cameraUp, result);
	return;
}
void Camera::RotateCamera(DirectX::XMFLOAT4 rotation)
{
	this->RotateCamera(rotation.x, rotation.y, rotation.z, rotation.w);
	return;
}
void Camera::RotateCamera(DirectX::XMVECTOR rotation)
{
	this->RotateCamera(DirectX::XMVectorGetX(rotation), DirectX::XMVectorGetY(rotation), DirectX::XMVectorGetZ(rotation), DirectX::XMVectorGetW(rotation));
	return;
}
void Camera::SetLocalTranslation(float x, float y, float z)
{
	//Define the three vectors that make up the cameras rotated coordinate system
	DirectX::XMVECTOR forwards = DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraPos));
	DirectX::XMVECTOR up = DirectX::XMLoadFloat4(&this->m_cameraUp);
	DirectX::XMVECTOR right = DirectX::XMVector3Cross(forwards, up);
	//The translation in along the 3 local axis
	forwards = DirectX::XMVectorScale(forwards, x);
	up = DirectX::XMVectorScale(up, y);
	right = DirectX::XMVectorScale(right, z);
	//Combine the three translations
	DirectX::XMFLOAT4 translation;
	DirectX::XMStoreFloat4(&translation, DirectX::XMVectorAdd(DirectX::XMVectorAdd(forwards, up), right));
	forwards = DirectX::XMVector3Normalize(forwards);
	//Apply the translation to the camera & lookAt position
	this->m_cameraPos.x = translation.x;
	this->m_cameraPos.y = translation.y;
	this->m_cameraPos.z = translation.z;
	this->m_lookAt.x = translation.x + DirectX::XMVectorGetX(forwards);
	this->m_lookAt.y = translation.y + DirectX::XMVectorGetY(forwards);
	this->m_lookAt.z = translation.z + DirectX::XMVectorGetZ(forwards);
}
void Camera::ApplyLocalTranslation(float x, float y, float z)
{
	//Define the three vectors that make up the cameras rotated coordinate system
	DirectX::XMVECTOR forwards = DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraPos));
	DirectX::XMVECTOR up = DirectX::XMLoadFloat4(&this->m_cameraUp);
	DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forwards);
	//The translation in along the 3 local axis
	forwards = DirectX::XMVectorScale(forwards, z);
	up = DirectX::XMVectorScale(up, y);
	right = DirectX::XMVectorScale(right, x);
	//Combine the three translations
	DirectX::XMFLOAT4 translation;
	DirectX::XMStoreFloat4(&translation, DirectX::XMVectorAdd(DirectX::XMVectorAdd(forwards, up), right));
	//Apply the translation to the camera & lookAt position
	this->m_cameraPos.x += translation.x;
	this->m_cameraPos.y += translation.y;
	this->m_cameraPos.z += translation.z;
	this->m_lookAt.x += translation.x;
	this->m_lookAt.y += translation.y;
	this->m_lookAt.z += translation.z;
}
void Camera::ApplyLocalTranslation(DirectX::XMFLOAT3 translation)
{
	this->ApplyLocalTranslation(translation.x, translation.y, translation.z);
}
DirectX::XMVECTOR Camera::GetRight()
{
	return m_camRightvector;
}
DirectX::XMVECTOR Camera::conjugate(DirectX::XMVECTOR quat)
{
	DirectX::XMVECTOR result = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	result = DirectX::XMVectorSetX(result, DirectX::XMVectorGetX(quat) * -1);
	result = DirectX::XMVectorSetY(result, DirectX::XMVectorGetY(quat) * -1);
	result = DirectX::XMVectorSetZ(result, DirectX::XMVectorGetZ(quat) * -1);
	result = DirectX::XMVectorSetW(result, DirectX::XMVectorGetW(quat));
	return result;
}
DirectX::XMVECTOR Camera::mult(DirectX::XMVECTOR a, DirectX::XMVECTOR b)
{
	DirectX::XMFLOAT4 C, A, B;
	DirectX::XMStoreFloat4(&A, a);
	DirectX::XMStoreFloat4(&B, b);
	//Old
	C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
	C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
	C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
	C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;

	return DirectX::XMLoadFloat4(&C);
}
DirectX::XMVECTOR Camera::m_Dir()
{
	return DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&m_lookAt), DirectX::XMLoadFloat4(&m_cameraPos)));
}
DirectX::XMVECTOR Camera::m_Right()
{
	return DirectX::XMVector4Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat4(&m_cameraUp), this->m_Dir()));
}
void Camera::m_updatePos()
{
	DirectX::XMVECTOR oldTarget = DirectX::XMLoadFloat4(&m_lookAt);

	DirectX::XMVECTOR finalFocus = DirectX::XMVectorAdd((*m_focusPoint), m_focusPointOffset);
	DirectX::XMVECTOR camPosVec = DirectX::XMVectorAdd(finalFocus, DirectX::XMVectorScale(m_camDirvector, -m_distance));
	
	float x = m_distance * cos(m_pitch) * sin(m_yaw);
	float y = m_distance * sin(m_pitch);
	float z = m_distance * cos(m_pitch) * cos(m_yaw);

	camPosVec = DirectX::XMVectorAdd(camPosVec, DirectX::XMVectorSet(-x, -y, -z, 0.0f));

	DirectX::XMStoreFloat4(&this->m_lookAt, finalFocus);
	DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);
}
#pragma endregion setters