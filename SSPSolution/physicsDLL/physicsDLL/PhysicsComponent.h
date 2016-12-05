#ifndef PHYSICSDLL_PHYSICS_PHYSICSCOMPONENT_H
#define PHYSICSDLL_PHYSICS_PHYSICSCOMPONENT_H

//#pragma once
//#define PHYSICSLIBRARY_EXPORTS
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API __declspec(dllimport)
#endif
#include <DirectXMath.h>

class PHYSICSDLL_PHYSICS_PHYSICSLIBRARY_API PhysicsComponent
{
private:
	DirectX::XMVECTOR m_pos;
	DirectX::XMVECTOR m_velocity;
	float m_mass;

	DirectX::XMVECTOR m_vel;
public:
	PhysicsComponent();
	~PhysicsComponent();

	void Update(const float &dt);

	DirectX::XMVECTOR GetPos()const;
	DirectX::XMVECTOR GetVelocity()const;
	float GetMass()const;

	void SetPos(DirectX::XMVECTOR pos);
	void SetVelocity(DirectX::XMVECTOR vel);
	void SetMass(float mass);

	void ApplyForce(DirectX::XMVECTOR force, const float &dt);

};

#endif
