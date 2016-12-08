#ifndef SSPAPPLICATION_ENTITIES_PLAYER_H
#define SSPAPPLICATION_ENTITIES_PLAYER_H
#include "Entity.h"
class Player :
	public Entity
{
private:

public:
	Player();
	~Player();


	int Update(float dT);
	int React(int entityID, EVENT reactEvent);

private:

};

#endif