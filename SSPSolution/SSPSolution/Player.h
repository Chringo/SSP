#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H
#include "Entity.h"
class Player :
	public Entity
{
private:
	//Meters per second
	float m_speed;
	Entity* m_grabbed;
public:
	Player();
	~Player();

	int Initialize();

	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

	//Returns the old speed
	float SetSpeed(float speed);

	PhysicsComponent* GetPhysicsComponentRef();
private:

};

#endif