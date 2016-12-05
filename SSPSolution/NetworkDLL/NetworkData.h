#ifndef NETWORKDLL_NETWORK_NETWORKDATA_H
#define NETWORKDLL_NETWORK_NETWORKDATA_H

#define MAX_PACKET_SIZE 1000000

#include <DirectXMath.h>
#include <ctime>

enum PacketTypes {

	CONNECTION_REQUEST = 0,
	CONNECTION_ACCEPTED = 1,
	DISCONNECT_REQUEST = 2,
	DISCONNECT_ACCEPTED = 3,
	UPDATE_ENTITY = 4,
	UPDATE_ANIMATION = 5,
	UPDATE_STATE = 6,
	UPDATE_CAMERA = 7,
	TEST_PACKET = 8,
};

struct Packet
{
	PacketTypes packet_type;
	int packet_ID;
	float timestamp;

	virtual void serialize(char * data)
	{			//Turn the PacketType into bytes
		memcpy(data, this, sizeof(Packet));
	}

	virtual void deserialize(char * data)
	{			//Turn bytes into PacketType
		memcpy(this, data, sizeof(Packet));
	}

};

struct SyncPacket: public Packet
{
	std::clock_t time_start;

	virtual void serialize(char * data)
	{			
		memcpy(data, this, sizeof(SyncPacket));
	}
	virtual void deserialize(char * data)
	{			
		memcpy(this, data, sizeof(SyncPacket));
	}
};

struct EntityPacket: public Packet
{											
	unsigned int entityID;					
	DirectX::XMFLOAT3 newPos;				
	DirectX::XMFLOAT3 newVelocity;			
	DirectX::XMFLOAT3 newRotation;			
	DirectX::XMFLOAT3 newRotationVelocity;	

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(EntityPacket));
	}
	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(EntityPacket));
	}
};

struct AnimationPacket : public Packet
{
	unsigned int entityID;	
	unsigned int state;		
	unsigned int keyframe;	

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(AnimationPacket));
	}
	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(AnimationPacket));
	}
};

struct StatePacket : public Packet
{
	unsigned int entityID;
	bool newState;		

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(StatePacket));
	}
	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(StatePacket));
	}
};

struct CameraPacket : public Packet
{
	DirectX::XMFLOAT4 pos;
	//DirectX::XMFLOAT4 rotation;

	void serialize(char * data)
	{			//Turn the PacketType into bytes
		memcpy(data, this, sizeof(CameraPacket));
	}

	void deserialize(char * data)
	{			//Turn bytes into PacketType
		memcpy(this, data, sizeof(CameraPacket));
	}
};

#endif