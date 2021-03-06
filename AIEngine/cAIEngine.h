#ifndef _cAIEngine_HG_
#define _cAIEngine_HG_
#include "stdafx.h"

#ifdef AIENGINE_EXPORTS
#define AIEngine_API __declspec(dllexport)
#else
#define AIEngine_API __declspec(dllimport)
#endif // AIEngine_EXPORTS

namespace AIEngine {
	class cAIEngine_Impl; 

	///-------------------------------------------------------------------------------------------------
	/// <summary>	AI Engine. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	class cAIEngine {
		static cAIEngine *s_cAIEngine;
		// Boilerplate
		friend class cAIEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cAIEngine_Impl *impl() const;
		cAIEngine_Impl *impl();

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// Constructor is private therefore a new instance can not be made externally. Only available to
		/// members or friends of this class.
		/// </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///-------------------------------------------------------------------------------------------------
		cAIEngine() {};
		~cAIEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cAIEngine(const cAIEngine &) {}; // Disallow copy constructor
		cAIEngine &operator=(const cAIEngine &AIManager) {
		} // Disallow assignment operator

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Ai thread. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="lpParam">	[in,out] If non-null, the parameter. </param>
		///
		/// <returns>	A DWORD. </returns>
		///-------------------------------------------------------------------------------------------------

		static DWORD cAIEngine::aiThread(void *lpParam);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Updates based on the given deltaTime. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="deltaTime">	The delta time. </param>
		///-------------------------------------------------------------------------------------------------

		void update(float deltaTime);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Cleans up resources. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///-------------------------------------------------------------------------------------------------

		void cleanup();
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the instance. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	Null if it fails, else a pointer to a cAIEngine. </returns>
		///-------------------------------------------------------------------------------------------------

		static AIEngine_API  cAIEngine *instance();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the game state handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pGameState">	[in,out] If non-null, state of the game. </param>
		///-------------------------------------------------------------------------------------------------

		AIEngine_API void initializeGameStateHandle(iGameState* pGameState);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the world handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pWorld">	[in,out] If non-null, the world. </param>
		///-------------------------------------------------------------------------------------------------

		AIEngine_API void initializeWorldHandle(iWorld* pWorld);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initializes the debug renderer handle. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="pDebugRenderer">	[in,out] If non-null, the debug renderer. </param>
		///-------------------------------------------------------------------------------------------------

		AIEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);

	};

}
#endif