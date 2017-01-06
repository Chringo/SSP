#ifndef GRAPHICSDLL_DEBUGRENDERER
#define GRAPHICSDLL_DEBUGRENDERER
#include "../physicsDLL/PhysicsHandler.h"
#include "Shader.h"
class DebugRenderer
{

private:
	enum MeshTypes {
		M_CUBE    = 0,
		M_PLANE   = 1,
		M_SPHERE  = 2,
		M_NUM_TYPES
	};
	 int NUM_POINTS[M_NUM_TYPES];
	 int NUM_INDICES[M_NUM_TYPES];
	
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
	Point* m_points[M_NUM_TYPES];
	UINT*  m_indices[M_NUM_TYPES];

	ID3D11Buffer*	      m_PointBuffer [M_NUM_TYPES] ;
	ID3D11Buffer*	      m_IndexBuffer [M_NUM_TYPES] ;
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
	void Render(DirectX::XMVECTOR& pos, AABB&  box,   DirectX::XMVECTOR color = { 0.0f,1.0f,0.0f });
	void Render(DirectX::XMVECTOR& pos, OBB&   box,   DirectX::XMVECTOR color = { 1.0f,0.0f,0.0f });
	void Render(DirectX::XMVECTOR& pos, Plane& plane, DirectX::XMVECTOR color = { 0.0f,0.0f,1.0f });
	void Render(DirectX::XMVECTOR& pos, Sphere& sphere, DirectX::XMVECTOR color = { 0.0f,0.0f,1.0f });
	void SetActive();

private:
	ID3D11Buffer* GenerateLinelist(DirectX::XMVECTOR& pos, AABB& box , DirectX::XMVECTOR color);
	ID3D11Buffer* GenerateLinelist(DirectX::XMVECTOR& pos, OBB& box ,  DirectX::XMVECTOR color);
	ID3D11Buffer* GenerateLinelist(DirectX::XMVECTOR& pos, Plane& box, DirectX::XMVECTOR color);
	ID3D11Buffer* GenerateLinelist(DirectX::XMVECTOR& pos, Sphere& box, DirectX::XMVECTOR color);


};

#endif

