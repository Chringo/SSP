#ifndef GRAPHICSDLL_FinalShader
#define GRAPHICSDLL_FinalShader

#include "Shader.h"
#include "ScreenQuad.h"

class FinalShader :
	public Shader
{
private:
	ID3D11VertexShader*   m_vertexShader[4];
	ID3D11GeometryShader* m_geoShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout;

	ID3D11RenderTargetView* m_finalRTV;
	
	ID3D11SamplerState* m_samplerStateLinear;
	ID3D11SamplerState* m_samplerStatePoint;

	ScreenQuad* m_screenQuad;

	ID3D11ShaderResourceView** m_gBufferRTVs;
	ID3D11ShaderResourceView** m_nullResources;

public:
	FinalShader();
	~FinalShader();

	int Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	
	int SetActive(ShaderLib::ShaderVariations ShaderVariations);
	int SetRenderParameters(ID3D11RenderTargetView* backBufferRTV, ID3D11ShaderResourceView** gBuffers);
	
	void Release();

	int Draw();
	void ResetPSShaderResources();
};

#endif