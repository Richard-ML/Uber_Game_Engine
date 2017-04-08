#ifndef _cPhysicsEngine_HG_
#define _cPhysicsEngine_HG_
#include "stdafx.h"
#include <Windows.h>
#include "iRigidBody.h"
#include <string>
#include <time.h>
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

		cPhysicsEngine(); // Constructor is private therefore a new instance can not be made
						  // externally. Only available to members or friends of this class..
		~cPhysicsEngine() {}
		// Not defined to prevent copying of the only instance of the class.
		cPhysicsEngine(const cPhysicsEngine &) {}; // Disallow copy constructor
		cPhysicsEngine &operator=(const cPhysicsEngine &physicsManager) {
		} // Disallow assignment operator
		static DWORD __cdecl physicsThread(void* lpParam);
		void cleanup();
	public:
		//int loadGameFromXML(std::string filename);
		static PhysicsEngine_API  cPhysicsEngine *instance();
		static PhysicsEngine_API bool loadPhysicsComponent(rapidxml::xml_node<> *componentNode, iState* state);
		PhysicsEngine_API void initializeGameStateHandle(iGameState* pGameState);
	    PhysicsEngine_API void initializeWorldHandle(iWorld* pWorld);
		PhysicsEngine_API void initializeDebugRendererHandle(iDebugRenderer* pDebugRenderer);
		static PhysicsEngine_API bool addPhysicsObject(glm::vec3 position, iState* state);

	};

}
#endif