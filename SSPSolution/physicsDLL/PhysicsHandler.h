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

enum BoundingVolumeType
{
	BV_AABB,
	BV_OBB,
	BV_Sphere,
	BV_Plane
};

struct AABB
{
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
	DirectX::XMVECTOR PC_normalForce;
	double PC_gravityInfluence;
	int PC_active;
	int PC_entityID;
	float PC_mass;
	bool PC_is_Static;
	bool PC_coolides;
	float PC_friction;
	float PC_elasticity;
	BoundingVolumeType PC_BVtype;

	AABB PC_AABB;
	OBB PC_OBB;
	Sphere PC_Sphere;
	Plane PC_Plane;
};
struct ChainLink
{
	float CL_lenght;
	PhysicsComponent* CL_next; 
	PhysicsComponent* CL_previous;
};

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	std::vector<PhysicsComponent*> m_dynamicComponents;
	std::vector<ChainLink> m_links;
	int m_nrOfStaticObjects;

	DirectX::XMVECTOR m_gravity;


	const float m_offSet = 0.5f;
	bool IntersectAABB();

	//intersection tests
	bool ObbObbIntersectionTest(PhysicsComponent* objA, PhysicsComponent* objB);
	bool SphereAABBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objAABB);
	bool SphereOBBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objOBB);
	bool SphereSphereIntersectionTest(PhysicsComponent* objSphere1, PhysicsComponent* objSphere2);
	bool SpherePlaneIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objPlane, float dt);
	bool AABBPlaneIntersectionTest(PhysicsComponent* objAABB, PhysicsComponent* objPlane);
	bool OBBPlaneIntersectionTest(PhysicsComponent* objOBB, PhysicsComponent* objPlane);
	bool AABBAABBIntersectionTest(PhysicsComponent *obj1, PhysicsComponent *obj2, float dt);

	void CollitionDynamics(PhysicsComponent* obj1, PhysicsComponent* obj2, DirectX::XMVECTOR normal, float dt);

	//Math functions
	float DotProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	DirectX::XMFLOAT3 CrossProduct(const DirectX::XMFLOAT3 &v1, const DirectX::XMFLOAT3 &v2) const;
	float DotProduct(const DirectX::XMVECTOR &v1, const DirectX::XMVECTOR &v2) const;

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
	void Update(float deltaTime);

	DirectX::XMMATRIX RotateBB_X(PhysicsComponent* src, const float &radian);
	DirectX::XMMATRIX RotateBB_Y(PhysicsComponent* src, const float &radian);
	DirectX::XMMATRIX RotateBB_Z(PhysicsComponent* src, const float &radian);


	void TranslateBB(const DirectX::XMVECTOR &newPos, PhysicsComponent* src);
	void Add_toRotateVec(PhysicsComponent* src);

	void DoChainPhysics(ChainLink* link, float dt);
	void AdjustChainLinkPosition(ChainLink* link);

	void ApplyForceToComponent(PhysicsComponent* componentPtr, DirectX::XMVECTOR force, float dt);

	PhysicsComponent* CreatePhysicsComponent(const DirectX::XMVECTOR &pos, const bool &isStatic);

	void CreateChainLink(int index1, int index2, int nrOfLinks, float linkLenght);
	bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos);
	bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos, float &distanceToOBB);

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	int getNrOfComponents()const;
	PhysicsComponent* getDynamicComponentAt(int index)const;

	void SetBB_Rotation(const DirectX::XMVECTOR &rotVec, PhysicsComponent* toRotate);

	bool checkCollition();

#ifdef _DEBUG
	void GetPhysicsComponentOBB(OBB*& src, int index);
	void GetPhysicsComponentAABB(AABB*& src, int index);
	void GetPhysicsComponentPlane(Plane*& src, int index);
#endif
};

#endif
