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
