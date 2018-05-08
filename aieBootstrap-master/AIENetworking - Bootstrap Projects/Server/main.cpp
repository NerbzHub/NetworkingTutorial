/**
	main.cpp
	Purpose: Console application to run the server. Functions are called from
			 Server.lib to interact with RakNet.

	@author Nathan Nette
*/
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <fstream>
#include "ServerLib\includes\Server.h"


/**
	Main runs the application.

	@return 0.
*/
int main()
{
	// Assign a Port.
	const unsigned short PORT = 5456;

	// Create a new RakPeerInterface pointer.
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	// Console output startup the server.
	std::cout << "Starting up the server..." << std::endl;

	// Display the server's IP.
	std::cout << "The server's IP is: ";

	// Create a new Server.
	Server server;

	// Gets the IP address of the network the server is on.
	server.getIPAddress();

	// Initialize the raknet peer interface first.
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	// Create a socket descriptor to describe this connection.
	RakNet::SocketDescriptor sd(PORT, 0);

	// Call startup - max of 4 connections, on the assigned port.
	pPeerInterface->Startup(4, &sd, 1);

	// Set the maximum amount of incoming connections.
	pPeerInterface->SetMaximumIncomingConnections(32);

	// Handles the packets from the network.
	server.handleNetworkMessages(pPeerInterface);

	// Int functions must return a value.
	return 0;
}