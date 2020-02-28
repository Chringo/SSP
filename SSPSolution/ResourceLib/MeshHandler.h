#ifndef RESOURCELIB_MESHHANDLER_MESHHANDLER_H
#define RESOURCELIB_MESHHANDLER_MESHHANDLER_H
#include "Mesh.h"
#include "FileLoader.h"
#include "FileHeaders.h"
namespace Resources
{
	/*
	AUTHOR: Martin Clementson
	This class holds all the mesh data in ram.

	This class was optimized for the game.It pre allocated memory for a certain amount
	of meshes.Then it never deallocate them during runtime.It keeps track of available containers.
	When  a mesh is unloaded its buffers are released and the mesh object is put into "emptyContainers" which
	means that its ready to be used with new data.
	*/

	class MeshHandler
	{

	private:
		std::unordered_map<unsigned int, ResourceContainer> m_meshes;
		std::deque<Mesh*> m_emptyContainers;
		std::vector<std::vector<Mesh>*> m_containers;
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
		 Resources::Status ClearUnusedMemory(); // This Will go through the resourceLib and shrink all arrays and vectors to fit. Thus removing all other items in the resource pool

		void SetDevice(ID3D11Device* device) { this->m_device = device; };
		Resources::Status LoadAllMeshesInBPF();
	private:
		Resources::Status LoadPlaceHolderMesh();
		Mesh* GetEmptyContainer();
	};
}

#endif

