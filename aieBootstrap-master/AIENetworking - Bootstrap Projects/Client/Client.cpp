/**
	Client.h
	Purpose: The client side of the game.

	@author Nathan Nette
*/
#include "Client.h"
#include "Gizmos.h"
#include "Input.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "Font.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

/**
	Default Constructor.
*/
Client::Client()
{

}

/**
	Default Deconstructor.
*/
Client::~Client()
{

}

/**
	Startup is where most values are initialized.
	It is also where the camera is created.
*/
bool Client::startup()
{
	//Creates a renderer for the textures
	m_2dRenderer = new aie::Renderer2D();


	// If the player's ID is 1 then it's player 1 etc.

	// Player 1
	m_OldSchoolCarTexture = new aie::Texture("./bin/textures/OldSchoolCar.png");

	// Player 2
	m_PoliceCarTexture = new aie::Texture("./bin/textures/PoliceCar.png");

	// Player 3
	m_RaceCarTexture = new aie::Texture("./bin/textures/RaceCar.png");

	// Player 4
	m_UteCarTexture = new aie::Texture("./bin/textures/Ute.png");

	// Set the font
	m_font = new aie::Font("./bin/font/consolas.ttf", 32);

	// Sets the starting position of this client's gameObject.
	m_myGameObject.position = glm::vec3(300, 300, 0);

	// Sets the camera's x and y to 0.
	m_cameraX = 0;
	m_cameraY = 0;

	// Sets background colour to grey.
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// Create simple camera transforms.
	m_viewMatrix = glm::lookAt(vec3(0, 10, 10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	// Print to console enter the server's IP address.
	std::cout << "Enter the server's IP address: ";

	// Create a string for the IP the user will input to.
	std::string serverIP;

	// Take in input.
	std::cin >> serverIP;

	// Go to next line.
	std::cout << std::endl;

	// Allocate the input IP into the IP value.
	IP = serverIP.c_str();

	/*	Gets an instance of RakPeerInterface and initializes the
		RakNet peer interface first.*/
	handleNetworkConnection();

	return true;
}

/**
	Shutdown is called when the application is closed.
	Shutdown calls delete for any allocated memory to
	prevent memory leaks.
*/
void Client::shutdown()
{
	/*
		Calling all necessary deletes to deallocate memory.
	*/
	delete m_2dRenderer;
	delete m_OldSchoolCarTexture;
	delete m_PoliceCarTexture;
	delete m_RaceCarTexture;
	delete m_UteCarTexture;
	delete m_font;

	// Deletes all gizmos.
	Gizmos::destroy();
}

/**
	Update is called every frame, it is where majority of
	the game occurs.

	@param deltaTime The frames per second of the game.
*/
void Client::update(float deltaTime)
{
	// Query time since application started.
	float time = getTime();

	/*	Gets an instance of RakPeerInterface and initializes the
	RakNet peer interface first.*/
	handleNetworkMessages();

	// Allow for keyboard input.
	aie::Input* input = aie::Input::getInstance();


	/*
		If either W, A, S or D is pressed on the keyboard, move the client's
		GameObject by adding or subtracting float values to their x and y axis.

		After the calculation of the movement occurs, send the new position to
		the server to update the other clients connected to the server.
	*/
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_myGameObject.position.y += 250.0f * deltaTime;
		sendClientGameObject();
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_myGameObject.position.y -= 250.0f * deltaTime;
		sendClientGameObject();
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_myGameObject.position.x -= 250.0f * deltaTime;
		sendClientGameObject();
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_myGameObject.position.x += 250.0f * deltaTime;
		sendClientGameObject();
	}

	// Send any new data of the client's gameObject to the server.
	sendClientGameObject();

	// Renders everything that needs to be rendered to the screen.
	draw();

	// Quit the application if the user presses escape.
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/**
	Draws everything needing to be drawn to the screen
	every frame.
*/
void Client::draw()
{
	// Wipe the screen to the background colour.
	clearScreen();

	// Set the camera position before we begin rendering.
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// Begin drawing sprites.
	m_2dRenderer->begin();

	// The text to inform the user that to quit they can simply press esc.
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);


	/**
		This switch statement checks which client ID this client has and depending on
		which one it is, it will display a different car.
	*/
	switch (m_myClientID)
	{
	case 1:
	{
		/*	Draws OldSchoolCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1*/
		m_2dRenderer->drawSprite(m_OldSchoolCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;
	}

	case 2:
	{
		/*	Draws PoliceCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1*/
		m_2dRenderer->drawSprite(m_PoliceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;
	}

	case 3:
	{
		/*	Draws RaceCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1*/
		m_2dRenderer->drawSprite(m_RaceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;
	}

	case 4:
	{
		/*	Draws UteCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1*/
		m_2dRenderer->drawSprite(m_UteCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;
	}

	default:
		break;
	}

	/**
		For every other client, do what's in this loop.
	*/
	for (auto& otherClient : m_otherClientGameObjects)
	{
		// Create a rectangle that can have a texture on it.
		m_2dRenderer->setUVRect(0, 0, 1, 1);

		/**
			This switch statement checks the number of the client ID to see which texture 
			needs to be drawn for each other client connected to the server.
		*/
		switch (otherClient.first)
		{
		case 1:
		{
			/*	Draws OldSchoolCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1
			but only if the client ID is 1.*/
			m_2dRenderer->drawSprite(m_OldSchoolCarTexture, otherClient.second.position.x, otherClient.second.position.y, 32.0f, 57.0f, 0, 1);
			break;
		}

		case 2:
		{
			/*	Draws PoliceCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1
			but only if the client ID is 2.*/
			m_2dRenderer->drawSprite(m_PoliceCarTexture, otherClient.second.position.x, otherClient.second.position.y, 32.0f, 57.0f, 0, 1);
			break;
		}

		case 3:
		{
			/*	Draws RaceCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1
			but only if the client ID is 3.*/
			m_2dRenderer->drawSprite(m_RaceCarTexture, otherClient.second.position.x, otherClient.second.position.y, 32.0f, 57.0f, 0, 1);
			break;
		}

		case 4:
		{
			/*	Draws UteCarTexture to the position of this client's game object with a
			width of 32 and height of 57 pixels. It has no rotation and a depth of 1
			but only if the client ID is 4.*/
			m_2dRenderer->drawSprite(m_UteCarTexture, otherClient.second.position.x, otherClient.second.position.y, 32.0f, 57.0f, 0, 1);
			break;
		}

		default:
			break;
		}
	}

	// Done drawing sprites.
	m_2dRenderer->end();
}

/**
	Initializes the connection.
*/
void Client::initialiseClientConnection()
{
	// Create a socket descriptor to describe this connection.
	// No data needed, as we will be connection to a server.
	RakNet::SocketDescriptor sd;

	// Call connect to attempt to connect to the given server.
	m_pPeerInterface->Startup(1, &sd, 1);

	// Console output to display what the IP is for the clients to connect to.
	std::cout << "Connecting to server at: " << IP << std::endl;

	// Call connect to attempt to connect to the given server.
	RakNet::ConnectionAttemptResult res = m_pPeerInterface->Connect(IP, PORT, nullptr, 0);

	// Check to see if we connected, and if not, throw an error.
	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		// Console output that it can't connect and display the error number.
		std::cout << "Unable to start connection, Error number: " << res << std::endl;
	}
}

/**
	Handles incoming packets.
*/
void Client::handleNetworkConnection()
{
	// Initialize the Raknet peer interface first. 
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	// Initialize the connection to the server.
	initialiseClientConnection();
}

/**
	Handles incoming packets.
*/
void Client::handleNetworkMessages()
{
	// Create a RakNet packet.
	RakNet::Packet* packet;

	/**
		Checks the packets and if it matches any of these cases,
		console output these messages.
	*/
	for (packet = m_pPeerInterface->Receive(); packet;
		m_pPeerInterface->DeallocatePacket(packet),
		packet = m_pPeerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected.\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection.\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "Another client has connected.\n";
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Our connection request has been accepted.\n";
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected.\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost.\n";
			break;
		case ID_SERVER_SET_CLIENT_ID:
			onSetClientIDPacket(packet);
			break;
		case ID_CLIENT_CLIENT_DATA:
			onReceivedClientDataPacket(packet);
			break;
		default:
			std::cout << "Received a message with an unknown id:" << packet->data[0];
			break;
		}
	}
}

/**
	Sets the client ID to however many clients are connected + 1.

	@param packet A pointer to a RakNet packet.
*/
void Client::onSetClientIDPacket(RakNet::Packet* packet)
{
	// Creates a new BitStream with the parsed in packet.
	RakNet::BitStream bsIn(packet->data, packet->length, false);

	// Ignore the message ID.
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	// Checks the client ID to know what this client's ID is.
	bsIn.Read(m_myClientID);

	// Console output what my ID is.
	std::cout << "Set my client ID to: " << m_myClientID << std::endl;
}


/**
	Sends the Client's GameObject data across the network.
*/
void Client::sendClientGameObject()
{
	// Create a new Bitstream.
	RakNet::BitStream bs;

	// Write a message to identify what the packet is.
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_CLIENT_DATA);

	// Write this client's client ID.
	bs.Write(m_myClientID);

	// Write the client's GameObject
	bs.Write((char*)&m_myGameObject, sizeof(GameObject));

	// Send the packet to the server.
	m_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

/**
	Called when a data packet is recieved. If the client ID does not
	match our ID, we need to update our client GameObject information.

	@param packet A RakNet packet from the server.
*/
void Client::onReceivedClientDataPacket(RakNet::Packet * packet)
{
	// Create a new bitStream.
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	// Create a new int for clientID.
	int clientID;
	// Read the client ID.
	bsIn.Read(clientID);

	/* If the clientID does not match our ID, we need to update
	 our client GameObject information.*/
	if (clientID != m_myClientID)
	{
		// Create a new GameObject.
		GameObject clientData;

		// Read the client data GameObject.
		bsIn.Read((char*)&clientData, sizeof(GameObject));

		/* Set the recieved GameObject in to the array of other clients
			at the ID of the client.*/
		m_otherClientGameObjects[clientID] = clientData;

		// Output the Game Object information to the console.
		std::cout << "Client " << clientID <<
			" at: " << clientData.position.x <<
			" " << clientData.position.z << std::endl;
	}
}