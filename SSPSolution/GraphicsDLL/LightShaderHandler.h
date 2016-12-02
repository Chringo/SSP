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

	ScreenQuad* m_screenQuad;

	ID3D11ShaderResourceView** m_nullResources;

public:
	LightShaderHandler();
	~LightShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	int SetActive(ShaderLib::ShaderType shaderType);
	void Shutdown();

	int Draw();
	int SetShaderParameters(ShaderLib::LightConstantBuffer* shaderParams, ID3D11ShaderResourceView** gBuffers);
	void ResetPSShaderResources();
};

#endif