#ifndef _cPhysicsLibrary_HG_
#define _cPhysicsLibrary_HG_
#include "stdafx.h"
#include <string>
#ifdef PHYSICSLIBRARY_EXPORTS
#define PHYSICSLIBRARY_API __declspec(dllexport)
#else
#define PHYSICSLIBRARY_API __declspec(dllimport)
#endif // PHYSICSLIBRARY_EXPORTS
namespace PhysicsLibrary {
	class cPhysicsLibrary_Impl; // Purpose: Encapsulate private member variables and
							   // internal routines. Reduces make-time,
							   // compile-time, and the Fragile Binary Interface
							   // Problem.

	class cPhysicsLibrary {
		static cPhysicsLibrary *s_cPhysicsLibrary;
		// Boilerplate
		friend class cPhysicsLibrary_Impl; // The PIMPL idiom aka Compilation Firewall
		const cPhysicsLibrary_Impl *impl() const;
		cPhysicsLibrary_Impl *impl();

	public:
		//int loadGameFromXML(std::string filename);
		static  cPhysicsLibrary *instance();


	private:
		cPhysicsLibrary() {
		} // Constructor is private therefore a new instance can not be made
		  // externally. Only available to members or friends of this class..
		~cPhysicsLibrary() {}
		// Not defined to prevent copying of the only instance of the class.
		cPhysicsLibrary(const cPhysicsLibrary &); // Disallow copy constructor
		cPhysicsLibrary &operator=(const cPhysicsLibrary &physicsManager) {
		} // Disallow assignment operator
	};
}
#endif