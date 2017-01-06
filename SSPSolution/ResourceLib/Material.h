#ifndef RESOURCELIB_MATERIAL_MATERIAL_H
#define RESOURCELIB_MATERIAL_MATERIAL_H
#include "Texture.h"

namespace Resources 
{
	class Material :
		public Resource
	{
	private:
		Texture* m_Textures[5]{nullptr,nullptr,nullptr,nullptr,nullptr};
		float m_Metallic		= 0.0f;
		float m_Roughness		= 0.0f;
		float m_EmissiveValue	= 0.0f;
	public:
		DLL_OPERATION Material(Resource::RawResourceData resData);
		DLL_OPERATION Material();
		DLL_OPERATION virtual ~Material();
		
		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData);
		DLL_OPERATION Resources::Status Destroy(); // Deincrement references to connected data
		
		DLL_OPERATION Resources::Status SetTexture(Texture* texture, TextureType type);
		DLL_OPERATION Texture** GetAllTextures() { return m_Textures; };
		DLL_OPERATION Texture* GetTexture(TextureType type) { return (type == TextureType::TEXTURE_UNKNOWN ? nullptr : m_Textures[type]); }
		
		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);

		DLL_OPERATION void SetMetallic(const float& val) { m_Metallic = val; };
		DLL_OPERATION void SetRoughness(const float& val) { m_Roughness = val; };
		DLL_OPERATION void SetEmissive(const float& val) { m_EmissiveValue = val; };
		DLL_OPERATION void SetValues(const float& metal, const float& roughness, const float& emissive) {
			m_Metallic = metal;
			 m_Roughness = roughness;
			m_EmissiveValue = emissive;
		};
	};
}
#endif

