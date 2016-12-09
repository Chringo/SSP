#ifndef RESOURCELIB_ANIMATIONHANDLER_ANIMATIONHANDLER_H
#define RESOURCELIB_ANIMATIONHANDLER_ANIMATIONHANDLER_H
#include "Animation.h"
#include "FileLoader.h"
#include "FileHeaders.h"
namespace Resources {


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