#include "stdafx.h"
#include "cGraphicsEngine.h"
#include <stdio.h>
//#include <stdlib.h>
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace GraphicsEngine {
	cGraphicsEngine *cGraphicsEngine::s_cGraphicsEngine =
		0; // Allocating pointer to static instance of cGraphicsEngine (singleton)

	class cGraphicsEngine_Impl : public cGraphicsEngine {
		// Boilerplate
		friend class cGraphicsEngine;
	};
	inline const cGraphicsEngine_Impl *cGraphicsEngine::impl() const {
		return static_cast<const cGraphicsEngine_Impl *>(this);
	}
	inline cGraphicsEngine_Impl *cGraphicsEngine::impl() {
		return static_cast<cGraphicsEngine_Impl *>(this);
	}

	_declspec(dllexport) cGraphicsEngine * cGraphicsEngine::instance()
	{
		if (!s_cGraphicsEngine)
		{
			printf("Graphics Engine Initialized\n");
			s_cGraphicsEngine = new cGraphicsEngine();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&GraphicsEngine::cGraphicsEngine::GraphicsThread, reinterpret_cast<void*>(s_cGraphicsEngine), 0, &myThreadID);
		}
		return s_cGraphicsEngine;
	}

}