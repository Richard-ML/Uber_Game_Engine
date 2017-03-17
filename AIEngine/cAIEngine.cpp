#include "stdafx.h"
#include "cAIEngine.h"
#include "global.h"
#include <stdio.h>
//#include <stdlib.h>

#include <ctime>
#include <chrono>
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
		return s_cAIEngine;
	}

   DWORD cAIEngine::aiThread(void *lpParam) {
	   std::chrono::high_resolution_clock::time_point lastTime =
		   std::chrono::high_resolution_clock::now();
	   std::chrono::duration<float> deltaTime;
	   cAIEngine *aiEngine =
		   reinterpret_cast<cAIEngine *>(lpParam);

	   while (g_pGameState != 0 && g_pGameState->getGameState() != GAMESTATE_LOADING);
	   do {
		   std::chrono::high_resolution_clock::time_point t2 =
			   std::chrono::high_resolution_clock::now();
		   deltaTime =
			   std::chrono::duration_cast<std::chrono::duration<float>>(
				   std::chrono::high_resolution_clock::now() -
				   lastTime); // Get the time that as passed
		   aiEngine->update(deltaTime.count());
		   lastTime = std::chrono::high_resolution_clock::now();
		   Sleep(15); // Free the thread
	   } while (true);
	   return 0;
   }

   AIEngine_API void AIEngine::cAIEngine::update(float deltaTime)
   {
	   // Do AI stuff!
	   //printf("AI did stuff!\n");


	   return;

   }
   AIEngine_API void cAIEngine::initializeGameStateHandle(iGameState * pGameState)
   {
	   g_pGameState = pGameState;
   }
}
