#include "stdafx.h"
#include "cSoundEngine.h"
#include <stdio.h>
//#include <stdlib.h>

// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace SoundEngine {
	cSoundEngine *cSoundEngine::s_cSoundEngine =
		0; // Allocating pointer to static instance of cSoundEngine (singleton)

	class cSoundEngine_Impl : public cSoundEngine {
		// Boilerplate
		friend class cSoundEngine;
	};
	inline const cSoundEngine_Impl *cSoundEngine::impl() const {
		return static_cast<const cSoundEngine_Impl *>(this);
	}
	inline cSoundEngine_Impl *cSoundEngine::impl() {
		return static_cast<cSoundEngine_Impl *>(this);
	}

	_declspec(dllexport) cSoundEngine * cSoundEngine::instance()
	{
		if (!s_cSoundEngine)
		{
			printf("Sound Engine Initialized\n");
			s_cSoundEngine = new cSoundEngine();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&SoundEngine::cSoundEngine::SoundThread, reinterpret_cast<void*>(s_cSoundEngine), 0, &myThreadID);
		}
		return s_cSoundEngine;
	}

}