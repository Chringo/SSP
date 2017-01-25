#include "FinalShader.h"

FinalShader::FinalShader()
{
	this->m_finalRTV = nullptr;


	this->m_vertexShader = nullptr;
	this->m_pixelShader = nullptr;
	this->m_layout = nullptr;
}


FinalShader::~FinalShader()
{
}

int FinalShader::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution)
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

#ifdef _DEBUG
	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer, &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}
#ifdef _DEBUG
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &pixelShaderBuffer, &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}

	// Create the shaders \\

	hResult = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->m_vertexShader);
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



	// Create the sampler
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

int FinalShader::SetActive()
{

	m_deviceContext->OMSetRenderTargets(1, &this->m_finalRTV, NULL);
	//Set the sampler state in pixel shader
	this->m_deviceContext->PSSetSamplers(0, 1, &this->m_samplerStateLinear);
	this->m_deviceContext->PSSetSamplers(1, 1, &this->m_samplerStatePoint);

	if (this->m_gBufferRTVs) {
		//Set shader texture resource for pixel shader
		m_deviceContext->PSSetShaderResources(0, BUFFER_COUNT, m_gBufferRTVs);
	}

	m_deviceContext->IASetInputLayout(this->m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	m_deviceContext->GSSetShader(nullptr, NULL, 0);
	m_deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);

	this->m_screenQuad->SetBuffers(m_deviceContext);

	return 0;
}

int FinalShader::SetRenderParameters(ID3D11RenderTargetView *backBufferRTV, ID3D11ShaderResourceView ** gBuffers)
{
	this->m_finalRTV = backBufferRTV;
	this->m_gBufferRTVs = gBuffers;

	return 0;
}

void FinalShader::Release()
{
	Shader::Release();

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
	if (this->m_vertexShader)
	{
		this->m_vertexShader->Release();
		this->m_vertexShader = nullptr;
	}
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}
	if (this->m_layout)
	{
		this->m_layout->Release();
		this->m_layout = nullptr;
	}
}

int FinalShader::Draw()
{
	this->m_deviceContext->DrawIndexed(6, 0, 0);
	this->ResetPSShaderResources();

	return 0;
}


void FinalShader::ResetPSShaderResources()
{
	m_deviceContext->PSSetShaderResources(0, BUFFER_COUNT, this->m_nullResources);
}
