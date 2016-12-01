#include "SkeletonHandler.h"



Resources::SkeletonHandler::SkeletonHandler()
{
}


Resources::SkeletonHandler::SkeletonHandler(size_t skelAmount, ID3D11Device * device)
{
}

Resources::Status Resources::SkeletonHandler::GetSkeleton(const unsigned int & id, ResourceContainer *& skelPtr)
{
	return Resources::Status();
}

Resources::Status Resources::SkeletonHandler::LoadSkeleton(const unsigned int & id, ResourceContainer *& skelPtr)
{
	return Resources::Status();
}

Resources::Status Resources::SkeletonHandler::UnloadSkeleton(const unsigned int & id)
{
	return Resources::Status();
}

Resources::SkeletonHandler::~SkeletonHandler()
{
}
