#include "DeferredShader.h"

DeferredShader::DeferredShader() : Shader()
{
	this->m_samplerState = nullptr;

	for (int i = 0; i < 4; i++)
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
		this->m_deferredRTV[i] = nullptr;
		this->m_deferredSRV[i] = nullptr;
	}

	this->m_DSV = nullptr;
}


DeferredShader::~DeferredShader()
{
}

int DeferredShader::Initialize(ID3D11Device* device,  ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution)
{
	HRESULT hResult;
	ID3D10Blob* vertexShaderBuffer[4] = { nullptr };
	ID3D10Blob* geoShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;

	this->m_deviceContext = deviceContext;


	//Insert shader path here
	WCHAR* vsFilename = L"../GraphicsDLL/Shaders/GBuffer/GBufferVS.hlsl";
	WCHAR* vsAnimFilename = L"../GraphicsDLL/Shaders/GBuffer/AnimVS.hlsl";
	WCHAR* gsFilename = L"../GraphicsDLL/Shaders/GBuffer/GBuffer.hlsl";
	WCHAR* psFilename = L"../GraphicsDLL/Shaders/GBuffer/GBuffer.hlsl";

	// Compile the shaders \\

	hResult = D3DCompileFromFile(vsFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[ShaderLib::Normal], &errorMessage);
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);

		return 1;
	}
	hResult = D3DCompileFromFile(vsAnimFilename, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertexShaderBuffer[ShaderLib::Animated], &errorMessage);
	if (FAILED(hResult))
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsAnimFilename);

		return 1;
	}
	hResult = D3DCompileFromFile(gsFilename, NULL, NULL, "GS_main", "gs_5_0", D3D10_SHADER_DEBUG, 0, &geoShaderBuffer, &errorMessage);
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}
	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(hResult)) 
	{
		Shader::OutputShaderErrorMessage(errorMessage, vsFilename);
		return 1;
	}

	// Create the shaders \\

	hResult = device->CreateVertexShader(vertexShaderBuffer[ShaderLib::Normal]->GetBufferPointer(), vertexShaderBuffer[ShaderLib::Normal]->GetBufferSize(), NULL, &this->m_vertexShader[ShaderLib::Normal]);
	if (FAILED(hResult)) 
	{
		return 1;
	}
	hResult = device->CreateVertexShader(vertexShaderBuffer[ShaderLib::Animated]->GetBufferPointer(), vertexShaderBuffer[ShaderLib::Animated]->GetBufferSize(), NULL, &this->m_vertexShader[ShaderLib::Animated]);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreateGeometryShader(geoShaderBuffer->GetBufferPointer(), geoShaderBuffer->GetBufferSize(), NULL, &this->m_geoShader);
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
	hResult = device->CreateInputLayout(inputDescNormal, numElements, vertexShaderBuffer[0]->GetBufferPointer(), vertexShaderBuffer[0]->GetBufferSize(), &this->m_layout[IL_NORMAL]);
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
	hResult = device->CreateInputLayout(inputDescAnim, numElements, vertexShaderBuffer[0]->GetBufferPointer(), vertexShaderBuffer[0]->GetBufferSize(), &this->m_layout[IL_ANIMATED]);
	if (FAILED(hResult)) {
		return 1;
	}

	//Release and nullptr the buffers as they are no longer needed
	vertexShaderBuffer[0]->Release();
	vertexShaderBuffer[0] = nullptr;
	geoShaderBuffer->Release();
	geoShaderBuffer = nullptr;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;


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
	renderTextureDesc.Width = resolution.x;
	renderTextureDesc.Height = resolution.y;
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
	for (int i = 0; i < BUFFER_COUNT; i++) {
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
	for (int i = 0; i<BUFFER_COUNT; i++) {
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
	for (int i = 0; i < BUFFER_COUNT; i++) {
		hResult = device->CreateShaderResourceView(this->m_deferredT2D[i], &shaderResourceViewDesc, &this->m_deferredSRV[i]);
		if (FAILED(hResult)) {
			return 1;
		}
	}

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
	return 0;
}


int DeferredShader::SetActive()
{
	//if (m_activeVariation == ShaderVariations)
	//	return 0;
	//Shader::SetActive(ShaderVariations);

	//m_deviceContext->IASetInputLayout(this->m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	//m_deviceContext->VSSetShader(this->m_vertexShader[0], NULL, 0);
	m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);


	//this->Clear(); //clear rtv and dsv
	//Set the sampler state in pixel shader
	this->m_deviceContext->PSSetSamplers(0, 1, &this->m_samplerState);

	//Set the render target views
	this->m_deviceContext->OMSetRenderTargets(BUFFER_COUNT, this->m_deferredRTV, this->m_DSV);




	return 0;
}

int DeferredShader::SetVariation(ShaderLib::ShaderVariations ShaderVariations)
{
	switch (ShaderVariations)
	{
	case ShaderLib::Normal:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_NORMAL]);
		m_deviceContext->VSSetShader(this->m_vertexShader[ShaderLib::Normal], NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::Vertex);
		break;
	}
	case ShaderLib::Instanced:
		break;
	case ShaderLib::Animated:
	{
		m_deviceContext->IASetInputLayout(this->m_layout[IL_ANIMATED]);
		m_deviceContext->VSSetShader(this->m_vertexShader[ShaderLib::Animated], NULL, 0);
		m_vertexSize = sizeof(Resources::Mesh::VertexAnim);
		break;
	}
	case ShaderLib::InstancedAnimated:
		break;
	case ShaderLib::Grid:
		break;
	default:
		break;
	}

	return 0;
}

void DeferredShader::Release()
{
	Shader::Release();

	for (int i = 0; i < 4; i++)
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

	//Release the sampler state
	if (this->m_samplerState)
	{
		this->m_samplerState->Release();
		this->m_samplerState = nullptr;
	}
	//Release the deferred render targets
	for (int i = 0; i < BUFFER_COUNT; i++) {
		if (this->m_deferredT2D[i]) {
			this->m_deferredT2D[i]->Release();
			this->m_deferredT2D[i] = nullptr;
		}
		if (this->m_deferredRTV[i]) {
			this->m_deferredRTV[i]->Release();
			this->m_deferredRTV[i] = nullptr;
		}
		if (this->m_deferredSRV[i]) {
			this->m_deferredSRV[i]->Release();
			this->m_deferredSRV[i] = nullptr;
		}
		if (this->m_gridPixelShader)
			this->m_gridPixelShader->Release();
	}
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

	Resources::Material * mat     = model->GetMaterial();
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


	return 0;
}

int DeferredShader::Draw(Resources::Model * model, GraphicsComponent * component)
{
	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf		 = meshPtr->GetVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &m_vertexSize, &offset);
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


	return 0;
}

int DeferredShader::Draw(Resources::Model * model, penis * component)
{
	ConstantBufferHandler::GetInstance()->world.UpdateBuffer(&component->worldMatrix);
	ConstantBufferHandler::GetInstance()->skeleton.UpdateBuffer(&component->finalTransforms);
	Resources::Mesh* meshPtr = model->GetMesh();
	ID3D11Buffer* vBuf = meshPtr->GetAnimVerticesBuffer();
	ID3D11Buffer* iBuf = meshPtr->GetIndicesBuffer();
	UINT32 offset = 0;
	this->m_deviceContext->IASetVertexBuffers(0, 1, &vBuf, &m_vertexSize, &offset);
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
	for (int i = 0; i < BUFFER_COUNT; i++) {
		m_deviceContext->ClearRenderTargetView(this->m_deferredRTV[i], color);
	}

	//Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(this->m_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return 0;
}

ID3D11ShaderResourceView ** DeferredShader::GetShaderResourceViews()
{
	return this->m_deferredSRV;
}


int DeferredShader::DrawInstanced(/*RESOURCE*/ /*INSTANCE_COUNT*/)
{
	return 0;
}

int DeferredShader::DrawGrid(Resources::Model * model)
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
	ID3D10Blob* vertexShaderBuffer[4] = { nullptr };
	ID3D10Blob* geoShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage;




	//Insert shader path here
	WCHAR* psFilename = L"../GraphicsDLL/Shaders/GBuffer/GridPS.hlsl";

	// Compile the shaders \\

	hResult = D3DCompileFromFile(psFilename, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
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
