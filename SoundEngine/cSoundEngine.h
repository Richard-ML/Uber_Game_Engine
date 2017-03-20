#ifndef _cSoundEngine_HG_
#define _cSoundEngine_HG_
#include "stdafx.h"

#ifdef SOUNDENGINE_EXPORTS
#define SoundEngine_API __declspec(dllexport)
#else
#define SoundEngine_API __declspec(dllimport)
#endif // SoundEngine_EXPORTS

namespace SoundEngine {
	class cSoundEngine_Impl; // Purpose: Encapsulate private member variables and
							// internal routines. Reduces make-time,
							// compile-time, and the Fragile Binary Interface
							// Problem.

	class cSoundEngine {
		static cSoundEngine *s_cSoundEngine;
		// Boilerplate
		friend class cSoundEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cSoundEngine_Impl *impl() const;
		cSoundEngine_Impl *impl();

		cSoundEngine() {}; // Constructor is private therefore a new instance can not be made
						  // externally. Only available to members or friends of this class..
		~cSoundEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cSoundEngine(const cSoundEngine &) {}; // Disallow copy constructor
		cSoundEngine &operator=(const cSoundEngine &SoundManager) {
		} // Disallow assignment operator

	public:
		static SoundEngine_API  cSoundEngine *instance();
		static DWORD __cdecl soundThread(void* lpParam);
		SoundEngine_API void initializeGameStateHandle(iGameState* pGameState);
		static SoundEngine_API void initializeWorldHandle(iWorld* pWorld);
		static SoundEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);
	
	};

}
#endif