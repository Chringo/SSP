#ifndef GRAPHICSDLL_LIGHTSHADERHANDLER
#define GRAPHICSDLL_LIGHTSHADERHANDLER

#include "ShaderHandler.h"
#include "ScreenQuad.h"

class LightShaderHandler :
	public ShaderHandler
{
private:
	ID3D11SamplerState* m_samplerStateLinear;
	ID3D11SamplerState* m_samplerStatePoint;

	ScreenQuad screenQuad;

public:
	LightShaderHandler();
	~LightShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, const DirectX::XMFLOAT2& resolution);
	int SetActive(ID3D11DeviceContext* deviceContext, ShaderLib::ShaderType shaderType);
	void Shutdown();

	int SetShaderParameters(ID3D11DeviceContext* deviceContext, ShaderLib::LightConstantBuffer* shaderParams);
};

#endif