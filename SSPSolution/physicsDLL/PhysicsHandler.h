#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

#ifdef PHYSICSDLL_EXPORTS
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
	float ext[3];
	DirectX::XMMATRIX ort;
};

struct Ray
{
	DirectX::XMVECTOR Origin;
	DirectX::XMVECTOR RayDir;
};
struct Sphere
{
	float radius;
};

struct Plane
{
	DirectX::XMVECTOR PC_normal;
};
#pragma endregion

__declspec(align(16)) struct PhysicsComponent
{
	DirectX::XMVECTOR PC_pos;
	DirectX::XMVECTOR PC_velocity;
	DirectX::XMVECTOR PC_rotation;
	DirectX::XMVECTOR PC_rotationVelocity;
	double PC_gravityInfluence;
	int PC_active;
	int PC_entityID;
	float PC_mass;
	bool PC_is_Static;
	bool PC_coolides;

	AABB PC_AABB;
	OBB PC_OBB;
	Sphere PC_Sphere;
	Plane PC_Plane;
	//AABB PC_looseBoundingBox
	//BoundingVolume* PC_tightBoundingVolume; 
	//std::vector<int entityID, event EVENT> PC_eventlist;

};
struct Chain
{
	float CH_linkLenght;
	float CH_totalLenght;
	std::vector<PhysicsComponent*> CH_links;
};

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	std::vector<PhysicsComponent*> m_dynamicComponents;
	int m_nrOfStaticObjects;
	
	Chain m_chain;

	DirectX::XMVECTOR m_gravity;

	void IntersectionTesting();

	const float m_offSet = 0.5f;
	bool IntersectAABB();

	bool DoIntersectionTestOBB(PhysicsComponent* objA, PhysicsComponent* objB);
	bool SphereAABBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objAABB);
	bool SphereOBBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objOBB);
	bool SphereSphereIntersectionTest(PhysicsComponent* objSphere1, PhysicsComponent* objSphere2);
	bool SpherePlaneIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objPlane);
	bool AABBPlaneIntersectionTest(PhysicsComponent* objAABB, PhysicsComponent* objPlane);
	bool OBBPlaneIntersectionTest(PhysicsComponent* objOBB, PhysicsComponent* objPlane);

	//Math functions
	float DotProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	DirectX::XMFLOAT3 CrossProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	float CrossProductf(const DirectX::XMVECTOR &v1, const DirectX::XMVECTOR &v2) const;

	DirectX::XMFLOAT3 VectorSubstract(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;

	void UpdateAABB(PhysicsComponent* src);

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

	void RotateBB_X(PhysicsComponent* src);

	void TranslateBB(const DirectX::XMVECTOR &newPos, PhysicsComponent* src);
	void Add_toRotateVec(PhysicsComponent* src);

	void DoChainPhysics(PhysicsComponent* current, PhysicsComponent* next, float dt);
	void AdjustChainLinkPosition();

	void CreatePhysicsComponent(const DirectX::XMVECTOR &pos);
	bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos);

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponents(int index)const;

	void SetBB_Rotation(const DirectX::XMVECTOR &rotVec, PhysicsComponent* toRotate);

	bool checkCollition();

	void GetPhysicsComponentOBB(OBB* src, int index);
	void GetPhysicsComponentAABB(AABB* src, int index);

};

#endif
