#ifndef _cPhysicsEngine_HG_
#define _cPhysicsEngine_HG_
#include "stdafx.h"
#include "cCollisionShape.h"
#include <string>
#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class cPhysicsEngine_Impl; // Purpose: Encapsulate private member variables and
							   // internal routines. Reduces make-time,
							   // compile-time, and the Fragile Binary Interface
							   // Problem.

	class cPhysicsEngine {
		static cPhysicsEngine *s_cPhysicsEngine;
		// Boilerplate
		friend class cPhysicsEngine_Impl; // The PIMPL idiom aka Compilation Firewall
		const cPhysicsEngine_Impl *impl() const;
		cPhysicsEngine_Impl *impl();

	public:
		//int loadGameFromXML(std::string filename);
		static PhysicsEngine_API  cPhysicsEngine *instance();


	private:
		cPhysicsEngine() {
		} // Constructor is private therefore a new instance can not be made
		  // externally. Only available to members or friends of this class..
		~cPhysicsEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cPhysicsEngine(const cPhysicsEngine &); // Disallow copy constructor
		cPhysicsEngine &operator=(const cPhysicsEngine &physicsManager) {
		} // Disallow assignment operator
	};

}
#endif