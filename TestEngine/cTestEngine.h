#ifndef _cTestEngine_HG_
#define _cTestEngine_HG_
#include "stdafx.h"

#ifdef TESTENGINE_EXPORTS
#define TestEngine_API __declspec(dllexport)
#else
#define TestEngine_API __declspec(dllimport)
#endif // TestEngine_EXPORTS

namespace TestEngine {
	class cTestEngine_Impl; // Purpose: Encapsulate private member variables and
							// internal routines. Reduces make-time,
							// compile-time, and the Fragile Binary Interface
							// Problem.

	class cTestEngine {
		static cTestEngine *s_cTestEngine;
		// Boilerplate
		friend class cTestEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cTestEngine_Impl *impl() const;
		cTestEngine_Impl *impl();

		cTestEngine() {}; // Constructor is private therefore a new instance can not be made
						  // externally. Only available to members or friends of this class..
		~cTestEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cTestEngine(const cTestEngine &) {}; // Disallow copy constructor
		cTestEngine &operator=(const cTestEngine &TestManager) {
		} // Disallow assignment operator

	public:
		static TestEngine_API  cTestEngine *instance();
		//static DWORD __cdecl TestThread(void* lpParam);
	};
	
}
#endif