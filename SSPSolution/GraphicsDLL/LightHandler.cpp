#include "LightHandler.h"
LIGHTING::LightHandler::LightHandler()
{

}

LIGHTING::LightHandler::~LightHandler()
{
	ReleaseStructuredBuffer(NUM_LT); //Release all buffers
	delete[] m_lightData[LIGHT_TYPE::LT_POINT].dataPtr;
}

void LIGHTING::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	
	


	this->m_gDevice			 = device;
	this->m_gDeviceContext	 = deviceContext;

	for (size_t i = 0; i < NUM_LT; i++)
	{
		this->CreateStructuredBuffer(LIGHT_TYPE(i)); //Create all the structured buffers
	}

	m_constBufferData.NUM_POINTLIGHTS		= MAX_POINTLIGHTS;
	m_constBufferData.NUM_AREALIGHTS		= MAX_AREALIGHT;
	m_constBufferData.NUM_SPOTLIGHTS		= MAX_SPOTLIGHT;
	m_constBufferData.NUM_DIRECTIONALLIGHTS = MAX_DIRECTIONAL;
	ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);


	/* TEMPORARY*/
	Point* pointArray = new Point[3];
	pointArray[0].color.r = 1.0f;
	pointArray[0].color.g = 0.0f;
	pointArray[0].color.b = 0.0f;
	pointArray[0].position = { 10.0, -9.0, -3.0 };
	pointArray[0].intensity = 5.0f;
	//pointArray[0].isActive = TRUE;

	pointArray[1].color.r = 0.0f;
	pointArray[1].color.g = 1.0f;
	pointArray[1].color.b = 0.0f;
	pointArray[1].position = { 14.0, -9.0, -3.0 };
	pointArray[1].intensity = 5.0f;
	//pointArray[1].isActive = TRUE;

	pointArray[2].color.r = 0.0f;
	pointArray[2].color.g = 0.0f;
	pointArray[2].color.b = 1.0f;
	pointArray[2].position = { 18.0, -9.0,  -3.0 };
	pointArray[2].intensity = 5.0f;
	//pointArray[2].isActive = TRUE;
	
	this->SetLightData(pointArray, 3, LT_POINT);
	

	this->UpdateStructuredBuffer(LT_POINT);
	//////////////
	
}

LIGHTING::LightHandler* LIGHTING::LightHandler::GetInstance()
{
	static LightHandler instance;
	return &instance;
}


bool LIGHTING::LightHandler::CreateStructuredBuffer(LIGHT_TYPE type)
{
	if (type >= LIGHT_TYPE::NUM_LT)
		return false;

	if (lightBuffers[type] != nullptr || m_structuredBuffers[type] != nullptr) //if the buffers are already created, Release them
	{
		ReleaseStructuredBuffer(type);
	}

	size_t structSize = GetStructByteSize(type);

	
	HRESULT hr;
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.BindFlags		    = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags	    = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags		    = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth		    = structSize * MAX_NUM_LIGHTS[type]; //total size of the buffer
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
	srvDesc.Buffer.NumElements	     = MAX_NUM_LIGHTS[type];
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(lightBuffers[type], &srvDesc, &m_structuredBuffers[type]))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG
		return false;
	}
	this->m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOTS[type], 1, &m_structuredBuffers[type]);

	return true;
}

bool LIGHTING::LightHandler::ReleaseStructuredBuffer(LIGHT_TYPE type)
{
	if (type >= LIGHT_TYPE::NUM_LT) // if type >= NUM_LIGHTS, release all buffers
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

bool LIGHTING::LightHandler::UpdateStructuredBuffer(LIGHT_TYPE type)
{
	if (type >= LIGHT_TYPE::NUM_LT)
	{
		return false;
	}
	if (m_lightData[type].dataPtr == nullptr)
		return false;
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = S_OK;

	hr = m_gDeviceContext->Map(lightBuffers[type], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if (FAILED(hr)) {
#ifdef _DEBUG
		MessageBox(NULL, L"Failed to update lights buffer", L"Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG
		return false;
	}
	memset(mapRes.pData, 0, GetStructByteSize(type) * MAX_NUM_LIGHTS[type]);
	memcpy(mapRes.pData, (void*)m_lightData[type].dataPtr, GetStructByteSize(type)*m_lightData[type].numItems);
	m_gDeviceContext->Unmap(lightBuffers[type], 0);
	m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOTS[type], 1, &m_structuredBuffers[type]);

	return true;
}

bool LIGHTING::LightHandler::SetBuffersAsActive()
{
	for (size_t i = 0; i < NUM_LT; i++)
	{
		m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOTS[i], 1, &m_structuredBuffers[i]);
	}
	return true;
}

bool LIGHTING::LightHandler::SetLightData(Light * lightArray, unsigned int numLights, LIGHT_TYPE type)
{

	if (type >= NUM_LT)
		return false;
	m_lightData[type].dataPtr = lightArray;
	if (numLights > this->MAX_NUM_LIGHTS[type])
	{
		m_lightData[type].numItems = this->MAX_NUM_LIGHTS[type];
#ifdef _DEBUG
		std::cout << "The maximum lightamount has been reached for type :" << type << std::endl;
		std::cout << "Current limit is : " << this->MAX_NUM_LIGHTS[type] << "| Your amount : " << numLights << std::endl;
		std::cout << "Increase the maximum amount of lights available in LightHandler" << std::endl;
#endif // _DEBUG
		return false;
	}
	else {
		m_lightData[type].numItems = numLights;
		return true;
	}
}

bool LIGHTING::LightHandler::UpdateLight( unsigned int index, LIGHT_TYPE type)
{

	if (index >= this->MAX_NUM_LIGHTS[type])
		return false;

	//m_lightData->dataPtr->

	return true;
}

size_t LIGHTING::LightHandler::GetStructByteSize(LIGHT_TYPE type)
{
	size_t structSize = 0;
	assert(type < NUM_LT);
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
	}
	return structSize;
#pragma endregion
}
