#include "PostProcessShader.h"



PostProcessShader::PostProcessShader()
{
}


PostProcessShader::~PostProcessShader()
{
}

int PostProcessShader::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, D3D11_VIEWPORT * viewPort)
{
	HRESULT hResult;
	ID3D10Blob* errorMessage;
	ID3D10Blob* pixelShaderBuffer[NUM_TYPES] = { nullptr };
	this->m_deviceContext = deviceContext;
	WCHAR* filePaths[NUM_TYPES];
	
	for (int i = 0; i < NUM_TYPES; i++)
	{
		this->active[i] = true;
	}

	 filePaths[RAYTRACING]   = L"../Assets/Shaders/PostProcess/RayTrace_BoundingBox.hlsl";
	 filePaths[WATER]		 = L"../Assets/Shaders/PostProcess/RayTracing.hlsl";
	 filePaths[BLUR]		 = L"../Assets/Shaders/PostProcess/RayTracing.hlsl";
	 filePaths[FXAA]		 = L"../Assets/Shaders/PostProcess/FXAA.hlsl";

	 filePaths[SSR]			 = L"../Assets/Shaders/PostProcess/RayTracing.hlsl";

	 // Create the screen quad \\
	 
	 DirectX::XMINT2 resolution;
	 resolution.x = viewPort->Width;
	 resolution.y = viewPort->Height;
	 this->m_screenQuad = new ScreenQuad();
	 if (this->m_screenQuad->Initialize(device, resolution))
	 {
		 return 1;
	 }


	// Compile the shaders \\
	
	 for (size_t i = 0; i < NUM_TYPES; i++)
	 {
		 this->m_pixelShader[i] = nullptr;
#ifdef _DEBUG
		 hResult = D3DCompileFromFile(filePaths[i], NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer[i], &errorMessage);
#else
		 hResult = D3DCompileFromFile(filePaths[i], NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &pixelShaderBuffer[i], &errorMessage);
#endif // _DEBUG
		 if (FAILED(hResult)){
			// Shader::OutputShaderErrorMessage(errorMessage, filePaths[i]);
			 this->active[i] = false;
			 continue;
		 }
		 hResult = device->CreatePixelShader(pixelShaderBuffer[i]->GetBufferPointer(), pixelShaderBuffer[i]->GetBufferSize(), NULL, &this->m_pixelShader[i]);
		 if (FAILED(hResult)) {
			// Shader::OutputShaderErrorMessage(errorMessage, filePaths[i]);
			 this->active[i] = false;
		 }

	 }

	 D3D11_TEXTURE2D_DESC textureDesc;
	 D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	 D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	 ID3D11Texture2D* tex;

	 ZeroMemory(&textureDesc, sizeof(textureDesc));
	 //Set up the render texture desciption

	 textureDesc.Width      = resolution.x;
	 textureDesc.Height     = resolution.y;
	 textureDesc.MipLevels  = 1;
	 textureDesc.ArraySize  = 1;
	 textureDesc.Format		= DXGI_FORMAT_R8G8B8A8_UNORM;
	 textureDesc.SampleDesc.Count = 1;
	 textureDesc.Usage = D3D11_USAGE_DEFAULT;
	 textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	 textureDesc.CPUAccessFlags = 0;
	 textureDesc.MiscFlags = 0;

	 //set up description for render target view
	 renderTargetDesc.Format = textureDesc.Format;
	 renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	 renderTargetDesc.Texture2D.MipSlice = 0;

	 //Set up the shader resource view
	 resourceViewDesc.Format = textureDesc.Format;
	 resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	 resourceViewDesc.Texture2D.MostDetailedMip = 0;
	 resourceViewDesc.Texture2D.MipLevels = 1;



	 for (size_t i = 0; i < 2; i++)
	 {
		 hResult = device->CreateTexture2D(&textureDesc, NULL, &tex);
		 if (FAILED(hResult))
			 return 1;
		 hResult = device->CreateRenderTargetView(tex, &renderTargetDesc, &m_RenderTarget[i]);
		 if (FAILED(hResult))
			 return 1;

		 hResult = device->CreateShaderResourceView(tex, &resourceViewDesc, &m_ResourceView[i]);
		 if (FAILED(hResult))
			 return 1;

		 tex->Release();

	 }

	 this->active[PostEffects::RAYTRACING] = false;

	 return 0;
	
}

int PostProcessShader::SetActive()
{
	return 0;
}

int PostProcessShader::SetActive(PostEffects type)
{

	
	m_deviceContext->PSSetShader(this->m_pixelShader[type], NULL, 0);

	this->m_screenQuad->SetBuffers(m_deviceContext);

	return 0;
}

void PostProcessShader::Release()
{
	Shader::Release();

	for (size_t i = 0; i < NUM_TYPES; i++)
	{
		if (m_pixelShader[i] != nullptr)
		{
			m_pixelShader[i]->Release();
			m_pixelShader[i] = nullptr;
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		if (m_RenderTarget[i] != nullptr) {
		 m_RenderTarget[i]->Release();
		 m_RenderTarget[i] = nullptr;
		}
		if (m_ResourceView[i] != nullptr) {
		 m_ResourceView[i]->Release();
		 m_ResourceView[i] = nullptr;
		}

	}

	if (this->m_screenQuad)
	{
		this->m_screenQuad->Shutdown();
		delete this->m_screenQuad;
		this->m_screenQuad = nullptr;
	}


}

ID3D11RenderTargetView* PostProcessShader::Draw()
{

	ID3D11RenderTargetView* rtv = this->GetAvailableRTV();
	m_deviceContext->OMSetRenderTargets(1, &rtv, NULL);
	this->m_deviceContext->DrawIndexed(6, 0, 0);
	/*
		Set renderTarget as null 
		Set the Rendered frame as a shader resource.
		return the render Target
	*/
	ID3D11RenderTargetView* temp = nullptr;
	m_deviceContext->OMSetRenderTargets(1, &temp, NULL);
 	m_deviceContext->PSSetShaderResources(RESOURCEVIEW_SLOT, 1, &m_ResourceView[currRTVIndex]);
	return m_RenderTarget[currRTVIndex];
}

ID3D11RenderTargetView * PostProcessShader::GetAvailableRTV()
{
	if (currRTVIndex == 0)
	{
		currRTVIndex = 1;
		return m_RenderTarget[1];
	}
	else
	{
		currRTVIndex = 0;
		return m_RenderTarget[0];
	}

	
}


