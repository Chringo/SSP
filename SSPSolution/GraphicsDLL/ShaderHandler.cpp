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

int ShaderHandler::Initialize(ID3D11Device* device, HWND* windowHandle, const DirectX::XMFLOAT2&  resolution)
{
	return 0;
}

int ShaderHandler::SetActive(ID3D11DeviceContext * deviceContext, ShaderLib::ShaderType shaderType)
{
	//Set the input layout for vertex
	deviceContext->IASetInputLayout(this->m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(this->m_vertexShader[0], NULL, 0);
	deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	deviceContext->GSSetShader(this->m_geoShader, NULL, 0);

	return 0;
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