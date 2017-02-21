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
struct CollitionNormal
{
	DirectX::XMFLOAT3 CN_normal;
	int lifeTime;
	CollitionNormal(DirectX::XMFLOAT3 normal)
	{
		this->CN_normal = normal;
		this->lifeTime = 0;
	}
};
struct PhysicsComponent
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
	bool PC_collides;
	float PC_friction;
	float PC_elasticity;
	BoundingVolumeType PC_BVtype;
	bool PC_steadfast = false;
	int PC_IndexRigidBody = -1;

	AABB PC_AABB;
	OBB PC_OBB;
	Sphere PC_Sphere;
	Plane PC_Plane;

	std::vector<DirectX::XMFLOAT3> m_normals;
	std::vector<CollitionNormal> m_collition_Normals;

	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
	void operator delete(void* p) { _aligned_free(p); };

	void AddCollitionNormal(DirectX::XMFLOAT3 normal)
	{
		int nrOfNormals = this->m_collition_Normals.size();
		for (int i = 0; i < nrOfNormals; i++)
		{
			float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(
				DirectX::XMLoadFloat3(&normal), 
				DirectX::XMLoadFloat3(&this->m_collition_Normals.at(i).CN_normal)));
			if (dot != 1)
			{
				this->m_collition_Normals.push_back(CollitionNormal(normal));
			}
			else
			{
				this->m_collition_Normals.at(i).lifeTime += 5;
			}
		}
	}
};

enum PhysicsLinkType
{
	PL_CHAIN,
	PL_BODY,
	PL_BODY_LEFTLEG,
	PL_BODY_RIGHTLEG,
	PL_BODY_LEFTARM,
	PL_BODY_RIGHTARM,
	PL_BODY_CENTERBODY,
};
struct PhysicsLink
{
	PhysicsLinkType PL_type;
	float PL_lenght;
	PhysicsComponent* PL_next;
	PhysicsComponent* PL_previous;
};



//struct ChainLink
//{
//	float CL_lenght;
//	PhysicsComponent* CL_next;
//	PhysicsComponent* CL_previous;
//};

//struct Field
//{
//	OBB F_BV;
//
//	DirectX::XMFLOAT3 F_pos;
//
//	unsigned int F_entitityID1;
//	unsigned int F_entitityID2;
//	bool F_first_inside;
//	bool F_second_inside;
//	void* operator new(size_t i) { return _aligned_malloc(i, 16); };
//	void operator delete(void* p) { _aligned_free(p); };
//};

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
	
	btVector3 m_GravityAcc;

	void CreateDummyObjects();
	

	//apply changes to rigid bodys
	void applyVelocityOnRigidbody(PhysicsComponent* src);
	void applyRotationOnRigidbody(PhysicsComponent* src);
	void applyForcesToRigidbody(PhysicsComponent* src);

	//read the changes from the rigidbody
	DirectX::XMMATRIX GetNextFrameRotationMatrix(btTransform &transform);
	
	void IgnoreCollitionCheckOnPickupP1(PhysicsComponent* src);
	void IgnoreCollitionCheckOnPickupP2(PhysicsComponent* src);

	//force activation
	void forceDynamicObjectsToActive(PhysicsComponent* src);

	//apply stuff to bullet
	btTransform GetLastRotationToBullet(btRigidBody* rb, PhysicsComponent* src);
	btVector3 GetLastVelocityToBullet(btRigidBody* rb, PhysicsComponent* src);

	std::vector<btRigidBody*> m_rigidBodies;
public:
	
	PHYSICSDLL_API BulletInterpreter();
	PHYSICSDLL_API virtual ~BulletInterpreter();

	PHYSICSDLL_API void Initialize();

	PHYSICSDLL_API void UpdateBulletEngine(const float& dt); //lets time pass in the bullet engine, letting forces move and apply on the rigid bodies
	PHYSICSDLL_API void SyncGameWithBullet(PhysicsComponent* src);	//this function call and add forces on player and components during events

	PHYSICSDLL_API void SyncBulletWithGame(PhysicsComponent* src);

	PHYSICSDLL_API void Shutdown();

	PHYSICSDLL_API btRigidBody* GetRigidBody(int index);

	PHYSICSDLL_API DirectX::XMVECTOR FindNormalFromComponent(int index);

	PHYSICSDLL_API void AddNormalFromCollisions(PhysicsComponent* src, int index);
	PHYSICSDLL_API btDynamicsWorld* GetBulletWorld();

	PHYSICSDLL_API void SetIgnoreCollisions(PhysicsComponent* src1, PhysicsComponent* src2);
	PHYSICSDLL_API void SetCollisionShapeLocalScaling(PhysicsComponent* src, btVector3 scale);

	//type of rigidBodies
	PHYSICSDLL_API void CreatePlane(DirectX::XMVECTOR normal, DirectX::XMVECTOR pos); //planes is always a solid body
	PHYSICSDLL_API void CreateSphere(PhysicsComponent* src, int index);
	PHYSICSDLL_API void CreateOBB(PhysicsComponent* src, int index);
	PHYSICSDLL_API void CreateAABB(PhysicsComponent* src, int index);
	PHYSICSDLL_API void CreatePlayer(PhysicsComponent* src, int index);

	btVector3 crt_xmvecVec3(DirectX::XMVECTOR &src);
	DirectX::XMVECTOR crt_Vec3XMVEc(btVector3 &src); //this is posisions only, z value is 1
};
#endif
