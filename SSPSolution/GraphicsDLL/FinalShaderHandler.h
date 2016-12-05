#ifndef GRAPHICSDLL_FinalShaderHandler
#define GRAPHICSDLL_FinalShaderHandler

#include "ShaderHandler.h"
#include "ScreenQuad.h"

class FinalShaderHandler :
	public ShaderHandler
{
private:
	ID3D11RenderTargetView* m_finalRTV;
	
	ID3D11SamplerState* m_samplerStateLinear;
	ID3D11SamplerState* m_samplerStatePoint;

	ScreenQuad* m_screenQuad;

	ID3D11ShaderResourceView** m_gBufferRTVs;
	ID3D11ShaderResourceView** m_nullResources;

public:
	FinalShaderHandler();
	~FinalShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	
	int SetActive(ShaderLib::ShaderType shaderType);
	int SetRenderParameters(ID3D11RenderTargetView* backBufferRTV, ID3D11ShaderResourceView** gBuffers);
	int SetShaderParameters(ShaderLib::CameraConstantBuffer* shaderParams);
	
	void Shutdown();

	int Draw();
	void ResetPSShaderResources();
};

#endif