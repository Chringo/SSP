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

	enum CBuffer
	{
		WORLD,
		VIEW_PROJECTION,
		CAMERA
	};

	struct DeferredConstantBufferWorld
	{
		DirectX::XMFLOAT4X4 worldMatrix;
	};

	struct DeferredConstantBufferVP
	{
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
	};

	struct LightConstantBuffer
	{
		DirectX::XMFLOAT3 camPos;
		DirectX::XMFLOAT3 camDir;
		float padding1;
		float padding2;
	};
}

#endif