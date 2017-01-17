#ifndef SSPAPPLICATION_COMPONENT_COMPONENTSTRUCTS
#define SSPAPPLICATION_COMPONENT_COMPONENTSTRUCTS
#include <DirectXMath.h>
#include <vector>

enum EVENT 
{
	COLLIDED,
	BUTTON_PRESSED,
	BUTTON_TOGGLED
};
//
//struct GraphicsComponent 
//{
//	int active;
//	int modelID;
//	int materialID;
//	DirectX::XMMATRIX worldTransform;
//	bool animated;
//};
//
//struct PhysicsComponent 
//{
//	int active;
//	bool isDynamic;
//	int entityID;
//	DirectX::XMFLOAT3 position;
//	DirectX::XMFLOAT3 velocity;
//	DirectX::XMFLOAT3 rotation;
//	DirectX::XMFLOAT3 rotationVelocity;
//	double gravityInfluence;
//	bool collides;
//	std::vector<EVENT> eventList;
//};

#endif