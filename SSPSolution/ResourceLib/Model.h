#ifndef RESOURCELIB_MODEL_MODEL_H
#define RESOURCELIB_MODEL_MODEL_H
#include "Mesh.h"
#include "Material.h"

namespace Resources {

	class DLL_OPERATION Model :
		public Resource
	{
	public:
		struct RawModelData
		{
			unsigned int meshId;
			unsigned int materialId;
			
		};

	private:
		RawModelData* m_rawData = nullptr;
		Mesh* m_modelMesh		= nullptr;
		Material* m_material	= nullptr;
		//Skeleton* m_skeleton;
	public:
		Model(Resource::RawResourceData resData);
		Model();
		virtual ~Model();
		Resources::Status Create(Resource::RawResourceData* resData, RawModelData* = nullptr,bool keepRawData = false);
		Resources::Status Destroy(); // Deincrement references to connected data

		void SetMesh(Mesh* modelMesh) { this->m_modelMesh = modelMesh;};
		Mesh* GetMesh() const { return this->m_modelMesh; };

		void SetMaterial(Material* material) { this->m_material = material; };
		Material* GetMaterial() { return this->m_material; };
		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);

	};
}

#endif
