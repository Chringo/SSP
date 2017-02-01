#include "LightHandler.h"
LIGHT::LightHandler::LightHandler()
{

}

LIGHT::LightHandler::~LightHandler()
{
	ReleaseStructuredBuffer(NUM_LT); //Release all buffers
}

void LIGHT::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_LightVector.reserve(10);

	this->m_gDevice			 = device;
	this->m_gDeviceContext	 = deviceContext;


	for (size_t i = 0; i < NUM_LT; i++)
	{
		this->CreateStructuredBuffer(LIGHT_TYPE(i)); //Create all the structured buffers
	}
}

LIGHT::LightHandler* LIGHT::LightHandler::GetInstance()
{
	static LightHandler instance;
	return &instance;
}

LIGHT::Light* LIGHT::LightHandler::Get_Light(unsigned int id)
{
	for (size_t i = 0; i < this->m_LightVector.size(); i++)
	{
		//if (this->m_LightVector.at(i)->ID = id)
		//{
		//	return this->m_LightVector.at(i);
		//}
		return nullptr;
	}
}

void LIGHT::LightHandler::Add_Light(LIGHT::Light* light)
{
	this->m_LightVector.push_back(light);
}

void LIGHT::LightHandler::Remove_Light(unsigned int id)
{
	for (size_t i = 0; i < this->m_LightVector.size(); i++)
	{
		//if (this->m_LightVector.at(i)->ID = id)
		//{
		//	this->m_LightVector.erase(m_LightVector.begin() + i);
		//}
	}
}

bool LIGHT::LightHandler::CreateStructuredBuffer(LIGHT_TYPE type)
{
	if (lightBuffers[type] != nullptr || m_structuredBuffers[type] != nullptr) //if the buffers are already created, Release them
	{
		ReleaseStructuredBuffer(type);
	}

	size_t structSize = 0;
#pragma region Determine bytewith
	switch (type) {
	case LIGHT_TYPE::LT_POINT:
		structSize = sizeof(Point);
		break;
	case LIGHT_TYPE::LT_DIRECTIONAL:
		structSize = sizeof(Directional);
		break;
	case LIGHT_TYPE::LT_SPOT:
		structSize = sizeof(Spot);
		break;
	case LIGHT_TYPE::LT_AREA:
		structSize = sizeof(Area);
		break;
	default:
		false;
	}
#pragma endregion
	
	HRESULT hr;
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.BindFlags		    = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags	    = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags		    = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth		    = structSize;
	lightBufferDesc.StructureByteStride = structSize;

	if (FAILED(hr = m_gDevice->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffers[type]))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format					 = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			 = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.ElementOffset	 = 0;
	srvDesc.Buffer.ElementWidth		 = structSize;
	srvDesc.Buffer.NumElements	     = MAX_NUM_LIGHTS[type];
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(lightBuffers[type], &srvDesc, &m_structuredBuffers[type]))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG
		return false;
	}
	//this->m_gDeviceContext->GSSetShaderResources(POINTLIGHTS_BUFFER_INDEX, 1, &m_structuredBuffers[LT_POINT]); 
	this->m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOTS[type], 1, &m_structuredBuffers[type]);


	return true;
}

bool LIGHT::LightHandler::ReleaseStructuredBuffer(LIGHT_TYPE type)
{
	if (type == LIGHT_TYPE::NUM_LT) // if type == NUM_LIGHTS, relase all buffers
	{
		for (size_t i = 0; i < NUM_LT; i++)
		{
			lightBuffers[i]->Release();
			lightBuffers[i]		  = nullptr;
			m_structuredBuffers[i]->Release();
			m_structuredBuffers[i] = nullptr;
		}
		return true;
	}
	
	lightBuffers[type]		 ->Release();
	m_structuredBuffers[type]->Release();

	lightBuffers[type]		 = nullptr;
	m_structuredBuffers[type]= nullptr;
	return true;
}
