#include "DebugRenderer.h"



DebugRenderer::DebugRenderer()
{


	NUM_POINTS[M_CUBE]    = 8;
	NUM_INDICES[M_CUBE]   = 24;

	NUM_POINTS[M_PLANE]   = 4;
	NUM_INDICES[M_PLANE]  = 6;

	NUM_POINTS[M_SPHERE]  = 24;
	NUM_INDICES[M_SPHERE] = 24;


	m_indices[M_CUBE] = new UINT[NUM_INDICES[M_CUBE]]
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

	m_indices[M_PLANE] = new UINT[NUM_INDICES[M_PLANE]]
	{
		0,1,2,
		2,1,3
		
	};
	m_indices[M_SPHERE] = new UINT[NUM_INDICES[M_SPHERE]]
	{
		0,1,
		2,3,
		4,5,
		6,7,
		8,9,
		10,11,
		12,13,
		14,15,
		16,17,
		18,19,
		20,21,
		22,23
	};
}


DebugRenderer::~DebugRenderer()
{

	for (size_t i = 0; i < M_NUM_TYPES; i++)
	{
		delete[] m_points[i];
		delete[] m_indices[i];
	}
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


	D3D11_INPUT_ELEMENT_DESC inputDescNormal[2];
	inputDescNormal[0].SemanticName = "POSITION";
	inputDescNormal[0].SemanticIndex = 0;
	inputDescNormal[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal[0].InputSlot = 0;
	inputDescNormal[0].AlignedByteOffset = 0;
	inputDescNormal[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[0].InstanceDataStepRate = 0;

	inputDescNormal[1].SemanticName = "COLOR";
	inputDescNormal[1].SemanticIndex = 0;
	inputDescNormal[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputDescNormal[1].InputSlot = 0;
	inputDescNormal[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputDescNormal[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputDescNormal[1].InstanceDataStepRate = 0;

	hResult = device->CreateInputLayout(inputDescNormal, ARRAYSIZE(inputDescNormal), vertShaderBuffer->GetBufferPointer(), vertShaderBuffer->GetBufferSize(), &this->m_layout);
	if (FAILED(hResult)) {
		return 1;
	}


	for (size_t i = 0; i < M_NUM_TYPES; i++)
	{
		m_points[i] = new Point[NUM_POINTS[i]];
		D3D11_BUFFER_DESC vertexBufferDesc;

		D3D11_SUBRESOURCE_DATA vertexData;

		//Set the description of the static vertex buffer
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(Point) * NUM_POINTS[i];
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		//Give the subresource structure a pointer to the vertex data
		ZeroMemory(&vertexData, sizeof(vertexData));
		vertexData.pSysMem = &m_points[i];
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		//Create the vertex buffer
		hResult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &this->m_PointBuffer[i]);
		if (FAILED(hResult)) {
			return 1;
		}
		

		D3D11_BUFFER_DESC indexBufferDesc;
		D3D11_SUBRESOURCE_DATA indexData;

		//Set up the description of the static index buffer
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * NUM_INDICES[i];
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		ZeroMemory(&indexData, sizeof(indexData));
		indexData.pSysMem = m_indices[i];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//Create the index buffer
		hResult = device->CreateBuffer(&indexBufferDesc, &indexData, &this->m_IndexBuffer[i]);
		if (FAILED(hResult)) {
			return 1;
		}
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
	for (size_t i = 0; i < M_NUM_TYPES; i++)
	{
		if (this->m_PointBuffer[i])
		{		  
			this->m_PointBuffer[i]->Release();
			this->m_PointBuffer[i] = nullptr;
		}
		if (this->m_IndexBuffer[i])
		{
			this->m_IndexBuffer[i]->Release();
			this->m_IndexBuffer[i] = nullptr;
		}
	}

}

void DebugRenderer::Render(DirectX::XMVECTOR& pos,AABB & box, DirectX::XMVECTOR color)
{
	ID3D11Buffer* buf = GenerateLinelist(pos,box,color);
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);
	m_deviceContext->IASetVertexBuffers(0, 1, &buf, &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_CUBE], DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES[M_CUBE], 0, 0);
}

void DebugRenderer::Render(DirectX::XMVECTOR& pos,OBB & box, DirectX::XMVECTOR color)
{

	ID3D11Buffer* buf = GenerateLinelist(pos,box,color);
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_PointBuffer[M_CUBE], &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_CUBE], DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES[M_CUBE], 0, 0);
}

void DebugRenderer::Render(DirectX::XMVECTOR & pos, Plane & plane, DirectX::XMVECTOR color)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	ID3D11Buffer* buf = GenerateLinelist(pos, plane, color);
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_PointBuffer[M_PLANE], &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_PLANE], DXGI_FORMAT_R32_UINT, 0);
	
	m_deviceContext->DrawIndexed(NUM_INDICES[M_PLANE], 0, 0);
}


void DebugRenderer::Render(DirectX::XMVECTOR & pos, Sphere & sphere, DirectX::XMVECTOR color)
{
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	ID3D11Buffer* buf;
	UINT32 offset = 0;
	UINT32 m_vertexSize = sizeof(Point);

	//ring 1

	buf = GenerateLinelist(pos, sphere, color, 1);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_PointBuffer[M_SPHERE], &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_SPHERE], DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES[M_SPHERE], 0, 0);

	//ring 2

	buf = GenerateLinelist(pos, sphere, color, 2);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_PointBuffer[M_SPHERE], &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_SPHERE], DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES[M_SPHERE], 0, 0);

	//ring 3
	
	buf = GenerateLinelist(pos, sphere, color, 3);
	m_deviceContext->IASetVertexBuffers(0, 1, &m_PointBuffer[M_SPHERE], &m_vertexSize, &offset);
	m_deviceContext->IASetIndexBuffer(this->m_IndexBuffer[M_SPHERE], DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->DrawIndexed(NUM_INDICES[M_SPHERE], 0, 0);

}

void DebugRenderer::SetActive()
{
	m_deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	m_deviceContext->IASetInputLayout(this->m_layout);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

ID3D11Buffer * DebugRenderer::GenerateLinelist(DirectX::XMVECTOR& pos,AABB & box, DirectX::XMVECTOR color)
{
	

	static DirectX::XMVECTOR normXDir{ 1.0f, 0.0f, 0.0f };
	static DirectX::XMVECTOR normYDir{ 0.0f, 1.0f, 0.0f };
	static DirectX::XMVECTOR normZDir{ 0.0f, 0.0f, 1.0f };

	DirectX::XMVECTOR xDir ;
	DirectX::XMVECTOR yDir ;
	DirectX::XMVECTOR zDir ;

	DirectX::XMVECTOR zDirInv;
	xDir = DirectX::XMVectorScale(normXDir, box.ext[0]);
	yDir = DirectX::XMVectorScale(normYDir, box.ext[1]);
	zDir = DirectX::XMVectorScale(normZDir, box.ext[2]);

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslationFromVector(pos);

	//Create the points.
	//see end of file
	DirectX::XMVECTOR point;
	point = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(zDir, yDir), xDir);			// z + y - x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][0] = Point(point.m128_f32, color.m128_f32);		//0

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(zDir, yDir), xDir);		// z - y - x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][1] = Point(point.m128_f32, color.m128_f32);		//1

	point = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(zDir, yDir), xDir);			// z - y + x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][2] = Point(point.m128_f32, color.m128_f32);		//2

	point = DirectX::XMVectorAdd(DirectX::XMVectorAdd(zDir, yDir), xDir);				// z + y + x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][3] = Point(point.m128_f32, color.m128_f32);		//3

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(xDir, zDir), yDir);		//x - z - y
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][4] = Point(point.m128_f32, color.m128_f32);		//4

	point = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(xDir, zDir), yDir);			//x - z + y
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][5] = Point(point.m128_f32, color.m128_f32);		//5

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(yDir, xDir), zDir);		//y - x - z
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][6] = Point(point.m128_f32, color.m128_f32);		//6

	zDirInv = DirectX::XMVectorScale(zDir, -1);
	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(zDirInv, xDir), yDir);  //- y - x - z
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][7] = Point(point.m128_f32, color.m128_f32);	    //7





	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = m_deviceContext->Map(m_PointBuffer[M_CUBE], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)m_points[M_CUBE], sizeof(Point) * NUM_POINTS[M_CUBE]);
	m_deviceContext->Unmap(m_PointBuffer[M_CUBE], 0);

	return m_PointBuffer[M_CUBE];

}

ID3D11Buffer * DebugRenderer::GenerateLinelist(DirectX::XMVECTOR& pos,OBB & box, DirectX::XMVECTOR color)
{
	
	DirectX::XMVECTOR xDir = box.ort.r[0];
	DirectX::XMVECTOR yDir = box.ort.r[1];
	DirectX::XMVECTOR zDir = box.ort.r[2];
	
	DirectX::XMVECTOR zDirInv;
	xDir = DirectX::XMVectorScale(xDir, box.ext[0]);
	yDir = DirectX::XMVectorScale(yDir, box.ext[1]);
	zDir = DirectX::XMVectorScale(zDir, box.ext[2]);

	DirectX::XMMATRIX worldMatrix =  DirectX::XMMatrixTranslationFromVector(pos);
	
	//Create the points.
	//see end of file
	DirectX::XMVECTOR point;
	point = DirectX::XMVectorSubtract(DirectX::XMVectorAdd(zDir, yDir), xDir);			// z + y - x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][0] = Point(point.m128_f32, color.m128_f32);		//0

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(zDir, yDir), xDir);		// z - y - x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][1] = Point(point.m128_f32, color.m128_f32);		//1

	point = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(zDir, yDir), xDir);			// z - y + x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][2] = Point(point.m128_f32, color.m128_f32);		//2

	point = DirectX::XMVectorAdd(DirectX::XMVectorAdd(zDir, yDir), xDir);				// z + y + x
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][3] = Point(point.m128_f32, color.m128_f32);		//3

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(xDir, zDir), yDir);		//x - z - y
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][4] = Point(point.m128_f32, color.m128_f32);		//4

	point = DirectX::XMVectorAdd(DirectX::XMVectorSubtract(xDir, zDir), yDir);			//x - z + y
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][5] = Point(point.m128_f32, color.m128_f32);		//5

	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(yDir,xDir), zDir);		//y - x - z
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][6] = Point(point.m128_f32, color.m128_f32);		//6

	zDirInv = DirectX::XMVectorScale(zDir, -1);
	point = DirectX::XMVectorSubtract(DirectX::XMVectorSubtract(zDirInv, xDir),yDir );  //- y - x - z
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_CUBE][7] = Point(point.m128_f32, color.m128_f32);	    //7





	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = m_deviceContext->Map(m_PointBuffer[M_CUBE], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)m_points[M_CUBE], sizeof(Point) * NUM_POINTS[M_CUBE]);
	m_deviceContext->Unmap(m_PointBuffer[M_CUBE], 0);
	
	return m_PointBuffer[M_CUBE];
}
ID3D11Buffer * DebugRenderer::GenerateLinelist(DirectX::XMVECTOR & pos, Plane & box, DirectX::XMVECTOR color)
{
	
	
	static float SCALAR = 1000.0f;



	/*
		We must find a vector that is perpendicular to the normal of the plane.
	*/
	DirectX::XMVECTOR right = DirectX::XMVector3Orthogonal(box.PC_normal);
	
	DirectX::XMVECTOR rightInv = DirectX::XMVectorScale(right, -1);

	DirectX::XMVECTOR zDir = DirectX::XMVector3Cross(right, box.PC_normal);
	DirectX::XMVECTOR zDirInv = DirectX::XMVectorScale(zDir, -1);
/*
	x + z
	x - z
	- x + z
	-x -z 

*/	
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslationFromVector(pos);
	
	DirectX::XMVECTOR point;
	point = DirectX::XMVectorAdd(right, zDir);
	point = DirectX::XMVectorScale(point, SCALAR);
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_PLANE][0] = Point(point.m128_f32, color.m128_f32);		//0

	point = DirectX::XMVectorSubtract(right, zDir);
	point = DirectX::XMVectorScale(point, SCALAR);
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_PLANE][1] = Point(point.m128_f32, color.m128_f32);		//1

	point = DirectX::XMVectorAdd(rightInv,zDir);
	point = DirectX::XMVectorScale(point, SCALAR);
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_PLANE][2] = Point(point.m128_f32, color.m128_f32);		//2

	point = DirectX::XMVectorSubtract(rightInv, zDir);
	point = DirectX::XMVectorScale(point, SCALAR);
	point = DirectX::XMVector3TransformCoord(point, worldMatrix);
	m_points[M_PLANE][3] = Point(point.m128_f32, color.m128_f32);		//3

	

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = m_deviceContext->Map(m_PointBuffer[M_PLANE], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)m_points[M_PLANE], sizeof(Point) * NUM_POINTS[M_PLANE]);
	m_deviceContext->Unmap(m_PointBuffer[M_PLANE], 0);

	return m_PointBuffer[M_PLANE];
}
ID3D11Buffer * DebugRenderer::GenerateLinelist(DirectX::XMVECTOR & pos, Sphere & box, DirectX::XMVECTOR color, int ringIndex)
{
	box.radius;
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslationFromVector(pos);

	float step = DirectX::XM_PI*2 / NUM_POINTS[M_SPHERE];
	float theta = 0.0f;

	for (size_t i = 0; i < NUM_POINTS[M_SPHERE]; i++)
	{
		DirectX::XMVECTOR point = pos;

		if (ringIndex == 1)
		{
			point.m128_f32[0] += box.radius * DirectX::XMScalarCos(theta);
			point.m128_f32[1] += box.radius * DirectX::XMScalarSin(theta);
		}
		else if (ringIndex == 2)
		{
			point.m128_f32[2] += box.radius * DirectX::XMScalarCos(theta);
			point.m128_f32[0] += box.radius * DirectX::XMScalarSin(theta);
		}
		else if (ringIndex == 3)
		{
			point.m128_f32[1] += box.radius * DirectX::XMScalarCos(theta);
			point.m128_f32[2] += box.radius * DirectX::XMScalarSin(theta);
		}

		point = DirectX::XMVector3TransformCoord(point, worldMatrix);
		m_points[M_SPHERE][i] = Point(point.m128_f32, color.m128_f32);

		theta += step;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	HRESULT hr = m_deviceContext->Map(m_PointBuffer[M_SPHERE], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	Point* tempData = (Point*)mappedResource.pData;
	memcpy(tempData, (void*)m_points[M_SPHERE], sizeof(Point) * NUM_POINTS[M_SPHERE]);
	m_deviceContext->Unmap(m_PointBuffer[M_SPHERE], 0);

	return m_PointBuffer[M_SPHERE];
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