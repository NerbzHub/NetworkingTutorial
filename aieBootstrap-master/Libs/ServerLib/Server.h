#pragma once
/**
	Server.h
	Purpose: Server.lib is a library to interact with the RakNet library.
			 It is designed to assist creating a server for multiplayer 2D
			 games across LAN. 

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

/**
	Server is the only class in server.lib so the entirity
	of the lib is in here. 
*/
class Server
{
public:
	/**
		Default Constructor.
	*/
	Server();

	/**
		Default Deconstructor.
	*/
	~Server();

	/**
		Handles incoming packets from the network.

		@param pPeerInterface A pointer to a RakPeerInterface.
	*/
	void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);

	/**
		Sends a new client ID

		@param1 pPeerInterface A pointer to a RakPeerInterface
		@param2 address The IP Address of the network.
	*/
	void sendNewClientID(RakNet::RakPeerInterface* pPeerInterface, RakNet::SystemAddress& address);

	/**
		Returns the IPv4 address of the network this pc is running on.

		@return line Line is a string storing the IPv4 address of this network.
	*/
	std::string getIPAddress();

	/**
		Pings the client.

		@param pPeerInterface A RakPeerInterface pointer.
	*/
	void sendClientPing(RakNet::RakPeerInterface* pPeerInterface);

	/**
		GameMessages: A series of enums to store game messages to be called when
		certain events occur between the server and the client.
	*/
	enum GameMessages
	{
		ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
		ID_SERVER_SET_CLIENT_ID,
		ID_CLIENT_CLIENT_DATA
	};

protected:
	// Sets the first client to 1 rather than 0.
	int nextClientID = 1;
};

