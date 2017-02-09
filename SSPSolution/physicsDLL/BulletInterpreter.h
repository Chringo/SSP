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

enum PhysicsLinkType
{
	PL_CHAIN,
	PL_BODY
};
struct PhysicsLink
{
	PhysicsLinkType PL_type;
	float PL_lenght;
	PhysicsComponent* PL_next;
	PhysicsComponent* PL_previous;
};
enum BodyPartType
{
	BP_UPPERBODY,
	BP_LOWERBODY,
	BP_LEFT_ARM,
	BP_RIGHT_ARM,
	BP_LEFT_LEG,
	BP_RIGHT_LEG
};
struct BodyPart
{
	BodyPartType BP_type;
	PhysicsComponent* center;
	PhysicsComponent* next;
	PhysicsComponent* previous;
	PhysicsComponent* next2;
	PhysicsComponent* next3;
	PhysicsComponent* next4;
	PhysicsComponent* next5;
};
enum RagdollState
{
	ANIMATED,
	RAGDOLL
};
struct Ragdoll
{
	RagdollState state;

	PhysicsComponent* playerPC;
	PhysicsComponent* ballPC;

	BodyPart upperBody;
	BodyPart lowerBody;
	BodyPart rightArm;
	BodyPart leftArm;
	BodyPart rightLeg;
	BodyPart leftLeg;

	DirectX::XMMATRIX jointMatrixes[21];

	void SetBindPose(DirectX::XMMATRIX *jointMatrixes, DirectX::XMVECTOR pos)
	{
		//center of body
		this->lowerBody.center->PC_OBB.ort = this->jointMatrixes[0];
		this->lowerBody.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[0].r[3], pos);
		//1-4 lowerbody to head
		this->upperBody.next3->PC_OBB.ort = this->jointMatrixes[1];
		this->upperBody.next3->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[1].r[3], pos);

		this->upperBody.center->PC_OBB.ort = this->jointMatrixes[2];
		this->upperBody.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[2].r[3], pos);

		this->upperBody.next4->PC_OBB.ort = this->jointMatrixes[3];
		this->upperBody.next4->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[3].r[3], pos);

		this->upperBody.next5->PC_OBB.ort = this->jointMatrixes[4];
		this->upperBody.next5->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[4].r[3], pos);
		//-----

		//5-8 rightarm
		this->rightArm.center->PC_OBB.ort = this->jointMatrixes[5];
		this->rightArm.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[5].r[3], pos);

		this->rightArm.next->PC_OBB.ort = this->jointMatrixes[6];
		this->rightArm.next->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[6].r[3], pos);

		this->rightArm.next2->PC_OBB.ort = this->jointMatrixes[7];
		this->rightArm.next2->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[7].r[3], pos);

		this->rightArm.next3->PC_OBB.ort = this->jointMatrixes[8];
		this->rightArm.next3->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[8].r[3], pos);


		//----

		//9-12 leftarm
		this->leftArm.center->PC_OBB.ort = this->jointMatrixes[9];
		this->leftArm.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[9].r[3], pos);

		this->leftArm.next->PC_OBB.ort = this->jointMatrixes[10];
		this->leftArm.next->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[10].r[3], pos);

		this->leftArm.next2->PC_OBB.ort = this->jointMatrixes[11];
		this->leftArm.next2->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[11].r[3], pos);

		this->leftArm.next3->PC_OBB.ort = this->jointMatrixes[12];
		this->leftArm.next3->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[12].r[3], pos);

		//----

		//13-16 rightLeg - foot
		this->rightLeg.center->PC_OBB.ort = this->jointMatrixes[13];
		this->rightLeg.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[13].r[3], pos);

		this->rightLeg.next->PC_OBB.ort = this->jointMatrixes[14];
		this->rightLeg.next->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[14].r[3], pos);

		this->rightLeg.next2->PC_OBB.ort = this->jointMatrixes[15];
		this->rightLeg.next2->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[15].r[3], pos);

		this->rightLeg.next3->PC_OBB.ort = this->jointMatrixes[16];
		this->rightLeg.next3->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[16].r[3], pos);
		//-----

		//17-20 leftleg - foot
		this->leftLeg.center->PC_OBB.ort = this->jointMatrixes[17];
		this->leftLeg.center->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[17].r[3], pos);

		this->leftLeg.next->PC_OBB.ort = this->jointMatrixes[18];
		this->leftLeg.next->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[18].r[3], pos);

		this->leftLeg.next2->PC_OBB.ort = this->jointMatrixes[19];
		this->leftLeg.next2->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[19].r[3], pos);

		this->leftLeg.next3->PC_OBB.ort = this->jointMatrixes[20];
		this->leftLeg.next3->PC_pos = DirectX::XMVectorAdd(this->jointMatrixes[20].r[3], pos);
	}
};


//struct ChainLink
//{
//	float CL_lenght;
//	PhysicsComponent* CL_next;
//	PhysicsComponent* CL_previous;
//};

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
