#ifndef GRAPHICSDLL_POSTPROCESSSHADER
#define GRAPHICSDLL_POSTPROCESSSHADER
#include "../physicsDLL/PhysicsHandler.h"
#include "Shader.h"
class DebugRenderer
{

private:
	struct Point{
		float x, y, z;
	};
	Point cubePoints[8];
	ID3D11VertexShader*   m_vertexShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout;
	ID3D11Device*		  m_device;
	ID3D11DeviceContext*  m_deviceContext;
public:
	DebugRenderer();
	~DebugRenderer();
	int Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);

	void Render(AABB& box);
	void Render(OBB& box);
};

#endif

