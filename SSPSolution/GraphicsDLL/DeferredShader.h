#ifndef GRAPHICSDLL_SHADERS_DeferredShader_H
#define GRAPHICSDLL_SHADERS_DeferredShader_H

#include "ConstantBufferHandler.h"
#include "Shader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/ResourceHandler.h"

struct InstanceData {
	unsigned int modelID;
	Resources::Model* modelPtr;
	int amountOfInstances;
	DirectX::XMFLOAT4X4 componentSpecific[100];
};
class DeferredShader :
	public Shader
{
public:
	static const int MAX_INSTANCED_GEOMETRY = 100;
	static const int MAX_SHADOW_AMOUNT      = 2;
	static const int SHADOW_WIDTH  = 512;//Important! in order to bake static shadows, This value needs to be the same size as the shadowSize in GraphicsHandler
	static const int SHADOW_HEIGHT = 512;//Important! in order to bake static shadows, This value needs to be the same size as the shadowSize in GraphicsHandler
	enum VERTEX_SHADERS {
		VS_NORMAL,
		VS_ANIMATED,
		VS_INSTANCED_NORMAL,
		VS_SHADOW_NORMAL,
		VS_SHADOW_INSTANCED,
		VS_SHADOW_ANIMATED,
		VS_NUM_VERTEX_SHADERS
	};
	enum INPUT_LAYOUTS
	{
		IL_NORMAL,
		IL_ANIMATED,
		IL_INSTANCED_NORMAL,
		IL_TYPE_COUNT
	};
	D3D11_VIEWPORT*		  m_viewPort;

	ID3D11VertexShader*   m_vertexShader[VS_NUM_VERTEX_SHADERS];
	ID3D11GeometryShader* m_geoShader;
	ID3D11GeometryShader* m_ShadowGeoShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout[IL_TYPE_COUNT];

	ID3D11PixelShader*	 m_gridPixelShader;
	ID3D11SamplerState*  m_samplerState;

	ID3D11RenderTargetView*		m_deferredRTV[RTV_COUNT];
	ID3D11ShaderResourceView*	m_deferredSRV[BUFFER_COUNT]; //Use ShaderLib enum for access
	ID3D11Texture2D*			m_deferredT2D[BUFFER_COUNT]; //Use ShaderLib enum for access

	ID3D11Texture2D*		 m_depthStencilBuffer;
	ID3D11DepthStencilView*  m_DSV;
	ID3D11DepthStencilState* m_DSS;
	ID3D11DepthStencilView*	 m_shadowMapSV;
	D3D11_VIEWPORT *		 m_shadowVP = nullptr;

	ID3D11Buffer* m_instanceBuffer = nullptr;
	UINT32 m_vertexSize;
	bool m_shadowStateActive = false;
public:
	DeferredShader();
	~DeferredShader();

	int Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, D3D11_VIEWPORT * viewPort);
	
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

	ID3D11ShaderResourceView* GetShadowSRV() { return m_deferredSRV[ShaderLib::Shadow]; };
	ID3D11Texture2D* GetShadowTexture()      { return m_deferredT2D[ShaderLib::Shadow]; };
private:
};

#endif