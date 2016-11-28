#include "DeferredShaderHandler.h"

DeferredShaderHandler::DeferredShaderHandler() : ShaderHandler()
{
	this->m_samplerState = nullptr;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->m_deferredRenderTargetTextures[i] = nullptr;
		this->m_deferredRenderTargetViews[i] = nullptr;
		this->m_deferredShaderResources[i] = nullptr;
	}

	this->m_depthStencilView = nullptr;
}


DeferredShaderHandler::~DeferredShaderHandler()
{
}

int DeferredShaderHandler::Initialize(ID3D11Device * device, HWND * windowHandle, const DirectX::XMINT2& resolution)
{
	HRESULT hResult;
	ID3D10Blob* vertexShaderBuffer[4] = { nullptr };
	ID3D10Blob* geoShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;

	//Insert shader path here
	WCHAR* vsFilename = L"../GraphicsDLL/Shaders/GBuffer/GBufferVS.hlsl";
	WCHAR* gsFilename = L"../GraphicsDLL/Shaders/GBuffer/GBuffer.hlsl";
	WCHAR* psFilename = L"../GraphicsDLL/Shaders/GBuffer/GBuffer.hlsl";

	// Compile the shaders \\

	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[0], &errorMessage);
	if (FAILED(hResult)) 
	{
		ShaderHandler::OutputShaderErrorMessage(errorMessage, vsFilename);

		return 1;
	}
	hResult = D3DCompileFromFile(gsFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_DEBUG, 0, &geoShaderBuffer, &errorMessage);
	if (FAILED(hResult)) 
	{
		ShaderHandler::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(hResult)) 
	{
		ShaderHandler::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}

	// Create the shaders \\

	hResult = device->CreateVertexShader(vertexShaderBuffer[0]->GetBufferPointer(), vertexShaderBuffer[0]->GetBufferSize(), NULL, &this->m_vertexShader[0]);
	if (FAILED(hResult)) 
	{
		return 1;
	}
	hResult = device->CreateGeometryShader(geoShaderBuffer->GetBufferPointer(), geoShaderBuffer->GetBufferSize(), NULL, &this->m_geoShader);
	if (FAILED(hResult)) {
		return 1;
	}
	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(hResult)) {
		return 1;
	}

	// Create the input layout \\

	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TANGENT";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TEXCOORD";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;


	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//Create the vertex input layout.
	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer[0]->GetBufferPointer(), vertexShaderBuffer[0]->GetBufferSize(), &this->m_layout);
	if (FAILED(hResult)) {
		return 1;
	}

	//Release and nullptr the buffers as they are no longer needed
	vertexShaderBuffer[0]->Release();
	vertexShaderBuffer[0] = nullptr;
	geoShaderBuffer->Release();
	geoShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Create the matrix buffers \\

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	//Fill the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ShaderLib::DeferredConstantBufferWorld);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &this->m_worldMatrixBuffer);
	if (FAILED(hResult)) {
		return 1;
	}

	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	//Fill the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ShaderLib::DeferredConstantBufferVP);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &this->m_viewProjMatrixBuffer);
	if (FAILED(hResult)) {
		return 1;
	}

	// Create the sampler \\

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	//Fill the texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samplerDesc.BorderColor[0] = 0;
	//samplerDesc.BorderColor[1] = 0;
	//samplerDesc.BorderColor[2] = 0;
	//samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler state
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerState);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the render target and shader resource views \\

	D3D11_TEXTURE2D_DESC renderTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	//Initialize the render target texture description
	ZeroMemory(&renderTextureDesc, sizeof(renderTextureDesc));

	//Setup the render target texture description
	renderTextureDesc.Width = resolution.x;
	renderTextureDesc.Height = resolution.y;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	renderTextureDesc.SampleDesc.Count = 1; //No MSAA
	renderTextureDesc.SampleDesc.Quality = 0;

	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTextureDesc.CPUAccessFlags = 0;
	renderTextureDesc.MiscFlags = 0;

	//Create the render target textures
	for (int i = 0; i < BUFFER_COUNT; i++) {
		hResult = device->CreateTexture2D(&renderTextureDesc, NULL, &this->m_deferredRenderTargetTextures[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	//Setup the description of the render target views
	renderTargetViewDesc.Format = renderTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	//Create the render target views
	for (int i = 0; i<BUFFER_COUNT; i++) {
		hResult = device->CreateRenderTargetView(this->m_deferredRenderTargetTextures[i], &renderTargetViewDesc, &this->m_deferredRenderTargetViews[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	//Setup the description of the shader resource view
	shaderResourceViewDesc.Format = renderTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	//Create the shader resource views
	for (int i = 0; i < BUFFER_COUNT; i++) {
		hResult = device->CreateShaderResourceView(this->m_deferredRenderTargetTextures[i], &shaderResourceViewDesc, &this->m_deferredShaderResources[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	// Create the depth buffer and view \\

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = resolution.x;
	depthBufferDesc.Height = resolution.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.SampleDesc.Count = 1; //No MSAA
	depthBufferDesc.SampleDesc.Quality = 0;

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hResult = device->CreateTexture2D(&depthBufferDesc, NULL, &this->m_depthStencilBuffer);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = device->CreateDepthStencilView(this->m_depthStencilBuffer, NULL, &this->m_depthStencilView);
	if (FAILED(hResult))
	{
		return 1;
	}

	return 0;
}

int DeferredShaderHandler::SetActive(ID3D11DeviceContext * deviceContext, ShaderLib::ShaderType shaderType)
{
	ShaderHandler::SetActive(deviceContext, shaderType);

	//Set the sampler state in pixel shader
	deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	//Set the render target views
	deviceContext->OMSetRenderTargets(BUFFER_COUNT, this->m_deferredRenderTargetViews, this->m_depthStencilView);

	return 0;
}

void DeferredShaderHandler::Shutdown()
{
	ShaderHandler::Shutdown();

	//Release the sampler state
	if (this->m_samplerState)
	{
		this->m_samplerState->Release();
		this->m_samplerState = nullptr;
	}
	//Release the deferred render targets
	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (this->m_deferredRenderTargetTextures[i]) {
			this->m_deferredRenderTargetTextures[i]->Release();
			this->m_deferredRenderTargetTextures[i] = nullptr;
		}
		if (this->m_deferredRenderTargetViews[i]) {
			this->m_deferredRenderTargetViews[i]->Release();
			this->m_deferredRenderTargetViews[i] = nullptr;
		}
		if (this->m_deferredShaderResources[i]) {
			this->m_deferredShaderResources[i]->Release();
			this->m_deferredShaderResources[i] = nullptr;
		}
	}
}

int DeferredShaderHandler::SetShaderParameters(ID3D11DeviceContext * deviceContext, void* shaderParams, ShaderLib::CBuffer type)
{

	



	switch (type)
	{
	case ShaderLib::WORLD:
	{
		this->BindWorldCbuffer(deviceContext, (ShaderLib::DeferredConstantBufferWorld *)shaderParams);
		break;
	}
	case ShaderLib::VIEW_PROJECTION:
	{
		this->BindViewProjectionCbuffer(deviceContext, (ShaderLib::DeferredConstantBufferVP *)shaderParams);
		break;
	}
	default:
		break;
	}



	return 0;
}

int DeferredShaderHandler::BindWorldCbuffer(ID3D11DeviceContext * deviceContext, ShaderLib::DeferredConstantBufferWorld * world)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ShaderLib::DeferredConstantBufferWorld * dataPtr;

	DirectX::XMMATRIX transposedWorld = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&world->worldMatrix));
	DirectX::XMStoreFloat4x4(&world->worldMatrix, transposedWorld);

	//Map the constant buffer so we can write to it (denies GPU access)
	hResult = deviceContext->Map(this->m_worldMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult)) {
		return 1;
	}

	//Get pointer to the data
	dataPtr = (ShaderLib::DeferredConstantBufferWorld *)mappedResource.pData;
	//Copy the matrices to the constant buffer
	dataPtr->worldMatrix = world->worldMatrix;

	//Unmap the constant buffer to give the GPU access agin
	deviceContext->Unmap(this->m_worldMatrixBuffer, 0);


	//Set the constant buffer in vertex and pixel shader with updated values
	deviceContext->VSSetConstantBuffers(0, 1, &this->m_worldMatrixBuffer);
	deviceContext->GSSetConstantBuffers(0, 1, &this->m_worldMatrixBuffer);

	return 0;
}

int DeferredShaderHandler::BindViewProjectionCbuffer(ID3D11DeviceContext * deviceContext, ShaderLib::DeferredConstantBufferVP * viewProjection)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ShaderLib::DeferredConstantBufferVP * dataPtr;

	DirectX::XMMATRIX transposedView = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&viewProjection->viewMatrix));
	DirectX::XMStoreFloat4x4(&viewProjection->viewMatrix, transposedView);
	DirectX::XMMATRIX transposedProjection = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&viewProjection->projectionMatrix));
	DirectX::XMStoreFloat4x4(&viewProjection->projectionMatrix, transposedProjection);

	//Map the constant buffer so we can write to it (denies GPU access)
	hResult = deviceContext->Map(this->m_viewProjMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult)) {
		return 1;
	}

	//Get pointer to the data
	dataPtr = (ShaderLib::DeferredConstantBufferVP *)mappedResource.pData;

	//Copy the matrices to the constant buffer
	dataPtr->viewMatrix = viewProjection->viewMatrix;
	dataPtr->projectionMatrix = viewProjection->projectionMatrix;
	//Unmap the constant buffer to give the GPU access agin
	deviceContext->Unmap(this->m_viewProjMatrixBuffer, 0);

	//Set the constant buffer in vertex and pixel shader with updated values
	deviceContext->VSSetConstantBuffers(1, 1, &this->m_viewProjMatrixBuffer);


	return 0;
}

int DeferredShaderHandler::ClearRenderTargetViews(ID3D11DeviceContext * deviceContext)
{
	float color[4];

	color[0] = 0.0f;
	color[1] = 1.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	//Clear the render target textures
	for (int i = 0; i < BUFFER_COUNT; i++) {
		deviceContext->ClearRenderTargetView(this->m_deferredRenderTargetViews[i], color);
	}

	//Clear the depth buffer
	deviceContext->ClearDepthStencilView(this->m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return 0;
}

ID3D11ShaderResourceView ** DeferredShaderHandler::GetShaderResourceViews()
{
	return this->m_deferredShaderResources;
}
