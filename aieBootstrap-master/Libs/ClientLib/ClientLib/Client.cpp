#include "Client.h"

Client::Client()
{

}

Client::~Client()
{

}

void Client::handleNetworkMessages(RakNet::RakPeerInterface* pPeerInterface)
{
	RakNet::Packet* packet;

	for (packet = pPeerInterface->Receive(); packet;
		pPeerInterface->DeallocatePacket(packet),
		packet = pPeerInterface->Receive())
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
		/*case ID_CLIENT_CLIENT_DATA:
			onReceivedClientDataPacket(packet);
			break;*/
		default:
			std::cout << "Received a message with an unknown id:" << packet->data[0];
			break;
		}
	}
}

void Client::initialiseClientConnection(RakNet::RakPeerInterface* pPeerInterface)
{
	// Create a socket descriptor to describe this connection
	// No data needed, as we will be connection to a server
	RakNet::SocketDescriptor sd;

	// Now call connect to attempt to connect to the given server
	pPeerInterface->Startup(1, &sd, 1);

	std::cout << "Connecting to server at: " << IP << std::endl;

	// Now call connect to attempt to connect to the given server
	RakNet::ConnectionAttemptResult res = pPeerInterface->Connect(IP, PORT, nullptr, 0);

	// Finally, check to see if we connected, and if not, throw an error
	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Unable to start connection, Error number: " << res << std::endl;
	}
}

void Client::handleNetworkConnection(RakNet::RakPeerInterface* pPeerInterface)
{
	//Initialize the Raknet peer interface first 
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	initialiseClientConnection(pPeerInterface);
}

void Client::onSetClientIDPacket(RakNet::Packet* packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(m_myClientID);

	std::cout << "Set my client ID to: " << m_myClientID << std::endl;
}

void Client::sendClientGameObject(RakNet::RakPeerInterface* pPeerInterface, int myClientID, GameObject* myGameObject)
{
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_CLIENT_DATA);
	bs.Write(myClientID);
	bs.Write((char*)&myGameObject, sizeof(GameObject));
	/*bs.Write((char*)&posX, sizeof(float));
	bs.Write((char*)&posY, sizeof(float));*/


	pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED,
		0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void Client::onReceivedClientDataPacket(RakNet::Packet * packet, std::unordered_map<int, GameObject> otherClientGameObjects)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int clientID;
	bsIn.Read(clientID);

	//If the clientID does not match our ID, we need to update
	//our client GameObject information.
	if (clientID != m_myClientID)
	{
		GameObject clientData;
		bsIn.Read((char*)&clientData, sizeof(GameObject));

		otherClientGameObjects[clientID] = clientData;

		//For now, just output the Game Object information to the
		//console
		std::cout << "Client " << clientID <<
			" at: " << clientData.position.x <<
			" " << clientData.position.z << std::endl;
	}
}