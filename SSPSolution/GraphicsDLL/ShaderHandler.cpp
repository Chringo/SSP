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

int ShaderHandler::Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext * deviceContext, const DirectX::XMINT2&  resolution)
{
	return 0;
}

int ShaderHandler::SetActive(ShaderLib::ShaderType shaderType)
{
	//Set the input layout for vertex
	m_deviceContext->IASetInputLayout(this->m_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	m_deviceContext->VSSetShader(this->m_vertexShader[0], NULL, 0);
	m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);

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

void ShaderHandler::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	//Get a pointer to the error message text buffer
	compileErrors = (char*)errorMessage->GetBufferPointer();

	//Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	//Open a file to write error messages to
	fout.open("shader_error.txt");

	//Write the error message to the file
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	//Close the file
	fout.close();

	//Release the error message
	errorMessage->Release();
	errorMessage = nullptr;

	return;
}