#ifndef RESOURCELIB_SKELETON_SKELETON_H
#define RESOURCELIB_SKELETON_SKELETON_H
#include "Resource.h"
#include "Animation.h"
namespace Resources {

	class Skeleton :
		public Resource
	{
	public:
		struct Joint {
			float invBindPose[16];
			int jointIndex   = 0;
			int parentIndex  = 0;
		};
		struct RawSkeletonData {
			unsigned int jointCount		= 0;
			Joint*		 joints		    = nullptr;
			RawSkeletonData& RawSkeletonData::operator= (const RawSkeletonData & other)
			{
				if (this == &other)
					return *this;
				delete this->joints;
				jointCount = other.jointCount;
				this->joints = new Joint[jointCount];
				memcpy(joints, other.joints, sizeof(Joint) * jointCount);
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
		Skeleton();
		Skeleton(Resource::RawResourceData* resData);
		Skeleton(Resource::RawResourceData* resData,RawSkeletonData* skelData);
		virtual ~Skeleton();

		Resources::Status Create(Resource::RawResourceData* resData, RawSkeletonData* skelData);
		Resources::Status Destroy(); // Deincrement references to connected data, free container
		Resources::Status AddAnimation(Animation* anim, int index);
		
		const unsigned int				  GetNumAnimations()		const { return m_numAnimations; };
		const RawSkeletonData			  GetSkeletonData()			const { return m_skelData;		 };
		const Animation*				  GetAnimation(int& index)  const { return (index < m_numAnimations ? m_animations[index] : nullptr); };
		const std::vector<unsigned int> * GetAllAnimationIds()		const { return &m_animationIds; };
		virtual std::shared_ptr<char>	  GetDataAsBinary(size_t* size, bool* result = nullptr);


	};
}

#endif
