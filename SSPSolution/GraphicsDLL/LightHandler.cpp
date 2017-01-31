#include "LightHandler.h"
LIGHT::LightHandler::LightHandler()
{

}

LIGHT::LightHandler::~LightHandler()
{
}

void LIGHT::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_gDevice = nullptr;
	this->m_gDeviceContext = nullptr;
	this->m_LightVector.reserve(10);

	this->m_gDevice			 = device;
	this->m_gDeviceContext	 = deviceContext;



	// POINT LIGHTS
	//////////////////////////////////////////////////////////////////////////////////////////
#pragma region Point lights buffer init
	//////////////////////////////////////////////////////////////////////////////////////////
	//Light buffer for structured buffer
	HRESULT hr;
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.BindFlags	   = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage		   = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags	   = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth	   = sizeof(Point);
	lightBufferDesc.StructureByteStride = sizeof(Point);

	if (FAILED(hr = m_gDevice->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffers[LT_POINT])))
		MessageBox(NULL, L"Failed to create PointLight buffer", L"Error", MB_ICONERROR | MB_OK);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format				    = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.ElementOffset	= 0;
	srvDesc.Buffer.ElementWidth		= sizeof(Point);
	srvDesc.Buffer.NumElements		= MAX_NUM_POINTLIGHTS;
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(lightBuffers[LT_POINT], &srvDesc, &m_structuredBuffers[LT_POINT])))
		MessageBox(NULL, L"Failed to create PointLight buffer", L"Error", MB_ICONERROR | MB_OK);
	//this->m_gDeviceContext->GSSetShaderResources(POINTLIGHTS_BUFFER_INDEX, 1, &m_structuredBuffers[LT_POINT]);
	this->m_gDeviceContext->PSSetShaderResources(LIGHT_BUFFER_SLOTS::POINTLIGHT_BUFFER, 1, &m_structuredBuffers[LT_POINT]);
#pragma endregion


}

LIGHT::LightHandler* LIGHT::LightHandler::getInstance()
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
