#ifndef RESOURCELIB_MESHHANDLER_MESHHANDLER_H
#define RESOURCELIB_MESHHANDLER_MESHHANDLER_H
#include "Mesh.h"
#include "FileLoader.h"
#include "FileHeaders.h"
namespace Resources
{

	class MeshHandler
	{

	private:
		std::unordered_map<unsigned int, ResourceContainer> m_meshes;
		std::deque<Mesh*> m_emptyContainers;
		std::vector<Mesh> m_containers;
		ID3D11Device* m_device = nullptr;

		Mesh* m_placeHolder = nullptr;
		MeshHandler();
	public:
		MeshHandler(size_t meshAmount, ID3D11Device* device = nullptr);
		virtual ~MeshHandler();

		Mesh* GetPlaceHolderMesh();
		Resources::Status GetMesh (const unsigned int& id,   ResourceContainer*& meshPtr);
		Resources::Status LoadMesh(const unsigned int & id,  ResourceContainer*& meshPtr );
		Resources::Status UnloadMesh(const unsigned int & id);

		void SetDevice(ID3D11Device* device) { this->m_device = device; };

	private:
		Resources::Status LoadPlaceHolderMesh();
	};
}

#endif

