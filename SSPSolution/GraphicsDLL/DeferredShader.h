#ifndef GRAPHICSDLL_SHADERS_DeferredShader_H
#define GRAPHICSDLL_SHADERS_DeferredShader_H

#include "ConstantBufferHandler.h"
#include "Shader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

struct InstanceData {
	unsigned int modelID;
	int amountOfInstances;
	DirectX::XMFLOAT4X4 componentSpecific[100];
};
class DeferredShader :
	public Shader
{
public:
	static const int MAX_INSTANCED_GEOMETRY = 100;
	static const int MAX_SHADOW_AMOUNT      = 2;
	static const int SHADOW_WIDTH  = 1280;
	static const int SHADOW_HEIGHT = 720;
	enum VERTEX_SHADERS {
		VS_NORMAL,
		VS_ANIMATED,
		VS_INSTANCED_NORMAL,
		VS_SHADOW_NORMAL,
		VS_SHADOW_INSTANCED,
		VS_NUM_VERTEX_SHADERS
	};
	enum INPUT_LAYOUTS
	{
		IL_NORMAL,
		IL_ANIMATED,
		IL_INSTANCED_NORMAL,
		IL_TYPE_COUNT
	};

	ID3D11VertexShader*   m_vertexShader[VS_NUM_VERTEX_SHADERS];
	ID3D11GeometryShader* m_geoShader;
	ID3D11GeometryShader* m_ShadowGeoShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout[IL_TYPE_COUNT];

	ID3D11PixelShader*	 m_gridPixelShader;
	ID3D11SamplerState*  m_samplerState;

	ID3D11RenderTargetView*		m_deferredRTV[RTV_COUNT];
	ID3D11ShaderResourceView*	m_deferredSRV[BUFFER_COUNT];
	ID3D11Texture2D*			m_deferredT2D[BUFFER_COUNT];

	ID3D11Texture2D*		 m_depthStencilBuffer;
	ID3D11DepthStencilView*  m_DSV;
	ID3D11DepthStencilState* m_DSS;
	ID3D11DepthStencilView*	 m_shadowMapSV;

	ID3D11Buffer* m_instanceBuffer = nullptr;
	UINT32 m_vertexSize;
	bool m_shadowStateActive = false;
public:
	DeferredShader();
	~DeferredShader();

	int Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	
	ID3D11ShaderResourceView** GetShaderResourceViews();
	int SetActive();
	int SetVariation(ShaderLib::ShaderVariations ShaderVariations);
	void SetShadowDataToRead();
	void Release();

	
	int Draw(Resources::Model* model);
	int Draw(Resources::Model* model, GraphicsComponent * component);
	int Draw(Resources::Model* model, GraphicsAnimationComponent * component);
	int DrawInstanced(InstanceData* data, int iteration = 0);

	int Clear();
	int InitializeGridShader(ID3D11Device * device);

	int DrawGrid(Resources::Model * model);
	int DrawFromEditor(Resources::Model * model1);
	ID3D11DepthStencilView*  GetDepthStencilView() const { return this->m_DSV; };
private:
};

#endif