/**
	Server.cpp
	Purpose: Server.lib is a library to interact with the RakNet library.
			 It is designed to assist creating a server for multiplayer 2D
			 games across LAN.

	@author Nathan Nette
*/
#include "Server.h"

/**
	Default Constructor.
*/
Server::Server()
{
}

/**
	Default Deconstructor.
*/
Server::~Server()
{
}

/**
	Handles incoming packets from the network.

	@param pPeerInterface A pointer to a RakPeerInterface.
*/
void Server::handleNetworkMessages(RakNet::RakPeerInterface * pPeerInterface)
{
	// Create a packet pointer and initialized to nullptr.
	RakNet::Packet* packet = nullptr;

	/**
		While true, checks the packets and if it matches any of these cases,
		console output these messages.
	*/
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

/**
	Sends a new client ID

	@param1 pPeerInterface A pointer to a RakPeerInterface
	@param2 address The IP Address of the network.
*/
void Server::sendNewClientID(RakNet::RakPeerInterface * pPeerInterface, RakNet::SystemAddress & address)
{
	
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)ID_SERVER_SET_CLIENT_ID);
	bs.Write(nextClientID);
	nextClientID++;

	pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED,
		0, address, false);

}

/**
	Returns the IPv4 address of the network this pc is running on.

	@return line Line is a string storing the IPv4 address of this network.
*/
std::string Server::getIPAddress()
{
	std::string line;
	std::ifstream IPFile;
	int offset;
	const char* search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

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

/**
	Pings the client.

	@param pPeerInterface A RakPeerInterface pointer.
*/
void Server::sendClientPing(RakNet::RakPeerInterface * pPeerInterface)
{
	while (true)
	{
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)ID_SERVER_TEXT_MESSAGE);
		bs.Write("Ping!");

		pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
