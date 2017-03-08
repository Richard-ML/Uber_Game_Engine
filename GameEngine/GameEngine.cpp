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
	std::cout << "GameEngine Initialized\n";



	// TODO: Crate window using g_pGraphicsEngine interface..
	// .. Load XML data & create entities
	g_pEntityManager->loadGameFromXML("GameAssets.xml");
	//g_pEntityManager->loadGameEntitiesFromXML(0); // Load game on lowest difficulty
	// Test save game..
	//g_pEntityManager->saveGameToXML(0);

	// START THE ENGINES!
	//g_pGameState->setLoadStatus(false);
	std::chrono::high_resolution_clock::time_point lastTime =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime;
	do {
		switch (g_pGameState->getGameState()) {
		case GAMESTATE_LOADING:
			g_pEntityManager->loadGameEntitiesFromXML(g_pGameState->getDifficulty());
			g_pGameState->setGameState(GAMESTATE_RUNNING); // Loading is complete at this point
			break;

		case GAMESTATE_SAVING:
			g_pEntityManager->saveGameToXML(g_pGameState->getDifficulty());
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
