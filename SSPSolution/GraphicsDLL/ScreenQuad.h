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