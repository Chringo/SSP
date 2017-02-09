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
#pragma warning(push, 1)
#include "bullet3-2.85.1\src\btBulletDynamicsCommon.h"
#pragma warning(pop)

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
	//DirectX::XMVECTOR quat;
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
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
	
	//for impulse
	DirectX::XMVECTOR PC_ForceDir;
	float PC_Power;

	double PC_gravityInfluence;
	int PC_active;
	int PC_entityID;
	float PC_mass;
	bool PC_is_Static;
	bool PC_collides;
	float PC_friction;
	float PC_elasticity;
	BoundingVolumeType PC_BVtype;
	bool PC_steadfast = false;
	bool PC_ApplyImpulse = false;
	bool PC_NotExistInBulletWorld;
	int PC_IndexRigidBody = -1;
	bool PC_Bullet_AffectedByGravity = true;
	bool PC_GotGrabbedByP1 = false;
	bool PC_GotGrabbedByP2 = false;

	AABB PC_AABB;
	OBB PC_OBB;
	Sphere PC_Sphere;
	Plane PC_Plane;

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

	void ApplyForce(DirectX::XMVECTOR dir, float strength)
	{
		if (this->PC_IndexRigidBody != -1)
		{
			//if the component is OBB or AABB
			this->PC_ApplyImpulse = true;
			this->PC_ForceDir = dir;
			this->PC_Power = strength;
			this->PC_Bullet_AffectedByGravity = true;
		}
	}
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

	DirectX::XMFLOAT3 F_pos;

	unsigned int F_entitityID1;
	unsigned int F_entitityID2;
	bool F_first_inside;
	bool F_second_inside;
	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };
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

	std::vector<int> m_physicsHandlerIndex;
	
	btVector3 m_GravityAcc;

	void CreateDummyObjects();
	btVector3 crt_xmvecVec3(DirectX::XMVECTOR &src);
	DirectX::XMVECTOR crt_Vec3XMVEc(btVector3 &src); //this is posisions only, z value is 1

	PhysicsComponent* player1;
	PhysicsComponent* player2;

	DirectX::XMMATRIX BulletInterpreter::RotateBB(PhysicsComponent* src);

	//player specifics
	void ApplyMovementPlayer1(float dt);
	void ApplyMovementPlayer2();

	void ApplyImpulseOnPC(PhysicsComponent* src);
	void UpdatePhysicsComponentTransformWithBullet(PhysicsComponent* src);

	void applyLinearVelocityOnSrc(PhysicsComponent* src);
public:
	std::vector<btRigidBody*> m_rigidBodies;
	
	PHYSICSDLL_API BulletInterpreter();
	PHYSICSDLL_API virtual ~BulletInterpreter();

	PHYSICSDLL_API void Initialize();

	PHYSICSDLL_API void UpdateBulletEngine(const float& dt); //lets time pass in the bullet engine, letting forces move and apply on the rigid bodies
	PHYSICSDLL_API void Update(PhysicsComponent* src, int index, float dt);	//this function call and add forces on player and components during events

	PHYSICSDLL_API void SyncPosWithBullet(PhysicsComponent* src);	//forces updates in bullet world, when the ball is grabed, the ball needs a new position in bullet
	PHYSICSDLL_API void SyncBulletWithGame(PhysicsComponent* src);

	PHYSICSDLL_API void Shutdown();

	PHYSICSDLL_API void CreateRigidBody(PhysicsComponent* fromGame);

	//test functions
	PHYSICSDLL_API void TestBulletPhysics();
	PHYSICSDLL_API void RegisterBox(int index);

	//type of rigidBodies
	PHYSICSDLL_API void CreatePlane(DirectX::XMVECTOR normal, DirectX::XMVECTOR pos); //planes is always a solid body
	PHYSICSDLL_API void CreateSphere(float radius, DirectX::XMVECTOR pos, float mass);
	PHYSICSDLL_API void CreateOBB(PhysicsComponent* src,int index);
	PHYSICSDLL_API void CreateAABB(PhysicsComponent* src, int index);

	PHYSICSDLL_API btRigidBody* GetRigidBody(int index);

	PHYSICSDLL_API void SetPlayer1(PhysicsComponent* p1);
	PHYSICSDLL_API void SetPlayer2(PhysicsComponent* p2);


};
#endif
