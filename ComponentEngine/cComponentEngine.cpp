#include "stdafx.h"
#include "stdafx.h"
#include "cComponentEngine.h"
#include <stdio.h>
//#include <stdlib.h>
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace ComponentEngine {
	cComponentEngine *cComponentEngine::s_cComponentEngine =
		0; // Allocating pointer to static instance of cComponentEngine (singleton)

	class cComponentEngine_Impl : public cComponentEngine {
		// Boilerplate
		friend class cComponentEngine;
	};
	inline const cComponentEngine_Impl *cComponentEngine::impl() const {
		return static_cast<const cComponentEngine_Impl *>(this);
	}
	inline cComponentEngine_Impl *cComponentEngine::impl() {
		return static_cast<cComponentEngine_Impl *>(this);
	}

	_declspec(dllexport) cComponentEngine * cComponentEngine::instance()
	{
		if (!s_cComponentEngine)
		{
			printf("Component Engine Initialized\n");
			s_cComponentEngine = new cComponentEngine();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&ComponentEngine::cComponentEngine::ComponentThread, reinterpret_cast<void*>(s_cComponentEngine), 0, &myThreadID);
		}
		return s_cComponentEngine;
	}

}