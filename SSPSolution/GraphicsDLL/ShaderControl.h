#ifndef GRAPHICSDLL_SHADERCONTROL_H
#define GRAPHICSDLL_SHADERCONTROL_H
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
	ID3D11Device		*m_Device		 = nullptr;
	ID3D11DeviceContext *m_DeviceContext = nullptr;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11ShaderResourceView* backBufferSRV;
	Shaders m_activeShader;
public:
	Shader* m_shaders[NUM_SHADERS];
	ShaderControl();
	~ShaderControl();
	bool Initialize(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const DirectX::XMINT2& resolution);
	void Release();

	void SetActive(Shaders type);
	void SetVariation(ShaderLib::ShaderVariations ShaderVariations);
	int  SetBackBuffer(ID3D11RenderTargetView* backBufferRTV, ID3D11ShaderResourceView* backBufferSRV);
	void PostProcess();
	void Draw(Resources::Model* model);
	void DrawFinal();
	int ClearFrame();

};


#endif

