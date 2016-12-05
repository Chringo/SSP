#include "ConstantBufferHandler.h"

ConstantBufferHandler::ConstantBufferHandler()
{
}

ConstantBufferHandler::~ConstantBufferHandler()
{
}

int ConstantBufferHandler::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	this->m_device = device;
	this->m_deviceContext = deviceContext;

	HRESULT hResult;


	//Creating world constant buffer																 
	CD3D11_BUFFER_DESC bufferDescWorld;
	ZeroMemory(&bufferDescWorld, sizeof(bufferDescWorld));
	bufferDescWorld.ByteWidth = sizeof(ShaderLib::WorldConstantBuffer);
	bufferDescWorld.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescWorld.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescWorld.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescWorld.MiscFlags = 0;
	bufferDescWorld.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDescWorld, nullptr, &CBuffers.cbWorld.D3DBuffer);
	if (SUCCEEDED(hResult))
		this->m_deviceContext->VSSetConstantBuffers(CB_WORLD_B0, 1, &CBuffers.cbWorld.D3DBuffer);
	else
		return 1;

	CD3D11_BUFFER_DESC bufferDescVP;
	ZeroMemory(&bufferDescVP, sizeof(bufferDescVP));
	bufferDescVP.ByteWidth = sizeof(ShaderLib::VPConstantBuffer);
	bufferDescVP.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescVP.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescVP.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescVP.MiscFlags = 0;
	bufferDescVP.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDescVP, nullptr, &CBuffers.cbViewProj.D3DBuffer);
	if (SUCCEEDED(hResult))
	{
		deviceContext->VSSetConstantBuffers(CB_VIEW_PROJECTION_B1, 1, &CBuffers.cbViewProj.D3DBuffer);

	}
	else
		return 1;


	CD3D11_BUFFER_DESC bufferDescCam;
	ZeroMemory(&bufferDescCam, sizeof(bufferDescCam));
	bufferDescCam.ByteWidth = sizeof(ShaderLib::CameraConstantBuffer);
	bufferDescCam.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescCam.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescCam.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescCam.MiscFlags = 0;
	bufferDescCam.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDescCam, nullptr, &CBuffers.cbCamera.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->PSSetConstantBuffers(CB_CAMERA_B2, 1, &CBuffers.cbCamera.D3DBuffer);
	else
		return 1;

	CD3D11_BUFFER_DESC bufferDescSkele;
	ZeroMemory(&bufferDescSkele, sizeof(bufferDescSkele));
	bufferDescSkele.ByteWidth = sizeof(ShaderLib::SkeletonConstantBuffer);
	bufferDescSkele.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescSkele.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescSkele.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescSkele.MiscFlags = 0;
	bufferDescSkele.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDescSkele, nullptr, &CBuffers.cbSkeleton.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->PSSetConstantBuffers(CB_SKELETON_B5, 1, &CBuffers.cbSkeleton.D3DBuffer);
	else
		return 1;


	return 0;
}

int ConstantBufferHandler::Shutdown()
{
	
	return 0;
}

ID3D11DeviceContext * ConstantBufferHandler::GetDeviceContext()
{
	return this->m_deviceContext;
}

ConstantBufferHandler * ConstantBufferHandler::GetInstance()
{
	static ConstantBufferHandler instance;
	return &instance;
}

ConstantBufferHandler::ConstantBuffer * ConstantBufferHandler::GetCBuffers()
{
	return &this->CBuffers;
}
