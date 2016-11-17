#ifndef GRAPHICSDLL_DEFERREDSHADERHANDLER
#define GRAPHICSDLL_DEFERREDSHADERHANDLER

#include "ShaderHandler.h"

const int BUFFER_COUNT = 4; //Color, specular, normal, worldPos

class DeferredShaderHandler :
	public ShaderHandler
{
private:
	ID3D11SamplerState* samplerState;

	ID3D11RenderTargetView* deferredRenderTargetViews[BUFFER_COUNT];
	ID3D11Texture2D* deferredRenderTargetTextures[BUFFER_COUNT];
	ID3D11ShaderResourceView* deferredShaderResources[BUFFER_COUNT];
public:
	DeferredShaderHandler();
	~DeferredShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, DirectX::XMFLOAT2 resolution);
	int SetActive(ID3D11DeviceContext* deviceContext, ShaderLib::ShaderType shaderType);
	void Shutdown();

	ID3D11ShaderResourceView** GetShaderResourceViews();
};

#endif