#include "stdafx.h"
#include "cTestEngine.h"
//#include <stdio.h>
//#include <stdlib.h>
#include "cBlackBoxTests.h"
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace TestEngine {
	cTestEngine *cTestEngine::s_cTestEngine =
		0; // Allocating pointer to static instance of cTestEngine (singleton)

	class cTestEngine_Impl : public cTestEngine {
		// Boilerplate
		friend class cTestEngine;
	};
	inline const cTestEngine_Impl *cTestEngine::impl() const {
		return static_cast<const cTestEngine_Impl *>(this);
	}
	inline cTestEngine_Impl *cTestEngine::impl() {
		return static_cast<cTestEngine_Impl *>(this);
	}

   _declspec(dllexport) cTestEngine * cTestEngine::instance()
	{
		if (!s_cTestEngine)
		{
			printf("Test Engine Initialized\n");
			s_cTestEngine = new cTestEngine();
			BlackBoxTests::runTests();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&TestEngine::cTestEngine::TestThread, reinterpret_cast<void*>(s_cTestEngine), 0, &myThreadID);
		}
		return s_cTestEngine;
	}

}