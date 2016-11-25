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
	this->m_emptyContainers.resize(textureAmount);

	this->m_textures.reserve(textureAmount);
	this->m_containers.reserve(textureAmount);
	this->m_containers.insert(m_containers.begin(), textureAmount, Texture());
	for (size_t i = 0; i < textureAmount; i++)
	{
		m_emptyContainers.at(i) = &m_containers.at(i);
	}

	if (device != nullptr) {
		this->m_device = device;
	}

}

Resources::TextureHandler::~TextureHandler()
{
	delete placeHolder;
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

	Texture* newTexture = m_emptyContainers.front(); //Get an empty container
	st = newTexture->Create(resData);
	 if (st != ST_OK)
		 return st;
	
	TextureHeader* texData = (TextureHeader*)(data + sizeof(Resource::RawResourceData));

	 size_t length = strlen(texData->filePath);
	 wchar_t path[256];
	 mbstowcs_s(&length, path, texData->filePath, length);

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
		std::cout << "Could not open texture file : " << path << std::endl;
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

     /*T E M P*/
	Resource::RawResourceData temp;
	temp.m_resType = RES_TEXTURE;
	temp.m_name[0] = 'T';
	temp.m_name[1] = 'E';
	temp.m_name[2] = 'X';
	temp.m_name[3] = '\0';
	temp.m_id = 7869;
	newTexture->Create(&temp);   //Initialize it with data
	/***************/

	m_textures[newTexture->GetId()] = ResourceContainer(newTexture, 1); //put it into the map
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

Resources::Texture * Resources::TextureHandler::GetPlaceHolderTexture()
{
	if (this->placeHolder == nullptr)
	{
		LoadPlaceHolderTexture();
	}
	return placeHolder;
}

bool Resources::TextureHandler::LoadPlaceHolderTexture()
{

	if (placeHolder != nullptr)
		return false;

#ifdef _DEBUG
	std::cout << "Loading Placeholder Texture" << std::endl;
#endif // _DEBUG

	
	/*T E M P*/
	Resource::RawResourceData temp;
	temp.m_resType = RES_TEXTURE;
	temp.m_name[0] = 'T';
	temp.m_name[1] = 'E';
	temp.m_name[2] = 'X';
	temp.m_name[3] = '\0';
	temp.m_id = 7869;
	/***************/

	placeHolder = new Texture();
	Status st = placeHolder->Create(&temp);
	if (st != ST_OK)
		return false;

	std::string path_S = std::string("PLACEHOLDER_MODEL_ALBEDO.dds");

	size_t length = strlen(path_S.c_str());
	wchar_t path[256];
	mbstowcs_s(&length, path, path_S.c_str(), length);

	ID3D11ShaderResourceView* textureView   = nullptr;
	ID3D11Resource*			textureResource = nullptr;
	HRESULT hr = DirectX::CreateDDSTextureFromFile(m_device,
		path,
		&textureResource,
		&textureView,
		size_t(0),
		(DirectX::DDS_ALPHA_MODE*)DirectX::DDS_ALPHA_MODE_UNKNOWN);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		std::cout << "Could not open texture file : " << path << std::endl;
#endif // _DEBUG
		return false;
	}
	else {
#ifdef _DEBUG
		std::cout << "Opened file : " << path_S << std::endl;
#endif // _DEBUG
	}



	st = placeHolder->SetTexture(textureView, textureResource);
	if (st != ST_OK)
	{
		Resources::SAFE_RELEASE(textureView);
		Resources::SAFE_RELEASE(textureResource);
		return false;
	}

	return true;
}

void Resources::TextureHandler::SetDevice(ID3D11Device * device)
{
	this->m_device = device;
}
