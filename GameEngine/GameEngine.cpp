// GameEngine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "global.h"
#include <chrono>
int main()
{
	ComponentEngine::cComponentEngine * g_pComponentEngine = ComponentEngine::cComponentEngine::instance();
	PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
	GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
	SoundEngine::cSoundEngine * g_pSoundEngine = SoundEngine::cSoundEngine::instance();
	AIEngine::cAIEngine * g_pAIEngine = AIEngine::cAIEngine::instance();

	std::cout << "GameEngine Initialized\n";



	// TODO: Crate window using g_pGraphicsEngine interface..
	// .. Load XML data & create entities
	g_pEntityManager->loadGameFromXML("GameAssets.xml");
	
	// Test save game..
	g_pEntityManager->saveGameToXML();


	// START THE ENGINES!
	
	std::chrono::high_resolution_clock::time_point lastTime =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime;
	do {
		//Engines are running! CORE ROUTINE --- BEGIN
		std::chrono::high_resolution_clock::time_point t2 =
			std::chrono::high_resolution_clock::now();
		deltaTime =
			std::chrono::duration_cast<std::chrono::duration<float>>(
				std::chrono::high_resolution_clock::now() -
				lastTime); // Get the time that as passed
		/////////////////////////////////////////////////////////////
		g_pPhysicsEngine->update(deltaTime.count());
		g_pAIEngine->update(deltaTime.count()); // In seconds!
		g_pGraphicsEngine->update(deltaTime.count());

		// CORE ROUTINE --- END
		lastTime = std::chrono::high_resolution_clock::now();

	} while (true);



	system("pause");

    return 0;
}
