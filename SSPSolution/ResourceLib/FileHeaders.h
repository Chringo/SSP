#ifndef RESOURCELIB_FILEHEADERS_H
#define RESOURCELIB_FILEHEADERS_H

struct Vector2
{
	float u, v;
};
struct Vector3
{
	float x, y, z;
};
struct Vector4
{
	float x, y, z, w;
};

struct MainHeader
{
	unsigned int id;
	int resourceType;
};



struct MeshHeader{
	unsigned int numVerts    = 0;
	unsigned int indexLength = 0;
	bool skeleton;
};
struct BoundingBoxHeader
{
	float extension; //skalären
	Vector3 extensionDir;
	Vector3 position;
	//float min, max, centerPoint;
};
struct SkeletonHeader {
	unsigned int jointCount = 0;
	unsigned int animLayerCount = 0;
};
struct LayerIdHeader{
	unsigned int id = 0;
};
struct JointHeader {
	float invbindPose[16];
	int jointIndex;
	int parentIndex;
};
struct MaterialHeader
{
	unsigned int textureIDs[5]{0,0,0,0,0};

	float m_Metallic	  = 0.0f	;
	float m_Roughness	  = 0.0f	;
	float m_EmissiveValue = 0.0f	;

};

struct TextureHeader
{
	char filePath[256];
};
#endif
