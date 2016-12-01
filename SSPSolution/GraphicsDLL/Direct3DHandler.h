#ifndef GRAPHICSDLL_DIRECT3DHANDLER
#define GRAPHICSDLL_DIRECT3DHANDLER

#include <d3d11.h>
#include <DirectXMath.h>
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")

class Direct3DHandler
{
private:
	ID3D11Device* m_gDevice;
	ID3D11DeviceContext* m_gDeviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_backBufferRTV;
	HWND* m_activeWindow;
	D3D11_VIEWPORT* m_viewport;
	ID3D11RasterizerState* m_rasterizerState;

	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11DepthStencilState* m_depthStencilState;
public:
	Direct3DHandler();
	~Direct3DHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution);

	int ClearDepthAndRTV();
	int ClearDepthAndRTV(ID3D11DepthStencilView*);
	int PresentScene();
	void Shutdown();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	int SetBackBuffer();
	int SetBackBuffer(ID3D11DepthStencilView*);
};

#endif