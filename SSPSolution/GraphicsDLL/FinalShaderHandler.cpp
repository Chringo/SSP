#include "FinalShaderHandler.h"

FinalShaderHandler::FinalShaderHandler()
{
	this->m_finalRTV = nullptr;
}


FinalShaderHandler::~FinalShaderHandler()
{
}

int FinalShaderHandler::Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution)
{
	HRESULT hResult;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;

	this->m_deviceContext = deviceContext;

	//Insert shader path here
	WCHAR* vsFilename = L"../GraphicsDLL/Shaders/PBR/PbrLightVS.hlsl";
	WCHAR* psFilename = L"../GraphicsDLL/Shaders/PBR/PbrLightPass.hlsl";

	// Compile the shaders \\

	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer, &errorMessage);
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

	hResult = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->m_vertexShader[0]);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(hResult)) {
		return 1;
	}

	// Create the input layout \\

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	//Create the vertex input layout.
	hResult = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &this->m_layout);
	if (FAILED(hResult)) {
		return 1;
	}

	//Release and nullptr the buffers as they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// Create the matrix buffer \\

	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	//Fill the description of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(ShaderLib::CameraConstantBuffer);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hResult = device->CreateBuffer(&matrixBufferDesc, NULL, &this->m_matrixBuffer);
	if (FAILED(hResult)) {
		return 1;
	}

	// Create the sampler \\

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	//Fill the texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
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
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerStatePoint);
	if (FAILED(hResult))
	{
		return 1;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	//Create the texture sampler state
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerStateLinear);
	if (FAILED(hResult))
	{
		return 1;
	}


	// Create the screen quad \\

	this->m_screenQuad = new ScreenQuad();
	if (this->m_screenQuad->Initialize(device, resolution))
	{
		return 1;
	}

	this->m_nullResources = new ID3D11ShaderResourceView*[BUFFER_COUNT];
	for (int i = 0; i < BUFFER_COUNT; i++) {
		m_nullResources[i] = nullptr;
	}

	return 0;
}

int FinalShaderHandler::SetActive(ShaderLib::ShaderType shaderType)
{
	ShaderHandler::SetActive(shaderType);
	m_deviceContext->OMSetRenderTargets(1, &this->m_finalRTV, NULL);
	//Set the sampler state in pixel shader
	this->m_deviceContext->PSSetSamplers(0, 1, &this->m_samplerStateLinear);
	this->m_deviceContext->PSSetSamplers(1, 1, &this->m_samplerStatePoint);

	this->m_screenQuad->SetBuffers(m_deviceContext);

	return 0;
}

int FinalShaderHandler::SetRenderParameters(ID3D11RenderTargetView *backBufferRTV, ID3D11ShaderResourceView ** gBuffers)
{
	this->m_finalRTV = backBufferRTV;
	this->m_gBufferRTVs = gBuffers;

	return 0;
}

void FinalShaderHandler::Shutdown()
{
	ShaderHandler::Shutdown();

	//Release the sampler state
	if (this->m_samplerStatePoint)
	{
		this->m_samplerStatePoint->Release();
		this->m_samplerStatePoint = nullptr;
	}

	if (this->m_samplerStateLinear)
	{
		this->m_samplerStateLinear->Release();
		this->m_samplerStateLinear = nullptr;
	}

	if (this->m_screenQuad)
	{
		this->m_screenQuad->Shutdown();
		delete this->m_screenQuad;
		this->m_screenQuad = nullptr;
	}
	if (this->m_nullResources)
	{
		delete[] this->m_nullResources;
		this->m_nullResources = nullptr;
	}
}

int FinalShaderHandler::Draw()
{
	//delete shaderParamsXM;
	//delete shaderParamsVP;

	//this->m_d3dHandler->ClearDepthAndRTV(this->m_deferredSH->GetDSV());
	//this->m_d3dHandler->SetBackBuffer(this->m_deferredSH->GetDSV());
	//this->m_finalSH->SetActive(ShaderLib::ShaderType::Normal);

	//ShaderLib::CameraConstantBuffer* lShaderParams = new ShaderLib::CameraConstantBuffer;
	//lShaderParams->camPos = this->m_camera->GetCameraPos();
	//lShaderParams->camTar = this->m_camera->GetLookAt();

	//this->SetShaderParameters(lShaderParams, this->m_deferredSH->GetShaderResourceViews());
	//delete lShaderParams;
	//this->m_deviceContext->DrawIndexed(6, 0, 0);

	//this->ResetPSShaderResources();
	this->m_deviceContext->DrawIndexed(6, 0, 0);
	this->ResetPSShaderResources();

	return 0;
}

int FinalShaderHandler::SetShaderParameters(ShaderLib::CameraConstantBuffer * shaderParams)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ShaderLib::CameraConstantBuffer* dataPtr;
	unsigned int bufferNumber;

	//Map the constant buffer so we can write to it (denies GPU access)
	hResult = m_deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult)) {
		return 1;
	}

	//Get pointer to the data
	dataPtr = (ShaderLib::CameraConstantBuffer*)mappedResource.pData;


	dataPtr->camPos = shaderParams->camPos;
	dataPtr->camTar = shaderParams->camTar;

	//Unmap the constant buffer to give the GPU access agin
	m_deviceContext->Unmap(this->m_matrixBuffer, 0);

	//Set constant buffer position in vertex shader
	bufferNumber = 0;

	//Set the constant buffer in vertex and pixel shader with updated values
	m_deviceContext->PSSetConstantBuffers(3, 1, &this->m_matrixBuffer);

	if (this->m_gBufferRTVs) {
		//Set shader texture resource for pixel shader
		m_deviceContext->PSSetShaderResources(0, BUFFER_COUNT, m_gBufferRTVs);
	}

	return 0;
}

void FinalShaderHandler::ResetPSShaderResources()
{
	m_deviceContext->PSSetShaderResources(0, BUFFER_COUNT, this->m_nullResources);
}
