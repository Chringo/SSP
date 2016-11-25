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
		std::unordered_map<unsigned int, ResourceContainer> m_textures;
		std::deque<Texture*> m_emptyContainers;
		std::vector<Texture> m_containers;
		ID3D11Device* m_device = nullptr;
		
		Texture* placeHolder = nullptr;
		TextureHandler();
	public:
		TextureHandler(size_t textureAmount, ID3D11Device* device = nullptr);
		virtual ~TextureHandler();

		Resources::Status GetTexture(const unsigned int& id, ResourceContainer*& texturePtr);
		Resources::Status LoadTexture(const unsigned int& id, ResourceContainer*& texturePtr);
		Resources::Status UnloadTexture(const unsigned int& id);

		void SetDevice(ID3D11Device* device);

		Texture* GetPlaceHolderTexture();
	private:
		bool LoadPlaceHolderTexture();
	};
}

#endif
