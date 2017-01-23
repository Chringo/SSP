#ifndef RESOURCELIB_FILEHEADERS_H
#define RESOURCELIB_FILEHEADERS_H
#include <DirectXMath.h>
struct Vector2{
	float u, v;
};
struct Vector3{
	float x, y, z;
};
struct Vector4{
	float x, y, z, w;
};
struct MainHeader{
	unsigned int id;
	int resourceType;
};

struct MeshHeader{
	unsigned int numVerts    = 0;
	unsigned int indexLength = 0;
	bool skeleton;
};

struct BoundingBoxHeader{
	Vector3 position;		 //Positionen på boundingboxen
	float extension[3];		 //Skalären
	Vector3 extensionDir[3]; //Orto
	Vector3 PivotPosition;
};

struct SkeletonHeader {
	unsigned int jointCount		= 0;
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
struct KeyFrameHeader
{
	float timeValue;
	float translation[3];
	float rotation[3];
	float quaternion[4];
	float scale[3];
};
struct MaterialHeader{
	unsigned int textureIDs[5]{0,0,0,0,0};
	//unsigned int textureNameLength[5]{0,0,0,0,0};

	float m_Metallic	  = 0.0f	;
	float m_Roughness	  = 0.0f	;
	float m_EmissiveValue = 0.0f	;

};
struct TextureHeader{
	char filePath[256];
};

struct RegistryHeader {
	unsigned int numIds;
};
struct RegistryItem {
	unsigned int id;		// Resource id
	unsigned int startBit;  // Place in BPF file
	unsigned int byteSize;  // How many bytes to memcpy.
};
#endif
