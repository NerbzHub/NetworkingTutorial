#include <iostream>
#include <string>
#include <cstdio> // used for gets

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);

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

	handleNetworkMessages(pPeerInterface);

	return 0;
}

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface)
{
	RakNet::Packet* packet = nullptr;

	while (true)
	{
		for (packet = pPeerInterface->Receive(); packet;
			pPeerInterface->DeallocatePacket(packet),
			packet = pPeerInterface->Receive())
		{
			switch (packet->data[0])
			{
			case ID_NEW_INCOMING_CONNECTION:
				std::cout << "A connection is incoming. \n";
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected. \n";
				break;
			case ID_CONNECTION_LOST:
				std::cout << "A client lost the connection. \n";
				break;
			default:
				std::cout << "Received a message with unknown id: " <<
					packet->data[0];
				break;
			}
		}
	}
}