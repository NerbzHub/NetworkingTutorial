#include <iostream>
#include <string>
//#include <cstdio> // used for gets
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <fstream>
#include "ServerLib\includes\Server.h"
#include "ServerLib\includes\GameMessages.h"


//-------------------------need to add the .lib of serverlib and properly linked it.
int main()
{

	const unsigned short PORT = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	//Startup the server
	std::cout << "Starting up the server..." << std::endl;

	//Display the server's IP
	std::cout << "The server's IP is: ";
	Server server;
	server.getIPAddress();

	//Initialize the raknet peer interface first
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	//Create a socket descriptor to describe this connection.
	RakNet::SocketDescriptor sd(PORT, 0);

	// Call startup - max of 32 connections, on the assigned port
	pPeerInterface->Startup(32, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(32);

	server.handleNetworkMessages(pPeerInterface);

	// Startup a thread to ping clients every second.
	// std::thread pingThread(sendClientPing, pPeerInterface);

	return 0;
}