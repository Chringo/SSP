#include "TextureHandler.h"

 inline std::wstring charToWChar(const char* text)
{
	 const size_t size = std::strlen(text);
	 std::wstring wstr;
	 if (size > 0) {
		 wstr.resize(size);
		 std::mbstowcs(&wstr[0], text, size);
	 }
	 return wstr;
}

Resources::TextureHandler::TextureHandler()
{
}


Resources::TextureHandler::TextureHandler(size_t textureAmount, ID3D11Device * device)
{
	this->m_emptyContainers.resize(5);

	this->m_textures.reserve(5);
	this->m_containers.push_back(new std::vector<Texture>);
	this->m_containers.at(0)->insert(m_containers.at(0)->begin(), 5, Texture());
	this->m_containers.reserve(5);
	for (size_t i = 0; i < 5; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(0)->at(i);
	}

	if (device != nullptr) {
		this->m_device = device;
	}

}

Resources::TextureHandler::~TextureHandler()
{
	for (size_t i = 0; i < m_containers.size(); i++)
	{
		delete m_containers.at(i);
	}
	delete[] placeHolder;
	placeHolder = nullptr;
}

Resources::Status Resources::TextureHandler::GetTexture(const unsigned int & id, ResourceContainer *& texturePtr)
{
	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_textures.find(id);
	if (got == m_textures.end()) { // if the texture does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		texturePtr = &got->second;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::TextureHandler::GetTexture(const unsigned int & id, Texture *& texturePtr)
{
	std::unordered_map<unsigned int, ResourceContainer>::iterator got = m_textures.find(id);
	if (got == m_textures.end()) { // if the texture does not exists in memory

		return Resources::Status::ST_RES_MISSING;
	}
	else {
		texturePtr = (Texture*)&got->second.resource;
	}

	return Resources::Status::ST_OK;
}

Resources::Status Resources::TextureHandler::LoadTexture(const unsigned int & id, ResourceContainer *& texturePtr)
{
	char* data = nullptr;
	size_t size = 0;
	
	Status st = FileLoader::GetInstance()->LoadResource(id, data, &size);
	if (st != ST_OK)
		return st;

	Resource::RawResourceData* resData = (Resource::RawResourceData*)data;
	if (resData->m_resType != RES_TEXTURE)
	{
#ifdef _DEBUG
		std::cout << "Wrong resource type. Wanted Texture, got type: " << resData->m_id << std::endl;
#endif // _DEBUG
		return ST_WRONG_RESTYPE;
	}

	Texture* newTexture = GetEmptyContainer(); //Get an empty container
	st = newTexture->Create(resData);
	 if (st != ST_OK)
		 return st;
	
	TextureHeader* texData = (TextureHeader*)(data + sizeof(Resource::RawResourceData));
	
	std::string fullpath = TEXTURE_PATH;
	fullpath.append(texData->filePath);		//append the file name to the directory
	size_t length = strlen(fullpath.c_str());
	wchar_t path[256];
	mbstowcs_s(&length, path, fullpath.c_str(), length);
	
	ID3D11ShaderResourceView* textureView	=nullptr;
	ID3D11Resource*			textureResource		=nullptr;
	HRESULT hr = DirectX::CreateDDSTextureFromFile(m_device,
		 path,
		 &textureResource,
		 &textureView, 
		 size_t(0),
		 (DirectX::DDS_ALPHA_MODE*)DirectX::DDS_ALPHA_MODE_UNKNOWN);
	if (FAILED(hr))
	{
		newTexture->Destroy();
#ifdef _DEBUG
		std::cout << "Could not open texture file : " << fullpath << std::endl;
#endif // _DEBUG
		return Status::ST_ERROR_OPENING_FILE;
	}

	st = newTexture->SetTexture(textureView, textureResource);
	if (st != ST_OK)
	{
		Resources::SAFE_RELEASE(textureView		 );
		Resources::SAFE_RELEASE(textureResource	 );
		return st;
	}



	m_textures[newTexture->GetId()] = ResourceContainer(newTexture, 1); //put it into the map
	texturePtr = &m_textures[newTexture->GetId()];
	m_emptyContainers.pop_front();


		return Resources::Status::ST_OK;
}

Resources::Status Resources::TextureHandler::UnloadTexture(const unsigned int & id)
{

	ResourceContainer* textureRes = nullptr;

	Status st = GetTexture(id, textureRes);
	switch (st)
	{
	case ST_OK:
		textureRes->refCount -= 1;
		if (textureRes->refCount <= 0)
		{
			((Texture*)textureRes->resource)->Destroy();
			m_emptyContainers.push_back(((Texture*)textureRes->resource));
			m_textures.erase(id);
#ifdef _DEBUG
			std::cout << "Texture : " << id << ". Has been Unloaded" << std::endl;
#endif // _DEBUG
		}
	default:
		return st;

	}
	return Resources::Status::ST_OK;
}



Resources::Texture * Resources::TextureHandler::GetPlaceHolderTextures()
{
	if (this->placeHolder == nullptr)
	{
		LoadPlaceHolderTextures();
	}
	return placeHolder;
}

bool Resources::TextureHandler::LoadPlaceHolderTextures()
{

	if (placeHolder != nullptr)
		return false;

#ifdef _DEBUG
	std::cout << "Loading Placeholder Texture" << std::endl;
#endif // _DEBUG

	
	/*T E M P*/
	Resource::RawResourceData temp;
	temp.m_resType = RES_TEXTURE;
	temp.m_id = 7869;
	/***************/
	Status st;
	placeHolder = new Texture[5];
	for (size_t i = 0; i < 5; i++)
	{
			 st = placeHolder[i].Create(&temp);
			if (st != ST_OK)
				return false;
		}
#pragma region Load Textures
		std::string path_str[5];
		std::string path_str_EDITOR[5];
		wchar_t path[5][256];
		size_t length[5];
		ID3D11ShaderResourceView* textureView[5];
		ID3D11Resource*			textureResource[5];
		///*PBR test textures*/
		path_str[TEXTURE_ALBEDO]	= std::string("../ResourceLib/AssetFiles/PBRTEST/test_albedom.dds");
		path_str[TEXTURE_SPECULAR]	= std::string("../ResourceLib/AssetFiles/PBRTEST/test_metalness.dds");
		path_str[TEXTURE_ROUGHNESS] = std::string("../ResourceLib/AssetFiles/PBRTEST/test_roughness.dds");
		path_str[TEXTURE_NORMAL]	= std::string("../ResourceLib/AssetFiles/PBRTEST/test_normal.dds");
		path_str[TEXTURE_AO]		= std::string("../ResourceLib/AssetFiles/PBRTEST/test_ao.dds");

		
		path_str_EDITOR[TEXTURE_ALBEDO]     = std::string("../../ResourceLib/AssetFiles/PBRTEST/test_albedom.dds");
		path_str_EDITOR[TEXTURE_SPECULAR]   = std::string("../../ResourceLib/AssetFiles/PBRTEST/test_metalness.dds");
		path_str_EDITOR[TEXTURE_ROUGHNESS]  = std::string("../../ResourceLib/AssetFiles/PBRTEST/test_roughness.dds");
		path_str_EDITOR[TEXTURE_NORMAL]     = std::string("../../ResourceLib/AssetFiles/PBRTEST/test_normal.dds");
		path_str_EDITOR[TEXTURE_AO]			= std::string("../../ResourceLib/AssetFiles/PBRTEST/test_ao.dds");



		/*JOHN Textures*/
		//path_str[TEXTURE_ALBEDO]	 = std::string("../ResourceLib/AssetFiles/PLACEHOLDER_MODEL_ALBEDO.dds");
		//path_str[TEXTURE_SPECULAR]	 = std::string("../ResourceLib/AssetFiles/PLACEHOLDER_MODEL_METALLIC.dds");
		//path_str[TEXTURE_ROUGHNESS]  = std::string("../ResourceLib/AssetFiles/PLACEHOLDER_MODEL_ROUGHNESS.dds");
		//path_str[TEXTURE_NORMAL]	 = std::string("../ResourceLib/AssetFiles/PLACEHOLDER_MODEL_NORMAL.dds");
		//path_str[TEXTURE_AO]		 = std::string("../ResourceLib/AssetFiles/PLACEHOLDER_MODEL_AO.dds");

		for (size_t i = 0; i < 5; i++)
		{
			 length[i] = strlen(path_str[i].c_str());
		
			mbstowcs_s(&length[i], path[i], path_str[i].c_str(), length[i]);
	
		HRESULT hr = DirectX::CreateDDSTextureFromFile(m_device,
			path[i],
			&textureResource[i],
			&textureView[i],
			size_t(2048),
			(DirectX::DDS_ALPHA_MODE*)DirectX::DDS_ALPHA_MODE_UNKNOWN);

		if (FAILED(hr))
		{
			//Try the editor filepath
			length[i] = strlen(path_str_EDITOR[i].c_str());

			mbstowcs_s(&length[i], path[i], path_str_EDITOR[i].c_str(), length[i]);

			HRESULT hr = DirectX::CreateDDSTextureFromFile(m_device,
				path[i],
				&textureResource[i],
				&textureView[i],
				size_t(2048),
				(DirectX::DDS_ALPHA_MODE*)DirectX::DDS_ALPHA_MODE_UNKNOWN);

			if (FAILED(hr)) //If it still doesent work, there  is a problem
			{
#ifdef _DEBUG
				std::cout << "Could not open texture file : " << path_str[i] << std::endl;
#endif // _DEBUG
				return false;
			}
		}
		
#ifdef _DEBUG
			std::cout << "Opened file : " << path_str[i] << std::endl;
#endif // _DEBUG
		



		st = placeHolder[i].SetTexture(textureView[i], textureResource[i]);
		if (st != ST_OK)
		{
			Resources::SAFE_RELEASE(textureView[i]);
			Resources::SAFE_RELEASE(textureResource[i]);
			return false;
		}
	}
#pragma endregion

	return true;
}

Resources::Texture * Resources::TextureHandler::GetEmptyContainer()
{
	if (m_emptyContainers.size() < 1)
	{
		m_containers.push_back(new std::vector<Texture>(20));
		for (size_t i = 0; i < 20; i++)
		{
			m_emptyContainers.push_back(&m_containers.at(m_containers.size() - 1)->at(i));
		}
	}
	return m_emptyContainers.front();
}

void Resources::TextureHandler::SetDevice(ID3D11Device * device)
{
	this->m_device = device;
}

