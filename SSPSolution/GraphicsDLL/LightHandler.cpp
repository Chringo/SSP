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

	DirectX::XMVECTOR pos = { 2.0f,5.0f,-2.0f,1.0f };
	DirectX::XMVECTOR spak = { 0.0001f,0.0001f, 0.0001f,1.0f };
	DirectX::XMVECTOR up = { 0.0f,1.0f,0.0f,0.0f};
	m_shadowCb.cProjection = DirectX::XMMatrixPerspectiveFovLH((float)DirectX::XM_PI * 0.5, 1.0f, 0.0005f, 9.0f);

	m_constBufferData.SHADOWLIGHT_INDEX = 0;
	Light temp;
	temp.position = pos;
	//SetShadowCastingLight(&temp);

	this->m_activeLightIndex = 0;
	this->m_activeLightCheckTimer = 0.0f;
}

LIGHTING::LightHandler* LIGHTING::LightHandler::GetInstance()
{
	static LightHandler instance;
	return &instance;
}

int LIGHTING::LightHandler::Update(float dT, DirectX::XMFLOAT3 pointOfInterest)
{
	int result = 0;
	this->m_activeLightCheckTimer += dT;
	bool checkActiveLightForShadows = false;
	//Modulus operation
	int k = this->m_activeLightCheckTimer / LIGHT_CHECK_PAUSE_TIME;
	this->m_activeLightCheckTimer -= k * LIGHT_CHECK_PAUSE_TIME;
	checkActiveLightForShadows = k;
#ifndef CHECK_IF_EXITED_LIGHT_RADIUS
	if (checkActiveLightForShadows)
	{
		//The time has exceeded the timer and we need to calculate a new active light for shadow mapping
		int closestLightIndex = 0;
		closestLightIndex = this->GetClosestLightIndex(LIGHTING::LT_POINT, pointOfInterest);
		if (closestLightIndex > -1)
		{
			this->m_activeLightIndex = closestLightIndex;
			//We found a light close enough
			//m_constBufferData.SHADOWLIGHT_INDEX = closestLightIndex;
			Light temp = this->m_lightData[LIGHTING::LT_POINT].dataPtr[this->m_activeLightIndex];
			//SetShadowCastingLight(&temp);
			Light* commonData = this->m_lightData[LIGHTING::LT_POINT].dataPtr;
			Point* specializedData = static_cast<Point*>(commonData);
			
			SetShadowCastingLight(this->m_activeLightIndex);
			//SetShadowCastingLight(0);
		}
	}
#else
	if (checkActiveLightForShadows)
	{
		bool hasExitedOldRadius = false;
		Light temp = this->m_lightData[LIGHTING::LT_POINT].dataPtr[this->m_activeLightIndex];
		float dist = 0.0f;
		Point* specializedData = static_cast<Point*>(&temp);
		dist += pow(specializedData[this->m_activeLightIndex].position.m128_f32[0] - pointOfInterest.x, 2);		//X
		dist += pow(specializedData[this->m_activeLightIndex].position.m128_f32[1] - pointOfInterest.y, 2);		//Y
		dist += pow(specializedData[this->m_activeLightIndex].position.m128_f32[2] - pointOfInterest.z, 2);		//Z
		//Reduce the distance with the radius
		dist -= pow(specializedData[this->m_activeLightIndex].radius, 2);
		hasExitedOldRadius = dist <= 0;
		if (hasExitedOldRadius)
		{
			//The time has exceeded the timer and we need to calculate a new active light for shadow mapping
			int closestLightIndex = 0;
			closestLightIndex = this->GetClosestLightIndex(LIGHTING::LT_POINT, pointOfInterest);
			this->m_activeLightIndex = closestLightIndex;
			if (closestLightIndex > -1)
			{
				//We found a light close enough
				m_constBufferData.SHADOWLIGHT_INDEX = this->m_activeLightIndex;
				temp = this->m_lightData[LIGHTING::LT_POINT].dataPtr[this->m_activeLightIndex];
				SetShadowCastingLight(&temp);
			}
		}
	}
#endif
	return result;
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
	lightBufferDesc.ByteWidth		    = (UINT)structSize * amount; //total size of the buffer
	lightBufferDesc.StructureByteStride = (UINT)structSize;

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

 bool LIGHTING::LightHandler::SetStaticShadowsToGPU()
{


	// m_gDeviceContext->PSSetShaderResources(11, this->m_lightData->numItems, &this->m_lightData->shadowMaps);
	return true;
}

bool LIGHTING::LightHandler::SetBuffersAsActive()
{
	for (size_t i = 0; i < NUM_LT; i++)
	{
		m_gDeviceContext->GSSetShaderResources(BUFFER_SHADER_SLOTS[i], 1, &m_structuredBuffers[i]);
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

		m_lightData[LT_POINT].ReleaseShadowMaps();
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
			((Point*)m_lightData[LT_POINT].dataPtr)[i].padding[0] = 0.0f;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].padding[1] = 0.0f;
			((Point*)m_lightData[LT_POINT].dataPtr)[i].padding[2] = 0.0f;
		}
		SetLightData(m_lightData[LT_POINT].dataPtr, level->numPointLights, LT_POINT);
		UpdateStructuredBuffer(LT_POINT);
	}
	return true;
}

 bool LIGHTING::LightHandler::SetShadowCastingLight(Light * light)
 {
	 //Look along each coordinate axis

	 DirectX::XMVECTOR targets[6];
	 
		targets[0] = DirectX::XMVectorSet(light->position.m128_f32[0] + 1.0f, light->position.m128_f32[1]		 ,light->position.m128_f32[2]		,1.0f); // +X
		targets[1] = DirectX::XMVectorSet(light->position.m128_f32[0] - 1.0f, light->position.m128_f32[1]		 ,light->position.m128_f32[2]		,1.0f); // -X
		targets[2] = DirectX::XMVectorSet(light->position.m128_f32[0],		  light->position.m128_f32[1] + 1.0f ,light->position.m128_f32[2]		,1.0f); // +Y
		targets[3] = DirectX::XMVectorSet(light->position.m128_f32[0],		  light->position.m128_f32[1] - 1.0f ,light->position.m128_f32[2]		,1.0f); // -Y
		targets[4] = DirectX::XMVectorSet(light->position.m128_f32[0],		  light->position.m128_f32[1]		 ,light->position.m128_f32[2] + 1.0f,1.0f); // +Z
		targets[5] = DirectX::XMVectorSet(light->position.m128_f32[0],		  light->position.m128_f32[1]		 ,light->position.m128_f32[2] - 1.0f,1.0f); // -Z

	 

	 /* use world up vector (0,1,0) for all directions except +Y/-Y, In these cases we need a different up vector*/
		DirectX::XMVECTOR ups[6];
	
	 ups[0]= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f ,1.0f  ); // +X
	 ups[1]= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f ,1.0f  ); // -X
	 ups[2]= DirectX::XMVectorSet(0.0f, 0.0f, -1.0f,1.0f  ); // +Y
	 ups[3]= DirectX::XMVectorSet(0.0f, 0.0f, +1.0f,1.0f  ); // -Y
	 ups[4]= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f ,1.0f  ); // +Z
	 ups[5]= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f ,1.0f  ); // -Z

	 for (int i = 0; i < 6; i++)
	 {

		 m_shadowCb.cView[i] = DirectX::XMMatrixLookAtLH(light->position, targets[i], ups[i]);
	 }

	 ConstantBufferHandler::GetInstance()->shadow.UpdateBuffer(&m_shadowCb);
	return  true;
}

 bool LIGHTING::LightHandler::SetShadowCastingLight(int index)
{
	 if (m_lightData[LT_POINT].dataPtr == nullptr || m_lightData[LT_POINT].numItems <= index)
		 return false;

	 m_constBufferData.SHADOWLIGHT_INDEX = index;

	 Light* commonData = this->m_lightData[LIGHTING::LT_POINT].dataPtr;
	 Point* specializedData = static_cast<Point*>(commonData);

	return  SetShadowCastingLight(&specializedData[index]);
}


int LIGHTING::LightHandler::GetClosestLightIndex(LIGHT_TYPE type, DirectX::XMFLOAT3 pos)
{
	int result = -1;
	float distClose = FLT_MAX;
	float dist = 0.0f;
	//Local descriptive constants.
	enum { X = 0, Y = 1, Z = 2 };
	if (type >= 0 && type < NUM_LT)
	{
		if (this->m_lightData->numItems > 0)
		{
			Light* commonData = this->m_lightData[type].dataPtr;
			if (type == LIGHT_TYPE::LT_POINT)
			{

				for (unsigned int i = 0; i < this->m_lightData->numItems; i++)
				{
					Point* specializedData = static_cast<Point*>(commonData);
					dist = 0.0f;
					DirectX::XMVECTOR distanceVec = DirectX::XMVectorSet(specializedData[i].position.m128_f32[X]- pos.x, specializedData[i].position.m128_f32[Y] - pos.y, specializedData[i].position.m128_f32[Z] - pos.z, 0.0f);
					dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVec));
					dist -= specializedData[i].radius;
					//dist += pow(specializedData[i].position.m128_f32[X] - pos.x, 2);		//X
					//dist += pow(specializedData[i].position.m128_f32[Y] - pos.y, 2);		//Y
					//dist += pow(specializedData[i].position.m128_f32[Z] - pos.z, 2);		//Z
					////Reduce the distance with the radius
					//dist -= pow(specializedData[i].radius, 2);
					//Square root it for actual length. We will use the non squared length because
					//we don't care about actual length, only the relation between the lengths
					if (dist < distClose)
					{
						result = i;
						distClose = dist;
					}
				}
			}
			else
			{
				for (unsigned int i = 0; i < this->m_lightData->numItems; i++)
				{
					dist = 0.0f;
					dist += pow(commonData[i].position.m128_f32[X] - pos.x, 2);		//X
					dist += pow(commonData[i].position.m128_f32[Y] - pos.y, 2);		//Y
					dist += pow(commonData[i].position.m128_f32[Z] - pos.z, 2);		//Z
					//Loop the lights
					for (unsigned int i = 0; i < this->m_lightData->numItems; i++)
					{
						dist = 0.0f;
						dist += pow(this->m_lightData[type].dataPtr[i].position.m128_f32[X], 2);		//X
						dist += pow(this->m_lightData[type].dataPtr[i].position.m128_f32[Y], 2);		//Y
						dist += pow(this->m_lightData[type].dataPtr[i].position.m128_f32[Z], 2);		//Z

						//Square root it for actual length. We will use the non squared length because
						//we don't care about actual length, only the relation between the lengths
						if (dist < distClose)
						{
							result = i;
							distClose = dist;
						}
					}
				}
			}
		}
	}
	return result;
}

void LIGHTING::LightHandler::GetClosestLightIndex(LIGHT_TYPE type, DirectX::XMFLOAT3 pos, int &storeIn)
{
	storeIn = this->GetClosestLightIndex(type, pos);
	return;
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
