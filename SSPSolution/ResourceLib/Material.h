#ifndef RESOURCELIB_MATERIAL_MATERIAL_H
#define RESOURCELIB_MATERIAL_MATERIAL_H
#include "Texture.h"

namespace Resources {
	
	class DLL_OPERATION Material :
		public Resource
	{

	private:
		Texture* m_Textures[5]{nullptr,nullptr,nullptr,nullptr,nullptr};
		float m_Metallic		= 0.0f;
		float m_Roughness		= 0.0f;
		float m_EmissiveValue	= 0.0f;
	public:
		Material(Resource::RawResourceData resData);
		Material();
		virtual ~Material();

		Resources::Status Create(Resource::RawResourceData* resData);
		Resources::Status Destroy(); // Deincrement references to connected data
	
		Resources::Status SetTexture(Texture* texture, TextureType type);
		Texture** GetAllTextures() { return m_Textures; };
		Texture* GetTexture(TextureType type) { return (type == TextureType::TEXTURE_UNKNOWN ? nullptr : m_Textures[type]); }

		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);
	
		void SetMetallic(const float& val) { m_Metallic = val; };
		void SetRoughness(const float& val) { m_Roughness = val; };
		void SetEmissive(const float& val) { m_EmissiveValue = val; };
		void SetValues(const float& metal, const float& roughness, const float& emissive) {
			m_Metallic = metal;
			 m_Roughness = roughness;
			m_EmissiveValue = emissive;
		};
	};
}

#endif

