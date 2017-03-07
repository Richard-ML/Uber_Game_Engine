// GameEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "global.h"
#include <chrono>
int main()
{
	g_pComponentEngine = ComponentEngine::cComponentEngine::instance();
	g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
	g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
	// Get the number of CPU on this machine
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	std::cout << "Number of processors: " << std::to_string(sysInfo.dwNumberOfProcessors) << std::endl;


	g_pGameState = g_pGraphicsEngine->g_pGameState;

	g_pSoundEngine = SoundEngine::cSoundEngine::instance();
	g_pAIEngine = AIEngine::cAIEngine::instance();


	std::cout << "GameEngine Initialized\n";



	// TODO: Crate window using g_pGraphicsEngine interface..
	// .. Load XML data & create entities
	g_pEntityManager->loadGameFromXML("GameAssets.xml");
	g_pEntityManager->loadGameEntitiesFromXML(0); // Load game on lowest difficulty
	// Test save game..
	//g_pEntityManager->saveGameToXML(0);

	// START THE ENGINES!
	
	std::chrono::high_resolution_clock::time_point lastTime =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime;
	do {

		if (g_pGameState->_loadRequested == true)
		{
			g_pEntityManager->loadGameEntitiesFromXML(g_pGameState->_difficulty);
			g_pGameState->_loadRequested = false;
		}
		else
		if(g_pGameState->_saveRequested){
			g_pEntityManager->saveGameToXML(g_pGameState->_difficulty);
			g_pGameState->_saveRequested = false;
		}
		
		//Engines are running! CORE ROUTINE --- BEGIN
		std::chrono::high_resolution_clock::time_point t2 =
			std::chrono::high_resolution_clock::now();
		deltaTime =
			std::chrono::duration_cast<std::chrono::duration<float>>(
				std::chrono::high_resolution_clock::now() -
				lastTime); // Get the time that as passed
		/////////////////////////////////////////////////////////////
		g_pPhysicsEngine->update(deltaTime.count());
		//g_pAIEngine->update(deltaTime.count()); // In seconds! // NOTE: AI Engine now has its own thread! :)
		g_pGraphicsEngine->update(deltaTime.count());
		Sleep(90);
		// CORE ROUTINE --- END
		lastTime = std::chrono::high_resolution_clock::now();

	} while (true);



	system("pause");

    return 0;
}
