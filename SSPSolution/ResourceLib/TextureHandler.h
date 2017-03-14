#ifndef RESOURCELIB_TEXTUREHANDLER_TEXTUREHANDLER_H
#define RESOURCELIB_TEXTUREHANDLER_TEXTUREHANDLER_H
#include "FileLoader.h"
#include "FileHeaders.h"
#include "Texture.h"
#include "TextureLoader\DDSTextureLoader.h"
namespace Resources
{

	class TextureHandler
	{
	private:
    	 std::string TEXTURE_PATH = "../Assets/";
		std::unordered_map<unsigned int, ResourceContainer> m_textures;
		std::deque<Texture*> m_emptyContainers;
		std::vector<std::vector<Texture>*> m_containers;
		ID3D11Device* m_device = nullptr;
		
		Texture* placeHolder = nullptr;
		TextureHandler();
	public:
		DLL_OPERATION TextureHandler(size_t textureAmount, ID3D11Device* device = nullptr);
		DLL_OPERATION virtual ~TextureHandler();

		DLL_OPERATION Resources::Status GetTexture(const unsigned int& id, ResourceContainer*& texturePtr);
		DLL_OPERATION Resources::Status GetTexture(const unsigned int& id, Texture*& texturePtr);
		DLL_OPERATION Resources::Status LoadTexture(const unsigned int& id, ResourceContainer*& texturePtr);
		DLL_OPERATION Resources::Status UnloadTexture(const unsigned int& id);
		DLL_OPERATION Resources::Status ClearUnusedMemory(); // This Will go through the resourceLib and shrink all arrays and vectors to fit. Thus removing all other items in the resource pool
		DLL_OPERATION void SetDevice(ID3D11Device* device);

		DLL_OPERATION Texture* GetPlaceHolderTextures();
	private:
		bool LoadPlaceHolderTextures();
		Texture* GetEmptyContainer();
	};
}

#endif
