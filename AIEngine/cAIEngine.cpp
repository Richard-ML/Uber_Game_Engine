#include "stdafx.h"
#include "cAIEngine.h"
#include <stdio.h>
//#include <stdlib.h>
#include "global.h"
#include <ctime>
#include <chrono>
#include "cAIObject.h"
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace AIEngine {
	cAIEngine *cAIEngine::s_cAIEngine =
		0; // Allocating pointer to static instance of cAIEngine (singleton)
	class cAIEngine_Impl : public cAIEngine {
		// Boilerplate
		friend class cAIEngine;
	};
	inline const cAIEngine_Impl *cAIEngine::impl() const {
		return static_cast<const cAIEngine_Impl *>(this);
	}
	inline cAIEngine_Impl *cAIEngine::impl() {
		return static_cast<cAIEngine_Impl *>(this);
	}

   _declspec(dllexport) cAIEngine * cAIEngine::instance()
	{
	   if (!s_cAIEngine)
	   {
		   printf("AI Engine Initialized\n");
		   s_cAIEngine = new cAIEngine();

		   DWORD myThreadID;
		   HANDLE myHandle = CreateThread(NULL, 0, // stack size
			   (LPTHREAD_START_ROUTINE)&AIEngine::cAIEngine::aiThread, reinterpret_cast<void*>(s_cAIEngine), 0, &myThreadID);
	   }
			// One time setup code goes here!! Anything to initialize? 

		return s_cAIEngine;
	}
   DWORD cAIEngine::aiThread(void *lpParam) {
	   while (g_pGameState == nullptr) { Sleep(5); }
	   while (g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(5); /* Spin wait for load game process to complete */ }

	   std::chrono::high_resolution_clock::time_point lastTime =
		   std::chrono::high_resolution_clock::now();
	   std::chrono::duration<float> deltaTime;
	   cAIEngine *aiEngine =
		   reinterpret_cast<cAIEngine *>(lpParam);
	   do {
		   std::chrono::high_resolution_clock::time_point t2 =
			   std::chrono::high_resolution_clock::now();
		   deltaTime =
			   std::chrono::duration_cast<std::chrono::duration<float>>(
				   std::chrono::high_resolution_clock::now() -
				   lastTime); // Get the time that as passed
							  // DO STUFF!!!
		   while (g_pGameState == nullptr) { Sleep(5); }
		   while (g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(5); /* Spin wait for load game process to complete */ }
		   while (g_pGameState->getGameState() == GAMESTATE_SAVING) { Sleep(5); /* Spin wait for save game process to complete */ }

		
		   for each(cDalekAI* ai in aiEngine->vec_dalekAI)
		   {
			   ai->update(deltaTime.count());
		   }
	    	//////////////
		   lastTime = std::chrono::high_resolution_clock::now();
		   Sleep(5); // Free the thread // TODO: Try messing around with this! 
	   } while (true);

	   // TODO: Clean up resources..

	   return 0;
   }
   //vec_
   AIEngine_API void AIEngine::cAIEngine::update(float deltaTime)
   {
	   // Do AI stuff!
	   //printf("AI did stuff!\n");
	   return;

   }

   AIEngine_API void cAIEngine::generateAI(std::vector<iState*> states)
   {

	   std::vector<std::pair<int, int>> vec_availableTiles;
	   for (int nc = 0; nc < 26; nc++)
		   for (int nc2 = 0; nc2 < 26; nc2++)
			   if (nc == 0 || nc % 5 == 0 || nc2 == 0 || nc2 % 5 == 0)
			   {
				   // divisible by 5 or equal to zero
				   worldTiles[nc][nc2] = 1; // Tile is road-tile, and it's unoccupied place up-to 3 Dalak here
				   vec_availableTiles.push_back(std::make_pair(nc, nc2));
			   }else
				   worldTiles[nc][nc2] = -1;

	   for (unsigned int nc = 0; nc < states.size(); nc++)
	   {
		   cDalekAI * dalekAI = new cDalekAI();
		   dalekAI->pState = states.at(nc);
		   dalekAI->pState->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&cDalekAI::saveToXMLNode, dalekAI)));

		   int tileIndex = rand() % vec_availableTiles.size();
		   std::pair<int, int> tile = vec_availableTiles.at(tileIndex);

			//   glm::vec3 position;
			//   for each(cDalekAI* ai in vec_worldTiles[tile.first][tile.second])
			//   {
			//	   TRY_AGAIN:
			//	   position = glm::vec3(tile.first * 100.0f, 0.0f, tile.second* 100.0f);
			//	   position.x += -25 +  (rand() % 50);
			//	   position.z += -25 +  (rand() % 50);
			//
			//	   if (glm::abs(glm::distance(position, ai->pState->getPosition())) < 10.0f)
			//		   goto TRY_AGAIN;
			//   }
			//   vec_worldTiles[tile.first][tile.second].push_back(dalekAI);
			//   dalekAI->pState->setPosition(position);
			//
			  cAIEngine::s_cAIEngine->vec_dalekAI.push_back(dalekAI);
			//   if (worldTiles[tile.first][tile.second] != -1)
			//	   worldTiles[tile.first][tile.second]++;
			//   else
			//	  printf("Dalek is at an invalid location!\n");
			//   if(worldTiles[tile.first][tile.second] >= 90)
			//	   vec_availableTiles.erase(vec_availableTiles.begin() + tileIndex);
		   }

	   
	   // Start the threads! 
	   //for each(cDalekAI* dalekAI in cAIEngine::s_cAIEngine->vec_dalekAI)
	   //  dalekAI->startAIRoutine();

   }
   AIEngine_API void cAIEngine::initializeGameState(iGameState * gameState)
   {
	   // TODO: Spin lock.. 
	   g_pGameState = gameState;
   }
   AIEngine_API void cAIEngine::setDebugRenderer(iDebugRenderer * debugRenderer)
   {
	   g_pDebugRenderer = debugRenderer;
   }
   AIEngine_API bool cAIEngine::loadAIComponent(rapidxml::xml_node<>* componentNode, iState * state)
   {
	   		//for (rapidxml::xml_node<> *cRigidBody_node = componentNode->first_node("RigidBody");
			//cRigidBody_node; cRigidBody_node = cRigidBody_node->next_sibling("RigidBody")) {
			//// TODO: create base physics object that contains state pointer. Load offsets etcetera..
			//glm::mat4 transform;
			//glm::vec3 offset = glm::vec3(std::stof(cRigidBody_node->first_attribute("offsetX")->value()), std::stof(cRigidBody_node->first_attribute("offsetY")->value()), std::stof(cRigidBody_node->first_attribute("offsetZ")->value()));
			//transform[3] = glm::vec4(offset, 1.0f);
			//
		    //cRigidBody* rb = new cRigidBody();
			//rb->setPosition(offset);
			////state->registerComponentXMLDataCallback(rb->saveToXMLNode);
			//state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&cRigidBody::saveToXMLNode, rb)));
			//
			//state->setTransform(transform);
			//rb->state = state;
			//vec_rigidBodies.push_back(rb);

	   cDalekAI* dalekAI = new cDalekAI();
	   dalekAI->pState = state;

	   state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&cDalekAI::saveToXMLNode, dalekAI)));

	   s_cAIEngine->vec_dalekAI.push_back(dalekAI);

	   return true;
   }
}
