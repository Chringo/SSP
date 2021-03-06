#include "DeferredShader.h"

DeferredShader::DeferredShader() : Shader()
{
	this->m_samplerState = nullptr;

	for (int i = 0; i < VS_NUM_VERTEX_SHADERS; i++)
	{
		this->m_vertexShader[i] = nullptr;
	}
	this->m_geoShader = nullptr;
	this->m_pixelShader = nullptr;
	for (int i = 0; i < IL_TYPE_COUNT; i++)
	{
		this->m_layout[i] = nullptr;
	}
	this->m_gridPixelShader = nullptr;

	for (int i = 0; i < BUFFER_COUNT; i++) {
		this->m_deferredT2D[i] = nullptr;
		this->m_deferredSRV[i] = nullptr;
	}
	for (size_t i = 0; i < RTV_COUNT; i++)
	{
		this->m_deferredRTV[i] = nullptr;

	}
	this->m_shadowMapSV = nullptr;
	this->m_DSV   = nullptr;
}


DeferredShader::~DeferredShader()
{
}

int DeferredShader::Initialize(ID3D11Device* device,  ID3D11DeviceContext* deviceContext, D3D11_VIEWPORT *viewPort)
{
	HRESULT hResult;
	ID3D10Blob* vertexShaderBuffer[VS_NUM_VERTEX_SHADERS] = { nullptr };
	ID3D10Blob* geoShaderBuffer = nullptr;
	ID3D10Blob* geoShadowShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;
	this->m_viewPort = viewPort;
	this->m_deviceContext = deviceContext;

	//Insert shader path here
	WCHAR* vsFilename         = L"../Assets/Shaders/GBuffer/GBufferVS.hlsl";
	WCHAR* vsInstFilename     = L"../Assets/Shaders/GBuffer/GBufferVS_Instanced.hlsl";
	WCHAR* vsAnimFilename     = L"../Assets/Shaders/GBuffer/AnimVS.hlsl";
	WCHAR* gsFilename		  = L"../Assets/Shaders/GBuffer/GBuffer.hlsl";
	WCHAR* psFilename	      = L"../Assets/Shaders/GBuffer/GBuffer.hlsl";
	WCHAR* shadowFilename	  = L"../Assets/Shaders/Shadow/ShadowShader.hlsl";
	WCHAR* shadowInstFilename = L"../Assets/Shaders/Shadow/ShadowShader_Instanced.hlsl";
	WCHAR* shadowAnimFilename = L"../Assets/Shaders/Shadow/ShadowShader_Animation.hlsl";
	// Compile the shaders \\

#ifdef _DEBUG
	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_NORMAL], &errorMessage);
#else
	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_NORMAL], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}
#ifdef _DEBUG
	hResult = D3DCompileFromFile(vsAnimFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_ANIMATED], &errorMessage);
#else
	hResult = D3DCompileFromFile(vsAnimFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_ANIMATED], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsAnimFilename);

		return 1;
	}

#ifdef _DEBUG
	hResult = D3DCompileFromFile(vsInstFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_INSTANCED_NORMAL], &errorMessage);
#else
	hResult = D3DCompileFromFile(vsInstFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_INSTANCED_NORMAL], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsInstFilename);

		return 1;
	}

#ifdef _DEBUG
	hResult = D3DCompileFromFile(gsFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_DEBUG, 0, &geoShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(gsFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &geoShaderBuffer, &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, gsFilename);
		return 1;
	}
#ifdef _DEBUG
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &pixelShaderBuffer, &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, psFilename);
		return 1;
	}
#ifdef _DEBUG
	hResult = D3DCompileFromFile(shadowFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_NORMAL], &errorMessage);
#else
	hResult = D3DCompileFromFile(shadowFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_NORMAL], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, shadowFilename);
		return 1;
	}

#ifdef _DEBUG
	hResult = D3DCompileFromFile(shadowInstFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_INSTANCED ], &errorMessage);
#else
	hResult = D3DCompileFromFile(shadowInstFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_INSTANCED], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, shadowInstFilename);
		return 1;
	}


#ifdef _DEBUG
	hResult = D3DCompileFromFile(shadowAnimFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_ANIMATED], &errorMessage);
#else
	hResult = D3DCompileFromFile(shadowAnimFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_ANIMATED], &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, shadowAnimFilename);
		return 1;
	}

#ifdef _DEBUG
	hResult = D3DCompileFromFile(shadowFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_DEBUG, 0, &geoShadowShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(shadowFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &geoShadowShaderBuffer, &errorMessage);
#endif // _DEBUG
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, shadowFilename);
		return 1;
	}


	// Create the shaders \\

	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_NORMAL]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_NORMAL]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_NORMAL]);
	if (FAILED(hResult)) 
	{
		return 1;
	}
	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_ANIMATED]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_ANIMATED]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_ANIMATED]);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_INSTANCED_NORMAL]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_INSTANCED_NORMAL]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_INSTANCED_NORMAL]);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_NORMAL]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_NORMAL]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_NORMAL]);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_INSTANCED]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_INSTANCED]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_INSTANCED]);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = device->CreateVertexShader(vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_ANIMATED]->GetBufferPointer(), vertexShaderBuffer[VERTEX_SHADERS::VS_SHADOW_ANIMATED]->GetBufferSize(), NULL, &this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_ANIMATED]);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = device->CreateGeometryShader(geoShaderBuffer->GetBufferPointer(), geoShaderBuffer->GetBufferSize(), NULL, &this->m_geoShader);
	if (FAILED(hResult)) {
		return 1;
	}
	hResult = device->CreateGeometryShader(geoShadowShaderBuffer->GetBufferPointer(), geoShadowShaderBuffer->GetBufferSize(), NULL, &this->m_ShadowGeoShader);
	if (FAILED(hResult)) {
		return 1;
	}
	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(hResult)) {
		return 1;
	}
	// Create the input layout \\

	D3D11_INPUT_ELEMENT_DESC inputDescNormal[4];
	inputDescNormal[0].SemanticName = "POSITION";
	inputDescNormal[0].SemanticIndex = 0;
	inputDescNormal[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal[0].InputSlot = 0;
	inputDescNormal[0].AlignedByteOffset = 0;
	inputDescNormal[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[0].InstanceDataStepRate = 0;

	inputDescNormal[1].SemanticName = "NORMAL";
	inputDescNormal[1].SemanticIndex = 0;
	inputDescNormal[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal[1].InputSlot = 0;
	inputDescNormal[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescNormal[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[1].InstanceDataStepRate = 0;

	inputDescNormal[2].SemanticName = "TANGENT";
	inputDescNormal[2].SemanticIndex = 0;
	inputDescNormal[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal[2].InputSlot = 0;
	inputDescNormal[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescNormal[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[2].InstanceDataStepRate = 0;

	inputDescNormal[3].SemanticName = "TEXCOORD";
	inputDescNormal[3].SemanticIndex = 0;
	inputDescNormal[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDescNormal[3].InputSlot = 0;
	inputDescNormal[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescNormal[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[3].InstanceDataStepRate = 0;


	unsigned int numElements = sizeof(inputDescNormal) / sizeof(inputDescNormal[0]);
	//Create the vertex input layout.
	hResult = device->CreateInputLayout(inputDescNormal, numElements, vertexShaderBuffer[ShaderLib::Normal]->GetBufferPointer(), vertexShaderBuffer[ShaderLib::Normal]->GetBufferSize(), &this->m_layout[IL_NORMAL]);
	if (FAILED(hResult)) {
		return 1;
	}


	
	// Create the input layout  for instancing \\

	D3D11_INPUT_ELEMENT_DESC inputDescInstanced[8];
	inputDescInstanced[0].SemanticName		    = "POSITION";
	inputDescInstanced[0].SemanticIndex			= 0;
	inputDescInstanced[0].InputSlot				= 0;
	inputDescInstanced[0].AlignedByteOffset     = 0;
	inputDescInstanced[0].InstanceDataStepRate  = 0;
	inputDescInstanced[0].Format		        = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescInstanced[0].InputSlotClass	    = D3D11_INPUT_PER_VERTEX_DATA;

	inputDescInstanced[1].SemanticName		    = "NORMAL";
	inputDescInstanced[1].SemanticIndex			= 0;
	inputDescInstanced[1].InputSlot				= 0;
	inputDescInstanced[1].InstanceDataStepRate  = 0;
	inputDescInstanced[1].Format		        = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescInstanced[1].AlignedByteOffset     = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescInstanced[1].InputSlotClass	    = D3D11_INPUT_PER_VERTEX_DATA;

	inputDescInstanced[2].SemanticName		    = "TANGENT";
	inputDescInstanced[2].SemanticIndex	        = 0;
	inputDescInstanced[2].InputSlot		        = 0;
	inputDescInstanced[2].InstanceDataStepRate  = 0;
	inputDescInstanced[2].Format		        = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescInstanced[2].AlignedByteOffset     = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescInstanced[2].InputSlotClass        = D3D11_INPUT_PER_VERTEX_DATA;

	inputDescInstanced[3].SemanticName			= "TEXCOORD";
	inputDescInstanced[3].SemanticIndex		    = 0;
	inputDescInstanced[3].InputSlot			    = 0;
	inputDescInstanced[3].InstanceDataStepRate  = 0;
	inputDescInstanced[3].Format				= DXGI_FORMAT_R32G32_FLOAT;
	inputDescInstanced[3].AlignedByteOffset		= D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescInstanced[3].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;

	for (size_t i = 4; i < 8; i++)
	{
		//Create 4 vectors for the world data per instance
		inputDescInstanced[i].SemanticName          = "WORLD";
		inputDescInstanced[i].SemanticIndex         = i-4;
		inputDescInstanced[i].InputSlot				= 1;
		inputDescInstanced[i].InstanceDataStepRate  = 1;  
		inputDescInstanced[i].Format		        = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputDescInstanced[i].AlignedByteOffset		= 0 + (sizeof(float) * 4) * (i-4);
		inputDescInstanced[i].InputSlotClass	    = D3D11_INPUT_PER_INSTANCE_DATA;

	}

	 numElements = sizeof(inputDescInstanced) / sizeof(inputDescInstanced[0]);
	//Create the vertex input layout.
	hResult = device->CreateInputLayout(inputDescInstanced, numElements, vertexShaderBuffer[VS_INSTANCED_NORMAL]->GetBufferPointer(), vertexShaderBuffer[VS_INSTANCED_NORMAL]->GetBufferSize(), &this->m_layout[IL_INSTANCED_NORMAL]);
	if (FAILED(hResult)) {
		return 1;
	}
	// Create the input layout \\

	D3D11_INPUT_ELEMENT_DESC inputDescAnim[6];
	inputDescAnim[0].SemanticName = "POSITION";
	inputDescAnim[0].SemanticIndex = 0;
	inputDescAnim[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescAnim[0].InputSlot = 0;
	inputDescAnim[0].AlignedByteOffset = 0;
	inputDescAnim[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[0].InstanceDataStepRate = 0;

	inputDescAnim[1].SemanticName = "NORMAL";
	inputDescAnim[1].SemanticIndex = 0;
	inputDescAnim[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescAnim[1].InputSlot = 0;
	inputDescAnim[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescAnim[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[1].InstanceDataStepRate = 0;

	inputDescAnim[2].SemanticName = "TANGENT";
	inputDescAnim[2].SemanticIndex = 0;
	inputDescAnim[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescAnim[2].InputSlot = 0;
	inputDescAnim[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescAnim[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[2].InstanceDataStepRate = 0;

	inputDescAnim[3].SemanticName = "TEXCOORD";
	inputDescAnim[3].SemanticIndex = 0;
	inputDescAnim[3].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputDescAnim[3].InputSlot = 0;
	inputDescAnim[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescAnim[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[3].InstanceDataStepRate = 0;


	inputDescAnim[4].SemanticName = "WEIGHTS";
	inputDescAnim[4].SemanticIndex = 0;
	inputDescAnim[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputDescAnim[4].InputSlot = 0;
	inputDescAnim[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescAnim[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[4].InstanceDataStepRate = 0;

	inputDescAnim[5].SemanticName = "INFLUENCE";
	inputDescAnim[5].SemanticIndex = 0;
	inputDescAnim[5].Format = DXGI_FORMAT_R32G32B32A32_SINT;
	inputDescAnim[5].InputSlot = 0;
	inputDescAnim[5].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescAnim[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescAnim[5].InstanceDataStepRate = 0;

	numElements = sizeof(inputDescAnim) / sizeof(inputDescAnim[0]);
	//Create the vertex input layout.
	hResult = device->CreateInputLayout(inputDescAnim, numElements, vertexShaderBuffer[VS_ANIMATED]->GetBufferPointer(), vertexShaderBuffer[VS_ANIMATED]->GetBufferSize(), &this->m_layout[IL_ANIMATED]);
	if (FAILED(hResult)) {
		return 1;
	}

	//Release and nullptr the buffers as they are no longer needed
	for (size_t i = 0; i < VS_NUM_VERTEX_SHADERS; i++)
	{
		vertexShaderBuffer[i]->Release();
		vertexShaderBuffer[i] = nullptr;
	}
	geoShaderBuffer->Release();
	geoShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;
	geoShadowShaderBuffer->Release();
	geoShadowShaderBuffer = nullptr;

	// Create the sampler \\

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	//Fill the texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samplerDesc.BorderColor[0] = 0;
	//samplerDesc.BorderColor[1] = 0;
	//samplerDesc.BorderColor[2] = 0;
	//samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler state
	hResult = device->CreateSamplerState(&samplerDesc, &this->m_samplerState);
	if (FAILED(hResult))
	{
		return 1;
	}

	// Create the render target and shader resource views \\

	D3D11_TEXTURE2D_DESC renderTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	//Initialize the render target texture description
	ZeroMemory(&renderTextureDesc, sizeof(renderTextureDesc));
	
	//Setup the render target texture description
	renderTextureDesc.Width = viewPort->Width;
	renderTextureDesc.Height = viewPort->Height;
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	renderTextureDesc.SampleDesc.Count = 1; //No MSAA
	renderTextureDesc.SampleDesc.Quality = 0;

	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTextureDesc.CPUAccessFlags = 0;
	renderTextureDesc.MiscFlags = 0;

	//Create the render target textures
	for (int i = 0; i < BUFFER_COUNT -1; i++) { //shadow texture is created separately
		hResult = device->CreateTexture2D(&renderTextureDesc, NULL, &this->m_deferredT2D[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	//Setup the description of the render target views
	renderTargetViewDesc.Format = renderTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	//Create the render target views
	for (int i = 0; i< RTV_COUNT; i++) {
		hResult = device->CreateRenderTargetView(this->m_deferredT2D[i], &renderTargetViewDesc, &this->m_deferredRTV[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	//Setup the description of the shader resource view
	shaderResourceViewDesc.Format = renderTextureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	//Create the shader resource views
	for (int i = 0; i < BUFFER_COUNT -1 ; i++) { //Shadow srv is created separately
		hResult = device->CreateShaderResourceView(this->m_deferredT2D[i], &shaderResourceViewDesc, &this->m_deferredSRV[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

	// Create the depth buffer and view \\

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width =  viewPort->Width;
	depthBufferDesc.Height = viewPort->Height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.SampleDesc.Count = 1; //No MSAA
	depthBufferDesc.SampleDesc.Quality = 0;

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hResult = device->CreateTexture2D(&depthBufferDesc, NULL, &this->m_depthStencilBuffer);
	if (FAILED(hResult))
	{
		return 1;
	}

	hResult = device->CreateDepthStencilView(this->m_depthStencilBuffer, NULL, &this->m_DSV);
	if (FAILED(hResult))
	{
		return 1;
	}

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
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	hResult = device->CreateDepthStencilState(&depthStencilDesc, &this->m_DSS);
	if (FAILED(hResult))
	{
		return 1;
	}

	deviceContext->OMSetDepthStencilState(m_DSS, NULL);


#pragma region create shadowmap stuff
	

	D3D11_TEXTURE2D_DESC ShadowTexDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewShadowDesc;

	ZeroMemory(&ShadowTexDesc, sizeof(ShadowTexDesc));

	//Set up the render texture desciption

	ShadowTexDesc.Width				= (UINT)SHADOW_WIDTH;	//Resolution
	ShadowTexDesc.Height			= (UINT)SHADOW_HEIGHT;	//Resolution
	ShadowTexDesc.MipLevels			= 1;
	ShadowTexDesc.ArraySize			= 6;	//one for each axis
	ShadowTexDesc.Format			= DXGI_FORMAT_R32_TYPELESS;
	ShadowTexDesc.SampleDesc.Count   = 1;
	ShadowTexDesc.SampleDesc.Quality = 0;
	ShadowTexDesc.Usage				= D3D11_USAGE_DEFAULT;
	ShadowTexDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	ShadowTexDesc.CPUAccessFlags	= 0;
	ShadowTexDesc.MiscFlags		    = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//Create the render target Texture
	
	hResult = device->CreateTexture2D(&ShadowTexDesc, NULL, &m_deferredT2D[ShaderLib::Shadow]);
	if (FAILED(hResult))
	{
		return 1;
	}

	//create depth stencil

	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format						   = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension				   = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	descDSV.Texture2DArray.ArraySize	   = 6;
	descDSV.Texture2DArray.FirstArraySlice = 0;
	descDSV.Texture2DArray.MipSlice		   = 0;

	hResult = device->CreateDepthStencilView(m_deferredT2D[ShaderLib::Shadow], &descDSV, &m_shadowMapSV);
	if (FAILED(hResult))
		return 1;

	//Set up the shader resource view

	resourceViewShadowDesc.Format						  = DXGI_FORMAT_R32_FLOAT;
	resourceViewShadowDesc.ViewDimension				  = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceViewShadowDesc.Texture2DArray.ArraySize		  = 6;
	resourceViewShadowDesc.Texture2DArray.FirstArraySlice = 0;
	resourceViewShadowDesc.Texture2DArray.MostDetailedMip = 0;
	resourceViewShadowDesc.Texture2DArray.MipLevels		  = 1;

	//Create the resourceView;

	hResult = device->CreateShaderResourceView(m_deferredT2D[ShaderLib::Shadow], &resourceViewShadowDesc, &m_deferredSRV[ShaderLib::Shadow]);
	if (FAILED(hResult))
		return 1;

	this->m_shadowVP = new D3D11_VIEWPORT;
	m_shadowVP->Width = SHADOW_WIDTH;
	m_shadowVP->Height = SHADOW_HEIGHT;
	m_shadowVP->MinDepth = 0.0f;
	m_shadowVP->MaxDepth = 1.0f;
	m_shadowVP->TopLeftX = 0;
	m_shadowVP->TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, m_shadowVP);

#pragma endregion

	//-----------------------------------------------------------------------------------------------------------------------------------
	//Instanced geometry BUFFER
	//-----------------------------------------------------------------------------------------------------------------------------------

	D3D11_BUFFER_DESC bufferInstancedDesc;
	ZeroMemory(&bufferInstancedDesc, sizeof(bufferInstancedDesc));

	//InstancedObject buffer
	bufferInstancedDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferInstancedDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferInstancedDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferInstancedDesc.ByteWidth		= sizeof(DirectX::XMFLOAT4X4) * MAX_INSTANCED_GEOMETRY;
	HRESULT hr;
	if (FAILED(hr = device->CreateBuffer(&bufferInstancedDesc, nullptr, &m_instanceBuffer))) {
#ifdef _DEBUG
		MessageBox(NULL, L"Failed to create Instance buffer", L"Init Error", MB_ICONERROR | MB_OK);
#endif // _DEBUG
		return 1;
	}

	return 0;
}


int DeferredShader::SetActive()
{
	this->m_deviceContext->OMSetDepthStencilState(m_DSS, NULL);

	//if (m_activeVariation == ShaderVariations)
	//	return 0;
	//Shader::SetActive(ShaderVariations);

	//m_deviceContext->IASetInputLayout(this->m_layout);

	
	//this->Clear(); //clear rtv and dsv
	//Set the sampler state in pixel shader
	this->m_deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);
	this->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return 0;
}

int DeferredShader::SetVariation(ShaderLib::ShaderVariations ShaderVariations)
{
	switch (ShaderVariations)
	{
	case ShaderLib::Normal:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_NORMAL]);
		m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
		m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_NORMAL], NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		m_deviceContext->OMSetDepthStencilState(m_DSS, 0);
		//Set the render target views
		this->m_deviceContext->OMSetRenderTargets(BUFFER_COUNT - 1, this->m_deferredRTV, this->m_DSV); // -1 because one is not used
		m_shadowStateActive = false;
		m_deviceContext->RSSetViewports(1, m_viewPort);

		break;
	}
	case ShaderLib::Instanced:
		m_deviceContext->IASetInputLayout(this->m_layout[IL_INSTANCED_NORMAL]);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_INSTANCED_NORMAL], NULL, 0);
		m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);
		m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		m_deviceContext->OMSetDepthStencilState(m_DSS, 0);
		m_shadowStateActive = false;
		//Set the render target views
		this->m_deviceContext->OMSetRenderTargets(BUFFER_COUNT - 1, this->m_deferredRTV, this->m_DSV); // -1 because one is not used
		m_deviceContext->RSSetViewports(1, m_viewPort);

		break;
	case ShaderLib::Animated:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_ANIMATED]);
		m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
		m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_ANIMATED], NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::VertexAnim);
		m_deviceContext->OMSetDepthStencilState(m_DSS, 0);
		m_shadowStateActive = false;
		//Set the render target views
		this->m_deviceContext->OMSetRenderTargets(BUFFER_COUNT - 1, this->m_deferredRTV, this->m_DSV); // -1 because one is not used
		m_deviceContext->RSSetViewports(1, m_viewPort);
		break;
	}
	case ShaderLib::InstancedAnimated:
		break;
	case ShaderLib::Wireframe:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_NORMAL]);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_NORMAL], NULL, 0);
		m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);
		m_deviceContext->PSSetShader(m_gridPixelShader, NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		m_deviceContext->OMSetDepthStencilState(m_DSS, 0);
		m_shadowStateActive = false;
		//Set the render target views
		this->m_deviceContext->OMSetRenderTargets(BUFFER_COUNT - 1, this->m_deferredRTV, this->m_DSV); // -1 because one is not used
		m_deviceContext->RSSetViewports(1, m_viewPort);
		break;
	}
	case ShaderLib::Shadow:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_NORMAL]);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_NORMAL], NULL, 0);
		m_deviceContext->GSSetShader(this->m_ShadowGeoShader, NULL, 0);
		m_deviceContext->PSSetShader(nullptr, NULL, 0); //no pixel shader is used for shadows
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		m_deviceContext->OMSetDepthStencilState(NULL, 0);
		//Null the input of the shadowmap
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		this->m_deviceContext->PSSetShaderResources(10, 1, nullSRV);
		//Set the render target views
		this->m_deviceContext->OMSetRenderTargets(0, NULL, m_shadowMapSV); // no rtv for shadow map, only stencil
		m_shadowStateActive = true;
		m_deviceContext->RSSetViewports(1, m_shadowVP);

		break;
	}
	case ShaderLib::InstancedShadow:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_INSTANCED_NORMAL]);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_INSTANCED], NULL, 0);
		m_deviceContext->GSSetShader(this->m_ShadowGeoShader, NULL, 0);
		m_deviceContext->PSSetShader(nullptr, NULL, 0); //no pixel shader is used for shadows
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		m_deviceContext->OMSetDepthStencilState(NULL, 0);
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		this->m_deviceContext->PSSetShaderResources(10, 1, nullSRV);
		this->m_deviceContext->OMSetRenderTargets(0, NULL, m_shadowMapSV); // no rtv for shadow map, only stencil
		m_shadowStateActive = true;
		m_deviceContext->RSSetViewports(1, m_shadowVP);

		break;
	}
	case ShaderLib::AnimatedShadow:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_ANIMATED]);
		m_deviceContext->VSSetShader(this->m_vertexShader[VERTEX_SHADERS::VS_SHADOW_ANIMATED], NULL, 0);
		m_deviceContext->GSSetShader(this->m_ShadowGeoShader, NULL, 0);
		m_deviceContext->PSSetShader(nullptr, NULL, 0); //no pixel shader is used for shadows
		m_vertexSize = sizeof(Resources::Mesh::VertexAnim);
		m_deviceContext->OMSetDepthStencilState(NULL, 0);
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		this->m_deviceContext->PSSetShaderResources(10, 1, nullSRV);
		this->m_deviceContext->OMSetRenderTargets(0, NULL, m_shadowMapSV); // no rtv for shadow map, only stencil
		m_shadowStateActive = true;
		m_deviceContext->RSSetViewports(1, m_shadowVP);

		break;
	}
	default:
		break;
	}

	return 0;
}

void DeferredShader::SetShadowDataToRead()
{

	this->m_deviceContext->PSSetShaderResources(10, 1, &m_deferredSRV[ShaderLib::Shadow]);
}

void DeferredShader::Release()
{
	Shader::Release();

	for (int i = 0; i < VS_NUM_VERTEX_SHADERS; i++)
	{
		if (this->m_vertexShader[i])
		{
			this->m_vertexShader[i]->Release();
			this->m_vertexShader[i] = nullptr;
		}
	}
	if (this->m_geoShader)
	{
		this->m_geoShader->Release();
		this->m_geoShader = nullptr;
	}
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}
	for (int i = 0; i < IL_TYPE_COUNT; i++)
	{
		if (this->m_layout[i])
		{
			this->m_layout[i]->Release();
			this->m_layout[i] = nullptr;
		}
	}
	if (m_instanceBuffer)
	{
		m_instanceBuffer->Release();
		m_instanceBuffer = nullptr;
	}
	if (m_shadowMapSV)
	{
		m_shadowMapSV->Release();
		m_shadowMapSV = nullptr;
	}
	if (m_ShadowGeoShader)
	{
		m_ShadowGeoShader->Release();
		m_ShadowGeoShader = nullptr;
	}

	//Release the sampler state
	if (this->m_samplerState)
	{
		this->m_samplerState->Release();
		this->m_samplerState = nullptr;
	}
	for (size_t i = 0; i < RTV_COUNT; i++)
	{
		if (this->m_deferredRTV[i]) {
			this->m_deferredRTV[i]->Release();
			this->m_deferredRTV[i] = nullptr;
		}

	}
	//Release the deferred render targets
	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (this->m_deferredT2D[i]) {
			this->m_deferredT2D[i]->Release();
			this->m_deferredT2D[i] = nullptr;
		}
		if (this->m_deferredSRV[i]) {
			this->m_deferredSRV[i]->Release();
			this->m_deferredSRV[i] = nullptr;
		}
		if (this->m_gridPixelShader)
		{
			this->m_gridPixelShader->Release();
			this->m_gridPixelShader = nullptr;
		}
	}

	if (this->m_shadowVP != nullptr)
		delete this->m_shadowVP;
}


int DeferredShader::Draw(Resources::Model * model)
{
	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf		 = meshPtr->GetVerticesBuffer();
	//ID3D11Buffer* vBuf = meshPtr->GetAnimVerticesBuffer();
	ID3D11Buffer* iBuf		 = meshPtr->GetIndicesBuffer();
	UINT32 offset			 = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &m_vertexSize, &offset);
	this->m_deviceContext->IASetIndexBuffer(iBuf, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	if (!m_shadowStateActive)
	{
		Resources::Material * mat     = model->GetMaterial();
	if (mat == nullptr)
	{
		Resources::ResourceHandler::GetInstance()->GetModel(1337, model);
		mat = model->GetMaterial();
	}
		Resources::Texture** textures = mat->GetAllTextures();
		ID3D11ShaderResourceView* resViews[5];
		UINT numViews = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (textures[i] == nullptr)
				continue;

			resViews[numViews] = textures[i]->GetResourceView();
			numViews += 1;
		}


		this->m_deviceContext->PSSetShaderResources(0, numViews, resViews);
	}
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->m_deviceContext->DrawIndexed(meshPtr->GetNumIndices(), 0, 0);


	return 0;
}

int DeferredShader::Draw(Resources::Model * model, GraphicsComponent * component)
{
#ifdef _DEBUG
	if (model == nullptr) 												   //This is a check, If a component has no ptr to a model
		Resources::ResourceHandler::GetInstance()->GetModel(1337, model);  //This should never occur, But with hardcoded test components in the system
																		   //means that this is needed. This snippet should be removed in a later stage. As level loading fixes these problems
#endif // _DEBUG
	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf		 = meshPtr->GetVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &m_vertexSize, &offset);
	this->m_deviceContext->IASetIndexBuffer(iBuf, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	if (!m_shadowStateActive)
	{
		Resources::Material * mat = model->GetMaterial();
	if (mat == nullptr)
	{
		Resources::ResourceHandler::GetInstance()->GetModel(1337, model);
		mat = model->GetMaterial();
	}
		Resources::Texture** textures = mat->GetAllTextures();
		ID3D11ShaderResourceView* resViews[5];
		UINT numViews = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (textures[i] == nullptr)
				continue;

			resViews[numViews] = textures[i]->GetResourceView();
			numViews += 1;
		}


		this->m_deviceContext->PSSetShaderResources(0, numViews, resViews);
	}

	this->m_deviceContext->DrawIndexed(meshPtr->GetNumIndices(), 0, 0);


	return 0;
}

int DeferredShader::Draw(Resources::Model * model, GraphicsAnimationComponent * component)
{

	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	ConstantBufferHandler::GetInstance()->skeleton.UpdateBuffer(&component->finalJointTransforms);
	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf = meshPtr->GetAnimVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &m_vertexSize, &offset);
	this->m_deviceContext->IASetIndexBuffer(iBuf, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	if (!m_shadowStateActive)
	{
		Resources::Material * mat = model->GetMaterial();
	if (mat == nullptr)
	{
		Resources::ResourceHandler::GetInstance()->GetModel(1337, model);
		mat = model->GetMaterial();
	}
		Resources::Texture** textures = mat->GetAllTextures();
		ID3D11ShaderResourceView* resViews[5];
		UINT numViews = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (textures[i] == nullptr)
				continue;

			resViews[numViews] = textures[i]->GetResourceView();
			numViews += 1;
		}


		this->m_deviceContext->PSSetShaderResources(0, numViews, resViews);
	}

	this->m_deviceContext->DrawIndexed(meshPtr->GetNumIndices(), 0, 0);


	return 0;
}

int DeferredShader::DrawInstanced(InstanceData* data , int iteration)
{
	Resources::Model* model = data->modelPtr;
	
	//Resources::ResourceHandler::GetInstance()->GetModel(data->modelID, model);
	if (iteration == 0 && m_shadowStateActive == false)
	{
		Resources::Material * mat = model->GetMaterial();
		if (mat == nullptr)
		{
			Resources::ResourceHandler::GetInstance()->GetModel(1337, model);
			mat = model->GetMaterial();
		}
		Resources::Texture** textures = mat->GetAllTextures();
		ID3D11ShaderResourceView* resViews[5];
		UINT numViews = 0;
		for (size_t i = 0; i < 5; i++)
		{
			if (textures[i] == nullptr)
				continue;

			resViews[numViews] = textures[i]->GetResourceView();
			numViews += 1;
		}


		this->m_deviceContext->PSSetShaderResources(0, numViews, resViews);
	}

	DirectX::XMFLOAT4X4* matrixData = data->componentSpecific;
	int numInstances				= data->amountOfInstances;  // can be changed if the limit is exceeded
//	if (data->amountOfInstances > MAX_INSTANCED_GEOMETRY) //if we've reached the limit, split it up into multiple render passes
//	{
//		numInstances = MAX_INSTANCED_GEOMETRY; //Change the amount for the current pass to MAX_INSTANCED_GEOMETRY
//
//		InstanceData newBatch;
//		newBatch.modelID		   = data->modelID;
//		newBatch.componentSpecific = data->componentSpecific + MAX_INSTANCED_GEOMETRY; // Get a pointer to where it stopped rendering
//		newBatch.amountOfInstances = data->amountOfInstances - MAX_INSTANCED_GEOMETRY; // Discount the amount that has been rendered
//		DrawInstanced(&newBatch, iteration + 1 );
//#ifdef _DEBUG
//		std::cout << "The instance buffer has reached its limit, splitting the rendering up into another draw call| Iteration :"<< iteration + 1 << std::endl;
//#endif // _DEBUG
//	}

#pragma region
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	HRESULT hr;
	hr = m_deviceContext->Map(m_instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		std::cout << "Failed to map the instance buffer" << std::endl;
#endif // _DEBUG

		return 0;
	}

	DirectX::XMFLOAT4X4* tempStructMatrices = (DirectX::XMFLOAT4X4*)mappedResource.pData;
	memset(mappedResource.pData, 0, sizeof(DirectX::XMFLOAT4X4) * MAX_INSTANCED_GEOMETRY);
	memcpy(tempStructMatrices, (void*)matrixData, sizeof(DirectX::XMFLOAT4X4)* numInstances);

	m_deviceContext->Unmap(m_instanceBuffer, 0);
#pragma endregion Map the data to the instance buffer

	ID3D11Buffer* vertBuffer  = model->GetMesh()->GetVerticesBuffer();
	ID3D11Buffer* indexBuffer = model->GetMesh()->GetIndicesBuffer();
	
	UINT32 offset[2]     = { 0,0 };
	UINT32 size[2];
	size[0] = sizeof(Resources::Mesh::Vertex); //Size of each vertex
	size[1] = sizeof(DirectX::XMFLOAT4X4);	  //size of each instance

	ID3D11Buffer* vbs[2] = { vertBuffer,m_instanceBuffer};
		
	this->m_deviceContext->IASetVertexBuffers(0, 2, vbs, size, offset);
	this->m_deviceContext->IASetIndexBuffer(indexBuffer ,DXGI_FORMAT_R32_UINT, 0);
	this->m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_deviceContext->DrawIndexedInstanced(model->GetMesh()->GetNumIndices(), numInstances, 0, 0, 0);
	return 0;
}


int DeferredShader::Clear() //clears RTVs and DSV
{
	float color[4];

	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	//Clear the render target textures
	for (int i = 0; i < RTV_COUNT; i++) {
		m_deviceContext->ClearRenderTargetView(this->m_deferredRTV[i], color);
	}

	//Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(this->m_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_deviceContext->ClearDepthStencilView(this->m_shadowMapSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return 0;
}

ID3D11ShaderResourceView ** DeferredShader::GetShaderResourceViews()
{
	return this->m_deferredSRV;
}



int DeferredShader::DrawGrid(Resources::Model * model) //depricated
{
	m_deviceContext->PSSetShader(this->m_gridPixelShader, nullptr, NULL);


	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf = meshPtr->GetVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 size = sizeof(Resources::Mesh::Vertex);
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &size, &offset);
	this->m_deviceContext->IASetIndexBuffer(iBuf, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);


	this->m_deviceContext->DrawIndexed(meshPtr->GetNumIndices(), 0, 0);


	m_deviceContext->PSSetShader(this->m_pixelShader, nullptr, NULL);

	return 0;
}

int DeferredShader::DrawFromEditor(Resources::Model * model)
{
	m_deviceContext->PSSetShader(this->m_pixelShader, nullptr, NULL);

	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf = meshPtr->GetVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 size = sizeof(Resources::Mesh::Vertex);
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &size, &offset);
	this->m_deviceContext->IASetIndexBuffer(iBuf, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	Resources::Material * mat = model->GetMaterial();
	Resources::Texture** textures = mat->GetAllTextures();
	ID3D11ShaderResourceView* resViews[5];
	UINT numViews = 0;
	for (size_t i = 0; i < 5; i++)
	{
		if (textures[i] == nullptr)
			continue;

		resViews[numViews] = textures[i]->GetResourceView();
		numViews += 1;
	}


	this->m_deviceContext->PSSetShaderResources(0, numViews, resViews);

	this->m_deviceContext->DrawIndexed(meshPtr->GetNumIndices(), 0, 0);

	m_deviceContext->PSSetShader(this->m_pixelShader, nullptr, NULL);
	return 0;
}

int DeferredShader::InitializeGridShader(ID3D11Device * device)
{

	HRESULT hResult;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;




	//Insert shader path here
	WCHAR* psFilename = L"../GraphicsDLL/Shaders/GBuffer/GridPS.hlsl";

	// Compile the shaders \\

#ifdef _DEBUG
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
#else
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_OPTIMIZATION_LEVEL3, 0, &pixelShaderBuffer, &errorMessage);
#endif // _DEBUG

	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, psFilename);
		return 1;
	}

	// Create the shaders \\

	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_gridPixelShader);
	if (FAILED(hResult)) {
		return 1;
	}


	return 0;
}
