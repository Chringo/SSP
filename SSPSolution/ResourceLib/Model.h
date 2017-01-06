#ifndef RESOURCELIB_MODEL_MODEL_H
#define RESOURCELIB_MODEL_MODEL_H
#include "Mesh.h"
#include "Material.h"
#include "Skeleton.h"
#include "FileHeaders.h"
namespace Resources 
{
	class Model :
		public Resource
	{
	public:
		struct RawModelData
		{
			unsigned int meshId		= 0;
			unsigned int materialId	= 0;
			unsigned int skeletonId	= 0;
		};

	private:
		RawModelData* m_rawData     = nullptr;
		Mesh*	      m_modelMesh	= nullptr;
		Material*	  m_material	= nullptr;
		Skeleton*	  m_skeleton	= nullptr;	
		//Animation*  m_animations  = nullptr;
	public:
		DLL_OPERATION Model(Resource::RawResourceData resData);
		DLL_OPERATION Model();
		DLL_OPERATION virtual ~Model();
		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData, RawModelData* = nullptr,bool keepRawData = false);
		DLL_OPERATION Resources::Status Destroy(); // Deincrement references to connected data
		DLL_OPERATION RawModelData* GetRawModelData() { return this->m_rawData; }
		
		DLL_OPERATION void SetMesh(Mesh* modelMesh) { this->m_modelMesh = modelMesh;};
		DLL_OPERATION Mesh* GetMesh() const { return this->m_modelMesh; };
		
		DLL_OPERATION void SetMaterial(Material* material) { this->m_material = material; };
		DLL_OPERATION Material* GetMaterial() { return this->m_material; };
		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);
		
		DLL_OPERATION void SetSkeleton(Skeleton* skeleton) { this->m_skeleton = skeleton; };
		DLL_OPERATION Skeleton* GetSkeleton() { return this->m_skeleton; };

		DLL_OPERATION void SetOBBData(BoundingBoxHeader obbdata) { this->m_modelMesh->SetOBBData(obbdata); };
		DLL_OPERATION BoundingBoxHeader GetOBBData() { return this->m_modelMesh->GetOBBData(); };
	
	};
}

#endif
