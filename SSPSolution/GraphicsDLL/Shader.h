#ifndef GRAPHICSDLL_SHADERS_SHADER_H
#define GRAPHICSDLL_SHADERS_SHADER_H

#include <d3dcompiler.h>
#include <fstream>
#include "GraphicsComponent.h"
#include "ShaderLib.h"

const int BUFFER_COUNT = 5; //color metal  wpos RNAO SHADOW

const int RTV_COUNT = 4; // shadow does not use RTVs

class Shader
{
protected:
	ID3D11DeviceContext*  m_deviceContext;


	ShaderLib::ShaderVariations m_activeVariation;

public:
	Shader();
	virtual ~Shader();

	virtual int Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution) = 0;
	virtual int SetActive() = 0;
	virtual int SetVariation(ShaderLib::ShaderVariations variation);
	virtual void Release()  = 0;
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
};

#endif