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
#include "Client.h"

//extern "C"
//{


class Game : public aie::Application {

public:

	Game();
	virtual ~Game();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	

	float m_cameraX, m_cameraY;
	int m_ClientID;
	Client::GameObject m_myGameObject;
	std::unordered_map<int, Client::GameObject> m_otherClientGameObjects;
	Client*				m_client;
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

