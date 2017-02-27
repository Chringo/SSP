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

int Camera::Initialize(float farPlane, float screenAspect, float fieldOfView, float nearPlane)
{
	int result = 1;

	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_baseViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixIdentity());
	//The three vectors that defines the new coordinate system
	this->m_cameraPos = DirectX::XMFLOAT4(0.0f, 2.0f, 9.0f, 1.0f);
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
	this->m_maxDistance = 1.0f;
	this->m_distance = 1.0f;
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


	m_intersectionOBBs.reserve(150);

	return result;
}

int Camera::Update()
{
	int result = 1;
 	this->m_updatePos();

	//if player is thrown


	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&this->m_cameraPos), DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraUp)));

	return result;
}

GRAPHICSDLL_API int Camera::RagdollCameraUpdate(DirectX::XMVECTOR pos)
{
	DirectX::XMFLOAT4 newLookAt;
	DirectX::XMStoreFloat4(&newLookAt, pos);
	
	
	this->SetLookAt(newLookAt);

	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&this->m_cameraPos), DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraUp)));

	return 1;
}

int Camera::UpdateDeltaTime(float dt)
{
	

	//0.00150899997
	//if (m_deltaTime > 0.01f)
	this->m_deltaTime = dt;
	return 0;
}

int Camera::UpdateView()
{
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&this->m_cameraPos), DirectX::XMLoadFloat4(&this->m_lookAt), DirectX::XMLoadFloat4(&this->m_cameraUp)));
	return 1;
}

int Camera::UpdateProjectionMat()
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

int Camera::UpdateProjection(float farPlane, float screenAspect, float fieldOfView, float nearPlane) {

	//Update the projection matrix
	this->m_screenAspect = screenAspect;
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, nearPlane, farPlane));

	return 1;
}

int Camera::GetViewFrustrum(ViewFrustrum & storeIn)
{
	int result = 0;
	//Constants for descriptive code
	// was on dev
	//enum { PLANE_OUTWARDS = -1, RIGHT = 0, X = 0, LEFT = 1, Y = 1, PLANE_INWARDS = 1, TOP = 2, Z = 2, BOTTOM = 3, W = 3, NEAR = 4, FAR = 5, NUMBER_OF_PLANES = 6 };
	//enum {PLANE_NORMAL_DIRECTION_CHOICE = PLANE_OUTWARDS};
	//---
	enum { PLANE_OUTWARDS = -1, RIGHT = 0, X = 0, LEFT = 1, Y = 1, PLANE_INWARDS = 1, TOP = 2, Z = 2, BOTTOM = 3, W = 3, CNEAR = 4, CFAR = 5, NUMBER_OF_PLANES = 6 };
	enum { PLANE_NORMAL_DIRECTION_CHOICE = PLANE_OUTWARDS };

	DirectX::XMMATRIX clipSpaceMatrix = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&this->m_viewMatrix), DirectX::XMLoadFloat4x4(&this->m_projectionMatrix));
	DirectX::XMFLOAT4X4 M;
	DirectX::XMStoreFloat4x4(&M, clipSpaceMatrix);
	//Extract clipping planes
	//RIGHT
	
	storeIn.myPlanes[LEFT].normal.x = (M._14 + M._11) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[LEFT].normal.y = (M._24 + M._21) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[LEFT].normal.z = (M._34 + M._31) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[LEFT].normal.w = (M._44 + M._41) * PLANE_NORMAL_DIRECTION_CHOICE;
	//LEFT
	storeIn.myPlanes[RIGHT].normal.x = (M._14 - M._11) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[RIGHT].normal.y = (M._24 - M._21) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[RIGHT].normal.z = (M._34 - M._31) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[RIGHT].normal.w = (M._44 - M._41) * PLANE_NORMAL_DIRECTION_CHOICE;
	//BOTTOM
	storeIn.myPlanes[BOTTOM].normal.x = (M._14 + M._12) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[BOTTOM].normal.y = (M._24 + M._22) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[BOTTOM].normal.z = (M._34 + M._32) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[BOTTOM].normal.w = (M._44 + M._42) * PLANE_NORMAL_DIRECTION_CHOICE;
	//TOP
	storeIn.myPlanes[TOP].normal.x = (M._14 - M._12) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[TOP].normal.y = (M._24 - M._22) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[TOP].normal.z = (M._34 - M._32) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[TOP].normal.w = (M._44 - M._42) * PLANE_NORMAL_DIRECTION_CHOICE;
	//NEAR
	storeIn.myPlanes[CNEAR].normal.x = (M._14 + M._13) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CNEAR].normal.y = (M._24 + M._23) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CNEAR].normal.z = (M._34 + M._33) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CNEAR].normal.w = (M._44 + M._43) * PLANE_NORMAL_DIRECTION_CHOICE;
	//FAR
	storeIn.myPlanes[CFAR].normal.x = (M._14 - M._13) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CFAR].normal.y = (M._24 - M._23) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CFAR].normal.z = (M._34 - M._33) * PLANE_NORMAL_DIRECTION_CHOICE;
	storeIn.myPlanes[CFAR].normal.w = (M._44 - M._43) * PLANE_NORMAL_DIRECTION_CHOICE;

	//Normalize the planes
	for (int planeIndex = 0; planeIndex < NUMBER_OF_PLANES; planeIndex++)
	{
		/*float denominator = 1.0f / (DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat4(&storeIn.myPlanes[planeIndex].normal))));
		storeIn.myPlanes[planeIndex].normal.m128_f32[X] *= denominator;
		storeIn.myPlanes[planeIndex].normal.m128_f32[Y] *= denominator;
		storeIn.myPlanes[planeIndex].normal.m128_f32[Z] *= denominator;
		storeIn.myPlanes[planeIndex].normal.m128_f32[W] *= denominator;*/
		DirectX::XMStoreFloat4(&storeIn.myPlanes[planeIndex].normal, DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&storeIn.myPlanes[planeIndex].normal)));
		//storeIn.myPlanes[planeIndex].normal = DirectX::XMVector3Normalize(storeIn.myPlanes[planeIndex].normal);
	}

	return result;
}

int Camera::Reset()
{
	int result = 0;

	m_pitch = DirectX::XMConvertToRadians(-45.0);
	m_yaw = DirectX::XMConvertToRadians(-45.0);

	this->m_camDirvector = m_Dir();
	this->m_camRightvector = m_Right();

	m_updatePos();

	return result;
}

Camera::C_Ray Camera::CastRay() //returns a ray projected from the camera origin in the direction of the camrea
{
	C_Ray ray;
	DirectX::XMStoreFloat3(&ray.dir, this->m_Dir());
	ray.origin = DirectX::XMFLOAT3(this->m_cameraPos.x, this->m_cameraPos.y, this->m_cameraPos.z);

	//ray.RayDir = this->m_Dir();
	//ray.Origin = DirectX::XMLoadFloat4(&this->m_cameraPos);

	return ray;
}

Camera::C_Ray Camera::CastRayFromMaxDistance()
{
	C_Ray ray;
	DirectX::XMStoreFloat3(&ray.dir, this->m_Dir());

	DirectX::XMStoreFloat3(&ray.origin, m_cameraMaxDistancePos);

	//ray.RayDir = this->m_Dir();
	//ray.Origin = DirectX::XMLoadFloat4(&this->m_cameraPos);

	return ray;
}

int Camera::AddToIntersectCheck(DirectX::XMFLOAT4X4 ort, DirectX::XMFLOAT3 ext, DirectX::XMFLOAT3 pos)
{

	C_OBB obb;
	obb.ort = ort;
	obb.ext = ext;
	obb.pos = pos;



	this->m_intersectionOBBs.push_back(obb);
	//this->m_intersectionOBBspos.push_back(pos);

	return 0;
}

int Camera::ClearIntersectList()
{
	this->m_intersectionOBBs.clear();
	return 0;
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
float Camera::GetCameraDistance()
{
	return this->m_distance;
}
float Camera::GetCameraMaxDistance()
{
	return this->m_maxDistance;
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

DirectX::XMVECTOR Camera::GetDirection()
{
	return this->m_camDirvector;
}

DirectX::XMFLOAT3 Camera::GetFocusPoint()
{
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, *this->m_focusPoint);
	return result;
}

DirectX::XMFLOAT3 Camera::GetFocusPointOffset()
{
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, this->m_focusPointOffset);
	return result;
}

void Camera::GetFocusPoint(DirectX::XMFLOAT3 & storeIn)
{
	DirectX::XMStoreFloat3(&storeIn, *this->m_focusPoint);
	return;
}

void Camera::GetFocusPointOffset(DirectX::XMFLOAT3 & storeIn)
{
	DirectX::XMStoreFloat3(&storeIn, this->m_focusPointOffset);
	return;
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
	bool result = false;



	this->m_focusPoint = lockTarget;
	//this->m_distance = distance;
	this->m_maxDistance = distance;
	this->m_focusPointOffset = targetOffset;

	m_pitch = DirectX::XMConvertToRadians(-45.0);
	m_yaw = DirectX::XMConvertToRadians(-45.0);

	this->m_camDirvector = m_Dir();
	this->m_camRightvector = m_Right();
	//DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);
	
	m_updatePos();

	return;
}

void Camera::SetCameraPivotOffset(DirectX::XMVECTOR targetOffset, float distance)
{
	bool result = false;

	//this->m_distance = distance;
	this->m_maxDistance = distance;
	this->m_focusPointOffset = targetOffset;

	this->m_camDirvector = m_Dir();
	this->m_camRightvector = m_Right();
	//DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);

	m_updatePos();
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


	this->m_pitch += pitch * this->m_deltaTime;
	this->m_yaw -= yaw * this->m_deltaTime;

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
	float scalarSin = DirectX::XMScalarSin(float(angle) / 2.0f);
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
	result = DirectX::XMVector3Normalize(Mult(DirectX::XMVector3Normalize(Mult(temp, quatView)), Conjugate(temp)));
	//Move the lookAt vector back to the camera
	this->m_lookAt.x = DirectX::XMVectorGetX(result) + this->m_cameraPos.x;
	this->m_lookAt.y = DirectX::XMVectorGetY(result) + this->m_cameraPos.y;
	this->m_lookAt.z = DirectX::XMVectorGetZ(result) + this->m_cameraPos.z;
	quatView = DirectX::XMLoadFloat4(&this->m_cameraUp);
	result = DirectX::XMVector3Normalize(Mult(DirectX::XMVector3Normalize(Mult(temp, quatView)), Conjugate(temp)));
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

void Camera::SetDistance(float newDistance)
{
	this->m_distance = newDistance;
}

void Camera::DecreaseDistance(float amount)
{
	this->m_distance -= amount;


}

void Camera::IncreaseDistance(float amount)
{
	this->m_distance += amount;


}

DirectX::XMVECTOR Camera::GetRight()
{
	return m_camRightvector;
}
DirectX::XMVECTOR Camera::Conjugate(DirectX::XMVECTOR quat)
{
	DirectX::XMVECTOR result = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	result = DirectX::XMVectorSetX(result, DirectX::XMVectorGetX(quat) * -1);
	result = DirectX::XMVectorSetY(result, DirectX::XMVectorGetY(quat) * -1);
	result = DirectX::XMVectorSetZ(result, DirectX::XMVectorGetZ(quat) * -1);
	result = DirectX::XMVectorSetW(result, DirectX::XMVectorGetW(quat));
	return result;
}
DirectX::XMVECTOR Camera::Mult(DirectX::XMVECTOR a, DirectX::XMVECTOR b)
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
	this->m_calcDistance();

	DirectX::XMVECTOR oldTarget = DirectX::XMLoadFloat4(&m_lookAt);

	DirectX::XMVECTOR finalFocus = DirectX::XMVectorAdd((*m_focusPoint), m_focusPointOffset);
	DirectX::XMVECTOR camPosVec = DirectX::XMVectorAdd(finalFocus, DirectX::XMVectorScale(m_camDirvector, -m_distance));
	this->m_cameraMaxDistancePos = DirectX::XMVectorAdd(finalFocus, DirectX::XMVectorScale(m_camDirvector, -m_maxDistance));

	float x = m_distance * cos(m_pitch) * sin(m_yaw);
	float y = m_distance * sin(m_pitch);
	float z = m_distance * cos(m_pitch) * cos(m_yaw);

	float mx = m_maxDistance * cos(m_pitch) * sin(m_yaw);
	float my = m_maxDistance * sin(m_pitch);
	float mz = m_maxDistance * cos(m_pitch) * cos(m_yaw);

	camPosVec = DirectX::XMVectorAdd(camPosVec, DirectX::XMVectorSet(-x, -y, -z, 0.0f));
	this->m_cameraMaxDistancePos = DirectX::XMVectorAdd(m_cameraMaxDistancePos, DirectX::XMVectorSet(-mx, -my, -mz, 0.0f));


	DirectX::XMStoreFloat4(&this->m_lookAt, finalFocus);
	DirectX::XMStoreFloat4(&this->m_cameraPos, camPosVec);
}
float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

void Camera::m_calcDistance()
{
	const float EPSILON = 1e-5f;
	static float targetDistance = m_maxDistance;
	float intersectDistance = m_maxDistance + 0.3f;
	float hitDistance = m_maxDistance;
	float zoomSpeedFactor = 4.f;
	bool newDistance = false;
	bool raycollision = false;

	for (C_OBB i : m_intersectionOBBs)
	{
		OBB obb;
		obb.ext[0] = i.ext.x;
		obb.ext[1] = i.ext.y;
		obb.ext[2] = i.ext.z;

		obb.ort = DirectX::XMLoadFloat4x4(&i.ort);
		
		if (m_IntersectRayOBB(m_cameraMaxDistancePos, m_Dir(), obb, DirectX::XMLoadFloat3(&i.pos), hitDistance))
		{
			if (hitDistance < intersectDistance && fabs(hitDistance - m_targetDistance) > EPSILON)
			{
				newDistance = true;
				intersectDistance = hitDistance;
			}
		}
	}
	
	if (newDistance)
	{
		if ((intersectDistance < targetDistance) || (intersectDistance > targetDistance))
			targetDistance = intersectDistance;
		if (targetDistance > this->m_maxDistance)
			targetDistance = this->m_maxDistance;
		else if (targetDistance < 0.05f)
			targetDistance = 0.05f;
	}
	else if(targetDistance < m_maxDistance || targetDistance > m_maxDistance)
		targetDistance = m_maxDistance;

	if (!fabs(m_distance - targetDistance) < EPSILON)
	{
		float diffFactor = (abs(m_distance - targetDistance) * zoomSpeedFactor);
		this->m_distance = lerp(m_distance, targetDistance, this->m_deltaTime*diffFactor);
	}
	
}
#pragma endregion setters

CullingResult Camera::ViewFrustrum::TestAgainstAABB(C_AABB box)
{
	CullingResult result = FRUSTRUM_INSIDE;

#pragma region
	enum { RIGHT = 0, X = 0, LEFT = 1, Y = 1, TOP = 2, Z = 2, BOTTOM = 3, W = 3, DISTANCE = W, CNEAR = 4, CFAR = 5, NUMBER_OF_PLANES = 6 };
	for (size_t i = 0; i < NUMBER_OF_PLANES; i++)
	{
		float pos = this->myPlanes[i].normal.w;
		DirectX::XMVECTOR normal = DirectX::XMLoadFloat4(&this->myPlanes[i].normal);
		DirectX::XMVECTOR vmin, vmax;
		if (this->myPlanes[i].normal.x > 0)
		{
			vmin.m128_f32[X] = box.pos.x - box.ext.x;
			vmax.m128_f32[X] = box.pos.x + box.ext.x;
		}
		else
		{
			vmin.m128_f32[X] = box.pos.x + box.ext.x;
			vmax.m128_f32[X] = box.pos.x - box.ext.x;
		}
		if (this->myPlanes[i].normal.y > 0)
		{
			vmin.m128_f32[Y] = box.pos.y - box.ext.y;
			vmax.m128_f32[Y] = box.pos.y + box.ext.y;
		}
		else
		{
			vmin.m128_f32[Y] = box.pos.y + box.ext.y;
			vmax.m128_f32[Y] = box.pos.y - box.ext.y;
		}
		if (this->myPlanes[i].normal.z > 0)
		{
			vmin.m128_f32[Z] = box.pos.z - box.ext.z;
			vmax.m128_f32[Z] = box.pos.z + box.ext.z;
		}
		else
		{
			vmin.m128_f32[Z] = box.pos.z + box.ext.z;
			vmax.m128_f32[Z] = box.pos.z - box.ext.z;
		}

		if(DirectX::XMVector3Dot(DirectX::XMLoadFloat4(&this->myPlanes[i].normal), vmin) + this->myPlanes[i].normal.w > 0)
		{
			result = FRUSTRUM_OUTSIDE;
			break;
		}
		if (DirectX::XMVector3Dot(DirectX::XMLoadFloat4(&this->myPlanes[i].normal), vmax) + this->myPlanes[i].normal.w >= 0)
		{
			result = FRUSTRUM_INTERSECT;
		}
	}
#pragma endregion Uses only 2 corners

#pragma region
	//for (int i = 0; i < 6; i++)
	//{
	//	DirectX::XMVECTOR p = DirectX::XMLoadFloat4(&this->myPlanes[i].normal);
	//	DirectX::XMVECTOR v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(box.ext.x, box.ext.y, box.ext.z, 0.0f));
	//	//+Y
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(box.ext.x, box.ext.y, -box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(-box.ext.x, box.ext.y, -box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(-box.ext.x, box.ext.y, box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	//-Y
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(-box.ext.x, -box.ext.y, box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(box.ext.x, -box.ext.y, -box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(-box.ext.x, -box.ext.y, -box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}
	//	v0 = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&box.pos), DirectX::XMVectorSet(-box.ext.x, -box.ext.y, box.ext.z, 0.0f));
	//	if (DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0)) <= 0.0f)
	//	{
	//		continue;
	//	}

	//	result = FRUSTRUM_OUTSIDE;
	//}
#pragma endregion uses all corners

	return result;
}

CullingResult Camera::ViewFrustrum::TestAgainstBox(C_BOX box)
{
	CullingResult result = FRUSTRUM_INSIDE;

#pragma region
	enum { RIGHT = 0, X = 0, LEFT = 1, Y = 1, TOP = 2, Z = 2, BOTTOM = 3, W = 3, DISTANCE = W, CNEAR = 4, CFAR = 5, NUMBER_OF_PLANES = 6 };
	for (size_t i = 0; i < NUMBER_OF_PLANES; i++)
	{
		float pos = this->myPlanes[i].normal.w;
		DirectX::XMVECTOR normal = DirectX::XMLoadFloat4(&this->myPlanes[i].normal);
		DirectX::XMVECTOR vmin, vmax;
		if (this->myPlanes[i].normal.x > 0)
		{
			vmin.m128_f32[X] = box.min.x;
			vmax.m128_f32[X] = box.max.x;
		}
		else
		{
			vmin.m128_f32[X] = box.max.x;
			vmax.m128_f32[X] = box.min.x;
		}
		if (this->myPlanes[i].normal.y > 0)
		{
			vmin.m128_f32[Y] = box.min.y;
			vmax.m128_f32[Y] = box.max.y;
		}
		else
		{
			vmin.m128_f32[Y] = box.max.y;
			vmax.m128_f32[Y] = box.min.y;
		}
		if (this->myPlanes[i].normal.z > 0)
		{
			vmin.m128_f32[Z] = box.min.z;
			vmax.m128_f32[Z] = box.max.z;
		}
		else
		{
			vmin.m128_f32[Z] = box.max.z;
			vmax.m128_f32[Z] = box.min.z;
		}

		if(DirectX::XMVector3Dot(DirectX::XMLoadFloat4(&this->myPlanes[i].normal), vmin) + this->myPlanes[i].normal.w > 0)
		{
			result = FRUSTRUM_OUTSIDE;
			break;
		}
		if (DirectX::XMVector3Dot(DirectX::XMLoadFloat4(&this->myPlanes[i].normal), vmax) + this->myPlanes[i].normal.w >= 0)
		{
			result = FRUSTRUM_INTERSECT;
		}
	}
#pragma endregion Uses only 2 corners

	return result;
}

CullingResult Camera::ViewFrustrum::TestAgainstSphere(DirectX::XMFLOAT3 pos, float radius)
{
	CullingResult result = CullingResult::FRUSTRUM_INSIDE;
	float distance = 0.0f;
	enum { NUMBER_OF_PLANES = 6 };
	for (int i = 0; i < NUMBER_OF_PLANES; i++)
	{
		//Distance between point and plane
		distance = DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(DirectX::XMLoadFloat4(&this->myPlanes[i].normal), DirectX::XMLoadFloat3(&pos)));
		//Check distance against
		if (distance > -radius)
		{
			return CullingResult::FRUSTRUM_OUTSIDE;
		}
		else if (distance > radius)
		{
			result = CullingResult::FRUSTRUM_INTERSECT;
		}
	}

	return result;
}

CullingResult Camera::ViewFrustrum::TestAgainstOBBConservative(C_OBB box)
{
	CullingResult result = CullingResult::FRUSTRUM_INSIDE;
	enum { RIGHT = 0, X = 0, LEFT = 1, Y = 1, TOP = 2, Z = 2, BOTTOM = 3, W = 3, DISTANCE = W, CNEAR = 4, CFAR = 5, NUMBER_OF_PLANES = 6 };

	C_BOX testWith;
	//Get the new extensions after rotation the AABB
	DirectX::XMVECTOR newExtensions = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat3(&box.ext), DirectX::XMLoadFloat4x4(&box.ort));
	//Calculate the min - max vertex for the box
	testWith.min.x = box.pos.x - newExtensions.m128_f32[X];
	testWith.min.y = box.pos.y - newExtensions.m128_f32[Y];
	testWith.min.z = box.pos.z - newExtensions.m128_f32[Z];
	testWith.max.x = box.pos.x + newExtensions.m128_f32[X];
	testWith.max.y = box.pos.y + newExtensions.m128_f32[Y];
	testWith.max.z = box.pos.z + newExtensions.m128_f32[Z];
	//Use this box to do the box test
	result = this->TestAgainstBox(testWith);
	return result;
}

CullingResult Camera::ViewFrustrum::TestAgainstOBBExact(C_OBB box)
{
	CullingResult result = CullingResult::FRUSTRUM_INSIDE;


	return result;
}


DirectX::XMVECTOR Camera::C_AABB::GetPositiveVertex(const DirectX::XMVECTOR & normal)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&this->pos);
	DirectX::XMVECTOR ext = DirectX::XMLoadFloat3(&this->ext);
	DirectX::XMVECTOR result = DirectX::XMVectorSubtract(pos, ext);
	if (DirectX::XMVectorGetX(normal) >= 0)
		result = DirectX::XMVectorSetX(result, DirectX::XMVectorGetX(DirectX::XMVectorAdd(pos, ext)));
	if (DirectX::XMVectorGetY(normal) >= 0)
		result = DirectX::XMVectorSetY(result, DirectX::XMVectorGetY(DirectX::XMVectorAdd(pos, ext)));
	if (DirectX::XMVectorGetZ(normal) >= 0)
		result = DirectX::XMVectorSetZ(result, DirectX::XMVectorGetZ(DirectX::XMVectorAdd(pos, ext)));

	return DirectX::XMVectorAdd(pos, result);
}

DirectX::XMVECTOR Camera::C_AABB::GetNegativeVertex(const DirectX::XMVECTOR & normal)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&this->pos);
	DirectX::XMVECTOR ext = DirectX::XMLoadFloat3(&this->ext);
	DirectX::XMVECTOR result = DirectX::XMVectorSubtract(pos, ext);
	if (DirectX::XMVectorGetX(normal) >= 0)
		result = DirectX::XMVectorSetX(result, DirectX::XMVectorGetX(DirectX::XMVectorSubtract(pos, ext)));
	if (DirectX::XMVectorGetY(normal) >= 0)
		result = DirectX::XMVectorSetY(result, DirectX::XMVectorGetY(DirectX::XMVectorSubtract(pos, ext)));
	if (DirectX::XMVectorGetZ(normal) >= 0)
		result = DirectX::XMVectorSetZ(result, DirectX::XMVectorGetZ(DirectX::XMVectorSubtract(pos, ext)));

	return DirectX::XMVectorAdd(pos, result);
}

bool Camera::m_IntersectRayOBB(const DirectX::XMVECTOR & rayOrigin, const DirectX::XMVECTOR & rayDir, const OBB & obj, const DirectX::XMVECTOR & obbPos, float & distanceToOBB)
{
	Ray ray;
	ray.Origin = rayOrigin;
	ray.RayDir = rayDir;


	float t1, t2 = 0.0;
	const int NR_OF_NORMALS = 3;

	//Vec rayD = ray.d;
	DirectX::XMVECTOR radD = ray.RayDir;

	DirectX::XMVECTOR sideVector[NR_OF_NORMALS];

	sideVector[0] = obj.ort.r[0];
	sideVector[1] = obj.ort.r[1];
	sideVector[2] = obj.ort.r[2];

	float tMin;
	float tMax;

	tMin = -INFINITY;
	tMax = INFINITY;

	//Vec pointVec = this->Bcenter - ray.o;
	DirectX::XMVECTOR pointVec = DirectX::XMVectorSubtract(obbPos, ray.Origin);

	//rayD.Normalize();
	ray.RayDir = DirectX::XMVector3Normalize(radD);

	float temp;
	float length[NR_OF_NORMALS];

	length[0] = obj.ext[0];
	length[1] = obj.ext[1];
	length[2] = obj.ext[2];

	float e = 0.0f;
	float f = 0.0f;

	distanceToOBB = 0;
	for (int i = 0; i < NR_OF_NORMALS; i++)
	{

		e = DirectX::XMVector4Dot(sideVector[i], pointVec).m128_f32[0];
		f = DirectX::XMVector4Dot(sideVector[i], ray.RayDir).m128_f32[0];

		if (abs(f) > 1e-20f)
		{
			t1 = (e + length[i]) / f;
			t2 = (e - length[i]) / f;

			if (t1 > t2)
			{
				//swap
				temp = t2;
				t2 = t1;
				t1 = temp;
			}
			if (t1 > tMin)
			{
				tMin = t1;
			}
			if (t2 < tMax)
			{
				tMax = t2;
			}
			if (tMin > tMax)
			{
				return false;
			}
			if (tMax < 0)
			{
				return false;
			}
		}
		else if ((-e - length[i]) > 0 || (-e + length[i] < 0))
		{
			return false;
		}
	}

	if (tMin > 0)
	{
		//min intersect
		distanceToOBB = tMin;
	}
	else
	{
		//max intersect
		distanceToOBB = tMax;
	}

	return true;
}