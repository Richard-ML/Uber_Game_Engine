#ifndef _cAIEngine_HG_
#define _cAIEngine_HG_
#include "stdafx.h"

#ifdef AIENGINE_EXPORTS
#define AIEngine_API __declspec(dllexport)
#else
#define AIEngine_API __declspec(dllimport)
#endif // AIEngine_EXPORTS

namespace AIEngine {
	class cAIEngine_Impl; // Purpose: Encapsulate private member variables and
							 // internal routines. Reduces make-time,
							 // compile-time, and the Fragile Binary Interface
							 // Problem.

	class cAIEngine {
		static cAIEngine *s_cAIEngine;
		// Boilerplate
		friend class cAIEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cAIEngine_Impl *impl() const;
		cAIEngine_Impl *impl();

		cAIEngine() {}; // Constructor is private therefore a new instance can not be made
						   // externally. Only available to members or friends of this class..
		~cAIEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cAIEngine(const cAIEngine &) {}; // Disallow copy constructor
		cAIEngine &operator=(const cAIEngine &AIManager) {
		} // Disallow assignment operator
		static DWORD cAIEngine::aiThread(void *lpParam);
	public:
		static AIEngine_API  cAIEngine *instance();
		AIEngine_API void update(float deltaTime);
		AIEngine_API void initializeGameStateHandle(iGameState* pGameState);
		//AIEngine_API void initializeWorldEditorHandle(iWorldEditor* pWorldEditor);
		//AIEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);

	};

}
#endif