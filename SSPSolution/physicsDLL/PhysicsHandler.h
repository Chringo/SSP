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
//#include "..\ResourceLib\Skeleton.h"
#include "../ResourceLib/Skeleton.h"
#include "../GraphicsDLL/GraphicsComponent.h"
#include "../GraphicsDLL/AnimationHandler.h"

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
	RAGDOLL_TRANSITION,
	RAGDOLL,
	ANIMATED_TRANSITION,
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

	Resources::Skeleton::Joint *Skeleton;

	DirectX::XMMATRIX jointMatrixes[21];

	void SetBindPose(DirectX::XMMATRIX *jointMatrixes, DirectX::XMVECTOR pos)
	{
		float scale = 1.0f;

		//center of body
		this->lowerBody.center->PC_OBB.ort = this->jointMatrixes[0];
		this->lowerBody.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[0].r[3], scale), pos);
		this->lowerBody.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
		//1-4 lowerbody to head
		this->upperBody.next3->PC_OBB.ort = this->jointMatrixes[1];
		this->upperBody.next3->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[1].r[3], scale), pos);
		this->upperBody.next3->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->upperBody.center->PC_OBB.ort = this->jointMatrixes[2];
		this->upperBody.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[2].r[3], scale), pos);
		this->upperBody.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->upperBody.next4->PC_OBB.ort = this->jointMatrixes[3];
		this->upperBody.next4->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[3].r[3], scale), pos);
		this->upperBody.next4->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->upperBody.next5->PC_OBB.ort = this->jointMatrixes[4];
		this->upperBody.next5->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[4].r[3], scale), pos);
		this->upperBody.next5->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
		//-----

		//5-8 rightarm
		this->rightArm.center->PC_OBB.ort = this->jointMatrixes[5];
		this->rightArm.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[5].r[3], scale), pos);
		this->rightArm.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightArm.next->PC_OBB.ort = this->jointMatrixes[6];
		this->rightArm.next->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[6].r[3], scale), pos);
		this->rightArm.next->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightArm.next2->PC_OBB.ort = this->jointMatrixes[7];
		this->rightArm.next2->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[7].r[3], scale), pos);
		this->rightArm.next2->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightArm.next3->PC_OBB.ort = this->jointMatrixes[8];
		this->rightArm.next3->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[8].r[3], scale), pos);
		this->rightArm.next3->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);


		//----

		//9-12 leftarm
		this->leftArm.center->PC_OBB.ort = this->jointMatrixes[9];
		this->leftArm.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[9].r[3], scale), pos);
		this->leftArm.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftArm.next->PC_OBB.ort = this->jointMatrixes[10];
		this->leftArm.next->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[10].r[3], scale), pos);
		this->leftArm.next->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftArm.next2->PC_OBB.ort = this->jointMatrixes[11];
		this->leftArm.next2->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[11].r[3], scale), pos);
		this->leftArm.next2->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftArm.next3->PC_OBB.ort = this->jointMatrixes[12];
		this->leftArm.next3->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[12].r[3], scale), pos);
		this->leftArm.next3->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		//----

		//13-16 rightLeg - foot
		this->rightLeg.center->PC_OBB.ort = this->jointMatrixes[13];
		this->rightLeg.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[13].r[3], scale), pos);
		this->rightLeg.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightLeg.next->PC_OBB.ort = this->jointMatrixes[14];
		this->rightLeg.next->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[14].r[3], scale), pos);
		this->rightLeg.next->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightLeg.next2->PC_OBB.ort = this->jointMatrixes[15];
		this->rightLeg.next2->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[15].r[3], scale), pos);
		this->rightLeg.next2->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->rightLeg.next3->PC_OBB.ort = this->jointMatrixes[16];
		this->rightLeg.next3->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[16].r[3], scale), pos);
		this->rightLeg.next3->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
		//-----

		//17-20 leftleg - foot
		this->leftLeg.center->PC_OBB.ort = this->jointMatrixes[17];
		this->leftLeg.center->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[17].r[3], scale), pos);
		this->leftLeg.center->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftLeg.next->PC_OBB.ort = this->jointMatrixes[18];
		this->leftLeg.next->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[18].r[3], scale), pos);
		this->leftLeg.next->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftLeg.next2->PC_OBB.ort = this->jointMatrixes[19];
		this->leftLeg.next2->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[19].r[3], scale), pos);
		this->leftLeg.next2->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		this->leftLeg.next3->PC_OBB.ort = this->jointMatrixes[20];
		this->leftLeg.next3->PC_pos = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->jointMatrixes[20].r[3], scale), pos);
		this->leftLeg.next3->PC_OBB.ort.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);
	}
};
class PhysicsHandler
{
private:
	std::vector<PhysicsComponent*> m_physicsComponents;

	std::vector<PhysicsComponent*> m_dynamicComponents;
	std::vector<PhysicsComponent*> m_staticComponents;
	BulletInterpreter m_bullet;

	std::vector<PhysicsLink> m_links;

	std::vector<PhysicsComponent*> m_bodyPC;

	Ragdoll m_playerRagDoll;

	std::vector<Field> m_fields;


	DirectX::XMMATRIX TESTjointMatrixes[21];

	DirectX::XMVECTOR m_gravity;
	int m_ragdollNotMovingCounter;
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
	PHYSICSDLL_API PhysicsHandler();
	PHYSICSDLL_API ~PhysicsHandler();

	PHYSICSDLL_API bool Initialize();
	PHYSICSDLL_API void ShutDown();
	PHYSICSDLL_API void Update(float deltaTime);

	PHYSICSDLL_API void DoRagdollIntersection(float dt);

	PHYSICSDLL_API void CheckFieldIntersection();

	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_X(PhysicsComponent* src, const float &radian);
	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_Y(PhysicsComponent* src, const float &radian);
	PHYSICSDLL_API DirectX::XMMATRIX RotateBB_Z(PhysicsComponent* src, const float &radian);


	PHYSICSDLL_API void TranslateBB(const DirectX::XMVECTOR &newPos, PhysicsComponent* src);
	PHYSICSDLL_API void Add_toRotateVec(PhysicsComponent* src);

	PHYSICSDLL_API void DoChainPhysics(PhysicsLink* link, float dt);
	PHYSICSDLL_API void AdjustChainLinkPosition(PhysicsLink* link);

	PHYSICSDLL_API void ApplyForceToComponent(PhysicsComponent* componentPtr, DirectX::XMVECTOR force, float dt);

	PHYSICSDLL_API PhysicsComponent* CreatePhysicsComponent(const DirectX::XMVECTOR &pos, const bool &isStatic);

	PHYSICSDLL_API PhysicsComponent* CreateBodyPartPhysicsComponent(const DirectX::XMVECTOR &pos, const bool &isStatic);

	PHYSICSDLL_API void CreateChainLink(PhysicsComponent* playerComponent, PhysicsComponent* ballComponent, int nrOfLinks, float linkLenght);
	PHYSICSDLL_API void ResetChainLink();

	PHYSICSDLL_API void ResetRagdollToTPose(DirectX::XMVECTOR pos);

	PHYSICSDLL_API void CreateRagdollBody(DirectX::XMVECTOR pos, PhysicsComponent* playerPC);
	PHYSICSDLL_API void CreateRagdollBodyWithChainAndBall(Resources::Skeleton::Joint *Skeleton, DirectX::XMVECTOR pos, PhysicsComponent* playerPC, PhysicsComponent* ball);

	PHYSICSDLL_API void AdjustRagdoll(Ragdoll* ragdoll, float dt);
	PHYSICSDLL_API DirectX::XMVECTOR AdjustBodyPartDistance(PhysicsComponent* previous, PhysicsComponent* next, float lenght);
	PHYSICSDLL_API void AdjustBodyParts(BodyPart* bodypart, float dt);

	PHYSICSDLL_API bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos);
	PHYSICSDLL_API bool IntersectRayOBB(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const OBB &obj, const DirectX::XMVECTOR &obbPos, float &distanceToOBB);
	PHYSICSDLL_API bool IntersectRaySphere(const DirectX::XMVECTOR &rayOrigin, const DirectX::XMVECTOR &rayDir, const Sphere &obj, const DirectX::XMVECTOR &pos, float &distanceToOBB);

	PHYSICSDLL_API Field* CreateField(DirectX::XMVECTOR &pos, unsigned int entityID1, unsigned int entityID2, OBB* &obb);

	PHYSICSDLL_API void SimpleCollition(float dt);
	PHYSICSDLL_API void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	PHYSICSDLL_API int GetNrOfComponents()const;
	PHYSICSDLL_API PhysicsComponent* GetComponentAt(int index)const;

	PHYSICSDLL_API int GetNrOfDynamicComponents()const;
	PHYSICSDLL_API PhysicsComponent* GetDynamicComponentAt(int index)const;

	PHYSICSDLL_API int GetNrOfStaticComponents()const;
	PHYSICSDLL_API PhysicsComponent* GetStaticComponentAt(int index)const;

	PHYSICSDLL_API int GetNrOfBodyComponents()const;
	PHYSICSDLL_API PhysicsComponent* GetBodyComponentAt(int index)const;

	PHYSICSDLL_API Ragdoll* GetPlayerRagdoll();

	PHYSICSDLL_API int GetNrOfMagnets()const;
	//PHYSICSDLL_API Magnet* GetMagnetAt(int index);

	PHYSICSDLL_API int GetNrOfFields()const;
	PHYSICSDLL_API Field* GetFieldAt(int index);

	PHYSICSDLL_API void SetBB_Rotation(const DirectX::XMVECTOR &rotVec, PhysicsComponent* toRotate);

	PHYSICSDLL_API bool checkCollition();

	PHYSICSDLL_API void SortComponents(); //sorts the array so the dynamic components are first and static are last
	PHYSICSDLL_API PhysicsComponent* GetClosestComponent(PhysicsComponent* component, int minDistance);
	
	PHYSICSDLL_API void TransferBoxesToBullet(PhysicsComponent* src, int index);
	PHYSICSDLL_API void ApplyPlayer1ToBullet(PhysicsComponent* player1);
	PHYSICSDLL_API void ApplyPlayer2ToBullet(PhysicsComponent* player2);
	
	PHYSICSDLL_API btRigidBody* GetRigidBody(int index);
	PHYSICSDLL_API void SetRagdollToBindPose(Ragdoll* ragdoll, DirectX::XMVECTOR pos);
	PHYSICSDLL_API void SyncRagdollWithSkelton(Ragdoll* ragdoll);

	PHYSICSDLL_API DirectX::XMMATRIX CalcTransformMatrix(PhysicsComponent* joint1, PhysicsComponent* joint2, PhysicsComponent* joint3);

	PHYSICSDLL_API DirectX::XMMATRIX CalcNewRotationAxises(PhysicsComponent* joint1, PhysicsComponent* joint2);

#ifdef _DEBUG
	PHYSICSDLL_API void GetPhysicsComponentOBB(OBB*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentAABB(AABB*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentPlane(Plane*& src, int index);
	PHYSICSDLL_API void GetPhysicsComponentSphere(Sphere*&src, int index);
#endif
};

#endif
