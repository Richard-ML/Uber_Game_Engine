#include "stdafx.h"  
#include "cPhysicsEngine.h"
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace PhysicsEngine {
	cPhysicsEngine *cPhysicsEngine::s_cPhysicsEngine =
		0; // Allocating pointer to static instance of cPhysicsEngine (singleton)

	class cPhysicsEngine_Impl : public cPhysicsEngine {
		// Boilerplate
		friend class cPhysicsEngine;

	public:

	};
	inline const cPhysicsEngine_Impl *cPhysicsEngine::impl() const {
		return static_cast<const cPhysicsEngine_Impl *>(this);
	}
	inline cPhysicsEngine_Impl *cPhysicsEngine::impl() {
		return static_cast<cPhysicsEngine_Impl *>(this);
	}
	// Initialize vectors
	//std::vector<cGameObject *> cEntityManager_Impl::vec_Entities;
	//rapidxml::xml_node<> *cEntityManager_Impl::spiderNode;


	cPhysicsEngine * cPhysicsEngine::instance() {
		if (!s_cPhysicsEngine)
		{
			printf("Physics Engine Initialized\n");
			//TODO: Run initialize() for shapes
			s_cPhysicsEngine = new cPhysicsEngine();
		}
		return s_cPhysicsEngine;
	}

	cRigidBody * cPhysicsEngine::createRigidBody(const sRigidBody & rigidBody)
	{
		return new cRigidBody(rigidBody);
	}

	cRigidBody * cPhysicsEngine::createRigidBody()
	{
		return new cRigidBody();
	}


}