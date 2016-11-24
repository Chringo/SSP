#ifndef GRAPHICSDLL_SCREENQUAD_H
#define GRAPHICSDLL_SCREENQUAD_H

#include <d3d11.h>
#include <DirectXMath.h>

class ScreenQuad {
private:
	struct quadVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 UV;
	};

	const quadVertex vertices[6] =
	{
		DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(0.0f, 1.0f),			//v0 UV

		DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),	//v1
		DirectX::XMFLOAT2(0.0f, 0.0f),			//v1 UV

		DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), //v2
		DirectX::XMFLOAT2(1.0f, 1.0f),			//v2 UV

												//T2
		DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),	//v3 pos
		DirectX::XMFLOAT2(0.0f, 0.0f),			//v3 UV

		DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),	//v4
		DirectX::XMFLOAT2(1.0f, 0.0f),			//v4 UV

		DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),	//v5
		DirectX::XMFLOAT2(1.0f, 1.0f)			//v5 UV
	};

	const unsigned int indices[6] =
	{
		0, 1, 2, 3, 4, 5
	};

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

public:
	ScreenQuad();
	~ScreenQuad();

	int Initialize(ID3D11Device* device, DirectX::XMINT2 resolution);
	void SetBuffers(ID3D11DeviceContext* deviceContext);
	void Shutdown();
};

#endif