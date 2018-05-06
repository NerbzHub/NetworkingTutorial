#pragma once
#include <iostream>
//#include "Application.h"
//#include "Renderer2D.h"
//#include "Texture.h"
#include <glm/mat4x4.hpp>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <unordered_map>

class Client
{
public:
	Client();
	~Client();

	struct GameObject
	{
		int clientID;
		float rotation;
		glm::vec3 position;
	};

	enum GameMessages
	{
		ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
		ID_SERVER_SET_CLIENT_ID,
		ID_CLIENT_CLIENT_DATA
	};

	// Initialize the connection 
	void handleNetworkConnection();
	void initialiseClientConnection();

	// Handle incoming packets 
	void handleNetworkMessages();

	void onSetClientIDPacket(RakNet::Packet * packet);

	void sendClientGameObject();

	void onReceivedClientDataPacket(RakNet::Packet * packet);

	int getClientID() { return m_myClientID; };

protected:
	int m_myClientID;
	const char* IP;
	const unsigned short PORT = 5456;
	GameObject m_myGameObject;
	RakNet::RakPeerInterface* m_pPeerInterface;
	std::unordered_map<int, GameObject> m_otherClientGameObjects;
};