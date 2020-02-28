#ifndef RESOURCELIB_SKELETONHANDLER_SKELETONHANDLER_H
#define RESOURCELIB_SKELETONHANDLER_SKELETONHANDLER_H
#include "Skeleton.h"
#include "FileLoader.h"
#include "FileHeaders.h"
#include "AnimationHandler.h"
namespace Resources {
	/*
	AUTHOR: Martin Clementson
	This class holds all the skeleton data in ram.

	This class was optimized for the game.It pre allocated memory for a certain amount
	of skeletons.Then it never deallocate them during runtime.It keeps track of available containers.
	When a skeleton is unloaded its buffers are released and the skeleton object is put into "emptyContainers" which
	means that its ready to be used with new data.
	*/
	class SkeletonHandler
	{

	private:

		std::unordered_map<unsigned int, ResourceContainer> m_skeletons;
		std::deque <Skeleton*> m_emptyContainers;
		std::vector<std::vector<Skeleton>*>  m_containers;
		ID3D11Device*		   m_device		 = nullptr;
		AnimationHandler*	   m_animHandler = nullptr;
		Skeleton*			   m_placeHolder = nullptr;
		SkeletonHandler();
	public:
		SkeletonHandler(size_t skelAmount, ID3D11Device* device = nullptr);

		Resources::Status GetSkeleton(const unsigned int& id, ResourceContainer*& skelPtr);
		Resources::Status LoadSkeleton(const unsigned int& id, ResourceContainer*& skelPtr);
		Resources::Status UnloadSkeleton(const unsigned int& id);
		Resources::Status ClearUnusedMemory(); // This Will go through the resourceLib and shrink all arrays and vectors to fit. Thus removing all other items in the resource pool
		void SetDevice(ID3D11Device* device) { this->m_device = device; };
		virtual ~SkeletonHandler();

		AnimationHandler* GetAnimationHandler() const {return m_animHandler;}
		Resources::Status LoadAllSkeletonsInBPF();
	private:
		Skeleton* GetEmptyContainer();
	};

}

#endif