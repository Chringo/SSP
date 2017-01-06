#include "AnimationStorer.h"

AnimationStorer::AnimationStorer()
{
}

AnimationStorer::~AnimationStorer()
{
}

void AnimationStorer::Update(float dt)
{
	if (entityList.size() > 0)
	{
		for (int entityIndex = 0; entityIndex < (int)entityList.size(); entityIndex++)
		{
			anim.Update(dt);
		}
	}
}
