#include "LightHandler.h"
LIGHTING::LightHandler::LightHandler()
{

}

LIGHTING::LightHandler::~LightHandler()
{
	ReleaseStructuredBuffer(NUM_LT); //Release all buffers
	delete m_lightData[LIGHT_TYPE::LT_POINT].dataPtr; 
}

void LIGHTING::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_gDevice = device;
	this->m_gDeviceContext = deviceContext;
	for (size_t i = 0; i < NUM_LT; i++)
	{
		this->CreateStructuredBuffer(LIGHT_TYPE(i),3); //Create all the structured buffers  and update the constant buffer
	}

	m_shadowCb.cProjection = DirectX::XMMatrixPerspectiveFovLH(2 * DirectX::XM_PI, 1, 0, 100);
	m_shadowCb.cView = DirectX::XMMatrixIdentity();
	m_shadowCb.cShadowCasterAmount = 3;

	ConstantBufferHandler::GetInstance()->shadow.UpdateBuffer(&m_shadowCb);

///* TEMPORARY*/
//Point* pointArray = new Point[3];
//pointArray[0].color.r = 1.0f;
//pointArray[0].color.g = 0.0f;
//pointArray[0].color.b = 0.0f;
//pointArray[0].position.m128_f32[0] = 10.0f;
//pointArray[0].position.m128_f32[1] = -9.0f;
//pointArray[0].position.m128_f32[2] = -3.0f;
//pointArray[0].intensity = 5.0f;
////pointArray[0].isActive = TRUE;
//
//pointArray[1].color.r = 0.0f;
//pointArray[1].color.g = 1.0f;
//pointArray[1].color.b = 0.0f;
//pointArray[1].position.m128_f32[0] = 14.0f;
//pointArray[1].position.m128_f32[1] = -9.0f;
//pointArray[1].position.m128_f32[2] = -3.0f;
//pointArray[1].intensity = 5.0f;
//pointArray[1].isActive = TRUE;
//
//pointArray[2].color.r = 0.0f;
//pointArray[2].color.g = 0.0f;
//pointArray[2].color.b = 1.0f;
//pointArray[2].position.m128_f32[0] = 18.0f;
//pointArray[2].position.m128_f32[1] = -9.0f;
//pointArray[2].position.m128_f32[2] = -3.0f;
//pointArray[2].intensity = 5.0f;
//pointArray[2].isActive = TRUE;
//
//this->SetLightData(pointArray, 3, LT_POINT);
//
//
//this->UpdateStructuredBuffer(LT_POINT);
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

	if (m_lightBuffers[type] != nullptr || m_structuredBuffers[type] != nullptr) //if the buffers are already created, Release them
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

	if (FAILED(hr = m_gDevice->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffers[type]))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG	return false;
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format					 = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			 = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.ElementOffset	 = 0;
	srvDesc.Buffer.NumElements	     = amount; 
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(m_lightBuffers[type], &srvDesc, &m_structuredBuffers[type]))) {
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
			if (m_lightBuffers[i] != nullptr)
			{
				m_lightBuffers[i]->Release();
				m_lightBuffers[i]		  = nullptr;
			}


			if (m_structuredBuffers[i] != nullptr)
			{
				m_structuredBuffers[i]->Release();
				m_structuredBuffers[i] = nullptr;

			}
		}
		return true;
	}
	if(m_lightBuffers[type] != nullptr)
		m_lightBuffers[type]->Release();
	if (m_structuredBuffers[type] != nullptr)
		m_structuredBuffers[type]->Release();

	m_lightBuffers[type]		  = nullptr;
	m_structuredBuffers[type] = nullptr;

	NUM_LIGHTS[type] = 0;
	return true;
}

bool LIGHTING::LightHandler::UpdateStructuredBuffer(LIGHT_TYPE type)
{
	if (type >= LIGHT_TYPE::NUM_LT || m_lightBuffers[type] == nullptr)
	{
		return false;
	}
	if (m_lightData[type].dataPtr == nullptr)
		return false;
	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = S_OK;
	
	hr = m_gDeviceContext->Map(m_lightBuffers[type], 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if (FAILED(hr)) {
#ifdef _DEBUG
		MessageBox(NULL, L"Failed to update lights buffer", L"Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG	
		return false;
	}
	memset(mapRes.pData, 0, GetStructByteSize(type) * NUM_LIGHTS[type]);
	memcpy(mapRes.pData, (void*)m_lightData[type].dataPtr, GetStructByteSize(type)*NUM_LIGHTS[type]);
	m_gDeviceContext->Unmap(m_lightBuffers[type], 0);
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
	m_lightData[type].numItems = numLights;
	if (numLights > this->NUM_LIGHTS[type] || numLights < this->NUM_LIGHTS[type])
	{
		ReleaseStructuredBuffer(type);
		CreateStructuredBuffer(type, numLights);
		
		return true;
	}
	else {
		NUM_LIGHTS[type] = numLights;
		
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

void LIGHTING::LightHandler::SetAmbientLight(float r, float g, float b, float intensity)
{

	 m_constBufferData.AMBIENT_COLOR[0]  = r;
	 m_constBufferData.AMBIENT_COLOR[1]  = g;
	 m_constBufferData.AMBIENT_COLOR[2]  = b;
	 m_constBufferData.AMBIENT_INTENSITY = intensity;

	 ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);	
}

bool LIGHTING::LightHandler::LoadLevelLight(LevelData::Level * level)
{
	this->m_constBufferData.AMBIENT_COLOR[0] = level->ambientColor[0];
	this->m_constBufferData.AMBIENT_COLOR[1] = level->ambientColor[1];
	this->m_constBufferData.AMBIENT_COLOR[2] = level->ambientColor[2];

	this->m_constBufferData.AMBIENT_INTENSITY = level->ambientIntensity;
	ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);
	if (level->numPointLights > 0)
	{
		if (m_lightData[LT_POINT].dataPtr != nullptr)
			delete m_lightData[LT_POINT].dataPtr;

		m_lightData[LT_POINT].dataPtr = new LIGHTING::Point[level->numPointLights];

		for (size_t i = 0; i < level->numPointLights; i++) //convert from levelType point light to game pointlight
		{
			memcpy(&((Point*)m_lightData[LT_POINT].dataPtr)[i].color, level->pointLights[i].color, sizeof(float) * 3);
			memcpy(((Point*)m_lightData[LT_POINT].dataPtr)[i].position.m128_f32, level->pointLights[i].position, sizeof(float) * 3);

			((Point*)m_lightData[LT_POINT].dataPtr)[i].intensity		 = level->pointLights[i].intensity;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].falloff.quadratic = level->pointLights[i].falloff_quadratic;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].falloff.constant  = level->pointLights[i].falloff_constant;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].falloff.linear	 = level->pointLights[i].falloff_linear;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].radius			 = level->pointLights[i].radius;
		}
		SetLightData(m_lightData[LT_POINT].dataPtr, level->numPointLights, LT_POINT);
		UpdateStructuredBuffer(LT_POINT);
	}
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
