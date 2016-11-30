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
		CB_WORLD,
		CB_VIEW_PROJECTION,
		CB_CAMERA
	};

	enum DrawType
	{
		DRAW_STANDARD,
		DRAW_INSTANCED
	};

	struct DeferredConstantBufferWorld
	{
		DirectX::XMFLOAT4X4 worldMatrix;
	};

	struct DeferredConstantBufferWorldxm
	{
		DirectX::XMMATRIX worldMatrix;
	};

	struct DeferredConstantBufferVP
	{
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
	};

	struct DeferredConstantBufferVPxm
	{
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
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