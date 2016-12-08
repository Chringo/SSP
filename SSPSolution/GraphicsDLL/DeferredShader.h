#ifndef GRAPHICSDLL_DeferredShader
#define GRAPHICSDLL_DeferredShader

#include "ConstantBufferHandler.h"
#include "Shader.h"
#include "GraphicsComponent.h"
#include "../ResourceLib/Model.h"
#pragma comment (lib,"../Debug/ResourceLib")

class DeferredShader :
	public Shader
{
private:
	enum INPUT_LAYOUTS
	{
		IL_NORMAL,
		IL_ANIMATED,

		IL_TYPE_COUNT
	};

	ID3D11VertexShader*   m_vertexShader[4];
	ID3D11GeometryShader* m_geoShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout[IL_TYPE_COUNT];

	ID3D11PixelShader*	 m_gridPixelShader;
	ID3D11SamplerState*  m_samplerState;

	ID3D11RenderTargetView*		m_deferredRTV[BUFFER_COUNT];
	ID3D11ShaderResourceView*	m_deferredSRV[BUFFER_COUNT];
	ID3D11Texture2D*			m_deferredT2D[BUFFER_COUNT];

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
	
	int SetActive();
	int SetVariation(ShaderLib::ShaderVariations ShaderVariations);
	void Release();

	int Draw(ShaderLib::DrawType drawType);
	int Draw(Resources::Model* model);

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