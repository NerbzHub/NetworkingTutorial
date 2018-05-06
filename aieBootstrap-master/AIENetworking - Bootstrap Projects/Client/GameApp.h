#pragma once
#include <iostream>
#include "Application.h"
#include "Renderer2D.h"
#include "Texture.h"
#include <glm/mat4x4.hpp>
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <unordered_map>
#include "Client.h"

class GameApp : public aie::Application
{
public:
	GameApp();
	virtual ~GameApp();

	

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	//void sendClientGameObject();

protected:
	int m_myClientID;

	const char* IP;
	const unsigned short PORT = 5456;

	float m_cameraX, m_cameraY;

	Client::GameObject m_myGameObject;
	RakNet::RakPeerInterface* m_pPeerInterface;
	Client* client;

	std::unordered_map<int, Client::GameObject > m_otherClientGameObjects;
	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_OldSchoolCarTexture;
	aie::Texture*		m_RaceCarTexture;
	aie::Texture*		m_PoliceCarTexture;
	aie::Texture*		m_UteCarTexture;
	aie::Font*			m_font;
	glm::mat4			m_viewMatrix;
	glm::mat4			m_projectionMatrix;
};

