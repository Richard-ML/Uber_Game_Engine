#include "stdafx.h"
#include "cAIEngine.h"
#include <stdio.h>
//#include <stdlib.h>
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
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&AIEngine::cAIEngine::AIThread, reinterpret_cast<void*>(s_cAIEngine), 0, &myThreadID);
		}
		return s_cAIEngine;
	}

}