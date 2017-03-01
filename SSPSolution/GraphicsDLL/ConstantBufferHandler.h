#pragma once
#include <d3d11.h>
#include "ShaderLib.h"

#ifndef CONSTANT_BUFFER_HANDLER_H
#define MAX_SHADOW_LIGHTS  20 //Must be 4 * n + 2
class ConstantBufferHandler
{
	enum CBufferType
	{
		CB_WORLD_B0,
		CB_FRAME_B1,
		CB_MATERIAL_B2,
		CB_LIGHT_B3, 
		CB_SKELETON_B4,
		CB_SHADOW_B5,

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

		struct frame
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pView;
				DirectX::XMFLOAT4X4 pProjection;
				DirectX::XMFLOAT4 pPos;
				float pTimer, padding1 = 0.f, padding2 = 0.f, padding3 = 0.f;
			};
				D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			struct cbData
			{
				DirectX::XMMATRIX cView;
				DirectX::XMMATRIX cProjection;
				DirectX::XMVECTOR cPos;
				float cTimer;
			};
			ID3D11Buffer * D3DBuffer = nullptr;
			pData p;
			cbData c;
			pData GetPData()
			{
				DirectX::XMStoreFloat4x4(&p.pView, DirectX::XMMatrixTranspose(c.cView));
				DirectX::XMStoreFloat4x4(&p.pProjection, DirectX::XMMatrixTranspose(c.cProjection));
				DirectX::XMStoreFloat4(&p.pPos, c.cPos);
				p.pTimer = c.cTimer;
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
		public:
			struct arrayIndex {
				int index;
				int _Padding[3];
			};
			struct pData
			{
				
				UINT   NUM_POINTLIGHTS;
				UINT   DYNAMIC_SHADOWLIGHT_INDEX;
				UINT   padding[2];
				float  AMBIENT_COLOR[3] = { 1.0f,1.0f,1.0f };
				float  AMBIENT_INTENSITY = 0.2f;
				arrayIndex	SHADOWCASTING_LIGHTS[MAX_SHADOW_LIGHTS]; //Must be size  multiple of 4
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			ID3D11Buffer * D3DBuffer = nullptr;

			pData p;

			template <typename T>
			int UpdateBuffer(T* data) //Takes pointer to structs containing non-transposed XMVECTORS or XMMATRIX for transforms
			{
				p = *(pData*)data;

			
				ConstantBufferHandler::GetInstance()->GetDeviceContext()->Map(D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &p, sizeof(pData));
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

		struct shadow
		{
		private:
			struct pData
			{
				DirectX::XMFLOAT4X4 pView[6];
				DirectX::XMFLOAT4X4 pProjection;
			};
			D3D11_MAPPED_SUBRESOURCE mappedResource;
		public:
			struct cbData
			{
				DirectX::XMMATRIX cView[6];
				DirectX::XMMATRIX cProjection;
			};
			ID3D11Buffer * D3DBuffer = nullptr;
			pData p;
			cbData c;
			pData GetPData()
			{
				for (size_t i = 0; i < 6; i++)
				{

					DirectX::XMStoreFloat4x4(&p.pView[i], DirectX::XMMatrixTranspose(c.cView[i]));
				}
				DirectX::XMStoreFloat4x4(&p.pProjection, DirectX::XMMatrixTranspose(c.cProjection));
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
	ConstantBuffer::frame frame;
	ConstantBuffer::light light;
	ConstantBuffer::material material;
	ConstantBuffer::skeleton skeleton;
	ConstantBuffer::shadow shadow;

	int Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext);
	int ResetConstantBuffers();
	int Shutdown();

	

	ID3D11DeviceContext * GetDeviceContext();
	static ConstantBufferHandler * GetInstance();

};

#endif // !CONSTANT_BUFFER_HANDLER_H
