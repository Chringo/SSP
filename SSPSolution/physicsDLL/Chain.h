#ifndef CHAIN_H
#define CHAIN_H


#include "bullet3-2.85.1\src\LinearMath\btAlignedObjectArray.h"
#include "bullet3-2.85.1\src\LinearMath\btVector3.h"
#include "bullet3-2.85.1\src\btBulletDynamicsCommon.h"

class Chain
{
public:

	Chain();
	void CreateChain();
	void Initialize();
	btBoxShape* CreateBoxShape(const btVector3& halfExtent);
	btRigidBody * CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape, const btVector4&color = btVector4(1, 0, 0, 1));
	virtual void CreateEmptyDynamicsWorld();
	void update();
	~Chain();
private:
	const int TOTAL_CHAIN = 5; // Change number of chain links. 
	btDiscreteDynamicsWorld * m_dynamicsWorld;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btConstraintSolver* m_solver;
	btBroadphaseInterface* m_broadphase;
};

#endif // !CHAIN_H