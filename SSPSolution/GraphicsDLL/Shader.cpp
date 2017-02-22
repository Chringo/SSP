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


int Shader::SetVariation(ShaderLib::ShaderVariations variation)
{
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