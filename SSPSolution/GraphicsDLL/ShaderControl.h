#ifndef GRAPHICSDLL_SHADERS_SHADERCONTROL_H
#define GRAPHICSDLL_SHADERS_SHADERCONTROL_H
#include "FinalShader.h"
#include "DeferredShader.h"
#include "PostProcessShader.h"
#include "..\ResourceLib\Model.h"

// Named Shader Control because we had to remake shader handler 
// and we did not want to use the same name since that would generate huge merge conflicts.

class ShaderControl
{
public:
	enum Shaders {
		DEFERRED,
		FINAL,
		POSTPROCESS,
		//UI,
		//PARTICLES,
		//BILLBOARD
		NUM_SHADERS
	};
private: 	

	// pointers to the main device and context
	ID3D11Device			* m_Device				 = nullptr;
	ID3D11DeviceContext		* m_DeviceContext		 = nullptr;
	ID3D11RenderTargetView*   m_backBufferRTV		 = nullptr;
	ID3D11ShaderResourceView* m_backBufferSRV		 = nullptr;
	Shaders m_activeShader;
	Shader* m_shaders[NUM_SHADERS];
public:
	ShaderControl();
	~ShaderControl();
	bool Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, D3D11_VIEWPORT * viewPort);
	bool InitializeWireframe(ID3D11Device * device);
	void Release();

	void SetActive(Shaders type);
	void SetVariation(ShaderLib::ShaderVariations ShaderVariations);
	int  SetBackBuffer(ID3D11RenderTargetView* backBufferRTV, ID3D11ShaderResourceView* backBufferSRV);
	void PostProcess();
	void Draw(Resources::Model* model);
	void Draw(Resources::Model * model, GraphicsComponent * component);
	void Draw(Resources::Model * model, GraphicsAnimationComponent * component);
	void DrawInstanced(InstanceData* data);
	ID3D11DepthStencilView* GetBackBufferDSV() const {return((DeferredShader*)m_shaders[DEFERRED])->GetDepthStencilView();}

	void DrawFinal();

	int ClearFrame();

	ID3D11ShaderResourceView* GetShadowSRV()	 { return ((DeferredShader*)m_shaders[DEFERRED])->GetShadowSRV(); };
	ID3D11Texture2D*          GetShadowTexture() { return ((DeferredShader*)m_shaders[DEFERRED])->GetShadowTexture(); };

};


#endif

