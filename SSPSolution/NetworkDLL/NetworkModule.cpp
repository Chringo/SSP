#include "NetworkModule.h"

NetworkModule::NetworkModule()
{
	this->isLocked = false;
	this->client_id = 0;
	this->packet_ID = 0;
}

NetworkModule::~NetworkModule()
{
}

int NetworkModule::Initialize()
{
	printf("Trying to Initialized Network module \n");

	this->isLocked = false;
	// create WSADATA object
	WSADATA wsaData;
	// our sockets for the server
	this->listenSocket = INVALID_SOCKET;

	int iResult;	// for error checking return values

	struct addrinfo *result = NULL; // address info for the server to listen to
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 0;
	}

	// set address information
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;    // TCP
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);	//NULL = Dont need addres since it will be on local machine

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 0;
	}

	// Create a SOCKET for connecting to server
	this->listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (this->listenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}

	// Set the mode of the socket to be nonblocking
	u_long iMode = 1;
	iResult = ioctlsocket(this->listenSocket, FIONBIO, &iMode);

	if (iResult == SOCKET_ERROR) {
		printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
		closesocket(this->listenSocket);
		WSACleanup();
		return 0;
	}

	// Setup the TCP listening socket
	iResult = bind(this->listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(this->listenSocket);
		WSACleanup();
		return 0;
	}

	// no longer need address information
	freeaddrinfo(result);

	// start listening for new clients attempting to connect
	iResult = listen(this->listenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(this->listenSocket);
		WSACleanup();
		return 0;
	}

	this->GetMyIp();

	//Start the network system clock
	this->time_start = std::clock();
	
	printf("Network module Initialized\n");

	return 1;
}

int NetworkModule::Shutdown()
{
	int i = 0;
	i = this->connectedClients.size();

	//DISCONNECT_REQUESTs should already have been sent on disconnect,
	//but if there still is connected clients , send a DISCONNECT_REQUEST to them and dont wait for DISCONNECT_ACCEPTED
	if (i > 0)
	{
		const unsigned int packet_size = sizeof(Packet);
		char packet_data[packet_size];

		Packet packet;
		packet.packet_type = DISCONNECT_REQUEST;
		packet.packet_ID = this->packet_ID;
		this->packet_ID++;
		packet.timestamp = this->GetTimeStamp();

		packet.serialize(packet_data);
		this->SendToAll(packet_data, packet_size);
	}
	
	this->connectedClients.clear();
	printf("%d Clients has been removed on server shutdown\n", i);

	return 1;
}

void NetworkModule::Update()
{
	// Get any new clients
	this->AcceptNewClient(this->client_id);

	//Read messages
	this->ReadMessagesFromClients();

}

int NetworkModule::Join(char* ip)
{
	addrinfo *result = NULL;
	addrinfo *ptr = NULL;
	addrinfo hints;

	if (this->my_ip == ip)	//if my_ip is the same as the ip we try to connect to
	{
		printf("Cannot connect to %s as it is this machines local ip\n", ip);
		return 0;
	}
	else
	{
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;  //TCP

		int iResult = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);

		if (iResult != 0)
		{
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 0;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			this->connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

			if (this->connectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 0;
			}

			// Connect to server.
			std::clock_t start = std::clock();
			double dt = 0;
			iResult = SOCKET_ERROR;

			printf("Trying to connect to host...\n");

			iResult = connect(this->connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);	
			if (iResult == SOCKET_ERROR)
			{
				closesocket(this->connectSocket);
				this->connectSocket = INVALID_SOCKET;
				printf("The host %s is down... did not connect\n", ip);
				return 0;
			}
		}

		// no longer need address info for server
		freeaddrinfo(result);

		// if connection failed
		if (this->connectSocket == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 0;
		}

		// Set the mode of the socket to be nonblocking
		u_long iMode = 1;

		iResult = ioctlsocket(this->connectSocket, FIONBIO, &iMode);
		if (iResult == SOCKET_ERROR)
		{
			printf("ioctlsocket failed with error: %d\n", WSAGetLastError());
			closesocket(this->connectSocket);
			WSACleanup();
			return 0;
		}

		//Send CONNECTION_REQUEST package
		const unsigned int packet_size = sizeof(Packet);
		char packet_data[packet_size];

		Packet packet;
		packet.packet_type = CONNECTION_REQUEST;
		packet.packet_ID = this->packet_ID;
		this->packet_ID++;
		packet.timestamp = this->GetTimeStamp();

		packet.serialize(packet_data);
		NetworkService::sendMessage(this->connectSocket, packet_data, packet_size);
		printf("Sent CONNECTION_REQUEST to host\n");

		this->connectedClients.insert(std::pair<unsigned int, SOCKET>(this->client_id, this->connectSocket));
		printf("client %d has been connected to the this client\n", this->client_id);
		this->client_id++;

		return 1;
	}

}

void NetworkModule::SendFlagPacket(PacketTypes type)
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	//Fill the packet
	Packet packet;
	packet.packet_type = type;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void NetworkModule::SendSyncPacket()
{
	const unsigned int packet_size = sizeof(SyncPacket);
	char packet_data[packet_size];

	SyncPacket packet;
	packet.packet_type = CONNECTION_ACCEPTED;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();
	packet.time_start = this->time_start;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void NetworkModule::SendEntityUpdatePacket(unsigned int entityID, DirectX::XMFLOAT3 newPos, DirectX::XMFLOAT3 newVelocity, DirectX::XMFLOAT3 newRotation, DirectX::XMFLOAT3 newRotationVelocity)
{
	const unsigned int packet_size = sizeof(EntityPacket);
	char packet_data[packet_size];

	EntityPacket packet;
	packet.packet_type = UPDATE_ENTITY;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();
	packet.entityID = entityID;
	packet.newPos = newPos;
	packet.newRotation = newRotation;
	packet.newRotationVelocity = newRotationVelocity;
	packet.newVelocity = newVelocity;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void NetworkModule::SendAnimationPacket(unsigned int entityID)
{
	const unsigned int packet_size = sizeof(AnimationPacket);
	char packet_data[packet_size];

	AnimationPacket packet;
	packet.packet_type = UPDATE_ANIMATION;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();
	packet.entityID = entityID;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void NetworkModule::SendStatePacket(unsigned int entityID, bool newState)
{
	const unsigned int packet_size = sizeof(StatePacket);
	char packet_data[packet_size];

	StatePacket packet;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();
	packet.packet_type = UPDATE_STATE;
	packet.entityID = entityID;
	packet.newState = newState;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

void NetworkModule::SendCameraPacket(DirectX::XMFLOAT4 newPos /*, DirectX::XMFLOAT4 newRotation*/)
{
	const unsigned int packet_size = sizeof(CameraPacket);
	char packet_data[packet_size];

	CameraPacket packet;
	packet.packet_ID = this->packet_ID;
	packet.timestamp = this->GetTimeStamp();
	packet.packet_type = UPDATE_CAMERA;
	packet.pos = newPos;
	//packet.rotation = newRotation;

	packet.serialize(packet_data);
	this->SendToAll(packet_data, packet_size);
}

bool NetworkModule::AcceptNewClient(unsigned int & id)
{
	SOCKET otherClientSocket;
	// if client waiting, accept the connection and save the socket
	otherClientSocket = accept(this->listenSocket, NULL, NULL);

	if (otherClientSocket != INVALID_SOCKET)
	{
		//disable nagle on the client's socket
		char value = 1;
		setsockopt(otherClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));	//TCP Options
																						
		// insert new client into session id table 
		this->connectedClients.insert(std::pair<unsigned int, SOCKET>(id, otherClientSocket));
		printf("client %d has been connected to the server\n", this->client_id);
		this->client_id++;

		return true;
	}

	return false;
}

int NetworkModule::ReceiveData(unsigned int client_id, char * recvbuf)
{
	if (this->connectedClients.find(client_id) != this->connectedClients.end())
	{
		SOCKET currentSocket = this->connectedClients[client_id];
		int iResult = NetworkService::receiveMessage(currentSocket, recvbuf, MAX_PACKET_SIZE);
		if (iResult == 0)
		{
			printf("Connection closed\n");
			closesocket(currentSocket);
		}
		return iResult;
	}
	return 0;
}

void NetworkModule::ReadMessagesFromClients()
{
	char network_data[MAX_PACKET_SIZE];
	bool t = true;
	unsigned int header = -1;
	
	Packet p;
	SyncPacket syP;
	EntityPacket eP;
	AnimationPacket aP;
	StatePacket sP;
	CameraPacket cP;

	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = this->connectedClients.begin(); iter != this->connectedClients.end();)
	{
		
		// get data for that client
		int data_length = this->ReceiveData(iter->first, network_data);
		printf("We get here %d\n", data_length);
		if (data_length <= 0)
		{
			//no data recieved exit function
			iter++;
			continue;
		}

		//Read the header (skip the first 4 bytes since it is virtual function information)
		memcpy(&header, &network_data[4], sizeof(PacketTypes));

		switch (header)
		{

		case CONNECTION_REQUEST:

			printf("Host received connection packet from client\n");
			
			p.deserialize(network_data);
			
			this->SendSyncPacket();
			
			//delete p;
			//p = nullptr;
			iter++;
			break;

		case CONNECTION_ACCEPTED:

			printf("Client received CONNECTION_ACCEPTED packet from Host\n");
			
			syP.deserialize(network_data);
			
			//Sync clock
			this->time_current = syP.timestamp;
			this->time_start = syP.time_start;
			
			this->SendFlagPacket(TEST_PACKET);
	
			/*this->SendAnimationPacket(this->testID);
			this->testID++;
			this->SendStatePacket(this->testID, true);
			this->testID++;*/

			iter++;
			break;

		case DISCONNECT_REQUEST:
			
			printf("Host recived: DISCONNECT_REQUEST from Client %d \n", iter->first);
			
			p.deserialize(network_data);

			this->SendFlagPacket(DISCONNECT_ACCEPTED);
			this->RemoveClient(iter->first);	//Send the clientID

			iter = this->connectedClients.end();
			break;

		case DISCONNECT_ACCEPTED:
			
			printf("Client recived: DISCONNECT_ACCEPTED\n");
			
			p.deserialize(network_data);
			
			this->RemoveClient(iter->first);	//Send the clientID

			iter = this->connectedClients.end();
			break;

		case UPDATE_ENTITY:

			printf("Recived ENTITY_UPDATE packet\n");

			eP.deserialize(network_data);
			
			this->packet_Buffer_Entity.push_back(eP);

			//delete eP;
			//eP = nullptr;
			iter++;
			break;

		case UPDATE_ANIMATION:

			printf("Recived ANIMATION_UPDATE packet\n");

			aP.deserialize(network_data);

			this->packet_Buffer_Animation.push_back(aP);

			//delete aP;
			//aP = nullptr;
			iter++;
			break;

		case UPDATE_STATE:

			printf("Recived STATE_UPDATE packet\n");
			
			sP.deserialize(network_data);
			
			this->packet_Buffer_State.push_back(sP);

			//delete sP;
			//sP = nullptr;
			iter++;
			break;
		
		case UPDATE_CAMERA:

			printf("Recived CAMERA_UPDATE packet\n");

			cP.deserialize(network_data);

			this->packet_Buffer_Camera.push_back(cP);

			//delete cP;
			//cP = nullptr;
			iter++;
			break;

		case TEST_PACKET:

			printf("Recived TEST_PACKET packet\n");

			p.deserialize(network_data);

			printf("PacketID: %d, Timestamp: %f\n", p.packet_ID, p.timestamp);

			//this->SendFlagPacket(TEST_PACKET);

			iter++;
			break;


		default:
			printf("Unkown packet type %d\n", header);
		}
	}

}

float NetworkModule::GetTimeStamp()
{
	this->time_current = (std::clock() - this->time_start);
	return this->time_current;
}

int NetworkModule::GetMyIp()
{
	char ac[80];
	struct in_addr addr;

	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) 
	{
		printf("failed to get local ip");
		0;
	}

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		printf("failed to get local ip");
		0;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
	}
	unsigned char b1 = addr.S_un.S_un_b.s_b1;
	unsigned char b2 = addr.S_un.S_un_b.s_b2;
	unsigned char b3 = addr.S_un.S_un_b.s_b3;
	unsigned char b4 = addr.S_un.S_un_b.s_b4;

	//stream the data as int into the string
	std::stringstream ss;
	ss << (int)b1 << "." << (int)b2 << "." << (int)b3 << "." << (int)b4;
	
	this->my_ip.append(ss.str());

	return 1;
}

void NetworkModule::SendToAll(char * packets, int totalSize)
{
	SOCKET currentSocket;
	std::map<unsigned int, SOCKET>::iterator iter;
	int iSendResult;

	for (iter = this->connectedClients.begin(); iter != this->connectedClients.end(); iter++)
	{
		currentSocket = iter->second;	//Secound is the mapped socket
		iSendResult = NetworkService::sendMessage(currentSocket, packets, totalSize);

		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(currentSocket);
		}
		else	//If the message was sent, incresse the packet ID
		{
			this->packet_ID++;
		}

	}
}

bool NetworkModule::RemoveClient(unsigned int clientID)
{
			//Remove the Client from the list
	if (this->connectedClients.erase(clientID) != 0)	//Erase returns nummber of objects deleted
	{	
		printf("Client %d removed\n", clientID);
	}
	else
	{
		printf("No Client removed\n");
	}

	return true;
}

bool NetworkModule::PacketBuffer_isLocked()
{
	return this->isLocked;
}

void NetworkModule::PacketBuffer_Lock()
{
	this->isLocked = true;
}

void NetworkModule::PacketBuffer_UnLock()
{
	this->isLocked = false;
}

std::list<EntityPacket> NetworkModule::PacketBuffer_GetEntityPackets()
{
	std::list<EntityPacket> result;
	std::list<EntityPacket>::iterator iter;

	for (iter = this->packet_Buffer_Entity.begin(); iter != this->packet_Buffer_Entity.end();)
	{
		if (iter->packet_type == UPDATE_ENTITY)
		{
			result.push_back(*iter);
			iter = this->packet_Buffer_Entity.erase(iter);	//Returns the next element after the errased element	
		}
		else
		{
			iter++;
		}

	}

	return result;
}

std::list<AnimationPacket> NetworkModule::PacketBuffer_GetAnimationPackets()
{
	std::list<AnimationPacket> result;
	std::list<AnimationPacket>::iterator iter;

	for (iter = this->packet_Buffer_Animation.begin(); iter != this->packet_Buffer_Animation.end();)
	{
		if (iter->packet_type == UPDATE_ANIMATION)
		{
			result.push_back(*iter);
			iter = this->packet_Buffer_Animation.erase(iter);	//Returns the next element after the errased element
		}
		else
		{
			iter++;
		}

	}

	return result;
}

std::list<StatePacket> NetworkModule::PacketBuffer_GetStatePackets()
{
	std::list<StatePacket> result;
	std::list<StatePacket>::iterator iter;

	for (iter = this->packet_Buffer_State.begin(); iter != this->packet_Buffer_State.end();)
	{
		if (iter->packet_type == UPDATE_STATE)
		{
			result.push_back(*iter);
			iter = this->packet_Buffer_State.erase(iter);	//Returns the next element after the errased element		
		}
		else
		{
			iter++;
		}

	}

	return result;
}

std::list<CameraPacket> NetworkModule::PacketBuffer_GetCameraPackets()
{
	std::list<CameraPacket> result;
	std::list<CameraPacket>::iterator iter;

	for (iter = this->packet_Buffer_Camera.begin(); iter != this->packet_Buffer_Camera.end();)
	{
		if (iter->packet_type == UPDATE_CAMERA)
		{
				result.push_back(*iter);					//We should always be able to cast since the header is correct
				iter = this->packet_Buffer_Camera.erase(iter);	//Returns the next element after the errased element
		}
		else
		{
			iter++;
		}

	}

	return result;
}

int NetworkModule::GetNrOfConnectedClients()
{
	return this->connectedClients.size();
}
