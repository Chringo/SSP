#ifndef RESOURCELIB_ANIMATIONHANDLER_ANIMATIONHANDLER_H
#define RESOURCELIB_ANIMATIONHANDLER_ANIMATIONHANDLER_H
#include "Animation.h"
#include "FileLoader.h"
#include "FileHeaders.h"
namespace Resources 
{
	/*
	AUTHOR: Martin Clementson
	This class holds all the mesh data in ram.

	This class was optimized for the game.It pre allocated memory for a certain amount
	of animations .Then it never deallocate them during runtime.It keeps track of available containers.
	When an animation is unloaded its buffers are released and the animation object is put into "emptyContainers" which
	means that its ready to be used with new data.
	*/
	class AnimationHandler
	{
	private:
		std::unordered_map<unsigned int, ResourceContainer> m_animations;
		std::deque <Animation*> m_emptyContainers;
		std::vector<Animation>  m_containers;
		AnimationHandler();
	public:
		AnimationHandler(size_t animAmount);
		~AnimationHandler();

		Resources::Status GetAnimation   (const unsigned int& id, ResourceContainer *& animPtr);
		Resources::Status LoadAnimation  (const unsigned int& id, ResourceContainer *& animPtr);
		Resources::Status UnloadAnimation(const unsigned int& id);
	};
}

#endif