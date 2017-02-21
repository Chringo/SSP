#ifndef GRAPHICSDLL_SHADERS_POSTPROCESSSHADER
#define GRAPHICSDLL_SHADERS_POSTPROCESSSHADER
#include "Shader.h"
#include "ScreenQuad.h"
class PostProcessShader :
	public Shader
{
public:
	enum PostEffects{
		RAYTRACING,
		WATER,
		BLUR,
		FXAA,


		NUM_TYPES
	};
private:


	const static int RESOURCEVIEW_SLOT = 6;
	ID3D11PixelShader*	    m_pixelShader[NUM_TYPES];
	ScreenQuad*				m_screenQuad;

	ID3D11RenderTargetView*   m_RenderTarget[2];
	ID3D11ShaderResourceView* m_ResourceView[2];

	short int currRTVIndex = 0;

	bool active[NUM_TYPES]{ true };
	
public:
	PostProcessShader();
	virtual ~PostProcessShader();
	const bool IsEnabled(PostEffects type) const { return active[type]; };
	int Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3D11_VIEWPORT * viewPort);
	int SetActive(PostEffects type);
	void Release();
	ID3D11ShaderResourceView* GetActiveShaderResource() { return m_ResourceView[currRTVIndex]; };
	ID3D11RenderTargetView*  Draw(); // returns the last used RTV
private:
	int SetActive();
	ID3D11RenderTargetView*   GetAvailableRTV();
};

#endif