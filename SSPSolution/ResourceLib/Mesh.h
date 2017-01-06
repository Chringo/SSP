
#ifndef RESOURCELIB_MESH_MESH_H
#define RESOURCELIB_MESH_MESH_H

#include "Resource.h"

namespace Resources
{
	


	class Mesh :
		public Resources::Resource
	{
	
	public:

		struct Vertex
		{
			float position[3];
			float normal[3];
			float tangent[3];
			float uv[2];

		};
		struct VertexAnim
		{
			float position[3];
			float normal[3];
			float tangent[3];
			float uv[2];
			float weights[4];
			int influence[4];
		};
		struct RawMeshData 
		{
			unsigned int  m_numVerts      = 0;
			unsigned int  m_numIndices    = 0;
			Vertex*       m_vertices	  = nullptr;
			VertexAnim*   m_animVertices  = nullptr;
			unsigned int* m_indices		  = nullptr;
			bool hasAnimation			  = false;
			RawMeshData()
			{
				m_numVerts      = 0;
				m_numIndices	= 0;
				m_vertices		= nullptr;
				m_animVertices  = nullptr;
				m_indices		= nullptr;
				hasAnimation	= false;
			}
		};
	protected:
		RawMeshData	  m_meshData;
		ID3D11Buffer* m_vertBuffer		= nullptr;
		ID3D11Buffer* m_AnimVertBuffer	= nullptr;
		ID3D11Buffer* m_indexBuffer		= nullptr;
	public:
		DLL_OPERATION Mesh(Resource::RawResourceData resData, RawMeshData meshData, ID3D11Device* dev = nullptr,bool keepRawData = false);
		DLL_OPERATION Mesh(Resource::RawResourceData resData);
		DLL_OPERATION Mesh();

		DLL_OPERATION Resources::Status Create(Resource::RawResourceData* resData, ID3D11Device* dev = nullptr,RawMeshData* = nullptr, bool keepRawData = false);
		DLL_OPERATION Resources::Status Destroy();

		DLL_OPERATION virtual ~Mesh();
		DLL_OPERATION bool HasAnimation() { return m_meshData.hasAnimation; };
		/* Set */
		DLL_OPERATION bool SetVertices(Vertex* data, ID3D11Device* dev = nullptr, unsigned int numVerts = 0, bool keepRawData = false);
		DLL_OPERATION bool SetVertices(VertexAnim* data, ID3D11Device* dev = nullptr,unsigned int numVerts = 0, bool keepRawData = false);
		DLL_OPERATION bool SetIndices(unsigned int* indices, unsigned int numIndices, ID3D11Device* dev = nullptr, bool keepRawData = false);
		DLL_OPERATION bool SetMeshData(RawMeshData* newMeshData, ID3D11Device* dev = nullptr, bool keepRawData = false); //This function will delete the existing information and add the new data.
		/* Get */
		DLL_OPERATION ID3D11Buffer* GetVerticesBuffer()     const { return m_vertBuffer; };
		DLL_OPERATION ID3D11Buffer* GetAnimVerticesBuffer() const { return m_AnimVertBuffer; };
		DLL_OPERATION ID3D11Buffer* GetIndicesBuffer()      const { return m_indexBuffer; };
		DLL_OPERATION const unsigned int  GetNumIndices()   const { return m_meshData.m_numIndices; };
		DLL_OPERATION const unsigned int  GetNumVertices()  const { return m_meshData.m_numVerts; };

		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);

	private:
		bool EraseMeshData(); // Helper function to erase existing memory.
	};
}
#endif

