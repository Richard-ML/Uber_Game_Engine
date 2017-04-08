#include "stdafx.h"  
#include "cPhysicsEngine.h"
#include <Windows.h>
#include "global.h"
#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <chrono>
#include <map>
#include "_btRigidBody.h"
#include "_btCollisionShape.h"
#define BIT(x) (1<<(x))
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace PhysicsEngine {


	void gLock(int varNum) {
#if !defined(SKIP_LOCKING)  
		while (_InterlockedExchange(&g_pLock->lock, LOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)

#endif  
	}

	void gUnlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedExchange(&g_pLock->lock, UNLOCKED);
#endif  
	}

	cPhysicsEngine *cPhysicsEngine::s_cPhysicsEngine =
		0; // Allocating pointer to static instance of cPhysicsEngine (singleton)

	class cPhysicsEngine_Impl : public cPhysicsEngine {
		// Boilerplate
		friend class cPhysicsEngine;
	public:
		_btWorld* m_btWorld;
		iPhysicsWorld* m_cWorld;
	};
	inline const cPhysicsEngine_Impl *cPhysicsEngine::impl() const {
		return static_cast<const cPhysicsEngine_Impl *>(this);
	}
	inline cPhysicsEngine_Impl *cPhysicsEngine::impl() {
		return static_cast<cPhysicsEngine_Impl *>(this);
	}
	std::vector<iRigidBody*> vec_rigidBodies;
	//std::map<int, iRigidBody*> map_rigidBodies;
#define BULLET
#ifdef BULLET

	cPhysicsEngine::cPhysicsEngine() {
		this->impl()->m_btWorld = new _btWorld();
	}



	DWORD cPhysicsEngine::physicsThread(void *lpParam) {
		std::chrono::high_resolution_clock::time_point lastTime =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime;
		cPhysicsEngine *physicsEngine =
			reinterpret_cast<cPhysicsEngine *>(lpParam);
		do {
			while (g_pGameState == 0 || g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(50); }
			std::chrono::high_resolution_clock::time_point t2 =
				std::chrono::high_resolution_clock::now();
			deltaTime =
				std::chrono::duration_cast<std::chrono::duration<float>>(
					std::chrono::high_resolution_clock::now() -
					lastTime); // Get the time that as passed
			lastTime = std::chrono::high_resolution_clock::now();
			gLock(0);
			for each(_btRigidBody* rb in vec_rigidBodies) {
				rb->setCollision(false);//Proxy to state collision flag. Set to true via callback when collision occurs. 
				if (rb->m_rigidBody != 0) {
					glm::vec3 currentImpluse = rb->state->getImpluse();

						btTransform trans = rb->m_rigidBody->getWorldTransform();
						rb->m_rigidBody->setLinearVelocity(btVector3(currentImpluse.x, currentImpluse.y, currentImpluse.z) * 10);
						rb->state->setImpluse(glm::vec3(0.0f));

						glm::mat4 transformation = rb->state->getTransform();
						glm::vec3 scale;
						glm::quat rotation;
						glm::vec3 translation;
						glm::vec3 skew;
						glm::vec4 perspective;
						glm::decompose(transformation, scale, rotation, translation, skew, perspective);
						rotation = glm::conjugate(rotation);

						btQuaternion _btQuat;
						_btQuat.setX(rotation.x);
						_btQuat.setY(rotation.y);
						_btQuat.setZ(rotation.z);
						_btQuat.setW(rotation.w);
						trans.setRotation(_btQuat);
						rb->m_rigidBody->setWorldTransform(trans);
				}
			}
			physicsEngine->impl()->m_btWorld->step(deltaTime.count());
			//physicsEngine->impl()->m_btWorld->step(0.016f);

			for each(_btRigidBody* rb in vec_rigidBodies) {

				if (rb->m_rigidBody != 0) { // Objects can be added to the scene and not be initialized yet. Check to see if the bullet rigid body exists.
					rb->state->setIsColliding(rb->isCollision());
					if (rb->m_rigidBody->getInvMass() != 0) {

						btVector3 btVec = rb->m_rigidBody->getWorldTransform().getOrigin();
						glm::vec3 positionResult = glm::vec3(btVec[0], btVec[1], btVec[2]) - rb->state->getBoundingBox().position;
						//positionResult.y = 1.0f;
						//rb->state->setPosition(positionResult);
						btQuaternion rot = rb->m_rigidBody->getWorldTransform().getRotation();

						glm::mat4 transform;

						glm::quat quat;
						quat.x = rot.getX();
						quat.y = rot.getY();
						quat.z = rot.getZ();
						quat.w = rot.getW();

						sBoundingBox boundingBox = rb->state->getBoundingBox();
						boundingBox.rotation = quat;
						rb->state->setBoundingBox(boundingBox);

						transform = glm::mat4_cast(quat);
						transform[3] = glm::vec4(positionResult, 1.0f);
						rb->state->setTransform(transform);
					}
				}
			}
			gUnlock(0);
			//for (int nc = 0; nc < s_cPhysicsEngine->impl()->m_btWorld->m_dispatcher->getNumManifolds(); nc++)
			//{
			//	btPersistentManifold* contact = s_cPhysicsEngine->impl()->m_btWorld->m_dispatcher->getManifoldByIndexInternal(nc);
			//	_btRigidBody* rb1 = reinterpret_cast<_btRigidBody*>(contact->getBody0()->getUserPointer());
			//	rb1->state->setIsColliding(1);
			//
			//	_btRigidBody* rb2 = reinterpret_cast<_btRigidBody*>(contact->getBody1()->getUserPointer());;
			//	rb2->state->setIsColliding(1);
			//	//s_cPhysicsEngine->impl()->m_btWorld->m_dispatcher->clearManifold(contact); // O.o 
			//}


			Sleep(10); // Free the thread
		} while (true);
		return 0;
	}

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

	//PhysicsEngine_API void cPhysicsEngine::update(float deltaTime)
	//{
	//	impl()->m_btWorld->step(deltaTime);
	//}

	PhysicsEngine_API bool cPhysicsEngine::loadPhysicsComponent(rapidxml::xml_node<>* componentNode, iState * state)
	{
		for (rapidxml::xml_node<> *cRigidBody_node = componentNode->first_node("RigidBody");
		cRigidBody_node; cRigidBody_node = cRigidBody_node->next_sibling("RigidBody")) {
			// TODO: create base physics object that contains state pointer. Load offsets etcetera..
			glm::mat4 transform;
			glm::vec3 offset = glm::vec3(std::stof(cRigidBody_node->first_attribute("offsetX")->value()), std::stof(cRigidBody_node->first_attribute("offsetY")->value()), std::stof(cRigidBody_node->first_attribute("offsetZ")->value()));
			transform[3] = glm::vec4(offset, 1.0f);
			rapidxml::xml_attribute<char>* att = cRigidBody_node->first_attribute("hingeConstraint");
			int hingeID = 0;
			if (att != 0)
				hingeID = std::stoi(att->value());


			// Get the collsion details for this object
			// These details will include the collision mask for the specific object and the collsion masks of objects that it collides with. 
			rapidxml::xml_node<> * collisionInfo_node = cRigidBody_node->first_node("CollisionInfo");
			int collisionMask = std::stoi(collisionInfo_node->first_attribute("collisionMask")->value());

			int collisionFilterResult = 0; // No objects are assigned a collision mask of 0: Therefore objects without specified collision filters do not collides with anything
			_btRigidBody* rb = new _btRigidBody();

			rb->hingeID = hingeID;
			for (rapidxml::xml_node<> *CollisionFilter_node = collisionInfo_node->first_node("CollidesWith");
			CollisionFilter_node; CollisionFilter_node = CollisionFilter_node->next_sibling("CollidesWith")) {
				int filter = std::stoi(CollisionFilter_node->first_attribute("collisionMask")->value());
				rb->collisionFilters.push_back(filter);
				collisionFilterResult |= BIT(filter);
			}


			rb->setCollisionMask(collisionMask);
			rb->setCollisionFilter(collisionFilterResult);
			rb->state = state;
			state->registerComponentXMLDataCallback(std::function< std::string() >(std::bind(&_btRigidBody::saveToXMLNode, rb)));

			state->setTransform(transform);

			vec_rigidBodies.push_back(rb);

			glm::vec3 colShapePos = state->getBoundingBox().position;

			btTransform worldTransform;
			worldTransform.setIdentity();
			worldTransform.setOrigin(btVector3(offset.x, offset.y, offset.z) + btVector3(colShapePos.x, colShapePos.y, colShapePos.z));

			btScalar mass = std::stof(cRigidBody_node->first_attribute("mass")->value());
			rb->mass = mass;
			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);
			btVector3 localInertia(0.0f, 0.0f, 0.0f);
			glm::vec3 boxHalfWidth = state->getBoundingBox().scale;

			//rb->m_rigidBody->setRestitution(.0f);
			//rb->m_rigidBody->setFriction(.3f);


			if (att != 0)
			{
				switch (hingeID)
				{
				case 0:
				{
					// BOX SHAPES HERE AND PLANE ;)
					btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));

					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						bs->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);
					// Arc door 1
					btHingeConstraint* hinge = new btHingeConstraint(*rb->m_rigidBody, btVector3(5, 0, 0), btVector3(0, 1, 0), true);
					s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(hinge);
					break;
				}
				case 1: {
					// BOX SHAPES HERE AND PLANE ;)
					btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));

					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						bs->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);
					// Arc door 2
					btHingeConstraint* hinge = new btHingeConstraint(*rb->m_rigidBody, btVector3(-5, 0, 0), btVector3(0, 1, 0), true);
					s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(hinge);
					break;
				}
				case 2:
				{
					sBoundingBox boundingBox;
					boundingBox.scale = glm::vec3(2.0f);
					rb->state->setBoundingBox(boundingBox);
					// SPHERE SHAPE HERE
					btSphereShape* ss = new btSphereShape(boxHalfWidth.x / 2);
					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						ss->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, ss, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);


					// Spring constraint!! 
					btTransform frame2;
					frame2 = btTransform::getIdentity();
					frame2.setOrigin(btVector3(btScalar(0.), btScalar(0.), btScalar(0.)));

					btGeneric6DofSpringConstraint* springConstraint = new btGeneric6DofSpringConstraint(*rb->m_rigidBody, frame2, true);
					springConstraint->setLinearUpperLimit(btVector3(80.f, 0.f, 0.f));
					springConstraint->setLinearLowerLimit(btVector3(-80.f, 0.f, 0.f));

					springConstraint->setAngularLowerLimit(btVector3(-1.57f, 0.f, 0.0f));
					springConstraint->setAngularUpperLimit(btVector3(1.57f, 0.f, 0.0f));

					s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(springConstraint, true);
					springConstraint->enableSpring(0, true);
					springConstraint->setStiffness(0, 10.0f);
					springConstraint->setDamping(0, 0.01f);
					springConstraint->enableSpring(5, true);
					springConstraint->setStiffness(5, 10.0f);
					springConstraint->setDamping(0, 0.01f);
					springConstraint->setEquilibriumPoint();
					break;
				}
				case 3:
				{
					// BOX SHAPES HERE AND PLANE ;)
					btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));

					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						bs->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);
					// This is a plane. (Box with 0 scale on one axis..)
					btTransform frameInB;
					frameInB = btTransform::getIdentity();
					rb->m_rigidBody->setAngularFactor(btVector3(0, 1.0, 0));
					btSliderConstraint* sliderConstraint = new btSliderConstraint(*rb->m_rigidBody, frameInB, true);
					sliderConstraint->setLowerLinLimit(-30.0F);
					sliderConstraint->setUpperLinLimit(30.0F);

					sliderConstraint->setLowerAngLimit(-SIMD_PI / 3.0F);
					sliderConstraint->setUpperAngLimit(SIMD_PI / 3.0F);

					s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(sliderConstraint, true);
					break;
				}
				case 4:
				{
					// CONE SHAPE HERE
					btConeShape* cs = new btConeShape(boxHalfWidth.x / 2, boxHalfWidth.y / 2);

					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						cs->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, cs, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);
					rb->m_rigidBody->setFriction(0.1f); // less friction here.. Just a swinging light or something. Use your IMAGINATION James Lucas. ;)
					btVector3 constraintPivot(btVector3(0.0f, 8.0f, 0.0f));
					btTypedConstraint* p2p = new btPoint2PointConstraint(*rb->m_rigidBody, constraintPivot);
					s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(p2p, true);
					break;
				}
				case 5:
				{
					btConvexShape* capsuleShape = new btCapsuleShape(glm::max( boxHalfWidth.x, boxHalfWidth.z)/2.0f, boxHalfWidth.y/2.0f);

					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						capsuleShape->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, capsuleShape, localInertia);

					rb->m_rigidBody = new btRigidBody(rbInfo);

					// Friction will be applied manually..
					rb->m_rigidBody->setFriction(0.0f);
					rb->m_rigidBody->setRestitution(0.0f);
					//rb->m_rigidBody->setDamping

					rb->m_rigidBody->setAngularFactor(0.0f); // Keep the character from falling over..
					
					rb->m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
				
					break;
				}
				default:
				{
					// BOX SHAPES HERE AND PLANE ;)
					btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));

					btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
					if (isDynamic)
						bs->calculateLocalInertia(mass, localInertia);

					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);
					rb->m_rigidBody = new btRigidBody(rbInfo);
					break;
				}

				}


				if (isDynamic)
				{
					rb->m_rigidBody->activate(true);
					rb->m_rigidBody->forceActivationState(DISABLE_DEACTIVATION);
					rb->m_rigidBody->setCollisionFlags(rb->m_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
				}
			}
			s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addRigidBody(rb->m_rigidBody, BIT(collisionMask), collisionFilterResult);
			rb->m_rigidBody->setUserPointer(rb);
		}
		return true;
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

			//s_cPhysicsEngine->impl()->m_btWorld->addCollisionObject(rb);

			// TODO: Add static box to btWorld at transform[3].xyz. Then add character set pState->setIsColliding to change color status of pState's AABBs

			btCollisionObject colObj;
			//colObj.isStaticObject = true;
			//colObj.
			sBoundingBox boundingBox = state->getBoundingBox();

			btBoxShape shape(btVector3(boundingBox.scale.x, boundingBox.scale.y, boundingBox.scale.z));
			btRigidBodyData rbData;
		}
		return true;
	}
	DWORD cPhysicsEngine::physicsThread(void *lpParam) {
		std::chrono::high_resolution_clock::time_point lastTime =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime;
		cPhysicsEngine *physicsEngine =
			reinterpret_cast<cPhysicsEngine *>(lpParam);

		while (g_pGameState == 0 || g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(50); }
		do {
			std::chrono::high_resolution_clock::time_point t2 =
				std::chrono::high_resolution_clock::now();
			deltaTime =
				std::chrono::duration_cast<std::chrono::duration<float>>(
					std::chrono::high_resolution_clock::now() -
					lastTime); // Get the time that as passed

			physicsEngine->impl()->m_cWorld->step(deltaTime.count());
			lastTime = std::chrono::high_resolution_clock::now();
			Sleep(35); // Free the thread
		} while (true);
		return 0;
	}
	void cPhysicsEngine::update(float deltaTime)
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
	PhysicsEngine_API void cPhysicsEngine::initializeGameStateHandle(iGameState * pGameState)
	{
		g_pGameState = pGameState;
	}

	PhysicsEngine_API void cPhysicsEngine::initializeWorldHandle(iWorld * pWorld)
	{
		g_pWorld = pWorld;
	}

	PhysicsEngine_API void cPhysicsEngine::initializeDebugRendererHandle(iDebugRenderer * pDebugRenderer)
	{
		g_pDebugRenderer = pDebugRenderer;
	}

	cPhysicsEngine * cPhysicsEngine::instance() {
		if (!s_cPhysicsEngine)
		{
			printf("Physics Engine Initialized\n");
			//TODO: Run initialize() for shapes
			s_cPhysicsEngine = new cPhysicsEngine();
			sSpinLock* pSpinLock = new sSpinLock();
			pSpinLock->lock = 0; // Create a lock for vec rigid bodies
			g_pLock = pSpinLock;

			DWORD myThreadID;
			HANDLE myHandle = CreateThread(NULL, 0, // stack size
				(LPTHREAD_START_ROUTINE)&PhysicsEngine::cPhysicsEngine::physicsThread, reinterpret_cast<void*>(s_cPhysicsEngine), 0, &myThreadID);
		}
		return s_cPhysicsEngine;
	}
	PhysicsEngine_API bool cPhysicsEngine::addPhysicsObject(glm::vec3 position, iState * state)
	{
		gLock(0);
		_btRigidBody * rb = new _btRigidBody();
		state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&_btRigidBody::saveToXMLNode, rb)));

		rb->state = state;
		state->setPosition(position);

		rb->setCollisionMask(5);
		rb->setCollisionFilter(1);

		glm::vec3 colShapePos = state->getBoundingBox().position;

		btTransform worldTransform;
		worldTransform.setIdentity();
		worldTransform.setOrigin(btVector3(position.x, position.y, position.z) + btVector3(colShapePos.x, colShapePos.y, colShapePos.z));

		btScalar mass = 0.0f;

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		glm::vec3 boxHalfWidth = state->getBoundingBox().scale;
		btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));
		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
		if (isDynamic)
			bs->calculateLocalInertia(mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);
		rb->m_rigidBody = new btRigidBody(rbInfo);
		if (isDynamic) {
			rb->m_rigidBody->activate(true);
			rb->m_rigidBody->forceActivationState(DISABLE_DEACTIVATION);
			rb->m_rigidBody->setCollisionFlags(rb->m_rigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
			rb->m_rigidBody->setFriction(0.8f);
		}
		rb->hingeID = -1; // No hinge
		int collisionFilterResult = 0; // No objects are assigned a collision mask of 0: Therefore objects without specified collision filters do not collides with anything

		//for (int nc = 0; nc < 5; nc++)
		//{
		rb->collisionFilters.push_back(1);
		collisionFilterResult |= BIT(1);

		rb->collisionFilters.push_back(3);
		collisionFilterResult |= BIT(3);
		//}
		s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addRigidBody(rb->m_rigidBody, BIT(5), collisionFilterResult);
		rb->m_rigidBody->setUserPointer(rb);



		vec_rigidBodies.push_back(rb);
		//map_rigidBodies[rb->m_rigidBody->getUserIndex()] = rb;
		gUnlock(0);
		return true;
	}

}
