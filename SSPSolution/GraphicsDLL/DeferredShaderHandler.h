#ifndef GRAPHICSDLL_DEFERREDSHADERHANDLER
#define GRAPHICSDLL_DEFERREDSHADERHANDLER

#include "ShaderHandler.h"

class DeferredShaderHandler :
	public ShaderHandler
{
private:
	ID3D11SamplerState* m_samplerState;

	ID3D11RenderTargetView* m_deferredRenderTargetViews[BUFFER_COUNT];
	ID3D11Texture2D* m_deferredRenderTargetTextures[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_deferredShaderResources[BUFFER_COUNT];

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
public:
	DeferredShaderHandler();
	~DeferredShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, const DirectX::XMINT2& resolution);
	int SetActive(ID3D11DeviceContext* deviceContext, ShaderLib::ShaderType shaderType);
	void Shutdown();

	int SetShaderParameters(ID3D11DeviceContext* deviceContext, ShaderLib::DeferredConstantBuffer* shaderParams);
	int ClearRenderTargetViews(ID3D11DeviceContext* deviceContext);

	ID3D11ShaderResourceView** GetShaderResourceViews();
};

#endif