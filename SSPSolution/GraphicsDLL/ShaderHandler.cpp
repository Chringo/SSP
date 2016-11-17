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
