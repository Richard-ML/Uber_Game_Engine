#include "stdafx.h"  
#include "cPhysicsEngine.h"
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <chrono>


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
		btDiscreteDynamicsWorld* m_btWorld;
		cWorld* m_cWorld;
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
//#define BULLET
#ifdef BULLET

	cPhysicsEngine::cPhysicsEngine() {
		//TODO: CLEAN UP THESE POINTERS WHEN WE ARE DONE WITH THEM
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

		this->impl()->m_btWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}

	DWORD cPhysicsEngine::physicsThread(void *lpParam) {
		std::chrono::high_resolution_clock::time_point lastTime =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime;
		cPhysicsEngine *physicsEngine =
			reinterpret_cast<cPhysicsEngine *>(lpParam);
		do {
			std::chrono::high_resolution_clock::time_point t2 =
				std::chrono::high_resolution_clock::now();
			deltaTime =
				std::chrono::duration_cast<std::chrono::duration<float>>(
					std::chrono::high_resolution_clock::now() -
					lastTime); // Get the time that as passed
			physicsEngine->impl()->m_btWorld->stepSimulation(deltaTime.count());
			lastTime = std::chrono::high_resolution_clock::now();
			Sleep(35); // Free the thread
} while (true);
return 0;
}

	PhysicsEngine_API iCollisionShape * cPhysicsEngine::createCollisionShape(eShapeType shapeType)
	{
		// TODO: Store a local copy of shapes when they are created so we can reuse them!
		switch (shapeType)
		{
		case PhysicsEngine::UNKNOWN:
			return nullptr;
			break;
		case PhysicsEngine::SPHERE:
			return nullptr; // new cSphere();
			break;
		case PhysicsEngine::PLANE:
			return nullptr; // new cPlane();
			break;
		default:
			return nullptr;
			break;
		}
	}

	PhysicsEngine_API iRigidBody * cPhysicsEngine::createRigidBody(iCollisionShape * shape, sRigidBody & rigidBody)
	{
		cRigidBody* rb = new cRigidBody();
		rb->setCollisionShape(shape);
		return rb;
	}

	PhysicsEngine_API void cPhysicsEngine::addRigidBodyToWorld(iRigidBody* rigidBody) {
		s_cPhysicsEngine->impl()->m_btWorld->addCollisionObject(rigidBody);
		return;
	}

#else
	cPhysicsEngine::cPhysicsEngine() {
		this->impl()->m_cWorld = new cWorld();
	}
	/* Ignore this please going to use it later..
	//WORD cPhysicsEngine::physicsThread(void *lpParam) {
	//	std::chrono::high_resolution_clock::time_point lastTime =
	//		std::chrono::high_resolution_clock::now();
	//	std::chrono::duration<float> deltaTime;
	//	cPhysicsEngine *physicsEngine =
	//		reinterpret_cast<cPhysicsEngine *>(lpParam);
	//	do {
	//		std::chrono::high_resolution_clock::time_point t2 =
	//			std::chrono::high_resolution_clock::now();
	//		deltaTime =
	//			std::chrono::duration_cast<std::chrono::duration<float>>(
	//				std::chrono::high_resolution_clock::now() -
	//				lastTime); // Get the time that as passed
	//		physicsEngine->impl()->m_cWorld->step(deltaTime.count());
	//		lastTime = std::chrono::high_resolution_clock::now();
	//		Sleep(35); // Free the thread
	//	} while (true);
	//	return 0;
	*/
	
	PhysicsEngine_API iCollisionShape * cPhysicsEngine::createCollisionShape(eShapeType shapeType)
	{
		// TODO: Store a local copy of shapes when they are created so we can reuse them!
		switch (shapeType)
		{
		case PhysicsEngine::UNKNOWN:
			return nullptr;
			break;
		case PhysicsEngine::SPHERE:
			return new cSphere();
			break;
		case PhysicsEngine::PLANE:
			return new cPlane();
			break;
		default:
			return nullptr;
			break;
		}
	}

	PhysicsEngine_API iRigidBody * cPhysicsEngine::createRigidBody(iCollisionShape * shape, sRigidBody & rigidBody)
	{
		cRigidBody* rb = new cRigidBody(rigidBody);
		rb->setCollisionShape(shape);
		return rb;
	}
	PhysicsEngine_API void cPhysicsEngine::update(float deltaTime)
	{
		// Do physics stuff!! 
		this->impl()->m_cWorld->step(deltaTime);
		printf("Physics did stuff!\n");
		return;
	}

	PhysicsEngine_API void cPhysicsEngine::addRigidBodyToWorld(iRigidBody * rigidBody)
	{
		s_cPhysicsEngine->impl()->m_cWorld->addCollisionObject(rigidBody);
		return;
	}

#endif


	cPhysicsEngine * cPhysicsEngine::instance() {
		if (!s_cPhysicsEngine)
		{
			printf("Physics Engine Initialized\n");
			//TODO: Run initialize() for shapes
			s_cPhysicsEngine = new cPhysicsEngine();
			/* Ignore this please! 
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE) &PhysicsEngine::cPhysicsEngine::physicsThread, reinterpret_cast<void*>(s_cPhysicsEngine), 0, &myThreadID);
			*/
			// Do one time setup! Anything need to be initialized? 

		}
		return s_cPhysicsEngine;
	}

}