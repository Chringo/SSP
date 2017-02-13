#include "BulletInterpreter.h"
#include <fstream>


DirectX::XMMATRIX BulletInterpreter::RotateBB(PhysicsComponent* src)
{

	DirectX::XMMATRIX rotMatrix;
	DirectX::XMMATRIX toReturn;
	DirectX::XMVECTOR rotationScalars = src->PC_rotation;

	float xRad = 0;
	float yRad = 0;
	float zRad = 0;

	//get the rotation in degrees
	xRad = DirectX::XMVectorGetX(rotationScalars);
	yRad = DirectX::XMVectorGetY(rotationScalars);
	zRad = DirectX::XMVectorGetZ(rotationScalars);

	//convert to radian
	//xRad = (DirectX::XMConvertToRadians(xRad));
	//yRad = (DirectX::XMConvertToRadians(yRad));
	//zRad = (DirectX::XMConvertToRadians(zRad));

	////do some mathemagic
	//DirectX::XMMATRIX rotationMatrixX = DirectX::XMMatrixRotationX(xRad);
	//DirectX::XMMATRIX rotationMatrixY = DirectX::XMMatrixRotationY(yRad);
	//DirectX::XMMATRIX rotationMatrixZ = DirectX::XMMatrixRotationZ(zRad);

	//DirectX::XMMATRIX rotate = DirectX::XMMatrixMultiply(rotationMatrixX, rotationMatrixZ);
	//rotate = DirectX::XMMatrixMultiply(rotate, rotationMatrixY);
	
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(xRad, yRad, zRad);

	toReturn = DirectX::XMMatrixMultiply(src->PC_OBB.ort, rotate);

	return toReturn;
}

 void BulletInterpreter::applyLinearVelocityOnSrc(PhysicsComponent * src)
{

}

 btTransform BulletInterpreter::GetLastRotationToBullet(btRigidBody * rb, PhysicsComponent* src)
 {

	 btVector3 extends = btVector3(src->PC_OBB.ext[0], src->PC_OBB.ext[1], src->PC_OBB.ext[2]);
	 DirectX::XMMATRIX orth = DirectX::XMMatrixTranspose(src->PC_OBB.ort);

	 //creating a mothion state
	 btVector3 startTrans = this->crt_xmvecVec3(src->PC_pos);

	 btVector3 r1 = this->crt_xmvecVec3(orth.r[0]);
	 btVector3 r2 = this->crt_xmvecVec3(orth.r[1]);
	 btVector3 r3 = this->crt_xmvecVec3(orth.r[2]);

	 btMatrix3x3 test;
	 test.setValue
	 (
		 r1.getX(), r1.getY(), r1.getZ(),
		 r2.getX(), r2.getY(), r2.getZ(),
		 r3.getX(), r3.getY(), r3.getZ()
	 );

	 btTransform initialTransform = btTransform(test, startTrans);
	 return initialTransform;
 }

 btVector3 BulletInterpreter::GetLastVelocityToBullet(btRigidBody* rb, PhysicsComponent* src)
 {
	 btVector3 newVelocity = rb->getLinearVelocity();
	 newVelocity += this->crt_xmvecVec3(src->PC_velocity);



	 return newVelocity;
 }

BulletInterpreter::BulletInterpreter()
{
	this->m_broadphase = nullptr;
	this->m_collitionConfiguration = nullptr;
	this->m_dispatcher = nullptr;
	this->m_solver = nullptr;
	this->m_dynamicsWorld = nullptr;
}

BulletInterpreter::~BulletInterpreter()
{
}

void BulletInterpreter::Initialize()
{
	/*
	algorithms supported for bullet
	http://bulletphysics.org/mediawiki-1.5.8/index.php/Broadphase

	For more information
	http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Things
	*/

	this->m_broadphase = new btDbvtBroadphase();

	//The collision configuration allows you to fine tune the algorithms used for the full (not broadphase) collision detection.
	this->m_collitionConfiguration = new btDefaultCollisionConfiguration();
	this->m_dispatcher = new btCollisionDispatcher(this->m_collitionConfiguration);

	//physics solver, heaviest bottleneck in bullet physics
	this->m_solver = new btSequentialImpulseConstraintSolver();

	//now, we can finally instantiate the dynamics world
	this->m_dynamicsWorld = new btDiscreteDynamicsWorld
	(
		this->m_dispatcher,
		this->m_broadphase,
		this->m_solver,
		this->m_collitionConfiguration
	);

	this->m_GravityAcc = btVector3(0, -10, 0);
	this->m_dynamicsWorld->setGravity(this->m_GravityAcc);

	//this->timeStep = 0;
	//this->m_dynamicsWorld->getWorldUserInfo()
	//btInternalTickCallback* test = new btInternalTickCallback;
	//this->m_dynamicsWorld->setInternalTickCallback(BulletworldCallback);
}

void BulletInterpreter::UpdateBulletEngine(const float& dt)
{
	//for callback
	//this->timeStep = dt;
	
	//time will act on the objects
	int maxSubSteps = 6;
	btScalar timeStep = dt;

	#ifdef _DEBUG
	maxSubSteps = 14;
	#endif


	btScalar fixedTimeStep = btScalar(1.0)/btScalar(120); 
	float total = maxSubSteps * fixedTimeStep;

	this->m_dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);

}

void BulletInterpreter::SyncGameWithBullet(PhysicsComponent * src)
{
	DirectX::XMVECTOR result;
	if (src->PC_IndexRigidBody != -1)
	{
		btRigidBody* rigidBody = this->m_rigidBodies.at(src->PC_IndexRigidBody);
		btMotionState* ms = rigidBody->getMotionState();

		btVector3 bulletVelocity = rigidBody->getLinearVelocity();
		btVector3 bulletAnglularV = rigidBody->getAngularVelocity();

		btTransform bulletTransform;
		ms->getWorldTransform(bulletTransform);

		btVector3 bulletPos = bulletTransform.getOrigin();
		btMatrix3x3 bulletBasis = bulletTransform.getBasis();

		btQuaternion na = bulletTransform.getRotation();
		DirectX::XMVECTOR test = DirectX::XMVectorSet(na.getX(), na.getY(), na.getZ(), na.getW());

		DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixIdentity();
		rotMatrix = DirectX::XMMatrixRotationQuaternion(test);
		src->PC_OBB.ort = rotMatrix;
		
		//calculate the offset

		src->PC_pos = this->crt_Vec3XMVEc(bulletPos);
		src->PC_rotationVelocity = this->crt_Vec3XMVEc(bulletAnglularV);
		src->PC_velocity = this->crt_Vec3XMVEc(bulletVelocity);
	}
}

void BulletInterpreter::SyncBulletWithGame(PhysicsComponent * src)
{
	if (src->PC_IndexRigidBody != -1)
	{
		btVector3 PC_pos = this->crt_xmvecVec3(src->PC_pos);
		btVector3 PC_rotationVel = this->crt_xmvecVec3(src->PC_rotationVelocity);
		btVector3 PC_velocity = this->crt_xmvecVec3(src->PC_velocity);

		btRigidBody* rigidBody = nullptr;
		rigidBody = this->m_rigidBodies.at(src->PC_IndexRigidBody);

		if (src->PC_mass != 0 && src->PC_active == true)
		{
			rigidBody->activate();
		}

		rigidBody->setLinearVelocity(PC_velocity);
		rigidBody->setAngularVelocity(PC_rotationVel);

		if (src->PC_mass != 0)
		{
			rigidBody->activate();
		}
		rigidBody->setGravity(this->m_GravityAcc * src->PC_gravityInfluence);
		btTransform moveInWorld = rigidBody->getWorldTransform();
		DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationMatrix(src->PC_OBB.ort);

		btVector3 bulletQuat;
		btQuaternion quaturnion;

		quaturnion = btQuaternion(
			DirectX::XMVectorGetX(quat), 
			DirectX::XMVectorGetY(quat), 
			DirectX::XMVectorGetZ(quat),
			DirectX::XMVectorGetW(quat)
		);


		moveInWorld.setRotation(quaturnion);
		moveInWorld.setOrigin(PC_pos);
		
		if (src->PC_active == false)
		{
			const btCollisionObject* playerShape = this->m_rigidBodies.at(0);
			rigidBody->setIgnoreCollisionCheck(playerShape, true);
		}
		else
		{
			const btCollisionObject* playerShape = this->m_rigidBodies.at(0);
			rigidBody->setIgnoreCollisionCheck(playerShape, false);
		}

		btMotionState* ms = nullptr;
		ms = rigidBody->getMotionState();

		ms->setWorldTransform(moveInWorld);
		rigidBody->setMotionState(ms);
	}
}

void BulletInterpreter::Shutdown()
{

	if (this->m_dynamicsWorld != nullptr)
	{
		delete this->m_dynamicsWorld;
		this->m_dynamicsWorld = nullptr;
	}

	if (this->m_solver != nullptr)
	{
		delete this->m_solver;
		this->m_solver = nullptr;
	}

	if (this->m_dispatcher != nullptr)
	{
		delete this->m_dispatcher;
		this->m_dispatcher = nullptr;
	}

	if (this->m_collitionConfiguration != nullptr)
	{
		delete this->m_collitionConfiguration;
		this->m_collitionConfiguration = nullptr;
	}

	if (this->m_broadphase != nullptr)
	{
		delete this->m_broadphase;
		this->m_broadphase = nullptr;
	}

	int size = this->m_rigidBodies.size();
	for (int i = 0; i < size; i++)
	{
		delete this->m_rigidBodies.at(i);
	}
}

void BulletInterpreter::CreatePlane(DirectX::XMVECTOR normal, DirectX::XMVECTOR pos)
{
	btCollisionShape* planeShape = new btStaticPlaneShape(this->crt_xmvecVec3(normal), 1);

	//creating a mothion state
	btDefaultMotionState* planeMotionState = nullptr;
	btVector3 startTrans = this->crt_xmvecVec3(pos);
	startTrans.setY(startTrans.getY() - 1);

	
	btQuaternion startTransQ = btQuaternion(0, 0, 0, 1.0f);
	btTransform initialTransform = btTransform(startTransQ, startTrans);
	
	planeMotionState = new btDefaultMotionState(initialTransform);

	//by giving the body 0 in mass, it will be immovable
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI
	(
		0,					//mass
		planeMotionState, 
		planeShape, 
		crt_xmvecVec3(pos)
	);

	//create the rigid body
	btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
	rigidBody->setFriction(1);

	this->m_rigidBodies.push_back(rigidBody);

	//add it into the world
	this->m_dynamicsWorld->addRigidBody(rigidBody);
}

void BulletInterpreter::CreateSphere(PhysicsComponent* src, int index)
{
	btCollisionShape* sphereShape = new btSphereShape(src->PC_Sphere.radius);

	//creating a mothion state
	btVector3 startTrans = this->crt_xmvecVec3(src->PC_pos);

	btQuaternion startTransQ = btQuaternion(0, 0, 0, 1.0f);
	btTransform initialTransform = btTransform(startTransQ, startTrans);

	btDefaultMotionState* sphereMotionState = nullptr;
	sphereMotionState = new btDefaultMotionState(initialTransform);

	//by giving the body 0 in mass, it will be immovable
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI
	(
		src->PC_mass,					//mass
		sphereMotionState,
		sphereShape,
		startTrans
	);

	//create the rigid body
	btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
	rigidBody->setUserIndex(this->m_rigidBodies.size());
	rigidBody->setUserIndex2(this->m_rigidBodies.size());
	rigidBody->setAngularFactor(btVector3(0, 0, 0));

	this->m_rigidBodies.push_back(rigidBody);

	//add it into the world
	this->m_dynamicsWorld->addRigidBody(rigidBody);
	src->PC_IndexRigidBody = this->m_rigidBodies.size() - 1;

}

void BulletInterpreter::CreateOBB(PhysicsComponent* src, int index)
{
	if (index == 3)
	{
		int i = 0;
	}

	btVector3 extends = btVector3(src->PC_OBB.ext[0], src->PC_OBB.ext[1], src->PC_OBB.ext[2]);
	btCollisionShape* box = new btBoxShape(extends);
	DirectX::XMMATRIX orth = src->PC_OBB.ort;

	//src->PC_OBB.quat = DirectX::XMQuaternionRotationMatrix(src->PC_OBB.ort);

	//creating a mothion state
	btVector3 startTrans = this->crt_xmvecVec3(src->PC_pos);

	btVector3 r1 = this->crt_xmvecVec3(orth.r[0]);
	btVector3 r2 = this->crt_xmvecVec3(orth.r[1]);
	btVector3 r3 = this->crt_xmvecVec3(orth.r[2]);

	btMatrix3x3 test;
	test.setValue
	(
		r1.getX(), r1.getY(), r1.getZ(),
		r2.getX(), r2.getY(), r2.getZ(),
		r3.getX(), r3.getY(), r3.getZ()
	);

	btTransform initialTransform = btTransform(test, startTrans);

	btDefaultMotionState* boxMotionState = nullptr;
	boxMotionState = new btDefaultMotionState(initialTransform);

	btVector3 interia(0, 0, 0);
	if (src->PC_mass != 0)
	{
		
		box->calculateLocalInertia(src->PC_mass, interia);
	}

	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI
	(
		src->PC_mass,  //mass
		boxMotionState,
		box,
		interia		//Interia / masspunkt 
	);

	btRigidBody* rigidBody = new btRigidBody(boxRigidBodyCI);
	rigidBody->setFriction(src->PC_friction);

	if (index == 0 || index == 1)
	{
		rigidBody->setAngularFactor(btVector3(0, 0, 0));
		
	}

	rigidBody->setUserIndex(this->m_rigidBodies.size());
	rigidBody->setUserIndex2(this->m_rigidBodies.size());


	this->m_rigidBodies.push_back(rigidBody);
	this->m_dynamicsWorld->addRigidBody(rigidBody);
	int pos = this->m_rigidBodies.size() - 1;
	src->PC_IndexRigidBody = pos;


	int i = 0;

}

void BulletInterpreter::CreateAABB(PhysicsComponent* src, int index)
{
	//this is always static
	DirectX::XMVECTOR pos = src->PC_pos;
	DirectX::XMVECTOR ext = DirectX::XMVectorSet(src->PC_AABB.ext[0], src->PC_AABB.ext[1], src->PC_AABB.ext[2], 0);

	btVector3 extends = this->crt_xmvecVec3(ext);
	btCollisionShape* box = new btBoxShape(extends);

	//creating a mothion state
	btVector3 startTrans = this->crt_xmvecVec3(pos);

	btQuaternion startTransQ = btQuaternion(0, 0, 0, 1.0f);
	btTransform initialTransform = btTransform(startTransQ, startTrans);

	btDefaultMotionState* boxMotionState = nullptr;
	boxMotionState = new btDefaultMotionState(initialTransform);

	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI
	(
		0,  //mass
		boxMotionState,
		box,
		btVector3(0, 0, 0)
	);

	btRigidBody* rigidBody = new btRigidBody(boxRigidBodyCI);

	rigidBody->setUserIndex(this->m_rigidBodies.size());
	rigidBody->setUserIndex2(this->m_rigidBodies.size());


	this->m_rigidBodies.push_back(rigidBody);
	this->m_dynamicsWorld->addRigidBody(rigidBody);
	
	this->m_physicsHandlerIndex.push_back(index);
	src->PC_IndexRigidBody = this->m_rigidBodies.size() - 1;
}

btRigidBody * BulletInterpreter::GetRigidBody(int index)
{
	return this->m_rigidBodies.at(index);
}

DirectX::XMVECTOR BulletInterpreter::FindNormalFromComponent(int index)
{
	btPersistentManifold* pMan = nullptr;
	btCollisionWorld* colWorld = nullptr;
	DirectX::XMVECTOR toReturn = DirectX::XMVectorSet(0, 0, 0, -1);

	

	colWorld = this->m_dynamicsWorld->getCollisionWorld();
	int nrOfManifolds = this->m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0; i < nrOfManifolds; i++)
	{
		pMan = this->m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obj0 = pMan->getBody0();
		const btCollisionObject* obj1 = pMan->getBody1();
		int nrOfContancts = pMan->getNumContacts();

		btManifoldPoint* manifloldPoint = nullptr;
		for (int j = 0; j < nrOfContancts; j++)
		{
			manifloldPoint = &pMan->getContactPoint(j);
			
			if (obj0->getUserIndex() == index)
			{
				//index == objectA
				if (manifloldPoint->getDistance() < 1.0f)
				{

					const btVector3 obj0Point = manifloldPoint->getPositionWorldOnA();
					const btVector3 obj1Point = manifloldPoint->getPositionWorldOnB();

					const btVector3 normalOnB = manifloldPoint->m_normalWorldOnB;

					btVector3 toConv = normalOnB;

					toConv.normalize();
					toReturn = this->crt_Vec3XMVEc(toConv);
				}
			}
			if (obj1->getUserIndex() == index)
			{
				//index == objectB

				if (manifloldPoint->getDistance() < 1.0f)
				{

					const btVector3 obj0Point = manifloldPoint->getPositionWorldOnA();
					const btVector3 obj1Point = manifloldPoint->getPositionWorldOnB();

					const btVector3 normalOnB = manifloldPoint->m_normalWorldOnB;

					btVector3 toConv = normalOnB * -1;

					toConv.normalize();
					toReturn = this->crt_Vec3XMVEc(toConv);
				}
			}

		}
	}

	return toReturn;
}

void BulletInterpreter::AddNormalFromCollisions(PhysicsComponent* src, int index)
{
	btPersistentManifold* pMan = nullptr;
	btCollisionWorld* colWorld = nullptr;
	DirectX::XMVECTOR toReturn = DirectX::XMVectorSet(0, 0, 0, -1);



	colWorld = this->m_dynamicsWorld->getCollisionWorld();
	int nrOfManifolds = this->m_dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < nrOfManifolds; i++)
	{
		pMan = this->m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* obj0 = pMan->getBody0();
		const btCollisionObject* obj1 = pMan->getBody1();
		int nrOfContancts = pMan->getNumContacts();

		btManifoldPoint* manifloldPoint = nullptr;
		for (int j = 0; j < nrOfContancts; j++)
		{
			manifloldPoint = &pMan->getContactPoint(j);

			if (obj0->getUserIndex() == index)
			{
				//index == objectA
				if (manifloldPoint->getDistance() < 1.0f)
				{

					const btVector3 obj0Point = manifloldPoint->getPositionWorldOnA();
					const btVector3 obj1Point = manifloldPoint->getPositionWorldOnB();

					const btVector3 normalOnB = manifloldPoint->m_normalWorldOnB;

					btVector3 toConv = normalOnB;

					toConv.normalize();
					DirectX::XMFLOAT3 normal;
					DirectX::XMStoreFloat3(&normal,this->crt_Vec3XMVEc(toConv));
					src->m_normals.push_back(normal);
				}
			}
			if (obj1->getUserIndex() == index)
			{
				//index == objectB

				if (manifloldPoint->getDistance() < 1.0f)
				{

					const btVector3 obj0Point = manifloldPoint->getPositionWorldOnA();
					const btVector3 obj1Point = manifloldPoint->getPositionWorldOnB();

					const btVector3 normalOnB = manifloldPoint->m_normalWorldOnB;

					btVector3 toConv = normalOnB * -1;

					toConv.normalize();
					DirectX::XMFLOAT3 normal;
					DirectX::XMStoreFloat3(&normal,this->crt_Vec3XMVEc(toConv));
					src->m_normals.push_back(normal);
				}
			}

		}
	}
}

btDynamicsWorld * BulletInterpreter::GetBulletWorld()
{
	return this->m_dynamicsWorld;
}

void BulletInterpreter::CreateDummyObjects()
{
	//collition shapes
	btStaticPlaneShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btSphereShape* fallShape = new btSphereShape(1);

	//Rigid bodies

	//Motion state
	btDefaultMotionState* groundMotionState = nullptr;;

	btQuaternion temp = btQuaternion(0, 0, 0, 1); //do not know what this does exactlly
	btVector3 pos = btVector3(0, -1, 0);

	//creating a motionstate to ground
	groundMotionState = new btDefaultMotionState(btTransform(temp, pos));

	//call btRigidBodyCOnstructionInfo, this is a constructor
	//confused me more than it should...
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));

	//bind the default rigidBodyInfo to a rigid body
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	this->m_rigidBodies.push_back(groundRigidBody);

	//add it into the world
	this->m_dynamicsWorld->addRigidBody(groundRigidBody);

	//adding falling sphere
	btDefaultMotionState* fallMotionState = nullptr;

	temp = btQuaternion(0, 0, 0, 1); //do not know what this does exactlly
	pos = btVector3(0, 50, 0);

	//creating a motionstate to sphere
	fallMotionState = new btDefaultMotionState(btTransform(temp, pos));

	btScalar mass = 1;
	btVector3 fallInteria(0, 0, 0); //?
	fallShape->calculateLocalInertia(mass, fallInteria);

	//create the rigid body
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, btVector3(0, 0, 0));
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

	this->m_rigidBodies.push_back(fallRigidBody);
}

btVector3 BulletInterpreter::crt_xmvecVec3(DirectX::XMVECTOR & src)
{
	float x, y, z;
	x = DirectX::XMVectorGetX(src);
	y = DirectX::XMVectorGetY(src);
	z = DirectX::XMVectorGetZ(src);


	btVector3 toReturn(x, y, z);
	
	return toReturn;
}

DirectX::XMVECTOR BulletInterpreter::crt_Vec3XMVEc(btVector3 & src)
{
	DirectX::XMVECTOR result;
	result = DirectX::XMVectorSet(src.getX(), src.getY(), src.getZ(),0);

	return result;
}
