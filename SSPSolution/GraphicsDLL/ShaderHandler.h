#ifndef GRAPHICSDLL_SHADERHANDLER
#define GRAPHICSDLL_SHADERHANDLER

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#include "ShaderLib.h"

class ShaderHandler
{
protected:
	ID3D11VertexShader* m_vertexShader[4];
	ID3D11GeometryShader* m_geoShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

public:
	 ShaderHandler();
	 virtual ~ShaderHandler();

	 virtual int Initialize(ID3D11Device* device, HWND* windowHandle, DirectX::XMFLOAT2 resolution) = 0;
	 virtual int SetActive(ID3D11DeviceContext* deviceContext, ShaderLib::ShaderType shaderType) = 0;
	 virtual void Shutdown() = 0;
};

#endif