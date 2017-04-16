// GameEngine.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include "global.h"
#include <chrono>

///-------------------------------------------------------------------------------------------------
/// <summary>	Main entry-point for this application. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Exit-code for the process - 0 for success, else an error code. </returns>
///-------------------------------------------------------------------------------------------------

int main()
{
	// Set game state to loading. This way the threads will wait until content loading is complete before starting their routines.
	g_pGameState = new cGameState();
	g_pGameState->setGameState(GAMESTATE_LOADING);


	ComponentEngine::cComponentEngine * g_pComponentEngine = ComponentEngine::cComponentEngine::instance();
	PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
	GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
	SoundEngine::cSoundEngine * g_pSoundEngine = SoundEngine::cSoundEngine::instance();
	AIEngine::cAIEngine * g_pAIEngine = AIEngine::cAIEngine::instance();
	
	std::cout << "GameEngine Initialized\n";

	iGameState* gameState = dynamic_cast<iGameState*>(g_pGameState);

	g_pPhysicsEngine->initializeGameStateHandle(gameState);
	g_pGraphicsEngine->initializeGameStateHandle(gameState);
	g_pAIEngine->initializeGameStateHandle(gameState);
	g_pSoundEngine->initializeGameStateHandle(gameState);


	g_pDebugRenderer = g_pGraphicsEngine->getDebugRendererHandle();
	g_pPhysicsEngine->initializeDebugRendererHandle(g_pDebugRenderer);
	g_pSoundEngine->initializeDebugRendererHandle(g_pDebugRenderer);
	g_pAIEngine->initializeDebugRendererHandle(g_pDebugRenderer);

	g_pWorld = new cWorld();
	g_pWorld->generateObjectsAtSelectedTiles = std::bind(&cEntityManager::spawnObjectsAtSelectedTile, g_pEntityManager);
	g_pWorld->pPhysicsEngine = g_pPhysicsEngine;
	iWorld* world = dynamic_cast<iWorld*>(g_pWorld);
	g_pPhysicsEngine->initializeWorldHandle(world);
	g_pGraphicsEngine->initializeWorldHandle(world);
	g_pSoundEngine->initializeWorldHandle(world);
	g_pAIEngine->initializeWorldHandle(world);
	// TEST DEBUG RENDERER
	//g_pDebugRenderer->createCube(glm::vec3(0.0f, 6.0f, -12.0f),glm::vec3(6.0f, 5.0f, 6.0f), 1.0f, glm::vec3(0.4f, 0.5f, 0.25f));


	// TODO: Crate window using g_pGraphicsEngine interface..
	// .. Load XML data & create entities
	g_pEntityManager->loadGameFromXML("GameAssets.xml");

	// START THE ENGINES!
	g_pGameState->setGameState(GAMESTATE_RUNNING);
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
		case GAMESTATE_EXIT:
			g_pEntityManager->cleanup();
			g_pPhysicsEngine->cleanup();
			delete g_pDebugRenderer;
			delete g_pGameState;
			delete g_pWorld;
			_exit(EXIT_SUCCESS);
			break;
		}
		//Engines are running! CORE ROUTINE --- BEGIN
		std::chrono::high_resolution_clock::time_point t2 =
			std::chrono::high_resolution_clock::now();
		deltaTime =
			std::chrono::duration_cast<std::chrono::duration<float>>(
				std::chrono::high_resolution_clock::now() -
				lastTime); // Get the time that as passed
		lastTime = std::chrono::high_resolution_clock::now();

		/////////////////////////////////////////////////////////////
		// The GraphicsEngine is updated here on the main thread. The other engines have their own threads.
		g_pGraphicsEngine->update(deltaTime.count());
		// CORE ROUTINE --- END
		Sleep(10);


	} while (true);

	system("pause");

    return 0;
}
