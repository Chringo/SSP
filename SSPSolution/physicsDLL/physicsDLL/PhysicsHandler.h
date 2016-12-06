#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

#include <DirectXMath.h>
#include <vector>

#pragma region

struct AABB
{
	float pos[3];
	float ext[3];
};

struct OBB
{
	DirectX::XMFLOAT3 pos1;
	DirectX::XMVECTOR pos;
	float ext[3];
	//DirectX::XMFLOAT3 orth[3];
	DirectX::XMMATRIX ort;
};

struct Ray
{
	DirectX::XMVECTOR Origin;
	DirectX::XMVECTOR RayDir;
};

#pragma endregion

__declspec(align(16)) struct PhysicsComponent
{
	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_velocity;
	DirectX::XMVECTOR m_rotationVelocity;
	double m_gravityInfluence;
	int m_active;
	int m_entityID;
	float m_mass;
	bool m_is_Static;
	bool m_coolides;

	AABB m_AABB;
	OBB m_OBB;
	//AABB m_looseBoundingBox
	//BoundingVolume* m_tightBoundingVolume; 
	//std::vector<int entityID, event EVENT> m_eventlist;

};

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	std::vector<PhysicsComponent*> m_dynamicComponents;
	int m_nrOfStaticObjects;
	
	DirectX::XMVECTOR m_gravity;

	const float m_offSet = 0.5f;
	bool IntersectAABB();
	bool DoIntersectionTestOBB(PhysicsComponent* objA, PhysicsComponent* objB);

	//Math functions
	float DotProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	DirectX::XMFLOAT3 CrossProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	float CrossProductf(const DirectX::XMVECTOR &v1, const DirectX::XMVECTOR &v2) const;


	DirectX::XMFLOAT3 VectorSubstract(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;

	//void CreateBB();
	void CreateDefaultBB(const DirectX::XMVECTOR &pos, PhysicsComponent* src);

	void CreateDefaultAABB(const DirectX::XMVECTOR &pos, PhysicsComponent* src);
	void CreateDefaultOBB(const DirectX::XMVECTOR &pos, PhysicsComponent* src);
public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize();
	void ShutDown();
	void Update();

	void CreatePhysicsComponent(const DirectX::XMVECTOR &pos);
	bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj);

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponents(int index)const;
	bool checkCollition();

};

#endif
