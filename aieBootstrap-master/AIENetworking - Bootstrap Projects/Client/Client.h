#pragma once
/**
	Client.h
	Purpose: The client side of the game.

	@author Nathan Nette
*/
#include <iostream>
#include "Application.h"
#include "Renderer2D.h"
#include "Texture.h"
#include <glm/mat4x4.hpp>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <unordered_map>

/**
 GameObject: A struct to store the client ID and vec3 position
			 of the player gameObject.
*/
struct GameObject
{
	// The client ID number of the player when they connect to the server.
	int clientID;
	// Vector3 of the position of the GameObject in worldspace.
	glm::vec3 position;
};

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

/**
	Client: An aie::Bootstrap application that is the client's side of the
			game.
*/
class Client : public aie::Application {

public:
	/**
		Default constructor.
	*/
	Client();
	/**
		Default deconstructor.
	*/
	virtual ~Client();

	/**
		Startup is where most values are initialized.
		It is also where the camera is created.
	*/
	virtual bool startup();

	/**
		Shutdown is called when the application is closed.
		Shutdown calls delete for any allocated memory to
		prevent memory leaks.
	*/
	virtual void shutdown();

	/**
		Update is called every frame, it is where majority of
		the game occurs.

		@param deltaTime The frames per second of the game.
	*/
	virtual void update(float deltaTime);

	/**
		Draws everything needing to be drawn to the screen
		every frame.
	*/
	virtual void draw();


	/**
		Gets an instance of RakPeerInterface and initializes the
		RakNet peer interface first.
	*/
	void handleNetworkConnection();

	/**
		Initializes the connection.
	*/
	void initialiseClientConnection();

	/**
		Handles incoming packets.
	*/
	void handleNetworkMessages();

	/**
		Sets the client ID to however many clients are connected + 1.

		@param packet A pointer to a RakNet packet.
	*/
	void onSetClientIDPacket(RakNet::Packet* packet);

	/**
		Sends the Client's GameObject data across the network.
	*/
	void sendClientGameObject();

	/**
		Called when a data packet is recieved. If the client ID does not
		match our ID, we need to update our client GameObject information.

		@param packet A RakNet packet from the server.
	*/
	void onReceivedClientDataPacket(RakNet::Packet* packet);

	/**
		Returns the client ID number.

		@return The client ID number.
	*/
	int getClientID() { return m_myClientID; };

protected:
	// Client ID of this client.
	int m_myClientID;

	// IP of the server to connect to.
	const char* IP;

	// Assigned port.
	const unsigned short PORT = 5456;

	// Camera's x and y values.
	float m_cameraX, m_cameraY;

	// This client's gameObject.
	GameObject m_myGameObject;

	// A rakPeerInterface to interact with RakNet.
	RakNet::RakPeerInterface* m_pPeerInterface;

	// Keeps track of where each player is based on their client ID.
	std::unordered_map<int, GameObject> m_otherClientGameObjects;

	// Bootstrap class to draw to the screen.
	aie::Renderer2D*	m_2dRenderer;

	// Texture of player 1.
	aie::Texture*		m_OldSchoolCarTexture;

	// Texture of player 2.
	aie::Texture*		m_RaceCarTexture;

	// Texture of player 3.
	aie::Texture*		m_PoliceCarTexture;

	// Texture of player 4.
	aie::Texture*		m_UteCarTexture;

	// Font class so that text can be seen on the screen.
	aie::Font*			m_font;

	// Creating the camera for drawing.
	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;
};


