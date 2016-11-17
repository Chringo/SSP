#include "ShaderHandler.h"

ShaderHandler::ShaderHandler()
{
	for (int i = 0; i < 4; i++)
	{
		this->m_vertexShader[i] = nullptr;
	}
	this->m_geoShader = nullptr;
	this->m_pixelShader = nullptr;
	this->m_layout = nullptr;
	this->m_matrixBuffer = nullptr;
}


ShaderHandler::~ShaderHandler()
{
}

void ShaderHandler::Shutdown()
{
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
	if (this->m_layout)
	{
		this->m_layout->Release();
		this->m_layout = nullptr;
	}
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Release();
		this->m_matrixBuffer = nullptr;
	}
}