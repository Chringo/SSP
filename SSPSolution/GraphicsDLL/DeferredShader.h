#ifndef GRAPHICSDLL_DeferredShader
#define GRAPHICSDLL_DeferredShader

#include "Shader.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/Model.h"
#pragma comment (lib,"../Debug/ResourceLib")

class DeferredShader :
	public Shader
{
private:

	ID3D11PixelShader*	 m_gridPixelShader;
	ID3D11SamplerState*  m_samplerState;
	ID3D11Buffer *		 m_worldMatrixBuffer;
	ID3D11Buffer *		 m_viewProjMatrixBuffer;

	ID3D11RenderTargetView*		m_deferredRTV[BUFFER_COUNT];
	ID3D11Texture2D*			m_deferredT2D[BUFFER_COUNT];
	ID3D11ShaderResourceView*	m_deferredSRV[BUFFER_COUNT];

	ID3D11Texture2D*		 m_depthStencilBuffer;
	ID3D11DepthStencilView*  m_DSV;
	ID3D11DepthStencilState* m_DSS;

	GraphicsComponent** m_graphicsComponents;
	Resources::Model ** modelsPtr;
public:
	DeferredShader();
	~DeferredShader();

	int Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	int SetGraphicsParameters(GraphicsComponent** grapicsComponents, Resources::Model** modelsPtr);
	
	int SetActive(ShaderLib::ShaderVariations ShaderVariations);
	void Release();


	int Draw(ShaderLib::DrawType drawType);
	int SetShaderParameters( void* shaderParams, ShaderLib::CBuffer type);
	int BindWorldCbuffer(ShaderLib::DeferredConstantBufferWorld * world);
	int BindWorldCbuffer(ShaderLib::DeferredConstantBufferWorldxm * world);
	int BindViewProjectionCbuffer(ShaderLib::DeferredConstantBufferVP * viewProjection);
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