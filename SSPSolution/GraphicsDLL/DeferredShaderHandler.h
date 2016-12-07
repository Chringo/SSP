#ifndef GRAPHICSDLL_DEFERREDSHADERHANDLER
#define GRAPHICSDLL_DEFERREDSHADERHANDLER

#include "ConstantBufferHandler.h"
#include "ShaderHandler.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/Model.h"
#pragma comment (lib,"../Debug/ResourceLib")

class DeferredShaderHandler :
	public ShaderHandler
{
private:

	ID3D11PixelShader* m_gridPixelShader;
	ID3D11SamplerState* m_samplerState;


	ID3D11RenderTargetView* m_deferredRTV[BUFFER_COUNT];
	ID3D11Texture2D* m_deferredT2D[BUFFER_COUNT];
	ID3D11ShaderResourceView* m_deferredSRV[BUFFER_COUNT];

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_DSV;
	ID3D11DepthStencilState* m_DSS;

	GraphicsComponent** m_graphicsComponents;
	Resources::Model ** modelsPtr;
public:
	DeferredShaderHandler();
	~DeferredShaderHandler();

	int Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	int SetGraphicsParameters(GraphicsComponent** grapicsComponents, Resources::Model** modelsPtr);
	
	int SetActive(ShaderLib::ShaderType shaderType);
	void Shutdown();


	int Draw(ShaderLib::DrawType drawType);
	int Clear();
	int InitializeGridShader(ID3D11Device * device);
	ID3D11DepthStencilView* GetDSV() { return this->m_DSV; };

	ID3D11ShaderResourceView** GetShaderResourceViews();

private:
	int Draw();
	int DrawInstanced();
	int DrawGrid();
};

#endif