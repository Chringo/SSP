#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H
#include "Entity.h"
class Player :
	public Entity
{
private:
	//Meters per second
	float m_speed;
	float m_jumpSpeed;
	bool m_onGround;
public:
	Player();
	~Player();


	int Update(float dT, InputHandler* inputHandler);
	int React(int entityID, EVENT reactEvent);

private:

};

#endif