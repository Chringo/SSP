#ifndef GRAPHICSDLL_SHADERHANDLER
#define GRAPHICSDLL_SHADERHANDLER

#include <d3d11.h>
#include <DirectXMath.h>
#incldue "ShaderLib.h"

class ShaderHandler
{
private:

public:
	 ShaderHandler();
	 virtual ~ShaderHandler();

	 virtual int Initialize(ID3D11Device* device, HWND* windowHandle, DirectX::XMFLOAT2 resolution) = 0;
	 virtual int SetActive(ShaderLib::ShaderType shaderType) = 0;
	 virtual void Shutdown() = 0;
};

#endif