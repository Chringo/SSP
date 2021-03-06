#ifndef RESOURCELIB_MATERIALHANDLER_MATERIALHANDLER_H
#define RESOURCELIB_MATERIALHANDLER_MATERIALHANDLER_H
#include "Resource.h"
#include "Material.h"
#include "TextureHandler.h"
namespace Resources
{
	/*
	AUTHOR: Martin Clementson
	This class holds all the skeleton data in ram.

	This class was optimized for the game.It pre allocated memory for a certain amount
	of materials.Then it never deallocate them during runtime.It keeps track of available containers.
	When a material is unloaded its buffers are released and the material object is put into "emptyContainers" which
	means that its ready to be used with new data.
	*/

	class MaterialHandler
	{
	private:
		std::unordered_map<unsigned int, ResourceContainer> m_materials;
		std::deque<Material*> m_emptyContainers;
		std::vector<std::vector<Material>*> m_containers;
		ID3D11Device* m_device = nullptr;

		Material* m_placeholder = nullptr;
		TextureHandler* m_textureHandler;
		MaterialHandler();
	public:
		MaterialHandler(size_t materialAmount, ID3D11Device* device = nullptr);
		virtual ~MaterialHandler();
		 Resources::Status ClearUnusedMemory(); // This Will go through the resourceLib and shrink all arrays and vectors to fit. Thus removing all other items in the resource pool
		Resources::Status GetMaterial( const unsigned int& id, ResourceContainer *&materialPtr);
		Resources::Status LoadMaterial( unsigned int& id, ResourceContainer*& materialPtr);
		Resources::Status UnloadMaterial( const unsigned int & id);
		void SetDevice(ID3D11Device* device) { m_device = device; m_textureHandler->SetDevice(device);};
		Material* GetPlaceHolderMaterial();
	private:
		Resources::Status CreatePlaceHolder();
		Material* GetEmptyContainer();
	};

}
#endif