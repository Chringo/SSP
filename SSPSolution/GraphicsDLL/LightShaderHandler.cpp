#include "LightShaderHandler.h"

LightShaderHandler::LightShaderHandler()
{
}


LightShaderHandler::~LightShaderHandler()
{
}

int LightShaderHandler::Initialize(ID3D11Device * device, HWND * windowHandle, DirectX::XMFLOAT2 resolution)
{
	HRESULT hResult;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	//Insert shader path here
	WCHAR* vsFilename = L"../GraphicsDLL/LightVertexShader.hlsl";
	WCHAR* psFilename = L"../GraphicsDLL/LightPixelShader.hlsl";

	// Compile the shaders \\

	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer, NULL);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, NULL);
	if (FAILED(hResult))
	{
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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	/*polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;*/

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
	matrixBufferDesc.ByteWidth = sizeof(ShaderLib::LightConstantBuffer);
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
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler state
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerState);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the screen quad \\

	if (this->screenQuad.Initialize(device, resolution))
	{
		return 1;
	}

	return 0;
}

int LightShaderHandler::SetActive(ID3D11DeviceContext * deviceContext, ShaderLib::ShaderType shaderType)
{
	ShaderHandler::SetActive(deviceContext, shaderType);

	//Set the sampler state in pixel shader
	deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	this->screenQuad.SetBuffers(deviceContext);

	return 0;
}

void LightShaderHandler::Shutdown()
{
	ShaderHandler::Shutdown();

	//Release the sampler state
	if (this->m_samplerState)
	{
		this->m_samplerState->Release();
		this->m_samplerState = nullptr;
	}

	this->screenQuad.Shutdown();
}

int LightShaderHandler::SetShaderParameters(ID3D11DeviceContext * deviceContext, ShaderLib::LightConstantBuffer * shaderParams)
{
	HRESULT hResult;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ShaderLib::LightConstantBuffer* dataPtr;
	unsigned int bufferNumber;

	//Map the constant buffer so we can write to it (denies GPU access)
	hResult = deviceContext->Map(this->m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hResult)) {
		return 1;
	}

	//Get pointer to the data
	dataPtr = (ShaderLib::LightConstantBuffer*)mappedResource.pData;

	//Copy the matrices to the constant buffer
	dataPtr->viewMatrix = shaderParams->viewMatrix;
	dataPtr->projectionMatrix = shaderParams->projectionMatrix;

	dataPtr->resolution = shaderParams->resolution;

	dataPtr->camPos = shaderParams->camPos;

	//Unmap the constant buffer to give the GPU access agin
	deviceContext->Unmap(this->m_matrixBuffer, 0);

	//Set constant buffer position in vertex shader
	bufferNumber = 0;

	//Set the constant buffer in vertex and pixel shader with updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &this->m_matrixBuffer);
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &this->m_matrixBuffer);

	if (shaderParams->gBuffers) {
		//Set shader texture resource for pixel shader
		deviceContext->PSSetShaderResources(0, 4, shaderParams->gBuffers);
	}

	return 0;
}
