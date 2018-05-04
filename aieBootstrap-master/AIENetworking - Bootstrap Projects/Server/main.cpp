#include <iostream>
#include <string>
//#include <cstdio> // used for gets
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include "GameMessages.h"
#include <fstream>

void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);
void sendNewClientID(RakNet::RakPeerInterface* pPeerInterface, RakNet::SystemAddress& address);
int nextClientID = 1;
std::string getIPAddress();
//std::string ip = getIPAddress();

int main()
{
	const unsigned short PORT = 5456;
	RakNet::RakPeerInterface* pPeerInterface = nullptr;

	//Startup the server
	std::cout << "Starting up the server..." << std::endl;

	//Display the server's IP
	std::cout << "The server's IP is: ";
	getIPAddress();

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

std::string getIPAddress()
{
	std::string line;
	std::ifstream IPFile;
	int offset;
	char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

	system("ipconfig > ip.txt");

	IPFile.open("ip.txt");
	if (IPFile.is_open())
	{
		while (!IPFile.eof())
		{
			getline(IPFile, line);
			if ((offset = line.find(search0, 0)) != std::string::npos)
			{
				//   IPv4 Address. . . . . . . . . . . : 1
				//1234567890123456789012345678901234567890     
				line.erase(0, 39);
				std::cout << line;
			}
		}
	}
	return line;
}