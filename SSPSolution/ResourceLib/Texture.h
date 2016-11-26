#ifndef RESOURCELIB_TEXTURE_TEXTURE_H
#define RESOURCELIB_TEXTURE_TEXTURE_H
#include "Resource.h"

namespace Resources {

	class Texture :
		public Resource
	{
	private:
		char m_filePath[256];
		ID3D11ShaderResourceView* textureView	= nullptr;
		ID3D11Resource* textureResource			= nullptr;

	public:
		Texture();
		virtual ~Texture();
		Texture(Resource::RawResourceData resData);

		Resources::Status Create(Resource::RawResourceData* resData);
		Resources::Status Destroy(); 

		Resources::Status SetTexture(ID3D11ShaderResourceView* view, ID3D11Resource* texture);
		ID3D11ShaderResourceView* GetResourceView() { return textureView; };
		ID3D11Resource* GetTextureResource() { return textureResource; };

		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);
	};
}
#endif
