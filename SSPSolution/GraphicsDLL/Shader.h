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
	ID3D11VertexShader*   m_vertexShader[4];
	ID3D11GeometryShader* m_geoShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout;

public:
	Shader();
	virtual ~Shader();

	virtual int		 Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution) = 0;
	virtual int  SetActive(ShaderLib::ShaderVariations ShaderVariations) = 0;
	virtual void Release() = 0;
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
};

#endif