#include "stdafx.h"
#include "cAIEngine.h"
#include "global.h"
#include <stdio.h>
//#include <stdlib.h>

#include <ctime>
#include <chrono>

namespace AIEngine {
	cAIEngine *cAIEngine::s_cAIEngine =
		0; // Allocating pointer to static instance of cAIEngine (singleton)

	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// The PIMPL idiom aka Compilation Firewall Purpose: Encapsulate private member variables.
	/// Reduces make-time, compile-time, and the Fragile Binary Interface Problem.
	/// </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	class cAIEngine_Impl : public cAIEngine {
		// Boilerplate
		friend class cAIEngine;
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a const cAIEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline const cAIEngine_Impl *cAIEngine::impl() const {
		return static_cast<const cAIEngine_Impl *>(this);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cAIEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline cAIEngine_Impl *cAIEngine::impl() {
		return static_cast<cAIEngine_Impl *>(this);
	}

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Singleton Constructor. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///-------------------------------------------------------------------------------------------------

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

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Ai thread. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///
   /// <param name="lpParam">	[in,out] If non-null, the parameter. </param>
   ///
   /// <returns>	A DWORD. </returns>
   ///-------------------------------------------------------------------------------------------------

   DWORD cAIEngine::aiThread(void *lpParam) {
	   std::chrono::high_resolution_clock::time_point lastTime =
		   std::chrono::high_resolution_clock::now();
	   std::chrono::duration<float> deltaTime;
	   cAIEngine *aiEngine =
		   reinterpret_cast<cAIEngine *>(lpParam);

	   while (g_pGameState == 0 || g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(50); }
	   do {
		   std::chrono::high_resolution_clock::time_point t2 =
			   std::chrono::high_resolution_clock::now();
		   deltaTime =
			   std::chrono::duration_cast<std::chrono::duration<float>>(
				   std::chrono::high_resolution_clock::now() -
				   lastTime); // Get the time that as passed
		   lastTime = std::chrono::high_resolution_clock::now();
		   switch (g_pGameState->getGameState())
		   {
		   case GAMESTATE_EXIT:
			   s_cAIEngine->cleanup();
			   return 0;
			   break;
		   }
		   aiEngine->update(deltaTime.count());

		   Sleep(15); // Free the thread
	   } while (true);
	   return 0;
   }

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Updates based on given deltaTime. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///
   /// <param name="deltaTime">	The delta time. </param>
   ///-------------------------------------------------------------------------------------------------

   void AIEngine::cAIEngine::update(float deltaTime)
   {
	   // Do AI stuff!
	   //printf("AI did stuff!\n");
	   return;

   }

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Cleanups this resources. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///-------------------------------------------------------------------------------------------------

   void cAIEngine::cleanup()
   {
	   s_cAIEngine->~cAIEngine();

	  // NOTE: These global pointers are shared with each engine. The GameEngine Thread will delete them once it's ready to exit. 
	  // delete g_pDebugRenderer;
	  // delete g_pGameState;
	  // delete g_pWorld;
   }

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Initializes the game state handle. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///
   /// <param name="pGameState">	[in,out] If non-null, state of the game. </param>
   ///-------------------------------------------------------------------------------------------------

   AIEngine_API void cAIEngine::initializeGameStateHandle(iGameState * pGameState)
   {
	   g_pGameState = pGameState;
   }

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Initializes the world handle. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///
   /// <param name="pWorld">	[in,out] If non-null, the world. </param>
   ///-------------------------------------------------------------------------------------------------

   AIEngine_API void cAIEngine::initializeWorldHandle(iWorld * pWorld)
   {
	   g_pWorld = pWorld;
   }

   ///-------------------------------------------------------------------------------------------------
   /// <summary>	Initializes the debug renderer handle. </summary>
   ///
   /// <remarks>	Richard, 4/12/2017. </remarks>
   ///
   /// <param name="pDebugRenderer">	[in,out] If non-null, the debug renderer. </param>
   ///-------------------------------------------------------------------------------------------------

   AIEngine_API void cAIEngine::initializeDebugRendererHandle(iDebugRenderer * pDebugRenderer)
   {
	   g_pDebugRenderer = pDebugRenderer;
   }
}
