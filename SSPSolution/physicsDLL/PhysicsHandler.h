#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_API __declspec(dllexport)
#else
#define PHYSICSDLL_API __declspec(dllimport)
#endif

#include <DirectXMath.h>
#include <vector>
#include "BulletInterpreter.h"

static void BulletworldCallback(btDynamicsWorld* world, btScalar timeStep);

struct ChainLink
{
	float CL_lenght;
	PhysicsComponent* CL_next;
	PhysicsComponent* CL_previous;
};

struct Field
{
	OBB F_BV;

	DirectX::XMFLOAT3 F_pos;

	unsigned int F_entitityID1;
	unsigned int F_entitityID2;
	bool F_first_inside;
	bool F_second_inside;
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

};


class PhysicsHandler
{
private:
	std::vector<PhysicsComponent*> m_physicsComponents;

	std::vector<PhysicsComponent*> m_dynamicComponents;
	std::vector<PhysicsComponent*> m_staticComponents;
	BulletInterpreter m_bullet;

	std::vector<ChainLink> m_links;

	std::vector<Field> m_fields;

	DirectX::XMVECTOR m_gravity;

	int m_nrOfStaticObjects;
	unsigned int	m_startIndex;		// At what index to start to check colision
	unsigned int	m_numberOfDynamics;	// Number of dynamic objects to check since we only want half
	bool			m_isHost;			// isHost is to check if this client should check collision between dynamic entities

	const float m_offSet = 0.5f;
	bool IntersectAABB();

	//
	bool OBBOBBIntersectionTest(OBB* &obb1, DirectX::XMVECTOR obb1Pos, OBB* &obb2, DirectX::XMVECTOR obb2Pos);
	bool OBBAABBIntersectionTest(OBB* &obb, DirectX::XMVECTOR obbPos, AABB* &AABB, DirectX::XMVECTOR aabbPos);
	bool SphereAABBIntersectionTest(Sphere* &sphere, DirectX::XMVECTOR spherePos, AABB* &AABB, DirectX::XMVECTOR aabbPos);
	bool SphereOBBIntersectionTest(Sphere* &sphere, DirectX::XMVECTOR spherePos, OBB* &obb, DirectX::XMVECTOR obbPos, DirectX::XMVECTOR obbRotation);
	bool SphereSphereIntersectionTest(Sphere* &sphere1, DirectX::XMVECTOR sphere1Pos, Sphere* &sphere2, DirectX::XMVECTOR sphere2Pos);
	bool SpherePlaneIntersectionTest(Sphere* &sphere, DirectX::XMVECTOR spherePos, Plane* &plane, DirectX::XMVECTOR planePos);
	bool AABBPlaneIntersectionTest(AABB* &aabb, DirectX::XMVECTOR aabbPos, Plane* &plane, DirectX::XMVECTOR planePos);
	bool OBBPlaneIntersectionTest(OBB* &obb, DirectX::XMVECTOR obbPos, Plane* &plane, DirectX::XMVECTOR planePos);
	bool AABBAABBIntersectionTest(AABB* &aabb1, DirectX::XMVECTOR aabb1Pos, AABB* &aabb2, DirectX::XMVECTOR aabb2Pos);

	//

	//intersection tests
	bool ObbObbIntersectionTest(PhysicsComponent* objA, PhysicsComponent* objB, float dt);
	bool OBBAABBIntersectionTest(PhysicsComponent * objOBB, PhysicsComponent * objAABB, float dt);
	bool SphereAABBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objAABB, bool doPhysics, float dt);
	bool SphereOBBIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objOBB, float dt);
	bool SphereSphereIntersectionTest(PhysicsComponent* objSphere1, PhysicsComponent* objSphere2, float dt);
	bool SpherePlaneIntersectionTest(PhysicsComponent* objSphere, PhysicsComponent* objPlane, float dt);
	bool AABBPlaneIntersectionTest(PhysicsComponent* objAABB, PhysicsComponent* objPlane, float dt);
	bool OBBPlaneIntersectionTest(PhysicsComponent* objOBB, PhysicsComponent* objPlane, float dt);
	bool AABBAABBIntersectionTest(PhysicsComponent *obj1, PhysicsComponent *obj2, float dt);


	//collitionCorrection
	void ObbObbCollitionCorrectionBB(PhysicsComponent* obj1, PhysicsComponent* obj2, float dt);
	void ObbObbCollitionCorrection(PhysicsComponent* obj1, PhysicsComponent* obj2, float dt);
	DirectX::XMVECTOR FindCollitionPoint(PhysicsComponent* obj1, PhysicsComponent* obj2, float dt);

	bool IsPointInBox(DirectX::XMVECTOR point, OBB* &src, DirectX::XMVECTOR BoxPos);

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

	void SetStartIndex(unsigned int newStartIndex);
	void SetNumberOfDynamics(unsigned int newNumberOfDynamics);
	void SetIsHost(bool newIsHost);
public:
	float timeStep;
	PHYSICSDLL_API PhysicsHandler();
	PHYSICSDLL_API ~PhysicsHandler();

	PHYSICSDLL_API bool Initialize();
	PHYSICSDLL_API void ShutDown();
	PHYSICSDLL_API void Update(float deltaTime);

	PHYSICSDLL_API void CheckFieldIntersection();

	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_X(PhysicsComponent* src, const float &radian);
	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_Y(PhysicsComponent* src, const float &radian);
	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_Z(PhysicsComponent* src, const float &radian);


	PHYSICSDLL_API void TranslateBB(const DirectX::XMVECTOR &newPos, PhysicsComponent* src);
	PHYSICSDLL_API void Add_toRotateVec(PhysicsComponent* src);

	PHYSICSDLL_API void DoChainPhysics(ChainLink* link, float dt);
	PHYSICSDLL_API void AdjustChainLinkPosition(ChainLink* link);

	PHYSICSDLL_API void ApplyForceToComponent(PhysicsComponent* componentPtr, DirectX::XMVECTOR force, float dt);

	PHYSICSDLL_API PhysicsComponent* CreatePhysicsComponent(const DirectX::XMVECTOR &pos, const bool &isStatic);

	PHYSICSDLL_API void CreateChainLink(PhysicsComponent* playerComponent, PhysicsComponent* ballComponent, int nrOfLinks, float linkLenght);
	PHYSICSDLL_API void CreateLink(PhysicsComponent* previous, PhysicsComponent* next, float linkLenght);
	PHYSICSDLL_API void ResetChainLink();
	PHYSICSDLL_API bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos);
	PHYSICSDLL_API bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos, float &distanceToOBB);
	PHYSICSDLL_API bool IntersectRaySphere(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const Sphere &obj, const DirectX::XMVECTOR &pos, float &distanceToOBB);

	PHYSICSDLL_API Field* CreateField(DirectX::XMVECTOR &pos, unsigned int entityID1, unsigned int entityID2, OBB* &obb);

	PHYSICSDLL_API void SimpleCollition(float dt);
	PHYSICSDLL_API void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	PHYSICSDLL_API int GetNrOfComponents()const;
	PHYSICSDLL_API PhysicsComponent* GetDynamicComponentAt(int index)const;

	PHYSICSDLL_API bool checkCollition();
	PHYSICSDLL_API void SetBB_Rotation(const DirectX::XMVECTOR &rotVec, PhysicsComponent* toRotate);

	PHYSICSDLL_API BulletInterpreter* GetBulletInterpreterRef();

	PHYSICSDLL_API void SortComponents(); //sorts the array so the dynamic components are first and static are last
	PHYSICSDLL_API PhysicsComponent* GetClosestComponent(PhysicsComponent* component, int minDistance);
	
	PHYSICSDLL_API void TransferBoxesToBullet(PhysicsComponent* src, int index);
	PHYSICSDLL_API void ApplyPlayer1ToBullet(PhysicsComponent* player1);
	PHYSICSDLL_API void ApplyPlayer2ToBullet(PhysicsComponent* player2);
	
	PHYSICSDLL_API btRigidBody* GetRigidBody(int index);

	PHYSICSDLL_API void SyncAllPhyicsComponentsToBullet();
	PHYSICSDLL_API void SyncBulletToPhysicsComponents();

	PHYSICSDLL_API void DoChainPhysics(float dt);
	PHYSICSDLL_API void DoChainAjustPhysics();
	PHYSICSDLL_API void UpdateStaticPlatforms(float dt);

	PHYSICSDLL_API void ClearCollisionNormals();
	PHYSICSDLL_API void ProcessCallback(btScalar timestep);


#ifdef _DEBUG
	PHYSICSDLL_API void GetPhysicsComponentOBB(OBB*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentAABB(AABB*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentPlane(Plane*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentSphere(Sphere*&src, int index);
#endif
};

#endif
