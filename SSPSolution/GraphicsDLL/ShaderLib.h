#ifndef GRAPHICSDLL_SHADERLIB
#define GRAPHICSDLL_SHADERLIB

#include <d3d11.h>
#include <DirectXMath.h>

namespace ShaderLib
{
	enum ShaderType
	{
		Normal = 0,
		Instanced = 1,
		Animated = 2,
		InstancedAnimated = 3
	};

	struct DeferredConstantBuffer
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;

		DirectX::XMFLOAT4 diffColor;

		DirectX::XMFLOAT3 camPos;
	};
	struct LightConstantBuffer
	{
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;

		DirectX::XMFLOAT2 resolution;

		float padding[2];

		DirectX::XMFLOAT3 camPos;
	};
}

#endif