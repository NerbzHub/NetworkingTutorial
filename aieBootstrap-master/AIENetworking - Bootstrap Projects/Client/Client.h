#pragma once

//#ifdef CLIENTDLL_EXPORT
//
//#define CLIENTDLL_API __declspec(dllexport)
//#else
//#define CLIENTDLL_API __declspec(dllimport)
//#endif // CLIENTDLL_EXPORT

#include <iostream>
#include "Application.h"
#include "Renderer2D.h"
#include "Texture.h"
#include <glm/mat4x4.hpp>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <unordered_map>

//extern "C"
//{
struct GameObject
{
	float rotation;
	int clientID;
	glm::vec3 position;
	//glm::vec4 colour;
};

enum GameMessages
{
	ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_SERVER_SET_CLIENT_ID,
	ID_CLIENT_CLIENT_DATA
};

class Client : public aie::Application {

public:

	Client();
	virtual ~Client();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

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
	const char* IP = "127.0.0.1";
	const unsigned short PORT = 5456;

	float m_cameraX, m_cameraY;

	GameObject m_myGameObject;
	RakNet::RakPeerInterface* m_pPeerInterface;
	std::unordered_map<int, GameObject> m_otherClientGameObjects;
	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_OldSchoolCarTexture;
	aie::Texture*		m_RaceCarTexture;
	aie::Texture*		m_PoliceCarTexture;
	aie::Texture*		m_UteCarTexture;
	aie::Font*			m_font;
	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;
};
//}

