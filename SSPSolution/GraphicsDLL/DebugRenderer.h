#ifndef GRAPHICSDLL_DEBUGRENDERER
#define GRAPHICSDLL_DEBUGRENDERER
#include "../physicsDLL/PhysicsHandler.h"
#include "Shader.h"
class DebugRenderer
{

private:
	static const short int NUM_POINTS  = 8;
	static const short int NUM_INDICES = 24;
	struct Point{
		float x, y, z;
		float r, g, b;
		Point::Point() {};
		Point::Point(float pos[3], float color[3])
		{
			this->x = pos[0];
			this->y = pos[1];
			this->z = pos[2];
			this->r = color[0];
			this->g = color[1];
			this->b = color[2];
		}
		Point::Point(float x,float y,float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->r = 1.0f;
			this->g = 1.0f;
			this->b = 1.0f;
		}
		
	};
	Point cubePoints[NUM_POINTS];
	ID3D11Buffer*	      m_pointBuffer;
	ID3D11Buffer*	      m_boxIndexBuffer;
	ID3D11VertexShader*   m_vertexShader;
	ID3D11PixelShader*	  m_pixelShader;
	ID3D11InputLayout*    m_layout;
	ID3D11Device*		  m_device;
	ID3D11DeviceContext*  m_deviceContext;
public:
	DebugRenderer();
	~DebugRenderer();
	int Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const DirectX::XMINT2& resolution);
	void Release();
	void Render(AABB& box);
	void Render(OBB& box);
	void SetActive();

private:
	ID3D11Buffer* GenerateLinelist(AABB& box);
	ID3D11Buffer* GenerateLinelist(OBB& box);
};

#endif

