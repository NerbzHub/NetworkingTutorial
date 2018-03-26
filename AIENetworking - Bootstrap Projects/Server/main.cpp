#include <iostream>
#include <string>
#include <cstdio> // used for gets

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>


int main()
{
	const unsigned short PORT = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	//Startup the server
	std::cout << "Starting up the server..." << std::endl;

	//Initialize the raknet peer interface first
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	//Create a socket descriptor to describe this connection.
	RakNet::SocketDescriptor sd(PORT, 0);

	// Call startup - max of 32 connections, on the assigned port
	pPeerInterface->Startup(32, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(32);





	return 0;
}