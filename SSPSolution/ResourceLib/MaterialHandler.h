#ifndef RESOURCELIB_MATERIALHANDLER_MATERIALHANDLER_H
#define RESOURCELIB_MATERIALHANDLER_MATERIALHANDLER_H
#include "Resource.h"
#include "Material.h"
#include "TextureHandler.h"
namespace Resources
{

	class MaterialHandler
	{
	private:
		std::unordered_map<unsigned int, ResourceContainer> m_materials;
		std::deque<Material*> m_emptyContainers;
		std::vector<Material> m_containers;
		ID3D11Device* m_device = nullptr;

		Material* m_placeholder = nullptr;
		TextureHandler* m_textureHandler;
		MaterialHandler();
	public:
		MaterialHandler(size_t materialAmount, ID3D11Device* device = nullptr);
		virtual ~MaterialHandler();

		Resources::Status GetMaterial( const unsigned int& id, ResourceContainer *&materialPtr);
		Resources::Status LoadMaterial( unsigned int& id, ResourceContainer*& materialPtr);
		Resources::Status UnloadMaterial( const unsigned int & id);
		void SetDevice(ID3D11Device* device) { m_device = device; m_textureHandler->SetDevice(device);};
		Material* GetPlaceHolderMaterial();
	private:
		Resources::Status CreatePlaceHolder();
	};

}
#endif