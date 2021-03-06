#ifndef GRAPHICSDLL_DIRECT3DHANDLER_H
#define GRAPHICSDLL_DIRECT3DHANDLER_H

#include <d3d11_1.h>
#include <DirectXMath.h>
#include "../ResourceLib/ResourceHandler.h"

class Direct3DHandler
{
private:
	ID3D11Device*			  m_gDevice;
	ID3D11DeviceContext*	  m_gDeviceContext;
	ID3D11RenderTargetView*   m_backBufferRTV;
	ID3D11ShaderResourceView* m_backBufferSRV; //for post processing
	HWND*					  m_activeWindow;
	D3D11_VIEWPORT*			  m_viewport;
	ID3D11RasterizerState*	  m_rasterizerState;
	ID3D11RasterizerState*	  m_rasterizerStateWireFrame;
	IDXGISwapChain1*		  m_swapChain;
	
	ID3D11Texture2D* m_depthStencilBuffer;

	ID3D11BlendState* g_pBlendStateNoBlend = NULL;
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

public:
	Direct3DHandler();
	~Direct3DHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution, bool editorMode);
	int InitializeGridRasterizer();

	int PresentScene();
	void Shutdown();

	ID3D11Device*			  GetDevice();
	ID3D11DeviceContext*	  GetDeviceContext();
	ID3D11RenderTargetView*	  GetBackbufferRTV();
	ID3D11ShaderResourceView* GetBackbufferSRV();
	D3D11_VIEWPORT*			  GetViewPort();

	int SetRasterizerState(D3D11_FILL_MODE mode);
	int ClearBlendState();
};

#endif