#ifndef CHAIN_H
#define CHAIN_H


#include "bullet3-2.85.1\src\LinearMath\btAlignedObjectArray.h"
#include "bullet3-2.85.1\src\LinearMath\btVector3.h"
#include "bullet3-2.85.1\src\btBulletDynamicsCommon.h"
//#include "PhysicsHandler.h"

class Chain
{
public:

	Chain();
	void CreateChain();
	int GetNrOfChains();
	void Initialize();
	void Update();
	//void SyncGameData(PhysicsComponent* ptr); // Bullet physics sync with game. It's only a game why you have to be mad 
	//void SyncChainData(PhysicsComponent* ptr);

	~Chain();
private:
	virtual void CreateEmptyDynamicsWorld();
	btBoxShape* CreateBoxShape(const btVector3& halfExtent);
	btRigidBody * CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4&color = btVector4(1, 0, 0, 1));
	const int TOTAL_CHAIN = 5; // Change number of chain links. 
	btDiscreteDynamicsWorld * m_dynamicsWorld;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btBroadphaseInterface* m_broadphase;
	btVector3 m_position;
	btTransform startTransform;
	//BulletInterpreter  m_bullet;
	//PhysicsComponent* ptr;

	
};

#endif // !CHAIN_H