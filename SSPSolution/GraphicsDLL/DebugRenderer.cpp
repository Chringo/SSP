#include "DebugRenderer.h"



DebugRenderer::DebugRenderer()
{
}


DebugRenderer::~DebugRenderer()
{
}

int DebugRenderer::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const DirectX::XMINT2 & resolution)
{
	HRESULT hResult;
	WCHAR* filePathVS = L"../GraphicsDLL/Shaders/Debug/DebugVS.hlsl";
	WCHAR* filePathPS = L"../GraphicsDLL/Shaders/Debug/DebugPS.hlsl";;
	;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertShaderBuffer  = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	this->m_device				  = device;
	this->m_deviceContext		  = deviceContext;
	// Compile the shaders \\

	hResult = D3DCompileFromFile(filePathVS, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertShaderBuffer, &errorMessage);
	if (FAILED(hResult))
	{
		return 1;
	}


	this->m_pixelShader = nullptr;
	hResult = D3DCompileFromFile(filePathPS, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(hResult)) {
		return 1;
	}

	hResult = device->CreateVertexShader(vertShaderBuffer->GetBufferPointer(), vertShaderBuffer->GetBufferSize(), NULL, &this->m_vertexShader);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(hResult)) {
		return 1;
	}

	D3D11_INPUT_ELEMENT_DESC inputDescNormal;
	inputDescNormal.SemanticName		= "POSITION";
	inputDescNormal.SemanticIndex		= 0;
	inputDescNormal.Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal.InputSlot			= 0;
	inputDescNormal.AlignedByteOffset   = 0;
	inputDescNormal.InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal.InstanceDataStepRate = 0;

	hResult = device->CreateInputLayout(&inputDescNormal, 1, vertShaderBuffer->GetBufferPointer(), vertShaderBuffer->GetBufferSize(), &this->m_layout);
	if (FAILED(hResult)) {
		return 1;
	}
	return 0;
}
