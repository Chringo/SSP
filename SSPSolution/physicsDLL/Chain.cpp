#include "Chain.h"



Chain::Chain()
{
}

void Chain::CreateChain()
{
	btBoxShape * colShape = CreateBoxShape(btVector3(1,1,0.25));
	
	//btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(1.0f);

	btAlignedObjectArray<btRigidBody*>boxes;
	int lastBoxIndex = TOTAL_CHAIN - 1;

	for (int i = 0; i < TOTAL_CHAIN; ++i)
	{
		startTransform.setOrigin(btVector3(btScalar(0), btScalar(5 + i * 2), btScalar(0)));
		boxes.push_back(CreateRigidBody((i == lastBoxIndex) ? 0 : mass, startTransform, colShape));
	}

	for (int i = 0; i < TOTAL_CHAIN - 1; ++i)
	{
		btRigidBody * b1 = boxes[i];
		btRigidBody * b2 = boxes[i + 1];

		btPoint2PointConstraint* leftSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(-0.5, 1, 0), btVector3(-0.5, 1, 0));
		m_dynamicsWorld->addConstraint(leftSpring);

		btPoint2PointConstraint* rightSpring = new btPoint2PointConstraint(*b1, *b2, btVector3(0.5, 1, 0), btVector3(0.5, -1, 0));
		m_dynamicsWorld->addConstraint(rightSpring);
		
	}
}

void Chain::Initialize()
{
	CreateEmptyDynamicsWorld();
	CreateChain();
}

btBoxShape* Chain::CreateBoxShape(const btVector3 & halfExtent)
{

	btBoxShape * box = new btBoxShape(halfExtent);
	return box;
}

btRigidBody * Chain::CreateRigidBody(float mass, const btTransform & startTransform,
	btCollisionShape * shape, const btVector4 & color)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	bool isDynamic = (mass != 0.0f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
	{
		shape->calculateLocalInertia(mass, localInertia);
	}

#define USE_MOTIONSTATE 1;
#ifndef USE_MOTIONSTATE



	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
#else


	btRigidBody * body = new btRigidBody(mass, 0, shape, localInertia);

	body->setWorldTransform(startTransform);


#endif // ! USE_MOTIONSTATE

	body->setUserIndex(-1);
	m_dynamicsWorld->addRigidBody(body);

	return body;
}

void Chain::CreateEmptyDynamicsWorld()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	m_broadphase = new btDbvtBroadphase();

	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;

	m_solver = sol;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

}

void Chain::Update()
{
	CreateChain();
}

void Chain::SyncChainData(PhysicsComponent* ptr)
{
	PhysicsHandler* bajs;

	
	m_position = startTransform.getOrigin();

	//POSITION
	//Lenght
	//
}


Chain::~Chain()
{
}
