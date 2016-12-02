#ifndef NETWORKDLL_NETWORK_NETWORKMODULE_H
#define NETWORKDLL_NETWORK_NETWORKMODULE_H

#include "NetworkService.h"
#include <ws2tcpip.h>
#include <map>
#include "NetworkData.h"
#include <list>


#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "6881"

#ifdef NETWORKDLL_EXPORTS  
#define NETWORKDLL_API __declspec(dllexport)   
#else  
#define NETWORKDLL_API __declspec(dllimport)   
#endif  


class NETWORKDLL_API NetworkModule
{

private:
	bool			isLocked;
	unsigned int	client_id;
	int				packet_ID;
	int				time_start;		//time in ms
	int				time_current;	//nr of secounds since start (connected clients will sync to host)

	SOCKET							listenSocket;	
	SOCKET							connectSocket;		// Socket to listen for new connections	
	std::map<unsigned int, SOCKET>	connectedClients;	// table to keep track of each client's socket
	std::list<Packet>				packet_Buffer;

	int ReceiveData(unsigned int client_id, char * recvbuf);
	bool AcceptNewClient(unsigned int & id);			//accept new connections
	void SendToAll(char * packets, int totalSize);
	bool RemoveClient(unsigned int clientID);
	void ReadMessagesFromClients();
	float GetTimeStamp();

	//Test
	int testID = 0;
	DirectX::XMFLOAT3 testFloat3 = DirectX::XMFLOAT3(1.f,2.f,3.f);

public:
	NetworkModule();
	~NetworkModule();

	int Initialize();
	int Shutdown();    

	void Update();
	int Join(char* ip);

	//Public package functions (send to all other clients e.g the only other player)
	void SendFlagPacket(PacketTypes type);
	void SendSyncPacket();
	void SendEntityUpdatePacket(unsigned int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity);
	void SendAnimationPacket(unsigned int entityID);
	void SendStatePacket(unsigned int entityID, bool newState);

	bool PacketBuffer_isEmpty();
	bool PacketBuffer_isLocked();
	void PacketBuffer_Lock();
	void PacketBuffer_UnLock();

	std::list<Packet> PacketBuffer_GetPackets();					//Get ALL the packets in packet_Buffer	
	std::list<EntityPacket> PacketBuffer_GetEntityPackets();		//Get ALL EntityPackets in packet_Buffer	
	std::list<AnimationPacket> PacketBuffer_GetAnimationPackets();	//Get ALL AnimationPackets in packet_Buffer	
	std::list<StatePacket> PacketBuffer_GetStatePackets();			//Get ALL StatePackets in packet_Buffer	

	int GetNrOfConnectedClients();

};


#endif
