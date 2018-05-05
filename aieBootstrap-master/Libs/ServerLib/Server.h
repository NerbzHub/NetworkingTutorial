#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <fstream>

class Server
{
public:
	Server();
	~Server();

	void handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface);\

	void sendNewClientID(RakNet::RakPeerInterface* pPeerInterface, RakNet::SystemAddress& address);

	std::string getIPAddress();

	void sendClientPing(RakNet::RakPeerInterface* pPeerInterface);

	enum GameMessages
	{
		ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
		ID_SERVER_SET_CLIENT_ID,
		ID_CLIENT_CLIENT_DATA
	};

protected:

	int nextClientID = 1;
};

