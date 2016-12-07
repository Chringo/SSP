#ifndef GRAPHICSDLL_SHADERHANDLER
#define GRAPHICSDLL_SHADERHANDLER

#include <d3dcompiler.h>
#include <fstream>
#include "ConstantBufferHandler.h"
#include "ShaderLib.h"

const int BUFFER_COUNT = 6; //color metal rough normal ao wpos

class ShaderHandler
{
protected:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11VertexShader* m_vertexShader[4];
	ID3D11GeometryShader* m_geoShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

public:
	ShaderHandler();
	virtual ~ShaderHandler();

	virtual int Initialize(ID3D11Device* device, HWND* windowHandle, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution) = 0;
	virtual int SetActive(ShaderLib::ShaderType shaderType) = 0;
	virtual void Shutdown() = 0;
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* shaderFilename);
};

#endif