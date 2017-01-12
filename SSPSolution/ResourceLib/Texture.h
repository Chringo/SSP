#ifndef RESOURCELIB_TEXTURE_TEXTURE_H
#define RESOURCELIB_TEXTURE_TEXTURE_H
#include "Resource.h"

namespace Resources {

	class Texture :
		public Resource
	{
	private:
		char m_fileName[256];
		ID3D11ShaderResourceView* textureView	= nullptr;
		ID3D11Resource* textureResource			= nullptr;

	public:
		DLL_OPERATION Texture();
		DLL_OPERATION virtual ~Texture();
		DLL_OPERATION Texture(Resource::RawResourceData resData);

		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData);
		DLL_OPERATION Resources::Status Destroy(); 

		DLL_OPERATION void SetFileName(char * fileName, unsigned int nameLength) { memcpy((char*)&m_fileName, fileName, nameLength); }
		DLL_OPERATION Resources::Status SetTexture(ID3D11ShaderResourceView* view, ID3D11Resource* texture);
		DLL_OPERATION ID3D11ShaderResourceView* GetResourceView() { return textureView; };
		DLL_OPERATION ID3D11Resource* GetTextureResource() { return textureResource; };
		DLL_OPERATION char* GetFileName() { return m_fileName; };
		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);
	};
}
#endif
