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
	// Create a new instance of a BitStream.
	RakNet::BitStream bs;

	// Write a message to identify what the packet is.
	bs.Write((RakNet::MessageID)ID_SERVER_SET_CLIENT_ID);

	// Write the next client Id number.
	bs.Write(nextClientID);

	// Add 1 to the client ID for the next client.
	nextClientID++;

	// Send the packet to the server.
	pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED,
		0, address, false);
}

/**
	Returns the IPv4 address of the network this pc is running on.

	@return line Line is a string storing the IPv4 address of this network.
*/
std::string Server::getIPAddress()
{
	// A new string to store the IPv4 of the network this pc is on.
	std::string line;

	// This is for the ipconfig text to be saved to.
	std::ifstream IPFile;

	// An offset for how many lines to go down in the txt file.
	int offset;

	// This is used to search for this series of characters because the IPv4 is after it.
	const char* search0 = "IPv4 Address. . . . . . . . . . . :";

	// Inputs ipconfig into console to get the network data of the PC. Then writes it to ip.txt.
	system("ipconfig > ip.txt");

	// Open the file just created.
	IPFile.open("ip.txt");

	/* Check if the file is open or not. If it is, search through the text file to find
		search0 to then extract the IPv4 address from the text file.*/
	if (IPFile.is_open())
	{
		while (!IPFile.eof())
		{
			getline(IPFile, line);
			if ((offset = line.find(search0, 0)) != std::string::npos)
			{    
				line.erase(0, 39);
				std::cout << line;
			}
		}
	}

	// Return the IPv4 address of the network, extracted from the .txt file.
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
		// Create a new BitStream instance.
		RakNet::BitStream bs;

		// Write a message to identify what's inside this packet.
		bs.Write((RakNet::MessageID)ID_SERVER_TEXT_MESSAGE);

		// Console output Ping!.
		bs.Write("Ping!");

		// Send the packet across the network. 
		pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
			RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

		// Sleep just this thread for a second so that it only pings every second.
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
