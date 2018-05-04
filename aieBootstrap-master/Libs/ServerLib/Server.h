#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include "GameMessages.h"
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

protected:

	int nextClientID = 1;
};

