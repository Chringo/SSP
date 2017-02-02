#include "LightHandler.h"
LIGHTING::LightHandler::LightHandler()
{

}

LIGHTING::LightHandler::~LightHandler()
{
	ReleaseStructuredBuffer(NUM_LT); //Release all buffers
	//delete[] m_lightData[LIGHT_TYPE::LT_POINT].dataPtr; //TEMP
}

void LIGHTING::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{




	this->m_gDevice = device;
	this->m_gDeviceContext = deviceContext;

	for (size_t i = 0; i < NUM_LT; i++)
	{
		assert(this->CreateStructuredBuffer(LIGHT_TYPE(i),3) == true); //Create all the structured buffers  and update the constant buffer
	}

	/* TEMPORARY*/
	Point* pointArray = new Point[3];
	pointArray[0].color.r = 1.0f;
	pointArray[0].color.g = 0.0f;
	pointArray[0].color.b = 0.0f;
	pointArray[0].position.m128_f32[0] = 10.0f;
	pointArray[0].position.m128_f32[1] = -9.0f;
	pointArray[0].position.m128_f32[2] = -3.0f;
	pointArray[0].intensity = 5.0f;
	//pointArray[0].isActive = TRUE;

	pointArray[1].color.r = 0.0f;
	pointArray[1].color.g = 1.0f;
	pointArray[1].color.b = 0.0f;
	pointArray[1].position.m128_f32[0] = 14.0f;
	pointArray[1].position.m128_f32[1] = -9.0f;
	pointArray[1].position.m128_f32[2] = -3.0f;
	pointArray[1].intensity = 5.0f;
	pointArray[1].isActive = TRUE;

	pointArray[2].color.r = 0.0f;
	pointArray[2].color.g = 0.0f;
	pointArray[2].color.b = 1.0f;
	pointArray[2].position.m128_f32[0] = 18.0f;
	pointArray[2].position.m128_f32[1] = -9.0f;
	pointArray[2].position.m128_f32[2] = -3.0f;
	pointArray[2].intensity = 5.0f;
	pointArray[2].isActive = TRUE;
	
	this->SetLightData(pointArray, 3, LT_POINT);
	

	this->UpdateStructuredBuffer(LT_POINT);
	//////////////
	
}

LIGHTING::LightHandler* LIGHTING::LightHandler::GetInstance()
{
	static LightHandler instance;
	return &instance;
}


bool LIGHTING::LightHandler::CreateStructuredBuffer(LIGHT_TYPE type, int amount)
{
	if (type >= LIGHT_TYPE::NUM_LT)
		return false;

	if (lightBuffers[type] != nullptr || m_structuredBuffers[type] != nullptr) //if the buffers are already created, Release them
	{
		ReleaseStructuredBuffer(type);
	}

	size_t structSize = GetStructByteSize(type);
	NUM_LIGHTS[type] = amount;
	
	HRESULT hr;
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.BindFlags		    = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags	    = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags		    = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth		    = structSize * amount; //total size of the buffer
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
	srvDesc.Buffer.NumElements	     = amount; 
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(lightBuffers[type], &srvDesc, &m_structuredBuffers[type]))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG
		return false;
	}
	this->m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOTS[type], 1, &m_structuredBuffers[type]);


	switch (type)
	{
	case LIGHT_TYPE::LT_POINT:
		m_constBufferData.NUM_POINTLIGHTS = amount;
		break;
	case LIGHT_TYPE::LT_SPOT:
		m_constBufferData.NUM_SPOTLIGHTS = amount;
		break;
	case LIGHT_TYPE::LT_AREA:
		m_constBufferData.NUM_AREALIGHTS = amount;
		break;
	case LIGHT_TYPE::LT_DIRECTIONAL:
		m_constBufferData.NUM_DIRECTIONALLIGHTS = amount;
		break;
	}
	ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);

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

	lightBuffers[type]		  = nullptr;
	m_structuredBuffers[type] = nullptr;

	NUM_LIGHTS[type] = 0;
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
	memset(mapRes.pData, 0, GetStructByteSize(type) * NUM_LIGHTS[type]);
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

	if (type >= NUM_LT || numLights < 1)
		return false;
	m_lightData[type].dataPtr = lightArray;
	if (numLights > this->NUM_LIGHTS[type] || numLights < this->NUM_LIGHTS[type])
	{
		ReleaseStructuredBuffer(type);
		CreateStructuredBuffer(type, numLights);
		
		return true;
	}
	else {
		m_lightData[type].numItems = numLights;
		switch (type)
		{
		case LIGHT_TYPE::LT_POINT:
			m_constBufferData.NUM_POINTLIGHTS = numLights;
			break;
		case LIGHT_TYPE::LT_SPOT:
			m_constBufferData.NUM_SPOTLIGHTS = numLights;
			break;
		case LIGHT_TYPE::LT_AREA:
			m_constBufferData.NUM_AREALIGHTS = numLights;
			break;
		case LIGHT_TYPE::LT_DIRECTIONAL:
			m_constBufferData.NUM_DIRECTIONALLIGHTS = numLights;
			break;
		}
		ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);
		return true;
	}
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
