#ifndef SSPAPPLICATION_CORE_ANIMATIONSTORER_H
#define SSPAPPLICATION_CORE_ANIMATIONSTORER_H

#include "Animation.h"
#include "Entity.h"

struct AnimationComponent
{
	int startFrame;
	int endFrame;
	bool loop;
	int stateIndex;
};

/*Animation Storer class will handle animations from every "animatible" entity.*/
class AnimationStorer
{
public:
	AnimationStorer();
	~AnimationStorer();
	void Update(float dt);

private:
	Animation anim;
	std::vector<Entity> entityList;
};

#endif
