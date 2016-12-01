#include "NetworkService.h"

int NetworkService::sendMessage(SOCKET curSocket, char * message, int messageSize)
{
	return send(curSocket, message, messageSize, 0);	//0 is for flags
}

int NetworkService::receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	return recv(curSocket, buffer, bufSize, 0);	//if no error, returns the number of bytes received and the buffer, else error code
}
