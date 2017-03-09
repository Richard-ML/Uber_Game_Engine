#include "stdafx.h"  
#include "cPhysicsEngine.h"
#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <chrono>

#include "_btRigidBody.h"
#include "_btCollisionShape.h"
#include "global.h"

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
		 _btWorld* m_btWorld;
		iWorld* m_cWorld;
	};
	inline const cPhysicsEngine_Impl *cPhysicsEngine::impl() const {
		return static_cast<const cPhysicsEngine_Impl *>(this);
	}
	inline cPhysicsEngine_Impl *cPhysicsEngine::impl() {
		return static_cast<cPhysicsEngine_Impl *>(this);
	}

//#define BULLET
#ifdef BULLET

	cPhysicsEngine::cPhysicsEngine() {
		this->impl()->m_btWorld = new _btWorld();
	}

//	DWORD cPhysicsEngine::physicsThread(void *lpParam) {
//		std::chrono::high_resolution_clock::time_point lastTime =
//			std::chrono::high_resolution_clock::now();
//		std::chrono::duration<float> deltaTime;
//		cPhysicsEngine *physicsEngine =
//			reinterpret_cast<cPhysicsEngine *>(lpParam);
//		do {
//			std::chrono::high_resolution_clock::time_point t2 =
//				std::chrono::high_resolution_clock::now();
//			deltaTime =
//				std::chrono::duration_cast<std::chrono::duration<float>>(
//					std::chrono::high_resolution_clock::now() -
//					lastTime); // Get the time that as passed
//			physicsEngine->impl()->m_btWorld->step(deltaTime.count());
//			lastTime = std::chrono::high_resolution_clock::now();
//			Sleep(5); // Free the thread
//} while (true);
//return 0;
//}

	PhysicsEngine_API iCollisionShape * cPhysicsEngine::createCollisionShape(eShapeType shapeType)
	{
		// TODO: Store a local copy of shapes when they are created so we can reuse them!
		iCollisionShape* collisionShape = new _btCollisionShape(shapeType);
		return collisionShape;
	}

	PhysicsEngine_API iRigidBody * cPhysicsEngine::createRigidBody(iCollisionShape * shape, sRigidBody & rigidBody)
	{
		iRigidBody* rb = new _btRigidBody(rigidBody, shape);
		rb->setCollisionShape(shape);
		return rb;
	}

	PhysicsEngine_API void cPhysicsEngine::update(float deltaTime)
	{
		impl()->m_btWorld->step(deltaTime);
	}

	PhysicsEngine_API bool cPhysicsEngine::loadPhysicsComponent(rapidxml::xml_node<>* componentNode, iState * state)
	{
		for (rapidxml::xml_node<> *cRigidBody_node = componentNode->first_node("RigidBody");
			cRigidBody_node; cRigidBody_node = cRigidBody_node->next_sibling("RigidBody")) {
			// TODO: create base physics object that contains state pointer. Load offsets etcetera..
			glm::mat4 transform;
			glm::vec3 offset = glm::vec3(std::stof(cRigidBody_node->first_attribute("offsetX")->value()), std::stof(cRigidBody_node->first_attribute("offsetY")->value()), std::stof(cRigidBody_node->first_attribute("offsetZ")->value()));
			transform[3] = glm::vec4(offset, 1.0f);
			state->setTransform(transform);
		
		}
		return true;
	}

	PhysicsEngine_API bool cPhysicsEngine::addPhysicsObject(glm::vec3 position, iState * state)
	{
		return PhysicsEngine_API bool();
	}

	PhysicsEngine_API void cPhysicsEngine::addRigidBodyToWorld(iRigidBody* rigidBody) {
		s_cPhysicsEngine->impl()->m_btWorld->addCollisionObject(rigidBody);
		return;
	}

	PhysicsEngine_API void cPhysicsEngine::loadClothMesh(rapidxml::xml_node<>* clothMeshNode, iState * state)
	{
		
	}

#else
	cPhysicsEngine::cPhysicsEngine() {
		this->impl()->m_cWorld = new cWorld();
	}

	std::vector<iRigidBody*> vec_rigidBodies;
	PhysicsEngine_API bool cPhysicsEngine::loadPhysicsComponent(rapidxml::xml_node<>* componentNode, iState * state)
	{
		for (rapidxml::xml_node<> *cRigidBody_node = componentNode->first_node("RigidBody");
			cRigidBody_node; cRigidBody_node = cRigidBody_node->next_sibling("RigidBody")) {
			// TODO: create base physics object that contains state pointer. Load offsets etcetera..
			glm::mat4 transform;
			glm::vec3 offset = glm::vec3(std::stof(cRigidBody_node->first_attribute("offsetX")->value()), std::stof(cRigidBody_node->first_attribute("offsetY")->value()), std::stof(cRigidBody_node->first_attribute("offsetZ")->value()));
			transform[3] = glm::vec4(offset, 1.0f);

		    cRigidBody* rb = new cRigidBody();
			rb->state = state;
			rb->setPosition(offset);
			state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&cRigidBody::saveToXMLNode, rb)));
			
			state->setTransform(transform);

			vec_rigidBodies.push_back(rb);
		}
		return true;
	}
	//DWORD cPhysicsEngine::physicsThread(void *lpParam) {
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
	//}
	PhysicsEngine_API void cPhysicsEngine::update(float deltaTime)
	{
		impl()->m_cWorld->step(deltaTime);
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
			DWORD myThreadID;
			HANDLE myHandle = CreateThread(NULL, 0, // stack size
				(LPTHREAD_START_ROUTINE) &PhysicsEngine::cPhysicsEngine::physicsThread, reinterpret_cast<void*>(s_cPhysicsEngine), 0, &myThreadID);
		}
		return s_cPhysicsEngine;
	}
	PhysicsEngine_API void cPhysicsEngine::initializeGameState(iGameState * gameState)
	{
		// TODO: Spin lock.. 
		g_pGameState = gameState;
	}
	PhysicsEngine_API void cPhysicsEngine::setDebugRenderer(iDebugRenderer * debugRenderer)
	{
		g_pDebugRenderer = debugRenderer;
	}
	DWORD cPhysicsEngine::physicsThread(void * lpParam)
	{
		while (g_pGameState == nullptr) { Sleep(5); }
		while (g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(5); /* Spin wait for load game process to complete */ }
		while (g_pGameState->getGameState() == GAMESTATE_SAVING) { Sleep(5); /* Spin wait for save game process to complete */ }

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
							   // DO STUFF!!! 



			physicsEngine->update(deltaTime.count());
			//////////////
			lastTime = std::chrono::high_resolution_clock::now();
			Sleep(5); // Free the thread
		} while (true);

		// TODO: Clean up resources..

		return 0;
	}


	PhysicsEngine_API bool cPhysicsEngine::addPhysicsObject(glm::vec3 position, iState * state)
	{
		cRigidBody * rb = new cRigidBody();
		state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&cRigidBody::saveToXMLNode, rb)));
		rb->state = state;
		state->setPosition(position);
		vec_rigidBodies.push_back(rb);
		return true;
	}
}