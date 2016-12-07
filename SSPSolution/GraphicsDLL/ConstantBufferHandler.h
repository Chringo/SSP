#pragma once
#include <d3d11.h>
#include "ShaderLib.h"

#ifndef CONSTANT_BUFFER_HANDLER_H

class ConstantBufferHandler
{
	enum CBufferType
	{
		CB_WORLD_B0,
		CB_CAMERA_B1,
		CB_MATERIAL_B2,
		CB_LIGHT_B3, 
		CB_SKELETON_B4,

		CB_TYPE_COUNT
	};

	

	struct ConstantBuffer
	{
		/**BUFFER TYPE STRUCT DEFINITIONS**/
		struct world
		{
			struct cbData
			{
				DirectX::XMMATRIX cWorld;
			};
			cbData c;
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pWorld;
			};
			pData p;
		public:
			pData GetPData()
			{
				DirectX::XMStoreFloat4x4(&p.pWorld, c.cWorld);

				return p;
			};
		};

		struct camera
		{
			DirectX::XMMATRIX cView;
			DirectX::XMMATRIX cProjection;
			DirectX::XMVECTOR cPos;

		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pView;
				DirectX::XMFLOAT4X4 pProjection;
				DirectX::XMFLOAT4 pPos;
				DirectX::XMFLOAT3X3 padding;

			};
			pData p;
		public:
			pData GetPData()
			{
				DirectX::XMStoreFloat4x4(&p.pView, cView);
				DirectX::XMStoreFloat4x4(&p.pProjection, cProjection);
				DirectX::XMStoreFloat4(&p.pPos, cPos);

				return p;
			};
		};

		struct skeleton
		{
			DirectX::XMMATRIX cJoints[32];

		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pJoints[32];
			};
			pData p;
		public:
			pData GetPData()
			{
				for (int i = 0; i < 32; i++)
				{
					DirectX::XMStoreFloat4x4(&p.pJoints[i], cJoints[i]);
				}
				return p;
			};
		};



	public:
		struct Buffer
		{
			ID3D11Buffer * D3DBuffer = nullptr;
			CBufferType type;

			Buffer(CBufferType Type)
			{
				type = Type;
			};


			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(mappedResource));



				switch (type)
				{
				case ConstantBufferHandler::CB_WORLD_B0:
				{
					break;
				}
				case ConstantBufferHandler::CB_CAMERA_B1:
					break;
				case ConstantBufferHandler::CB_MATERIAL_B2:
					break;
				case ConstantBufferHandler::CB_LIGHT_B3:
					break;
				case ConstantBufferHandler::CB_SKELETON_B4:
					break;
				case ConstantBufferHandler::CB_TYPE_COUNT:
					break;
				default:
					break;
				}

				ConstantBufferHandler::GetInstance()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

				T* tempBufferData 0 (T*)mappedResource.pData;
				*tempBufferData = *data;

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);


			}
		};

		Buffer * cbWorld        = new Buffer(CB_WORLD_B0);
		Buffer * cbCamera		= new Buffer(CB_CAMERA_B1);
		Buffer * cbMaterial		= new Buffer(CB_MATERIAL_B2);
		Buffer * cbLight		= new Buffer(CB_LIGHT_B3);
		Buffer * cbSkeleton		= new Buffer(CB_SKELETON_B4);





		ConstantBuffer() {};
		~ConstantBuffer()
		{
			if (cbWorld->D3DBuffer)
				cbWorld->D3DBuffer->Release();
			if (cbCamera->D3DBuffer)
				cbCamera->D3DBuffer->Release();
			if (cbMaterial->D3DBuffer)
				cbMaterial->D3DBuffer->Release();
			if (cbLight->D3DBuffer)
				cbLight->D3DBuffer->Release();
			if (cbSkeleton->D3DBuffer)
				cbSkeleton->D3DBuffer->Release();

			delete cbWorld;
			delete cbViewProj;
			delete cbCamera;
			delete cbMaterial;
			delete cbLight;
			delete cbSkeleton;
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
