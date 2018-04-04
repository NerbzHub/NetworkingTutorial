#include <iostream>
#include <string>
//#include <cstdio> // used for gets
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include "GameMessages.h"

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);
void sendNewClientID(RakNet::RakPeerInterface* pPeerInterface,
	RakNet::SystemAddress& address);
int nextClientID = 1;


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

	// Startup a thread to ping clients every second.
	// std::thread pingThread(sendClientPing, pPeerInterface);

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
				sendNewClientID(pPeerInterface, packet->systemAddress);
				break;

			case ID_DISCONNECTION_NOTIFICATION:
				std::cout << "A client has disconnected. \n";
				break;
			case ID_CONNECTION_LOST:
				std::cout << "A client lost the connection. \n";
				break;
			case ID_CLIENT_CLIENT_DATA:
			{
				RakNet::BitStream bs(packet->data, packet->length, false);
				pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
					packet->systemAddress, true);
				break;
			}
			default:
				std::cout << "Received a message with unknown id: " <<
					packet->data[0];
				break;
			}
		}
	}
}

void sendNewClientID(RakNet::RakPeerInterface* pPeerInterface, 
					RakNet::SystemAddress& address)
{
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_SET_CLIENT_ID);
	bs.Write(nextClientID);
	nextClientID++;

	pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 
						0, address, false);
}
