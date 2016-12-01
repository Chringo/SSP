#ifndef RESOURCELIB_SKELETON_SKELETON_H
#define RESOURCELIB_SKELETON_SKELETON_H
#include "Resource.h"
#include "Animation.h"
namespace Resources {

	class Skeleton :
		public Resource
	{
		struct Joint {
			float bindPose[16];
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
				joints = new Joint[jointCount];
				memcpy(joints, other.joints, sizeof(Joint) * jointCount);
				return *this;
			}
			~RawSkeletonData()
			{
				delete[] joints;
				joints = nullptr;
				jointCount = 0;
			}
		};

	private:
		RawSkeletonData		     m_skelData;
		unsigned int		     m_numAnimations = 0;
		std::vector<Animation*>  m_animations;
		unsigned int*			 m_animationIds  = nullptr;
	public:
		Skeleton();
		Skeleton(Resource::RawResourceData* resData);
		Skeleton(Resource::RawResourceData* resData,RawSkeletonData* skelData);
		virtual ~Skeleton();

		Resources::Status Create(Resource::RawResourceData* resData, RawSkeletonData* skelData);
		Resources::Status Destroy(); // Deincrement references to connected data, free container
		Resources::Status AddAnimation(Animation* anim, int index);
		
		const unsigned int      GetNumAnimations()		 const { return m_numAnimations; };
		const RawSkeletonData   GetSkeletonData()	     const { return m_skelData;		 };
		const Animation*		GetAnimation(int& index) const { return (index < m_numAnimations ? m_animations[index] : nullptr); };

		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);


	};
}

#endif
