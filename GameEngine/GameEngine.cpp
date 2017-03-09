// GameEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "global.h"
#include <chrono>
//namespace GraphicsEngine {
//	cGameState* g_pGameState = 0;
//}
//namespace AIEngine {
//	cGameState* g_pGameState = 0;
//}
int main()
{

	g_pGameState = new cGameState();
	g_pGameState->setGameState(GAMESTATE_LOADING);

	// Get the number of CPU on this machine
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	std::cout << "Number of processors: " << std::to_string(sysInfo.dwNumberOfProcessors) << std::endl;


	g_pComponentEngine = ComponentEngine::cComponentEngine::instance();
	g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
	g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
	//g_pSoundEngine = SoundEngine::cSoundEngine::instance();
	g_pAIEngine = AIEngine::cAIEngine::instance();



	g_pPhysicsEngine->initializeGameState(g_pGameState);
	g_pAIEngine->initializeGameState(g_pGameState);
	g_pGraphicsEngine->initializeGameState(g_pGameState);

	g_pDebugRenderer = g_pGraphicsEngine->initializeDebugRenderer();

	g_pPhysicsEngine->setDebugRenderer(g_pDebugRenderer);
	
	// TEST DEBUG RENDERER!! :D
	// These shapes are going to be inside the world editor as placeholders. :)
	for (unsigned int nc = 0; nc < 50; nc++)
	{
		for (unsigned int nc2 = 0; nc2 < 50; nc2++)
		{
			g_pDebugRenderer->createCube(glm::vec3(-1100.0f + 64.0f * nc, 0.0f, -1100.0f + 64.0f * nc2), glm::vec3(60.0f, 2.0f, 60.0f), 1.0f, glm::vec3(0.4f, 0.5f, 0.25f));
			g_pDebugRenderer->createCube(glm::vec3(-1100.0f + 64.0f * nc + 32.0f, 14.5f, -1100.0f + 64.0f * nc2), glm::vec3(2.0f, 29.0f, 60.0f), 1.0f, glm::vec3(0.5f, 0.4f, 0.05f));
			g_pDebugRenderer->createCube(glm::vec3(-1100.0f + 64.0f * nc, 14.5f, -1100.0f + 64.0f * nc2 + 32.0f), glm::vec3(60.0f, 29.0f, 2.0f), 1.0f, glm::vec3(0.5f, 0.45f, 0.0f));

		}
	}

	std::cout << "GameEngine Initialized\n";



	// TODO: Crate window using g_pGraphicsEngine interface..
	// .. Load XML data & create entities
	g_pEntityManager->loadGameFromXML("GameAssets.xml");

	// START THE ENGINES!
	std::chrono::high_resolution_clock::time_point lastTime =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime;
	do {
		switch (g_pGameState->getGameState()) {
		case GAMESTATE_LOADING:
			Sleep(90);
			g_pEntityManager->loadGameEntitiesFromXML(g_pGameState->getDifficulty());
			g_pGameState->setGameState(GAMESTATE_RUNNING); // Loading is complete at this point
			break;

		case GAMESTATE_SAVING:
			Sleep(90);
			g_pEntityManager->saveGameToXML((int)g_pGameState->getDifficulty());
			g_pGameState->setGameState(GAMESTATE_RUNNING);
			break;
		}
		
		//Engines are running! CORE ROUTINE --- BEGIN
		std::chrono::high_resolution_clock::time_point t2 =
			std::chrono::high_resolution_clock::now();
		deltaTime =
			std::chrono::duration_cast<std::chrono::duration<float>>(
				std::chrono::high_resolution_clock::now() -
				lastTime); // Get the time that as passed
		/////////////////////////////////////////////////////////////
		g_pGraphicsEngine->update(deltaTime.count());
		Sleep(0);
		// CORE ROUTINE --- END
		lastTime = std::chrono::high_resolution_clock::now();

	} while (true);



	system("pause");

    return 0;
}
