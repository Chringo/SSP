#ifndef NETWORKDLL_NETWORK_NETWORKDATA_H
#define NETWORKDLL_NETWORK_NETWORKDATA_H

#define MAX_PACKET_SIZE 1000000

#include <DirectXMath.h>
#include <ctime>

enum PacketTypes {

	CONNECTION_REQUEST,
	CONNECTION_ACCEPTED,
	DISCONNECT_REQUEST,
	DISCONNECT_ACCEPTED,
	UPDATE_ENTITY,
	UPDATE_ANIMATION,
	UPDATE_WHEEL_STATE,
	UPDATE_BUTTON_STATE,
	UPDATE_LEVER_STATE,
	UPDATE_CAMERA,
	UPDATE_GRAB,
	SYNC_PHYSICS,
	SYNC_READY,
	TEST_PACKET,
};

struct Packet
{
	PacketTypes packet_type;
	int			packet_ID;
	float		timestamp;

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
	unsigned int		entityID;					
	DirectX::XMFLOAT3	newPos;				
	DirectX::XMFLOAT3	newVelocity;
	DirectX::XMFLOAT4X4	newRotation;
	//DirectX::XMVECTOR	newRotationVelocity;

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
	unsigned int	entityID;	
	int				newstate;
	float			transitionDuritation;
	int				blendingType;
	bool			isLooping;
	bool			lockAnimation;
	float			playingSpeed;
	float			velocity;
	int				jointIndex;
	DirectX::XMFLOAT4X4	finalJointTransform;

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(AnimationPacket));
	}
	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(AnimationPacket));
	}
};

struct StateWheelPacket : public Packet
{
	unsigned int entityID;
	int rotationState;
	float rotationAmount;

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(StateWheelPacket));
	}
	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(StateWheelPacket));
	}
};

struct StatePacket : public Packet
{
	unsigned int entityID;
	bool isActive;

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

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(CameraPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(CameraPacket));
	}
};

struct GrabPacket : public Packet
{
	unsigned int entityID;
	int grabbedID;

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(GrabPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(GrabPacket));
	}
};

struct SyncPhysicPacket : public Packet
{
	unsigned int	startIndex;
	unsigned int	nrOfDynamics;
	bool			isHost;
	unsigned int	levelID;
	unsigned int	checkpointID;

	void serialize(char * data)
	{
		memcpy(data, this, sizeof(SyncPhysicPacket));
	}

	void deserialize(char * data)
	{
		memcpy(this, data, sizeof(SyncPhysicPacket));
	}
};
#endif