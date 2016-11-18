#include "Direct3DHandler.h"

Direct3DHandler::Direct3DHandler()
{
	this->m_activeWindow = nullptr;
	this->m_backBufferRTV = nullptr;
	this->m_depthStencilBuffer = nullptr;
	this->m_depthStencilState = nullptr;
	this->m_depthStencilView = nullptr;
	this->m_gDevice = nullptr;
	this->m_gDeviceContext = nullptr;
	this->m_rasterizerState = nullptr;
	this->m_swapChain = nullptr;
	this->m_viewport = nullptr;
}


Direct3DHandler::~Direct3DHandler()
{
}

int Direct3DHandler::Initialize(HWND* windowHandle, DirectX::XMFLOAT2 resolution)
{
	HRESULT hResult;

	// Create the Device \\

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	hResult = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION, &this->m_gDevice,
		NULL, &this->m_gDeviceContext);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the swapchain \\

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Height = resolution.y;
	swapChainDesc.BufferDesc.Width = resolution.x;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60; //60hz
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1; //No MSAA
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = *windowHandle;
	swapChainDesc.Windowed = true; //Hardcoded windowed mode
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	IDXGIDevice* dxgiDevice = nullptr;
	hResult = this->m_gDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	if (FAILED(hResult))
	{
		return 1;
	}

	IDXGIAdapter* dxgiAdapter = nullptr;
	hResult = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	if (FAILED(hResult))
	{
		return 1;
	}

	IDXGIFactory* dxgiFactory = nullptr;
	hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = dxgiFactory->CreateSwapChain(this->m_gDevice, &swapChainDesc, &this->m_swapChain);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the backbuffer render target view \\

	ID3D11Texture2D* backBufferPrt;
	this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferPrt));

	hResult = this->m_gDevice->CreateRenderTargetView(backBufferPrt, 0, &this->m_backBufferRTV);
	if (FAILED(hResult))
	{
		return 1;
	}

	backBufferPrt->Release();

	// Create the rasterizer state \\

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK; //Enable backface culling
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hResult = this->m_gDevice->CreateRasterizerState(&rasterizerDesc, &this->m_rasterizerState);
	if (FAILED(hResult))
	{
		return 1;
	}

	this->m_gDeviceContext->RSSetState(this->m_rasterizerState); //Set the rasterstate

	// Create the depth buffer and view \\

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = resolution.x;
	depthBufferDesc.Height = resolution.y;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.SampleDesc.Count = 1; //No MSAA
	depthBufferDesc.SampleDesc.Quality = 0;

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hResult = this->m_gDevice->CreateTexture2D(&depthBufferDesc, NULL, &this->m_depthStencilBuffer);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = this->m_gDevice->CreateDepthStencilView(this->m_depthStencilBuffer, NULL, &this->m_depthStencilView);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the depth stecil state \\

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	//Frontfacing triangles
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Backfacing triangles
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hResult = this->m_gDevice->CreateDepthStencilState(&depthStencilDesc, &this->m_depthStencilState);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create and set the viewport \\
	
	this->m_viewport = new D3D11_VIEWPORT;
	this->m_viewport->TopLeftX = 0.0f;
	this->m_viewport->TopLeftY = 0.0f;
	this->m_viewport->Width = resolution.x;
	this->m_viewport->Height = resolution.y;
	this->m_viewport->MinDepth = 0.0f;
	this->m_viewport->MaxDepth = 1.0f;

	this->m_gDeviceContext->RSSetViewports(1, this->m_viewport);

	return 0;
}

int Direct3DHandler::ClearDepthAndRTV()
{
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	this->m_gDeviceContext->ClearRenderTargetView(this->m_backBufferRTV, black);

	this->m_gDeviceContext->ClearDepthStencilView(this->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return 0;
}

int Direct3DHandler::PresentScene()
{
	this->m_swapChain->Present(0, 0);

	return 0;
}

void Direct3DHandler::Shutdown()
{
	if (this->m_depthStencilState)
	{
		this->m_depthStencilState->Release();
		this->m_depthStencilState = nullptr;
	}
	if (this->m_depthStencilBuffer)
	{
		this->m_depthStencilBuffer->Release();
		this->m_depthStencilBuffer = nullptr;
	}
	if (this->m_depthStencilView)
	{
		this->m_depthStencilView->Release();
		this->m_depthStencilView = nullptr;
	}

	if (this->m_rasterizerState)
	{
		this->m_rasterizerState->Release();
		this->m_rasterizerState = nullptr;
	}

	if (this->m_backBufferRTV)
	{
		this->m_backBufferRTV->Release();
		this->m_backBufferRTV = nullptr;
	}

	if (this->m_swapChain)
	{
		this->m_swapChain->Release();
		this->m_swapChain = nullptr;
	}

	if (this->m_gDeviceContext)
	{
		this->m_gDeviceContext->Release();
		this->m_gDeviceContext = nullptr;
	}

	if (this->m_gDevice)
	{
		this->m_gDevice->Release();
		this->m_gDevice = nullptr;
	}
}

ID3D11Device * Direct3DHandler::GetDevice()
{
	return this->m_gDevice;
}

ID3D11DeviceContext * Direct3DHandler::GetDeviceContext()
{
	return this->m_gDeviceContext;
}
