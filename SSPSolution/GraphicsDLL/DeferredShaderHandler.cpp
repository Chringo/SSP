#include "DeferredShaderHandler.h"

DeferredShaderHandler::DeferredShaderHandler() : ShaderHandler()
{
	this->m_samplerState = nullptr;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->m_deferredRenderTargetTextures[i] = nullptr;
		this->m_deferredRenderTargetViews[i] = nullptr;
		this->m_deferredShaderResources[i] = nullptr;
	}
}


DeferredShaderHandler::~DeferredShaderHandler()
{
}

int DeferredShaderHandler::Initialize(ID3D11Device * device, HWND * windowHandle, DirectX::XMFLOAT2 resolution)
{
	return 0;
}

int DeferredShaderHandler::SetActive(ID3D11DeviceContext * deviceContext, ShaderLib::ShaderType shaderType)
{
	return 0;
}

void DeferredShaderHandler::Shutdown()
{
}

ID3D11ShaderResourceView ** DeferredShaderHandler::GetShaderResourceViews()
{
	return nullptr;
}
