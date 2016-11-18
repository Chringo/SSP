#ifndef GRAPHICSDLL_LIGHTSHADERHANDLER
#define GRAPHICSDLL_LIGHTSHADERHANDLER

#include "ShaderHandler.h"

class LightShaderHandler :
	public ShaderHandler
{
private:
	ID3D11SamplerState* m_samplerState;

public:
	LightShaderHandler();
	~LightShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, DirectX::XMFLOAT2 resolution);
	int SetActive(ID3D11DeviceContext* deviceContext, ShaderLib::ShaderType shaderType);
	void Shutdown();

	int SetShaderParameters(ID3D11DeviceContext* deviceContext, ShaderLib::LightConstantBuffer* shaderParams);
};

#endif