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

	toReturn = DirectX::XMMatrixMultiply(rotate, src->PC_OBB.ort);

	return toReturn;
}

void BulletInterpreter::ApplyMovementPlayer1(float dt)
{
	if (this->player1 != nullptr)
	{
		btRigidBody* rb = this->m_rigidBodies.at(this->player1->PC_IndexRigidBody);

		if (rb != nullptr)
		{
			btVector3 OldVelocity = rb->getLinearVelocity();

			btVector3 newVel = this->crt_xmvecVec3(this->player1->PC_velocity);

			int active = rb->getActivationState();


			//newVel /= 100;
			if (active != ACTIVE_TAG)
			{
				//sleep is for the weak
				rb->activate();
				//this->m_rigidBodies.at(this->player1->inde)
			}

			if (newVel.isZero() == false)
			{
				newVel = OldVelocity + newVel;
				rb->setLinearVelocity(newVel);
			}
			this->player1->PC_velocity = DirectX::XMVectorSet(0, 0, 0, 0);
		}
	}
}

void BulletInterpreter::ApplyMovementPlayer2()
{
}

void BulletInterpreter::ApplyImpulseOnPC(PhysicsComponent * src)
{
	this->SyncPosWithBullet(src);


	btVector3 force;
	btVector3 posAffectedByForce;


	force = this->crt_xmvecVec3(src->PC_ForceDir);
	force = force.normalize();
	//force = btVector3(0, 1, 1);

	force *= src->PC_Power;
	posAffectedByForce = btVector3(0, 0.2, 0);

	btRigidBody* holder = nullptr;
	holder = this->m_rigidBodies.at(src->PC_IndexRigidBody);

	holder->setActivationState(ACTIVE_TAG);
	
	//holder->clearForces();
	holder->applyImpulse(force, posAffectedByForce);

	src->PC_ForceDir = DirectX::XMVectorSet(0, 0, 0, 0);
	src->PC_Power = 0;
	src->PC_ApplyImpulse = false;
	src->PC_Bullet_AffectedByGravity = true;
}

void BulletInterpreter::UpdatePhysicsComponentTransformWithBullet(PhysicsComponent * src)
{
	btTransform trans;


	//get the position in the Bullet world
	this->m_rigidBodies.at(src->PC_IndexRigidBody)->getMotionState()->getWorldTransform(trans);
	btVector3 origin = trans.getOrigin();

	//update PhysicsComponent
	src->PC_pos = this->crt_Vec3XMVEc(origin);

	if (src->PC_steadfast == true)
	{
		btRigidBody* rigid = this->m_rigidBodies.at(src->PC_IndexRigidBody);

		btVector3 vel = this->crt_xmvecVec3(src->PC_velocity);
		rigid->setLinearVelocity(vel);
		rigid->setGravity(btVector3(0,0,0));

		trans = rigid->getWorldTransform();
		trans.setOrigin(this->crt_xmvecVec3(src->PC_pos));

		btMotionState* ms = nullptr;
		ms = rigid->getMotionState();

		ms->setWorldTransform(trans);

		rigid->setMotionState(ms);
	}
	else if (src->PC_BVtype == BV_OBB)
	{
		//convert the position to xmvector 
		btQuaternion rotation = trans.getRotation();
		DirectX::XMVECTOR rot = DirectX::XMVectorSet(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW());

		src->PC_OBB.ort = DirectX::XMMatrixRotationQuaternion(rot);
		//src->PC_OBB.quat = rot;

		//if (src->PC_Power == 1338)
		//{
		//	
		//	btMatrix3x3 test = trans.getBasis();
		//	int i = 0;

		//	btQuaternion tesQ = btQuaternion();
		//	
		//}
		//float x = rotation.getX();
		//float y = rotation.getY();
		//float z = rotation.getZ();
		//float w = rotation.getW();

		//
		//float roll = atan2f(2 * y*w - 2 * x*z, 1 - 2 * y * y - 2 * z*z);
		//float pitch = atan2f(2 * x*w - 2 * y*z, 1 - 2 * x*x - 2 * z*z);
		//float yaw = asinf(2 * x*y + 2 * z*w);

		////float roll = atan2f(2.0 *(x*y + z*w), 1.0 - 2.0 * (y * y  * z*z));
		////float pitch = asinf(2.0 * (x*z - w*y));
		////float yaw = atan2f(2.0 * (x*w + y*z), 1.0 - 2.0 * (y*y + z*z));

		//src->PC_rotation = DirectX::XMVectorSet(roll, pitch, yaw , 0);

		//DirectX::XMMATRIX test2 = DirectX::XMMatrixRotationRollPitchYawFromVector(src->PC_rotation);


		//int a = 0;
		////src->PC_rotation = 

		//btVector3 rotVel = this->crt_xmvecVec3(src->PC_rotationVelocity);
		//if (rotVel.isZero() != true)
		//{
		//	this->m_rigidBodies.at(src->PC_IndexRigidBody)->setAngularVelocity(rotVel);
		//}
	}

}

 void BulletInterpreter::applyLinearVelocityOnSrc(PhysicsComponent * src)
{

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
	this->player1 = nullptr;
	this->player2 = nullptr;

	this->m_GravityAcc = btVector3(0, -10, 0);
	this->m_dynamicsWorld->setGravity(this->m_GravityAcc);
}

void BulletInterpreter::UpdateBulletEngine(const float& dt)
{
	//time will act on the objects
	btScalar timeStep = dt;
	int maxSubSteps = 3;
	btScalar fixedTimeStep = btScalar(1.0)/btScalar(60); 

	this->m_dynamicsWorld->stepSimulation(timeStep, maxSubSteps, fixedTimeStep);
	//this->m_dynamicsWorld->stepSimulation(,);

	//update positions
	this->ApplyMovementPlayer1(dt);
	//this->ApplyMovementPlayer2();
	
}

void BulletInterpreter::Update(PhysicsComponent * src, int index, float dt)
{
	DirectX::XMVECTOR result;

	if(src->PC_IndexRigidBody != -1 && src->PC_BVtype != BV_Sphere)
	{
		if (src->PC_ApplyImpulse == true)
		{
			//not here
			this->ApplyImpulseOnPC(src);
		}

		if (src->PC_IndexRigidBody != -1)
		{
			this->UpdatePhysicsComponentTransformWithBullet(src);
		}

		if (src->PC_Bullet_AffectedByGravity == false)
		{
			this->SyncPosWithBullet(src);
			//if the gravity influence is zero, the component will not be affected by gravity
			//this->m_rigidBodies.at(index)->clearForces();
			this->m_rigidBodies.at(src->PC_IndexRigidBody)->setAngularVelocity(btVector3(0, 0, 0));
			this->m_rigidBodies.at(src->PC_IndexRigidBody)->setGravity(btVector3(0, 0, 0));
			this->m_rigidBodies.at(src->PC_IndexRigidBody)->setCollisionFlags(1);

			//PhysicsComponent* playor = this->player1;


			src->PC_active = false;
		}
		else
		{
			if (src->PC_IndexRigidBody == 2)
			{
				btRigidBody* debug = this->m_rigidBodies.at(src->PC_IndexRigidBody);
			}
			src->PC_active = true;
			btRigidBody* debug = this->m_rigidBodies.at(src->PC_IndexRigidBody);
			this->m_rigidBodies.at(src->PC_IndexRigidBody)->setCollisionFlags(0);

			this->m_rigidBodies.at(src->PC_IndexRigidBody)->setGravity(this->m_GravityAcc);
		}

	}
	
	
	//player movement
}

PHYSICSDLL_API void BulletInterpreter::SyncPosWithBullet(PhysicsComponent* src)
{
	btRigidBody* temp = this->m_rigidBodies.at(src->PC_IndexRigidBody);
	btTransform tTranform; 
	temp->getMotionState()->getWorldTransform(tTranform);
	
	btVector3 nPos = this->crt_xmvecVec3(src->PC_pos);
	tTranform.setOrigin(nPos);

	btMotionState* newMotionState = nullptr;
	newMotionState = temp->getMotionState();
	
	newMotionState->setWorldTransform(tTranform);
	temp->setMotionState(newMotionState);


}

void BulletInterpreter::SyncBulletWithGame(PhysicsComponent * src)
{
	if (src->PC_IndexRigidBody != -1)
	{
		btRigidBody* rigidBody = nullptr;
		rigidBody = this->m_rigidBodies.at(src->PC_IndexRigidBody);

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
		btMotionState* ms = nullptr;
		
		ms = rigidBody->getMotionState();
		
		ms->setWorldTransform(initialTransform);
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

void BulletInterpreter::CreateRigidBody(PhysicsComponent* fromGame)
{
	btQuaternion temp = btQuaternion(0, 0, 0, 1); //do not know what this does exactlly
	btVector3 pos = btVector3(0, -1, 0);

	if (fromGame->PC_is_Static == true)
	{
		//collition shapes
		if (fromGame->PC_BVtype == BV_Plane)
		{
			btVector3 norm = this->crt_xmvecVec3(fromGame->PC_Plane.PC_normal);
			btStaticPlaneShape* StaticShape = new btStaticPlaneShape(norm, 1);
		}

		if (fromGame->PC_BVtype == BV_OBB)
		{
			btBoxShape* boxShape = nullptr;
		}

		if (fromGame->PC_BVtype == BV_Sphere)
		{
			//btSphereShape* sphereShape = nullptr;
			//sphereShape = new btSphereShape(fromGame->PC_Sphere.radius);
		}
		
	}
	else
	{
		if (fromGame->PC_BVtype == BV_Plane)
		{

		}

		if (fromGame->PC_BVtype == BV_OBB)
		{
			btBoxShape* boxShape = nullptr;
			btCollisionObject* temp = new btCollisionObject;
		}

		if (fromGame->PC_BVtype == BV_Sphere)
		{
			//btSphereShape* sphereShape = nullptr;
			//sphereShape = new btSphereShape(fromGame->PC_Sphere.radius);
		}

		if (fromGame->PC_BVtype == BV_AABB)
		{
			//btBoxShape* boxShape = nullptr;
		}
	}
}

void BulletInterpreter::TestBulletPhysics()
{
	std::ofstream outputFile;
	outputFile.open("BulletData.txt");
	float interval = 1.0f / 60.0f;

	for (int i = 0; i < 300; i++)
	{
		m_dynamicsWorld->stepSimulation(interval, 10);

		btTransform trans;
		this->m_rigidBodies.at(1)->getMotionState()->getWorldTransform(trans);

		outputFile << trans.getOrigin().getY() << '\n';
	}

	outputFile.close();
}

void BulletInterpreter::RegisterBox(int index)
{
	this->m_physicsHandlerIndex.push_back(index);
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

void BulletInterpreter::CreateSphere(float radius ,DirectX::XMVECTOR pos, float mass)
{
	btCollisionShape* sphereShape = new btSphereShape(radius);

	//creating a mothion state
	btVector3 startTrans = this->crt_xmvecVec3(pos);

	btQuaternion startTransQ = btQuaternion(0, 0, 0, 1.0f);
	btTransform initialTransform = btTransform(startTransQ, startTrans);

	btDefaultMotionState* sphereMotionState = nullptr;
	sphereMotionState = new btDefaultMotionState(initialTransform);

	//by giving the body 0 in mass, it will be immovable
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI
	(
		mass,					//mass
		sphereMotionState,
		sphereShape,
		crt_xmvecVec3(pos)
	);

	//create the rigid body
	btRigidBody* rigidBody = new btRigidBody(groundRigidBodyCI);
	this->m_rigidBodies.push_back(rigidBody);

	//add it into the world
	this->m_dynamicsWorld->addRigidBody(rigidBody);
}

void BulletInterpreter::CreateOBB(PhysicsComponent* src, int index)
{
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



	btVector3 rotation = btVector3(1, 1, 1);
	//rigidBody->setAngularVelocity(rotation);
	if (index == 0 || index == 1)
	{
		rigidBody->setAngularFactor(btVector3(0, 0, 0));
	}

	this->m_rigidBodies.push_back(rigidBody);
	this->m_dynamicsWorld->addRigidBody(rigidBody);
	int pos = this->m_rigidBodies.size() - 1;
	src->PC_IndexRigidBody = pos;
}

void BulletInterpreter::CreateAABB(PhysicsComponent* src, int index)
{
	//this is always static
	DirectX::XMVECTOR pos = src->PC_pos;
	DirectX::XMVECTOR ext = DirectX::XMVectorSet(src->PC_OBB.ext[0], src->PC_OBB.ext[1], src->PC_OBB.ext[2], 0);

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

	this->m_rigidBodies.push_back(rigidBody);
	this->m_dynamicsWorld->addRigidBody(rigidBody);
	
	this->m_physicsHandlerIndex.push_back(index);
	src->PC_IndexRigidBody = this->m_rigidBodies.size() - 1;
}

btRigidBody * BulletInterpreter::GetRigidBody(int index)
{
	return this->m_rigidBodies.at(index);
}

void BulletInterpreter::SetPlayer1(PhysicsComponent * p1)
{
	this->player1 = p1;
	this->player1->PC_IndexRigidBody = 0;

	this->player1->PC_OBB.ort = this->RotateBB(this->player1);
}

void BulletInterpreter::SetPlayer2(PhysicsComponent * p2)
{
	this->player2 = p2;
	this->player2->PC_IndexRigidBody = 1;

	this->player2->PC_OBB.ort = this->RotateBB(p2);
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
	result = DirectX::XMVectorSet(src.getX(), src.getY(), src.getZ(), 1);

	return result;
}
