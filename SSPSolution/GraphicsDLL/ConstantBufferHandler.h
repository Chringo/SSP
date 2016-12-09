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

	
public:
	struct ConstantBuffer
	{

		/**BUFFER TYPE STRUCT DEFINITIONS**/
		struct world
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pWorld;
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			struct cbData
			{
				DirectX::XMMATRIX cWorld;
			};

			ID3D11Buffer * D3DBuffer = nullptr;
			cbData c;
			pData p;
			pData GetPData()
			{
				DirectX::XMStoreFloat4x4(&p.pWorld, DirectX::XMMatrixTranspose(c.cWorld));
				return p;
			};
			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				c = *(cbData*)data;
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

				memcpy(mappedResource.pData, &GetPData(), sizeof(pData));

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);

				return 0;
			}
		};

		struct camera
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pView;
				DirectX::XMFLOAT4X4 pProjection;
				DirectX::XMFLOAT4 pPos;
				DirectX::XMFLOAT4 padding1;
				DirectX::XMFLOAT4 padding2;
				DirectX::XMFLOAT4 padding3;
			};
				D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			struct cbData
			{
				DirectX::XMMATRIX cView;
				DirectX::XMMATRIX cProjection;
				DirectX::XMVECTOR cPos;
			};
			ID3D11Buffer * D3DBuffer = nullptr;
			pData p;
			cbData c;
			pData GetPData()
			{
				DirectX::XMStoreFloat4x4(&p.pView, DirectX::XMMatrixTranspose(c.cView));
				DirectX::XMStoreFloat4x4(&p.pProjection, DirectX::XMMatrixTranspose(c.cProjection));
				DirectX::XMStoreFloat4(&p.pPos, c.cPos);

				return p;
			};
			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				c = *(cbData*)data;

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &GetPData(), sizeof(pData));
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);

				return 0;
			}
		};

		struct material
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 temp;
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			ID3D11Buffer * D3DBuffer = nullptr;

			pData p;

			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				p = *(cbData*)data;

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &GetPData(), sizeof(pData));
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);

				return 0;
			}
		};

		struct light
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 temp;
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			ID3D11Buffer * D3DBuffer = nullptr;

			pData p;

			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				p = *(cbData*)data;


				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &GetPData(), sizeof(pData));
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);

				return 0;
			}
		};

		struct skeleton
		{
			struct cbData
			{
				DirectX::XMMATRIX cJoints[32];
			};

		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pJoints[32];
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			ID3D11Buffer * D3DBuffer = nullptr;
			pData p;
			cbData c;
			pData GetPData()
			{
				for (int i = 0; i < 32; i++)
				{
					DirectX::XMStoreFloat4x4(&p.pJoints[i], DirectX::XMMatrixTranspose(c.cJoints[i]));
				}
				return p;
			};
			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				c = *(cbData*)data;

				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &GetPData(), sizeof(pData));
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Unmap(D3DBuffer, 0);

				return 0;
			}
		};
	};

private:
	ConstantBufferHandler();
	
	ID3D11Device * m_device;
	ID3D11DeviceContext * m_deviceContext;

public:

	~ConstantBufferHandler();


	ConstantBuffer::world world;
	ConstantBuffer::camera camera;
	ConstantBuffer::light light;
	ConstantBuffer::material material;
	ConstantBuffer::skeleton skeleton;

	int Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	int Shutdown();

	

	ID3D11DeviceContext * GetDeviceContext();
	static ConstantBufferHandler * GetInstance();

};

#endif // !CONSTANT_BUFFER_HANDLER_H
