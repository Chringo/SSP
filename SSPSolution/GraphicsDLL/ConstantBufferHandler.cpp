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
	CD3D11_BUFFER_DESC bufferDesc;
	

	//Creating world constant buffer																 
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth			= sizeof(ConstantBuffer::world::p);
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage			    = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride  = 0;
	
	hResult = device->CreateBuffer(&bufferDesc, nullptr, &world.D3DBuffer);
	if (SUCCEEDED(hResult))
		this->m_deviceContext->VSSetConstantBuffers(CB_WORLD_B0, 1, &world.D3DBuffer);
	else
		return 1;


	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth		 = sizeof(ConstantBuffer::frame::p);
	bufferDesc.BindFlags		 = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage			 = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags	 = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		 = 0;
	bufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDesc, nullptr, &frame.D3DBuffer);
	if (SUCCEEDED(hResult))
	{
		deviceContext->VSSetConstantBuffers(CB_FRAME_B1, 1, &frame.D3DBuffer);
		deviceContext->PSSetConstantBuffers(CB_FRAME_B1, 1, &frame.D3DBuffer);
	}
	else
		return 1;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth		= sizeof(ConstantBuffer::material::p);
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;
	bufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDesc, nullptr, &material.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->VSSetConstantBuffers(CB_SKELETON_B4, 1, &material.D3DBuffer);
	else
		return	1;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth		= sizeof(ConstantBuffer::light::p);
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;
	bufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDesc, nullptr, &light.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->PSSetConstantBuffers(CB_LIGHT_B3, 1, &light.D3DBuffer);
	else
		return	1;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(ConstantBuffer::skeleton::p);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDesc, nullptr, &skeleton.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->VSSetConstantBuffers(CB_SKELETON_B4, 1, &skeleton.D3DBuffer);
	else
		return	1;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth = sizeof(ConstantBuffer::shadow::p);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	hResult = device->CreateBuffer(&bufferDesc, nullptr, &shadow.D3DBuffer);
	if (SUCCEEDED(hResult))
		deviceContext->GSSetConstantBuffers(CB_SHADOW_B5, 1, &shadow.D3DBuffer);
	else
		return	1;


	return 0;
}

int ConstantBufferHandler::ResetConstantBuffers()
{
	this->m_deviceContext->VSSetConstantBuffers(CB_WORLD_B0, 1, &world.D3DBuffer);

	//this->m_deviceContext->VSSetConstantBuffers(CB_CAMERA_B1, 1, &camera.D3DBuffer);
	//this->m_deviceContext->PSSetConstantBuffers(CB_CAMERA_B1, 1, &camera.D3DBuffer);

	//this->m_deviceContext->VSSetConstantBuffers(CB_SKELETON_B4, 1, &material.D3DBuffer);
	//this->m_deviceContext->VSSetConstantBuffers(CB_SKELETON_B4, 1, &light.D3DBuffer);
	//this->m_deviceContext->VSSetConstantBuffers(CB_SKELETON_B4, 1, &skeleton.D3DBuffer);

	return 0;
}

int ConstantBufferHandler::Shutdown()
{
	if (world.D3DBuffer)
		world.D3DBuffer->Release();
	if (frame.D3DBuffer)
		frame.D3DBuffer->Release();
	if (skeleton.D3DBuffer)
		skeleton.D3DBuffer->Release();
	

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