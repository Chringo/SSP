#ifndef GRAPHICSDLL_Shader
#define GRAPHICSDLL_Shader

#include <d3dcompiler.h>
#include <fstream>

#include "ShaderLib.h"

const int BUFFER_COUNT = 6; //color metal rough normal ao wpos

class Shader
{
protected:
	ID3D11DeviceContext*  m_deviceContext;


	ShaderLib::ShaderVariations m_activeVariation;

public:
	Shader();
	virtual ~Shader();

	virtual int		 Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution) = 0;
	virtual int  SetActive(ShaderLib::ShaderVariations ShaderVariations) = 0;
	virtual void Release() = 0;
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
};

#endif