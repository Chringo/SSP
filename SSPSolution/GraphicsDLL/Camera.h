#ifndef GRAPHICSDLL_CAMERA_H
#define GRAPHICSDLL_CAMERA_H
#include <DirectXMath.h>

class Camera
{
private:
	DirectX::XMMATRIX view;
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAt;
	DirectX::XMVECTOR cameraUp;

	float roll;
	float pitch;
	float yaw;
public:
	Camera();
	virtual ~Camera();

private:
};

#endif