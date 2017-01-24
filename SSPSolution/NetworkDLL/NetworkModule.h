#ifndef NETWORKDLL_NETWORK_NETWORKMODULE_H
#define NETWORKDLL_NETWORK_NETWORKMODULE_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "NetworkService.h"
#include <ws2tcpip.h>
#include <map>
#include "NetworkData.h"
#include <sstream>
#include <list>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_PORT "6881"

#ifdef NETWORKDLL_EXPORTS  
#define NETWORKDLL_API __declspec(dllexport)   
#else  
#define NETWORKDLL_API __declspec(dllimport)   
#endif  

class NetworkModule
{

private:
	bool			isLocked;		// Mutex lock if is  needed
	unsigned int	client_id;		// ID that is assigned for the next connected client. Is incressed upon assignment
	int				packet_ID;		// ID for the next packet to be sent. Will increse upon packet sent
	int				time_start;		// Start time for the network (This has no practical use currently since the stamps will be different on diffrent machines)
	int				time_current;	// The up time for the network. (Togheter with time_start will calculate the time stamp for a packet)
	std::string		my_ip;
	bool			isHost;			// Bool to see if this client is a host or not

	SOCKET							listenSocket;		// Socket that will listen for incoming data
	SOCKET							connectSocket;		// Socket that connects to a SPECIFIC socket (is used when we want to comunicate with one specific client)
	std::map<unsigned int, SOCKET>	connectedClients;	// Table that maps a clients socket with an ID
	
	// Reciver-buffers where all decoded packets will be placed in.
	// Depending on the packet it will be put into the correct buffer
	std::list<EntityPacket>		packet_Buffer_Entity;
	std::list<AnimationPacket>	packet_Buffer_Animation;
	std::list<StatePacket>		packet_Buffer_State;
	std::list<CameraPacket>		packet_Buffer_Camera;
	std::list<SyncPhysicPacket> packet_Buffer_Physic;

	// Help functions
	int		ReceiveData(unsigned int client_id, char * recvbuf);	// Recive the binary data and stores it into recvbuf
	bool	AcceptNewClient(unsigned int & id);						// Accept new connections and store them into connectedClients
	void	SendToAll(char * packets, int totalSize);				// Send a Packet to all connected clients (Altho we only have one connected client)
	bool	RemoveClient(unsigned int clientID);					// Remove a client based on ID
	void	ReadMessagesFromClients();								// Decode the incoming data into packets and store them into the packet buffers
	float	GetTimeStamp();											// Get the current up time of the network when called
	int		GetMyIp();												// Sets my_ip to the machines local ip-address

	//Test variables
	int testID = 0;
	DirectX::XMFLOAT3 testFloat3 = DirectX::XMFLOAT3(1.f,2.f,3.f);

public:
	NETWORKDLL_API NetworkModule();
	NETWORKDLL_API ~NetworkModule();

	NETWORKDLL_API int Initialize();	// When initialized we will automaticly be joinable and clients can connect to our ip
	NETWORKDLL_API int Shutdown();		// On Shutdown we will remove all still connected clients
	
	NETWORKDLL_API void	Update();				// Accept new clients and read incoming packets 
	NETWORKDLL_API int	Join(char* ip);			// Will try to Join a host with the chosen ip
	NETWORKDLL_API int	GetNrOfConnectedClients();	// Return the number of conencted clients
	NETWORKDLL_API bool	IsHost();					//Return if whatever this client is Host or not

	//Public packet functions (send to all other clients e.g the only other player)
	NETWORKDLL_API void SendFlagPacket(PacketTypes type);
	NETWORKDLL_API void SendSyncPacket();
	NETWORKDLL_API void SendEntityUpdatePacket(unsigned int entityID, DirectX::XMVECTOR newPos, DirectX::XMVECTOR newVelocity, DirectX::XMVECTOR newRotation/*, DirectX::XMVECTOR newRotationVelocity*/);
	NETWORKDLL_API void SendAnimationPacket(unsigned int entityID);
	NETWORKDLL_API void SendStatePacket(unsigned int entityID, bool newState);
	NETWORKDLL_API void SendCameraPacket(DirectX::XMFLOAT4 newPos);
	NETWORKDLL_API void SendPhysicSyncPacket(unsigned int startIndex, unsigned int nrOfDynamics, bool isHost);

	// Mutex functions
	NETWORKDLL_API bool PacketBuffer_isLocked();
	NETWORKDLL_API void PacketBuffer_Lock();
	NETWORKDLL_API void PacketBuffer_UnLock();

	// PacketBuffer functions
	NETWORKDLL_API std::list<EntityPacket>		PacketBuffer_GetEntityPackets();		//Get all packets in packet_Buffer_Entity	
	NETWORKDLL_API std::list<AnimationPacket>	PacketBuffer_GetAnimationPackets();		//Get all packets in packet_Buffer_Animation	
	NETWORKDLL_API std::list<StatePacket>		PacketBuffer_GetStatePackets();			//Get all packets in packet_Buffer_State	
	NETWORKDLL_API std::list<CameraPacket>		PacketBuffer_GetCameraPackets();		//Get all packets in packet_Buffer_Camera
	NETWORKDLL_API std::list<SyncPhysicPacket>	PacketBuffer_GetPhysicPacket();			//Get all packets in packet_Buffer_Physic

};


#endif
