#include "Shader.h"

Shader::Shader()
{

	
}


Shader::~Shader()
{
}

int Shader::Initialize(ID3D11Device* device,ID3D11DeviceContext * deviceContext, const DirectX::XMINT2& resolution)
{
	return 0;
}

int Shader::SetActive(ShaderLib::ShaderVariations ShaderVariations)
{

	//if (m_activeVariation == ShaderVariations)
	//	return 0;
		//Set the input layout for vertex
	//m_deviceContext->IASetInputLayout(this->m_layout);

	////Set the vertex and pixel shaders that will be used to render this triangle
	//m_deviceContext->VSSetShader(this->m_vertexShader[0], NULL, 0);
	//m_deviceContext->PSSetShader(this->m_pixelShader, NULL, 0);
	//m_deviceContext->GSSetShader(this->m_geoShader, NULL, 0);

	//m_activeVariation = ShaderVariations;
	return 0;
}

void Shader::Release()
{

}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename)
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