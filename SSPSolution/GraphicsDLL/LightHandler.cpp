#include "LightHandler.h"
LIGHTING::LightHandler::LightHandler()
{

}

LIGHTING::LightHandler::~LightHandler()
{
	ReleaseStructuredBuffer(); //Release all buffers
	if(m_lightData.dataPtr != nullptr)
		delete m_lightData.dataPtr; 
}

void LIGHTING::LightHandler::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->m_gDevice = device;
	this->m_gDeviceContext = deviceContext;
	
	this->CreateStructuredBuffer(3); //Create the structured buffer  and update the constant buffer
	

	DirectX::XMVECTOR pos  = { 2.0f,5.0f,-2.0f,1.0f };
	DirectX::XMVECTOR spak = { 0.0001f,0.0001f, 0.0001f,1.0f };
	DirectX::XMVECTOR up   = { 0.0f,1.0f,0.0f,0.0f};
	m_shadowCb.cProjection = DirectX::XMMatrixPerspectiveFovLH((float)DirectX::XM_PI * 0.5, 1.0f, 0.0005f, 9.0f);
	
	m_constBufferData.DYNAMIC_SHADOWLIGHT_INDEX = 0;
	for (size_t i = 0; i < MAX_SHADOW_LIGHTS; i++)
	{
		m_constBufferData.SHADOWCASTING_LIGHTS[i].index = -1;
	}
	m_lightData.currentDynamicShadowIndex = -1;
	Light temp;
	temp.position = pos;
	SetShadowCastingLight(-1);

	this->m_activeLightIndex	  = 0;
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
		closestLightIndex = this->GetClosestLightIndex(pointOfInterest);

		if (closestLightIndex > -1)
		{
			this->m_activeLightIndex = closestLightIndex;
			//We found a light close enough
			//m_constBufferData.SHADOWLIGHT_INDEX = closestLightIndex;
	
			Point* specializedData = this->m_lightData.dataPtr;
			
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
		DirectX::XMVECTOR distanceVec = DirectX::XMVectorSet(specializedData[this->m_activeLightIndex].position.m128_f32[0] - pointOfInterest.x, specializedData[this->m_activeLightIndex].position.m128_f32[1] - pointOfInterest.y, specializedData[this->m_activeLightIndex].position.m128_f32[2] - pointOfInterest.z, 0.0f);
		dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVec));
		//Reduce the distance with the radius
		dist -= specializedData[this->m_activeLightIndex].radius;
		hasExitedOldRadius = dist >= 0;
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


bool LIGHTING::LightHandler::CreateStructuredBuffer(int amount)
{
	

	if (m_lightBuffer != nullptr || m_structuredBuffer != nullptr) //if the buffer is already created, Release it
	{
		ReleaseStructuredBuffer();
	}

	size_t structSize = sizeof(Point);
	NUM_LIGHTS = amount;
	
	HRESULT hr;
	D3D11_BUFFER_DESC lightBufferDesc;
#ifndef USE_CONST_BUFFER_FOR_LIGHTS
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.BindFlags		    = D3D11_BIND_SHADER_RESOURCE;
	lightBufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags	    = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags		    = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	lightBufferDesc.ByteWidth		    = (UINT)structSize * amount; //total size of the buffer
	lightBufferDesc.StructureByteStride = (UINT)structSize;

	if (FAILED(hr = m_gDevice->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG	return false;
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format					 = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension			 = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.ElementOffset	 = 0;
	srvDesc.Buffer.NumElements	     = amount; 
	if (FAILED(hr = m_gDevice->CreateShaderResourceView(m_lightBuffer, &srvDesc, &m_structuredBuffer))) {
#ifdef  _DEBUG
		MessageBox(NULL, L"Failed to create light buffer", L"Error", MB_ICONERROR | MB_OK);
#endif //  _DEBUG
		return false;
	}
	this->m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOT, 1, &m_structuredBuffer);

#else
	ZeroMemory(&lightBufferDesc, sizeof(lightBufferDesc));
	lightBufferDesc.ByteWidth           = sizeof(Point) * MAX_LIGHT_AMOUNT;
	lightBufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.Usage		        = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags	        = 0;
	lightBufferDesc.StructureByteStride = 0;

	hr = m_gDevice->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
	if (SUCCEEDED(hr))
	{
		this->m_gDeviceContext->PSSetConstantBuffers(6, 1, &m_lightBuffer);
		this->m_gDeviceContext->GSSetConstantBuffers(6, 1, &m_lightBuffer);
	}
	else
		return 1;
#endif

	m_constBufferData.NUM_POINTLIGHTS = amount;

	ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);

	return true;
}

bool LIGHTING::LightHandler::ReleaseStructuredBuffer()
{

	if(m_lightBuffer != nullptr)
		m_lightBuffer->Release();
	if (m_structuredBuffer != nullptr)
		m_structuredBuffer->Release();

	m_lightBuffer		  = nullptr;
	m_structuredBuffer    = nullptr;

	NUM_LIGHTS = 0;
	return true;
}


bool LIGHTING::LightHandler::UpdateStructuredBuffer()
{
	if (m_lightBuffer == nullptr)
		return false;
	if (m_lightData.dataPtr == nullptr)
		return false;



	D3D11_MAPPED_SUBRESOURCE mapRes;
	HRESULT hr = S_OK;
	
	hr = m_gDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	if (FAILED(hr)) {
#ifdef _DEBUG
		MessageBox(NULL, L"Failed to update lights buffer", L"Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG	
		return false;
	}
	memset(mapRes.pData, 0, sizeof(Point) * NUM_LIGHTS);
	memcpy(mapRes.pData, (void*)m_lightData.dataPtr, sizeof(Point) * NUM_LIGHTS);
	m_gDeviceContext->Unmap(m_lightBuffer, 0);
	m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOT, 1, &m_structuredBuffer);
	return true;
}

 bool LIGHTING::LightHandler::SetStaticShadowsToGPU()
{
	 m_gDeviceContext->PSSetShaderResources(11, 1, &this->m_lightData.shadowMaps);
	return true;
}

bool LIGHTING::LightHandler::SetBufferAsActive()
{
#ifdef  USE_CONST_BUFFER_FOR_LIGHTS
		this->m_gDeviceContext->PSSetConstantBuffers(6, 1, &m_lightBuffer);
		this->m_gDeviceContext->GSSetConstantBuffers(6, 1, &m_lightBuffer);
#else
		m_gDeviceContext->GSSetShaderResources(BUFFER_SHADER_SLOT, 1, &m_structuredBuffer);
		m_gDeviceContext->PSSetShaderResources(BUFFER_SHADER_SLOT, 1, &m_structuredBuffer);

#endif // !1
	
	return true;
}

bool LIGHTING::LightHandler::SetLightData(Point * lightArray, unsigned int numLights)
{

	if ( numLights < 1)
		return false;
	m_lightData.dataPtr  = lightArray;
	m_lightData.numItems = numLights;
	NUM_LIGHTS			 = numLights;
	//if (numLights != this->NUM_LIGHTS)
	//{
	//	ReleaseStructuredBuffer();
	//	CreateStructuredBuffer(numLights);
	//	
	//	return true;
	//}
	//else {
		
		m_constBufferData.NUM_POINTLIGHTS = numLights;

		ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);
		return true;
//	}
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
		if (m_lightData.dataPtr != nullptr) {
			delete m_lightData.dataPtr;
			m_lightData.dataPtr = nullptr;
		}

		m_lightData.ReleaseShadowMaps();
		m_lightData.dataPtr = new LIGHTING::Point[level->numPointLights];
	
		m_lightData.numShadowLights = 0;
		memcpy(m_lightData.shadowLightIndex, level->shadowCastIndexes, sizeof(int) * MAX_SHADOW_LIGHTS);

		for (size_t i = 0; i < MAX_SHADOW_LIGHTS; i++)
		{

			m_constBufferData.SHADOWCASTING_LIGHTS[i].index = level->shadowCastIndexes[i];
		}
		
		for (size_t i = 0; i < MAX_SHADOW_LIGHTS; i++)
		{
			if (m_lightData.shadowLightIndex[i] != -1)
				m_lightData.numShadowLights += 1;
		}

		for (size_t i = 0; i < level->numPointLights; i++) //convert from levelType point light to game pointlight
		{
			memcpy(&((Point*)m_lightData.dataPtr)[i].color, level->pointLights[i].color, sizeof(float) * 3);
			memcpy(((Point*)m_lightData.dataPtr)[i].position.m128_f32, level->pointLights[i].position, sizeof(float) * 3);
			m_lightData.dataPtr[i].intensity		   = level->pointLights[i].intensity;
			m_lightData.dataPtr[i].falloff.quadratic   = level->pointLights[i].falloff_quadratic;
			m_lightData.dataPtr[i].falloff.constant    = level->pointLights[i].falloff_constant;
			m_lightData.dataPtr[i].falloff.linear	   = level->pointLights[i].falloff_linear;
			m_lightData.dataPtr[i].radius			   = level->pointLights[i].radius;
			m_lightData.dataPtr[i].padding[0]		   = FALSE;
			m_lightData.dataPtr[i].padding[1]		   = FALSE;
			m_lightData.dataPtr[i].padding[2]		   = FALSE;
		}
		SetLightData(m_lightData.dataPtr, level->numPointLights);
#ifndef USE_CONST_BUFFER_FOR_LIGHTS
		UpdateStructuredBuffer();
#endif // ! USE_CONST_BUFFER_FOR_LIGHTS

	}
	return true;
}

 bool LIGHTING::LightHandler::SetShadowCastingLight(Point * light)
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
	 if (m_lightData.dataPtr == nullptr || m_lightData.numItems <= index || m_lightData.currentDynamicShadowIndex == index)
		 return false;
	
	 m_lightData.currentDynamicShadowIndex = index;
	// m_constBufferData.DYNAMIC_SHADOWLIGHT_INDEX = index;
	//
	// ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData);
	 Point* specializedData = this->m_lightData.dataPtr;

	return  SetShadowCastingLight(&specializedData[index]);
}

  bool LIGHTING::LightHandler::UpdateActiveLightsToGPU(std::vector<int>* indices)
 {
#pragma region 
	  /*
	  make an array of Point[indices.size()]
	  make a vector of ints for the new shadow indices. No set size. We don't know how many of the lights will cast shadows
	  for each item in indices
		{	
			copy the light at m_lightData.dataPtr[item] 

			for each shadowIndex in m_lightData.shadowLightIndex
				
				Check if this light is a light that casts shadows
				if( item == shadowIndex)
					pushback the shadowvector with the current index in the new Point array
	  
	  }

	  Update lightBuffer with the lights
	  Update LightInfo buffer with numLights and shadowcasting lights
	  */
#pragma endregion PSUEDO

#pragma region
	  if (m_lightData.dataPtr == nullptr)
		  return false;

	  static std::vector<Point> lightArray;
	  static std::vector<ConstantBufferHandler::ConstantBuffer::light::arrayIndex>	shadowIndices;
	if(m_lightData.currentDynamicShadowIndex != -1)
		 lightArray.push_back(m_lightData.dataPtr[m_lightData.currentDynamicShadowIndex]);			  // Make sure that the dynamic shadow casting light is in the buffer
	  
	  ConstantBufferHandler::ConstantBuffer::light::arrayIndex item;
	  item.index = 0;
	  //item.shadowMapIndex
		  for (int i = 0; i < (int)m_lightData.numShadowLights; i++) //detta är pinsamt
		  {
			  if (m_lightData.shadowLightIndex[i] == m_lightData.currentDynamicShadowIndex) {
				  item.shadowMapIndex = i;
			  }
			  else if (m_lightData.shadowLightIndex[i] == -1)
				  break;
		  }
		  if (m_lightData.currentDynamicShadowIndex != -1) {

			 shadowIndices.push_back(item); // Make sure that the dynamic shadow casting light is in the buffer
			 m_constBufferData.DYNAMIC_SHADOWLIGHT_INDEX = 0;
	}

	  for (int i = 0; i < (int)indices->size(); i++) //for each index sent into this function
	  {
		  if (indices->at(i) == m_lightData.currentDynamicShadowIndex) { // if the light is casting dynamic shadow, then it has already been added
			  continue;
		  }

		  lightArray.push_back(m_lightData.dataPtr[indices->at(i)]); // pushback the light data

		  for (int shadowIndex = 0; shadowIndex < (int)m_lightData.numShadowLights; shadowIndex++) //go through the shadow array
		  {
			  if (indices->at(i) == m_lightData.shadowLightIndex[shadowIndex]) {	// if this light index resides in the shdaowIndex array
				  item.index = lightArray.size() - 1;
				  item.shadowMapIndex = shadowIndex;
				  shadowIndices.push_back(item);					// pushback the current index in the new Lightarray (The new index in the const buffer)
				  break;
			  }
			  else if (m_lightData.shadowLightIndex[shadowIndex] == -1) //if we've reached a -1 value in the array, we've reached the end of available shadows indices
				  break;
		  }
	  }

	 D3D11_MAPPED_SUBRESOURCE mapRes;
	 HRESULT hr = m_gDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	  if (FAILED(hr)) {
#ifdef _DEBUG
		  MessageBox(NULL, L"Failed to update lights buffer", L"Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG	
		  return false;
  }
#ifdef USE_CONST_BUFFER_FOR_LIGHTS
	  memset(mapRes.pData, 0, sizeof(Point) * MAX_LIGHT_AMOUNT); //Null the data in the buffer
#else
	  memset(mapRes.pData, 0, sizeof(Point) * NUM_LIGHTS);
#endif
	  memcpy(mapRes.pData, (void*)lightArray.data(), sizeof(Point) * lightArray.size()); //copy the lights
	  m_gDeviceContext->Unmap(m_lightBuffer, 0);

	  m_constBufferData.NUM_POINTLIGHTS = lightArray.size();	//Update Num pointLights

	  //Update the shadowIndices array
	  memset(m_constBufferData.SHADOWCASTING_LIGHTS, -1, sizeof(ConstantBufferHandler::ConstantBuffer::light::arrayIndex) * MAX_SHADOW_LIGHTS); //Null the data in the buffer
	  memcpy(m_constBufferData.SHADOWCASTING_LIGHTS, (void*)shadowIndices.data(), sizeof(ConstantBufferHandler::ConstantBuffer::light::arrayIndex)*  shadowIndices.size()); //copy the shadow indices
	
	  ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData); // update the lightInfo buffer


	  //update structured buffer

	  lightArray.clear();
	  shadowIndices.clear();
#pragma endregion Implementation
	  return true;
 }

bool LIGHTING::LightHandler::UpdateActiveLightsToGPUeditor(std::vector<int>* indices)
  {
#pragma region 
	  /*
	  make an array of Point[indices.size()]
	  make a vector of ints for the new shadow indices. No set size. We don't know how many of the lights will cast shadows
	  for each item in indices
	  {
	  copy the light at m_lightData.dataPtr[item]

	  for each shadowIndex in m_lightData.shadowLightIndex

	  Check if this light is a light that casts shadows
	  if( item == shadowIndex)
	  pushback the shadowvector with the current index in the new Point array

	  }

	  Update lightBuffer with the lights
	  Update LightInfo buffer with numLights and shadowcasting lights
	  */
#pragma endregion PSUEDO

#pragma region
	  if (m_lightData.dataPtr == nullptr)
		  return false;

	  static std::vector<Point> lightArray;
	  static std::vector<ConstantBufferHandler::ConstantBuffer::light::arrayIndex>	shadowIndices;
	 
	 
	  m_lightData.currentDynamicShadowIndex = 1337;

	  ConstantBufferHandler::ConstantBuffer::light::arrayIndex item;
	 // for (size_t i = 0; i < m_lightData.numShadowLights; i++)
	 // {
	//	  item.index = m_lightData.shadowLightIndex[i];
	//	  shadowIndices.push_back(item);
	 // };
	  

	  //for (int i = 0; i < (int)m_lightData.numShadowLights; i++) //detta är pinsamt
	  //{
		 // if (m_lightData.shadowLightIndex[i] == m_lightData.currentDynamicShadowIndex) {
			//  item.shadowMapIndex = i;
		 // }
		 // else if (m_lightData.shadowLightIndex[i] == -1)
			//  break;
	  //}
	 
	  m_constBufferData.DYNAMIC_SHADOWLIGHT_INDEX = 1337;

	  for (int i = 0; i < (int)indices->size(); i++) //for each index sent into this function
	  {
		 
		  lightArray.push_back(m_lightData.dataPtr[indices->at(i)]); // pushback the light data

		  for (int shadowIndex = 0; shadowIndex < (int)m_lightData.numShadowLights; shadowIndex++) //go through the shadow array
		  {
			  if (indices->at(i) == m_lightData.shadowLightIndex[shadowIndex]) {	// if this light index resides in the shdaowIndex array
				  item.index = lightArray.size() - 1;
				  item.shadowMapIndex = shadowIndex;
				  shadowIndices.push_back(item);					// pushback the current index in the new Lightarray (The new index in the const buffer)
				  break;
			  }
			  else if (m_lightData.shadowLightIndex[shadowIndex] == -1) //if we've reached a -1 value in the array, we've reached the end of available shadows indices
				  break;
		  }
	  }

	  D3D11_MAPPED_SUBRESOURCE mapRes;
	  HRESULT hr = m_gDeviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapRes);
	  if (FAILED(hr)) {
#ifdef _DEBUG
		  MessageBox(NULL, L"Failed to update lights buffer", L"Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG	
		  return false;
	  }
#ifdef USE_CONST_BUFFER_FOR_LIGHTS
	  memset(mapRes.pData, 0, sizeof(Point) * MAX_LIGHT_AMOUNT); //Null the data in the buffer
#else
	  memset(mapRes.pData, 0, sizeof(Point) * NUM_LIGHTS);
#endif
	  memcpy(mapRes.pData, (void*)lightArray.data(), sizeof(Point) * lightArray.size()); //copy the lights
	  m_gDeviceContext->Unmap(m_lightBuffer, 0);

	  m_constBufferData.NUM_POINTLIGHTS = lightArray.size();	//Update Num pointLights

																//Update the shadowIndices array
	  memset(m_constBufferData.SHADOWCASTING_LIGHTS, -1, sizeof(ConstantBufferHandler::ConstantBuffer::light::arrayIndex) * MAX_SHADOW_LIGHTS); //Null the data in the buffer
	  memcpy(m_constBufferData.SHADOWCASTING_LIGHTS, (void*)shadowIndices.data(), sizeof(ConstantBufferHandler::ConstantBuffer::light::arrayIndex)*  shadowIndices.size()); //copy the shadow indices

	  ConstantBufferHandler::GetInstance()->light.UpdateBuffer(&m_constBufferData); // update the lightInfo buffer


																					//update structured buffer

	  lightArray.clear();
	  shadowIndices.clear();
#pragma endregion Implementation
	  return true;
  }

  bool LIGHTING::LightHandler::SetShadowLightIndexList(std::vector<int>* indices)
  {
	  m_lightData.numShadowLights = indices->size();

	  for (int i = 0; i < m_lightData.numShadowLights; i++)
	  {
		 m_lightData.shadowLightIndex[i] = indices->at(i);
	  }


	  return true;
  }


int LIGHTING::LightHandler::GetClosestLightIndex(DirectX::XMFLOAT3 pos)
{
	int result = -1;
	float distClose = FLT_MAX;
	float dist = 0.0f;
	//Local descriptive constants.
	enum { X = 0, Y = 1, Z = 2 };

	if (this->m_lightData.numShadowLights > 0)
	{
		Point* commonData = this->m_lightData.dataPtr;

			for (unsigned int i = 0; i < this->m_lightData.numShadowLights; i++)
			{
				int lightIndex = m_lightData.shadowLightIndex[i];
						Point* specializedData = commonData;
						dist = 0.0f;
						DirectX::XMVECTOR distanceVec = DirectX::XMVectorSet(specializedData[lightIndex].position.m128_f32[X]- pos.x, specializedData[lightIndex].position.m128_f32[Y] - pos.y, specializedData[lightIndex].position.m128_f32[Z] - pos.z, 0.0f);
						dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVec));
						dist -= specializedData[lightIndex].radius;
						//dist += pow(specializedData[i].position.m128_f32[X] - pos.x, 2);		//X
						//dist += pow(specializedData[i].position.m128_f32[Y] - pos.y, 2);		//Y
						//dist += pow(specializedData[i].position.m128_f32[Z] - pos.z, 2);		//Z
						////Reduce the distance with the radius
						//dist -= pow(specializedData[i].radius, 2);
						//Square root it for actual length. We will use the non squared length because
						//we don't care about actual length, only the relation between the lengths
						if (dist < distClose)
						{
							result = lightIndex;
							distClose = dist;
						}			
			}
	}
	
	return result;
}

void LIGHTING::LightHandler::GetClosestLightIndex(DirectX::XMFLOAT3 pos, int &storeIn)
{
	storeIn = this->GetClosestLightIndex(pos);
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
