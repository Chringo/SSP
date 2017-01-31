#ifndef GRAPHICSDLL_DIRECT3DHANDLER_H
#define GRAPHICSDLL_DIRECT3DHANDLER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "../ResourceLib/ResourceHandler.h"
#pragma comment (lib,"../Debug/ResourceLib")


//include this to find out VRAM
//#include "stdafx.h"
//#include <DXGI1_3.h>
//#include <DXGI1_4.h>

class Direct3DHandler
{
private:
	ID3D11Device*			  m_gDevice;
	ID3D11DeviceContext*	  m_gDeviceContext;
	IDXGISwapChain*			  m_swapChain;
	ID3D11RenderTargetView*   m_backBufferRTV;
	ID3D11ShaderResourceView* m_backBufferSRV; //for post processing
	HWND*					  m_activeWindow;
	D3D11_VIEWPORT*			  m_viewport;
	ID3D11RasterizerState*	  m_rasterizerState;
	ID3D11RasterizerState*	  m_rasterizerStateWireFrame;

	ID3D11Texture2D* m_depthStencilBuffer;
	/*
	remove comment here to find vram
	

#ifdef _DEBUG
	IDXGIAdapter3* dxgiAdapter3;	//for VRAM check
#endif
	*/



public:
	Direct3DHandler();
	~Direct3DHandler();

	int Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution);
	int InitializeGridRasterizer();

	int PresentScene();
	void Shutdown();

	ID3D11Device*			  GetDevice();
	ID3D11DeviceContext*	  GetDeviceContext();
	ID3D11RenderTargetView*	  GetBackbufferRTV();
	ID3D11ShaderResourceView* GetBackbufferSRV();

	int SetRasterizerState(D3D11_FILL_MODE mode);
};

#endif