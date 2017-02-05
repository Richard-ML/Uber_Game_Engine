#ifndef _cComponentEngine_HG_
#define _cComponentEngine_HG_
#include "stdafx.h"

#ifdef COMPONENTENGINE_EXPORTS
#define ComponentEngine_API __declspec(dllexport)
#else
#define ComponentEngine_API __declspec(dllimport)
#endif // ComponentEngine_EXPORTS

namespace ComponentEngine {
	class cComponentEngine_Impl; // Purpose: Encapsulate private member variables and
						  // internal routines. Reduces make-time,
						  // compile-time, and the Fragile Binary Interface
						  // Problem.

	class cComponentEngine {
		static cComponentEngine *s_cComponentEngine;
		// Boilerplate
		friend class cComponentEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cComponentEngine_Impl *impl() const;
		cComponentEngine_Impl *impl();

		cComponentEngine() {}; // Constructor is private therefore a new instance can not be made
						// externally. Only available to members or friends of this class..
		~cComponentEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cComponentEngine(const cComponentEngine &) {}; // Disallow copy constructor
		cComponentEngine &operator=(const cComponentEngine &ComponentManager) {
		} // Disallow assignment operator

	public:
		static ComponentEngine_API  cComponentEngine *instance();
		//static DWORD __cdecl ComponentThread(void* lpParam);
	};

}
#endif