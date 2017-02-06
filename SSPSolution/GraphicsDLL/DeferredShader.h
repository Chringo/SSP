#ifndef GRAPHICSDLL_SHADERS_DeferredShader_H
#define GRAPHICSDLL_SHADERS_DeferredShader_H

#include "ConstantBufferHandler.h"
#include "Shader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

struct InstanceData {
	int modelID;
	int amountOfInstances;
	DirectX::XMFLOAT4X4* componentSpecific;
};
class DeferredShader :
	public Shader
{
private:
	static const int MAX_INSTANCED_GEOMETRY = 50;
	enum INPUT_LAYOUTS
	{
		IL_NORMAL,
		IL_ANIMATED,
		IL_INSTANCED_NORMAL,
		IL_TYPE_COUNT
	};

	ID3D11VertexShader*   m_vertexShader[IL_TYPE_COUNT];
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

	ID3D11Buffer* m_instanceBuffer = nullptr;
	UINT32 m_vertexSize;
public:
	DeferredShader();
	~DeferredShader();

	int Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	
	ID3D11ShaderResourceView** GetShaderResourceViews();
	int SetActive();
	int SetVariation(ShaderLib::ShaderVariations ShaderVariations);
	void Release();

	
	int Draw(Resources::Model* model);
	int Draw(Resources::Model* model, GraphicsComponent * component);
	int Draw(Resources::Model* model, GraphicsAnimationComponent * component);
	int DrawInstanced(InstanceData* data);

	int Clear();
	int InitializeGridShader(ID3D11Device * device);

	int DrawGrid(Resources::Model * model);
	int DrawFromEditor(Resources::Model * model1);
	ID3D11DepthStencilView*  GetDepthStencilView() const { return this->m_DSV; };
private:
};

#endif