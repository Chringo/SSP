#pragma once
#include <d3d11.h>
#include "ShaderLib.h"

#ifndef CONSTANT_BUFFER_HANDLER_H

class ConstantBufferHandler
{
	enum CBufferType
	{
		CB_WORLD_B0,
		CB_VIEW_PROJECTION_B1,
		CB_CAMERA_B2,
		CB_MATERIAL_B3,
		CB_LIGHT_B4, 
		CB_SKELETON_B5,

		CB_TYPE_COUNT
	};

	struct ConstantBuffer
	{
		struct BufferData
		{
			ID3D11Buffer * D3DBuffer = nullptr;
			CBufferType type;

			BufferData()
			{
				type = CB_TYPE_COUNT;
			};


			template <typename T>
			int UpdateBuffer(T* data, CBufferType Type)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(mappedResource));

				ConstantBufferHandler::GetInstance()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

				T* tempBufferData 0 (T*)mappedResource.pData;
				*tempBufferData = *data;

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);


			}
		};

		BufferData cbWorld;
		BufferData cbViewProj;
		BufferData cbCamera;
		BufferData cbMaterial;
		BufferData cbLight;
		BufferData cbSkeleton;

		ConstantBuffer() {};
		~ConstantBuffer()
		{
			if (cbWorld.D3DBuffer)
				cbWorld.D3DBuffer->Release();
			if (cbViewProj.D3DBuffer)
				cbViewProj.D3DBuffer->Release();
			if (cbCamera.D3DBuffer)
				cbCamera.D3DBuffer->Release();
			if (cbMaterial.D3DBuffer)
				cbMaterial.D3DBuffer->Release();
			if (cbLight.D3DBuffer)
				cbLight.D3DBuffer->Release();
			if (cbSkeleton.D3DBuffer)
				cbSkeleton.D3DBuffer->Release();
		}

	};

private:
	ConstantBufferHandler();
	
	ID3D11Device * m_device;
	ID3D11DeviceContext * m_deviceContext;
	ConstantBuffer CBuffers;

public:
	~ConstantBufferHandler();

	int Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	int Shutdown();

	ID3D11DeviceContext * GetDeviceContext();
	static ConstantBufferHandler * GetInstance();
	ConstantBuffer * GetCBuffers();

};

#endif // !CONSTANT_BUFFER_HANDLER_H
