#ifndef RESOURCELIB_SKELETON_SKELETON_H
#define RESOURCELIB_SKELETON_SKELETON_H
#include "Resource.h"
#include "Animation.h"
namespace Resources 
{
	class Skeleton :
		public Resource
	{
	public:
		struct Joint 
		{
			float invBindPose[16];
			int jointIndex   = 0;
			int parentIndex  = 0;
		};
		struct RawSkeletonData 
		{
			unsigned int jointCount		= 0;
			Joint*		 joints		    = nullptr;
			RawSkeletonData& RawSkeletonData::operator= (const RawSkeletonData & other)
			{
				if (this == &other)
					return *this;
				delete this->joints;
				jointCount = other.jointCount;
				this->joints = new Joint[jointCount];
				memcpy(joints, other.joints, (sizeof(Joint) * jointCount));
				return *this;
			}
			~RawSkeletonData()
			{
				delete this->joints;
				joints = nullptr;
				jointCount = 0;
			}
		};

	private:
		const static int	       MAX_ANIMATIONS = 6;
		RawSkeletonData		       m_skelData;
		unsigned int		       m_numAnimations = 0;
		std::vector<Animation*>    m_animations;
		std::vector<unsigned int>  m_animationIds;
	public:
		DLL_OPERATION Skeleton();
		DLL_OPERATION Skeleton(Resource::RawResourceData* resData);
		DLL_OPERATION Skeleton(Resource::RawResourceData* resData,RawSkeletonData* skelData);
		DLL_OPERATION virtual ~Skeleton();

		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData, RawSkeletonData* skelData);
		DLL_OPERATION Resources::Status Destroy(); // Deincrement references to connected data, free container
		DLL_OPERATION Resources::Status AddAnimation(Animation* anim, int index);
		
		DLL_OPERATION const unsigned int				  GetNumAnimations()		const { return m_numAnimations; };
		DLL_OPERATION const RawSkeletonData*			  GetSkeletonData()			const { return &m_skelData;		 };
		DLL_OPERATION const Animation*					  GetAnimation(int& index)  const { return ((unsigned int)index < m_numAnimations ? m_animations[index] : nullptr); };
		DLL_OPERATION const std::vector<unsigned int> *	  GetAllAnimationIds()		const { return &m_animationIds; };
		DLL_OPERATION virtual std::shared_ptr<char>		  GetDataAsBinary(size_t* size, bool* result = nullptr);
		DLL_OPERATION void								  SetNumAnimations(unsigned int m_numAnimations) { this->m_numAnimations = m_numAnimations; };

	};
}

#endif
