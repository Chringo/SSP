#ifndef PHYSICSDLL_PHYSICS_PHYSICSINTERPRETER_H
#define PHYSICSDLL_PHYSICS_PHYSICSINTERPRETER_H

//dll export declarations
#pragma region
#ifdef PHYSICSDLL_EXPORTS
#define PHYSICSDLL_API __declspec(dllexport)
#else
#define PHYSICSDLL_API __declspec(dllimport)
#endif

#include <DirectXMath.h>
#include <vector>
#include "bullet3-2.85.1\src\btBulletDynamicsCommon.h"

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

struct PhysicsComponent
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
	bool PC_collides;
	float PC_friction;
	float PC_elasticity;
	BoundingVolumeType PC_BVtype;
	bool PC_steadfast;
	bool PC_NotExistInBulletWorld;

	AABB PC_AABB;
	OBB PC_OBB;
	Sphere PC_Sphere;
	Plane PC_Plane;

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
};
struct ChainLink
{
	float CL_lenght;
	PhysicsComponent* CL_next;
	PhysicsComponent* CL_previous;
};

struct Field
{
	OBB F_BV;

	unsigned int F_entitityID1;
	unsigned int F_entitityID2;
	bool F_first_inide;
	bool F_second_inside;

};

#pragma endregion



/*
This class is responsible for transfering all the nessesary information
physics data from "Earl Graves prison of puzzles" do physics calculations
using bullet physics engine, and the transfering the data back to
the game.

*/

class BulletInterpreter
{
private:
	//Bullet physics components
	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collitionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	std::vector<btRigidBody*> m_rigidBodies;
	std::vector<int> m_physicsHandlerIndex;
	
	void CreateDummyObjects();
	btVector3 crt_xmvecVec3(DirectX::XMVECTOR &src);
	DirectX::XMVECTOR crt_Vec3XMVEc(btVector3 &src); //this is posisions only, z value is 1

public:
	PHYSICSDLL_API BulletInterpreter();
	PHYSICSDLL_API virtual ~BulletInterpreter();

	PHYSICSDLL_API void Initialize();
	PHYSICSDLL_API void Update(const float& dt);
	PHYSICSDLL_API void GetNextPos(PhysicsComponent* src, int index);
	PHYSICSDLL_API void Shutdown();

	PHYSICSDLL_API void CreateRigidBody(PhysicsComponent* fromGame);

	//test functions
	PHYSICSDLL_API void TestBulletPhysics();
	PHYSICSDLL_API void RegisterBox(int index);

	//type of rigidBodies
	void CreatePlane(DirectX::XMVECTOR normal, DirectX::XMVECTOR pos); //planes is always a solid body
	void CreateSphere(float radius, DirectX::XMVECTOR pos, float mass);
	void CreateBox(DirectX::XMVECTOR ext,const DirectX::XMMATRIX &ortho, DirectX::XMVECTOR pos);
	void CreateBox(DirectX::XMVECTOR ext, DirectX::XMVECTOR pos);

};
#endif
