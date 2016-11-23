#ifndef PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H
#define PHYSICSDLL_PHYSICS_PHYSICSHANDLER_H

//#pragma once
//#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif

#include "PhysicsComponent.h"

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsHandler
{
private:
	//Chain m_chain;

	PhysicsComponent* m_components;
	int m_capacity;
	int m_nrOfComponents;
	//std::vector<Plane> m_walls;

	DirectX::XMVECTOR m_gravity;

	const float m_offSet = 0.5f;
public:
	PhysicsHandler();
	~PhysicsHandler();

	bool Initialize();
	void Update();

	void SimpleCollition(float dt);
	void SimpleGravity(PhysicsComponent* componentPtr, const float &dt);

	int getNrOfComponents()const;
	PhysicsComponent* getPComponents()const;


	//bool SpherePlaneIntersevtion(PhysicsComponent* pComponent, float radius, Plane plane, float dt);
	//bool PointInTriangleTest(DirectX::XMVECTOR point, Triangle triangle);


};

#endif
