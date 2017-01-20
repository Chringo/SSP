#include "Direct3DHandler.h"

Direct3DHandler::Direct3DHandler()
{
	this->m_activeWindow = nullptr;
	this->m_backBufferRTV = nullptr;
	this->m_depthStencilBuffer = nullptr;
	this->m_gDevice = nullptr;
	this->m_gDeviceContext = nullptr;
	this->m_rasterizerState = nullptr;
	this->m_rasterizerStateWireFrame = nullptr;
	this->m_swapChain = nullptr;
	this->m_viewport = nullptr;
}


Direct3DHandler::~Direct3DHandler()
{
}

int Direct3DHandler::Initialize(HWND* windowHandle, const DirectX::XMINT2& resolution)
{
	HRESULT hResult;

	// Create the Device \\

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	hResult = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &this->m_gDevice,
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

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
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

	ID3D11Texture2D* backBufferPrt = nullptr;
	this->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)(&backBufferPrt));
	
	hResult = this->m_gDevice->CreateRenderTargetView(backBufferPrt, NULL, &this->m_backBufferRTV);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = this->m_gDevice->CreateShaderResourceView(backBufferPrt, nullptr, &this->m_backBufferSRV);

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

	
	this->m_viewport = new D3D11_VIEWPORT;
	this->m_viewport->TopLeftX = 0.0f;
	this->m_viewport->TopLeftY = 0.0f;
	this->m_viewport->Width = float(resolution.x);
	this->m_viewport->Height = float(resolution.y);
	this->m_viewport->MinDepth = 0.0f;
	this->m_viewport->MaxDepth = 1.0f;

	this->m_gDeviceContext->RSSetViewports(1, this->m_viewport);

	Resources::ResourceHandler::GetInstance()->SetDeviceAndContext(this->m_gDevice, this->m_gDeviceContext);

#ifdef _DEBUG
	IDXGIAdapter* firstAdapter = nullptr;
	hResult = dxgiFactory->EnumAdapters(0, &firstAdapter);

	dxgiAdapter3 = nullptr;
	if (SUCCEEDED(firstAdapter->QueryInterface(__uuidof(IDXGIAdapter3), (void**)&dxgiAdapter3)))
	{
		//failed creating adapter3
	}
#endif

	return 0;
}

int Direct3DHandler::InitializeGridRasterizer()
{
	HRESULT hResult;

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_NONE; //Enable backface culling
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	hResult = this->m_gDevice->CreateRasterizerState(&rasterizerDesc, &this->m_rasterizerStateWireFrame);
	if (FAILED(hResult))
	{
		return 1;
	}

	return 0;
}



int Direct3DHandler::PresentScene()
{
	this->m_swapChain->Present(0, 0);
#ifdef _DEBUG
	DXGI_QUERY_VIDEO_MEMORY_INFO info;
	if (SUCCEEDED(dxgiAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info)))
	{
		int memoryUsage = info.CurrentUsage / 1024 / 1024; //MiB

		char msg[100];
		sprintf_s(msg, "%d", memoryUsage);
		printf("GPU MiB: %d", memoryUsage);
	}
#endif

	return 0;
}

void Direct3DHandler::Shutdown()
{
	if (this->m_depthStencilBuffer)
	{
		this->m_depthStencilBuffer->Release();
		this->m_depthStencilBuffer = nullptr;
	}

	if (this->m_rasterizerState)
	{
		this->m_rasterizerState->Release();
		this->m_rasterizerState = nullptr;
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

	if (this->m_backBufferSRV)
	{
		this->m_backBufferSRV->Release();
		this->m_backBufferSRV = nullptr;
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

	if (this->m_viewport)
	{
		delete this->m_viewport;
		this->m_viewport = nullptr;
	}

	if (this->m_rasterizerStateWireFrame)
	{
		m_rasterizerStateWireFrame->Release();
		this->m_rasterizerStateWireFrame = nullptr;
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

ID3D11RenderTargetView * Direct3DHandler::GetBackbufferRTV()
{
	return this->m_backBufferRTV;
}

ID3D11ShaderResourceView * Direct3DHandler::GetBackbufferSRV()
{
	return this->m_backBufferSRV;
}

int Direct3DHandler::SetRasterizerState(D3D11_FILL_MODE mode)
{
	switch (mode)
	{
	case D3D11_FILL_WIREFRAME:
	{
		this->m_gDeviceContext->RSSetState(this->m_rasterizerStateWireFrame);
		break;
	}
	case D3D11_FILL_SOLID:
	{
		this->m_gDeviceContext->RSSetState(this->m_rasterizerState);
		break;
	}
	default:
		break;
	}

	return 0;
}

