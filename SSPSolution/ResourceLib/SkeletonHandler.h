#ifndef RESOURCELIB_SKELETONHANDLER_SKELETONHANDLER_H
#define RESOURCELIB_SKELETONHANDLER_SKELETONHANDLER_H
#include "Skeleton.h"
namespace Resources {

	class SkeletonHandler
	{

	private:

		std::unordered_map<unsigned int, ResourceContainer> m_skeletons;
		std::deque <Skeleton*> m_emptyContainers;
		std::vector<Skeleton>  m_containers;
		ID3D11Device*		   m_device		 = nullptr;
		Skeleton*			   m_placeHolder = nullptr;
		SkeletonHandler();
	public:
		SkeletonHandler(size_t skelAmount, ID3D11Device* device = nullptr);

		Resources::Status GetSkeleton(const unsigned int& id, ResourceContainer*& skelPtr);
		Resources::Status LoadSkeleton(const unsigned int& id, ResourceContainer*& skelPtr);
		Resources::Status UnloadSkeleton(const unsigned int& id);

		void SetDevice(ID3D11Device* device) { this->m_device = device; };
		virtual ~SkeletonHandler();
	};

}

#endif