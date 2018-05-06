#include "Game.h"
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

Game::Game()
{

}

Game::~Game()
{

}

bool Game::startup()
{
	//Creates a renderer for the textures
	m_2dRenderer = new aie::Renderer2D();

	m_client = new Client();

	m_ClientID = m_client->getClientID();

	// If the player's ID is 1 then it's player 1 etc.
	//
	//	Cops and robbers and the second player is the cop so that it can be 2-4 
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

	m_myGameObject.position = glm::vec3(0, 0, 0);
	//m_myGameObject.colour = glm::vec4(1, 0, 0, 1);

	m_cameraX = 0;
	m_cameraY = 0;
	m_myGameObject.rotation = 0;
	setBackgroundColour(0.25f, 0.25f, 0.25f);



	// initialise gizmo primitive counts
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(0, 10, 10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	std::cout << "Enter the server's IP address: ";

	std::string serverIP;
	std::cin >> serverIP;
	std::cout << std::endl;
	const char* ip = m_client->getIP();
	ip = serverIP.c_str();// new char[serverIP.length()];
						  // trcpy(IP, );


	m_client->handleNetworkConnection();

	return true;
}

void Game::shutdown()
{
	delete m_2dRenderer;
	delete m_OldSchoolCarTexture;
	delete m_PoliceCarTexture;
	delete m_RaceCarTexture;
	delete m_UteCarTexture;
	delete m_font;

	//Gizmos::destroy();
}

void Game::update(float deltaTime)
{

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	//Gizmos::clear();

	m_client->handleNetworkMessages();



	// Allow for input
	aie::Input* input = aie::Input::getInstance();


	//--------------------------------------------------------
	//
	// I need to make the d and a keys rotation and keep its
	// local position so that it can go forward.
	//--------------------------------------------------------

	// WASD controls
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_myGameObject.position.y += 250.0f * deltaTime;
		m_client->sendClientGameObject(&m_myGameObject);
	}
	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_myGameObject.position.y -= 250.0f * deltaTime;
		m_client->sendClientGameObject(&m_myGameObject);
	}
	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_myGameObject.position.x -= 250.0f * deltaTime;
		m_client->sendClientGameObject(&m_myGameObject);
	}
	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_myGameObject.position.x += 250.0f * deltaTime;
		m_client->sendClientGameObject(&m_myGameObject);
	}


	m_client->sendClientGameObject(&m_myGameObject);

	draw();

	// quit if we press escape
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Game::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// Exit text
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	switch (m_ClientID)
	{
	case 1:
		m_2dRenderer->drawSprite(m_OldSchoolCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;

	case 2:
		m_2dRenderer->drawSprite(m_PoliceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;

	case 3:
		m_2dRenderer->drawSprite(m_RaceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;

	case 4:
		m_2dRenderer->drawSprite(m_UteCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
		break;
	default:
		break;
	}

	m_2dRenderer->setUVRect(0, 0, 1, 1);
	m_2dRenderer->drawSprite(m_OldSchoolCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);

	// update perspective in case window resized
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
	//getWindowWidth() / (float)getWindowHeight(),
	//0.1f, 1000.f);
	//m_projectionMatrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, 1.0f, 100.0f);
	//Gizmos::addSphere(m_myGameObject.position, 1.0f, 32, 32, m_myGameObject.colour);
	//Gizmos::add2DAABBFilled(m_myGameObject.position, glm::vec2(32, 32), m_myGameObject.colour);

	for (auto& otherClient : m_otherClientGameObjects)
	{
		//Gizmos::addSphere(otherClient.second.position,
		//	1.0f, 32, 32, otherClient.second.colour);
		m_2dRenderer->setUVRect(0, 0, 1, 1);

		switch (m_ClientID)
		{
		case 1:
			m_2dRenderer->drawSprite(m_OldSchoolCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
			break;

		case 2:
			m_2dRenderer->drawSprite(m_PoliceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
			break;

		case 3:
			m_2dRenderer->drawSprite(m_RaceCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
			break;

		case 4:
			m_2dRenderer->drawSprite(m_UteCarTexture, m_myGameObject.position.x, m_myGameObject.position.y, 32.0f, 57.0f, 0, 1);
			break;

		default:
			break;
		}

		m_2dRenderer->drawSprite(m_OldSchoolCarTexture, otherClient.second.position.x, otherClient.second.position.y, 32.0f, 57.0f, m_myGameObject.rotation);

		/*Gizmos::add2DAABBFilled(otherClient.second.position,
		glm::vec2(32, 32), otherClient.second.colour);*/
	}

	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);

	// done drawing sprites
	m_2dRenderer->end();
}
