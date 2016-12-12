#include "DebugRenderer.h"



DebugRenderer::DebugRenderer()
{
}


DebugRenderer::~DebugRenderer()
{
}

int DebugRenderer::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, const DirectX::XMINT2 & resolution)
{
	HRESULT hResult;
	WCHAR* filePathVS = L"../GraphicsDLL/Shaders/Debug/DebugVS.hlsl";
	WCHAR* filePathPS = L"../GraphicsDLL/Shaders/Debug/DebugPS.hlsl";
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertShaderBuffer  = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	this->m_device				  = device;
	this->m_deviceContext		  = deviceContext;
	// Compile the shaders \\

	hResult = D3DCompileFromFile(filePathVS, NULL, NULL, "VS_main", "vs_5_0", D3D10_SHADER_DEBUG, 0, &vertShaderBuffer, &errorMessage);
	if (FAILED(hResult))
	{
		return 1;
	}


	this->m_pixelShader = nullptr;
	hResult = D3DCompileFromFile(filePathPS, NULL, NULL, "PS_main", "ps_5_0", D3D10_SHADER_DEBUG, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(hResult)) {
		return 1;
	}

	hResult = device->CreateVertexShader(vertShaderBuffer->GetBufferPointer(), vertShaderBuffer->GetBufferSize(), NULL, &this->m_vertexShader);
	if (FAILED(hResult))
	{
		return 1;
	}
	hResult = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->m_pixelShader);
	if (FAILED(hResult)) {
		return 1;
	}

	D3D11_INPUT_ELEMENT_DESC inputDescNormal;
	inputDescNormal.SemanticName		 = "POSITION";
	inputDescNormal.SemanticIndex		 = 0;
	inputDescNormal.Format				 = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal.InputSlot			 = 0;
	inputDescNormal.AlignedByteOffset    = 0;
	inputDescNormal.InputSlotClass		 = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal.InstanceDataStepRate = 0;

	hResult = device->CreateInputLayout(&inputDescNormal, 1, vertShaderBuffer->GetBufferPointer(), vertShaderBuffer->GetBufferSize(), &this->m_layout);
	if (FAILED(hResult)) {
		return 1;
	}
	cubePoints[0] = Point(-0.5, 0.5, 0.5);		//0
	cubePoints[1] = Point(-0.5, -0.5, 0.5);		//1
	cubePoints[2] = Point(0.5, -0.5, 0.5);		//2
	cubePoints[3] = Point(0.5, 0.5, 0.5);		//3
	cubePoints[4] = Point(0.5, -0.5, -0.5);		//4
	cubePoints[5] = Point(0.5, 0.5, -0.5);		//5
	cubePoints[6] = Point(-0.5, 0.5, -0.5);		//6
	cubePoints[7] = Point(-0.5, -0.5, -0.5);	//7
	D3D11_BUFFER_DESC vertexBufferDesc;

	D3D11_SUBRESOURCE_DATA vertexData;

	//Set the description of the static vertex buffer
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth           = sizeof(Point) * NUM_POINTS;
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags		     = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource structure a pointer to the vertex data
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &cubePoints;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_pointBuffer);
	if (FAILED(hResult)) {
		return 1;
	}
	UINT indices[NUM_INDICES] =
	{
		0,1,
		0,3,
		0,6,
		1,7,
		1,2,
		2,3,
		2,4,
		3,5,
		5,6,
		5,4,
		4,7,
		6,7,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	//Set up the description of the static index buffer
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * NUM_INDICES;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &this->m_boxIndexBuffer);
	if (FAILED(hResult)) {
		return 1;
	}

	return 0;
}

void DebugRenderer::Release()
{
	if (this->m_vertexShader)
	{
		this->m_vertexShader->Release();
		this->m_vertexShader = nullptr;
	}
	if (this->m_pixelShader)
	{
		this->m_pixelShader->Release();
		this->m_pixelShader = nullptr;
	}
	if (this->m_layout)
	{
		this->m_layout->Release();
		this->m_layout = nullptr;
	}

	if (this->m_pointBuffer)
	{		  
		this->m_pointBuffer->Release();
		this->m_pointBuffer = nullptr;
	}
	if (this->m_boxIndexBuffer)
	{
		this->m_boxIndexBuffer->Release();
		this->m_boxIndexBuffer = nullptr;
	}

}

void DebugRenderer::Render(AABB & box)
{
	ID3D11Buffer* buf = GenerateLinelist(box);
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);
	m_deviceContext->IASetVertexBuffers(0, 1, &buf, &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES, 0, 0);
}

void DebugRenderer::Render(OBB & box)
{

	ID3D11Buffer* buf = GenerateLinelist(box);
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);
	m_deviceContext->IASetVertexBuffers(0, 1, &buf, &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES, 0, 0);
}

void DebugRenderer::SetActive()
{
	m_deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

ID3D11Buffer * DebugRenderer::GenerateLinelist(AABB & box)
{
	//Create the max points
	Point maxX, minX, maxY, minY, maxZ, minZ;

	cubePoints[0] =	Point(-0.5,  0.5,  0.5);		//0
	cubePoints[1] =	Point(-0.5, -0.5,  0.5);		//1
	cubePoints[2] =	Point(0.5,  -0.5,  0.5);		//2
	cubePoints[3] =	Point(0.5,   0.5,  0.5);		//3
	cubePoints[4] =	Point(0.5 ,	 -0.5, -0.5);		//4
	cubePoints[5] =	Point(0.5 ,	 0.5, -0.5);		//5
	cubePoints[6] =	Point(-0.5 , 0.5, -0.5);		//6
	cubePoints[7] =	Point(-0.5 , -0.5, -0.5);		//7
	

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_deviceContext->Map(m_pointBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)cubePoints, sizeof(Point)*NUM_POINTS);
	m_deviceContext->Unmap(m_pointBuffer, 0);

	return m_pointBuffer;

}

ID3D11Buffer * DebugRenderer::GenerateLinelist(OBB & box)
{
	
	//Create the max points
	Point maxX, minX, maxY, minY, maxZ, minZ;
	//see end of file
	cubePoints[0] = Point(-0.5, 0.5, 0.5);		//0
	cubePoints[1] = Point(-0.5, -0.5, 0.5);		//1
	cubePoints[2] = Point(0.5, -0.5, 0.5);		//2
	cubePoints[3] = Point(0.5, 0.5, 0.5);		//3
	cubePoints[4] = Point(0.5, -0.5, -0.5);		//4
	cubePoints[5] = Point(0.5, 0.5, -0.5);		//5
	cubePoints[6] = Point(-0.5, 0.5, -0.5);		//6
	cubePoints[7] = Point(-0.5, -0.5, -0.5);	//7




	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_deviceContext->Map(m_pointBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)cubePoints, sizeof(Point)*NUM_POINTS);
	m_deviceContext->Unmap(m_pointBuffer, 0);

	return m_pointBuffer;



}
	/*
		 _________________________
      6 / _____________________  /|5
       / / ___________________/ / |
      / / /| |               / /  |
     / / / | |              / / . |
    / / /| | |             / / /| |
   / / / | | |            / / / | |
  / / /  | | |           / / /| | |
 / /_/__________________/ / / | | |
/0______________________3/ /  | | |
| ______________________ | |  | | |
| | |    | | |_________| | |__| | |
| | |  7 | |___________| | |____| |4
| | |   / / ___________| | |_  / /
| | |  / / /           | | |/ / /
| | | / / /            | | | / /
| | |/ / /             | | |/ /
| | | / /              | | ' /
| | |/_/_______________| |  /
| |____________________| | /
|1_______________________2/
	*/