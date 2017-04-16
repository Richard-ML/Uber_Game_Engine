#include "stdafx.h"  
#include "cPhysicsEngine.h"
#include <Windows.h>
#include "global.h"


#define BIT(x) (1<<(x))
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
	std::map<std::string, btTriangleMesh*>  m_map_MeshNameToTriangleMesh;
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a const cPhysicsEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline const cPhysicsEngine_Impl *cPhysicsEngine::impl() const {
		return static_cast<const cPhysicsEngine_Impl *>(this);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cPhysicsEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline cPhysicsEngine_Impl *cPhysicsEngine::impl() {
		return static_cast<cPhysicsEngine_Impl *>(this);
	}
	/// <summary>	The vector rigid bodies. </summary>
	std::vector<iRigidBody*> vec_rigidBodies;
	//std::map<int, iRigidBody*> map_rigidBodies;

	cPhysicsEngine::cPhysicsEngine() {
		g_btWorld = new _btWorld();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Physics thread. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="lpParam">	[in,out] If non-null, the parameter. </param>
	///
	/// <returns>	A DWORD. </returns>
	///-------------------------------------------------------------------------------------------------

	DWORD cPhysicsEngine::physicsThread(void *lpParam) {
		while (g_pGameState == 0 || g_pGameState->getGameState() == GAMESTATE_LOADING) { Sleep(50); }
		std::chrono::high_resolution_clock::time_point lastTime =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime;
		cPhysicsEngine *physicsEngine =
			reinterpret_cast<cPhysicsEngine *>(lpParam);

		do {
			while (g_pGameState->getGameState() != GAMESTATE_RUNNING) { Sleep(50); }
			switch (g_pGameState->getGameState())
			{
			case GAMESTATE_EXIT:
				physicsEngine->s_cPhysicsEngine->cleanup();
				physicsEngine->s_cPhysicsEngine->~cPhysicsEngine();
				return 0;
				break;
			}
			std::chrono::high_resolution_clock::time_point t2 =
				std::chrono::high_resolution_clock::now();
			deltaTime =
				std::chrono::duration_cast<std::chrono::duration<float>>(
					std::chrono::high_resolution_clock::now() -
					lastTime); // Get the time that as passed
			lastTime = std::chrono::high_resolution_clock::now();
			//physicsEngine->gLock(0);
			for each(_btRigidBody* rb in vec_rigidBodies) {
				rb->setCollision(false);//Proxy to state collision flag. Set to true via callback when collision occurs. 
				if (rb->m_rigidBody != 0) {
					glm::vec3 currentImpluse = rb->state->getImpluse();

					btTransform trans = rb->m_rigidBody->getWorldTransform();
					rb->m_rigidBody->setLinearVelocity(btVector3(currentImpluse.x, currentImpluse.y + 1, currentImpluse.z) * 10);
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
			//physicsEngine->impl()->m_btWorld->step(deltaTime.count());
			g_btWorld->step(0.016f);

			for each(_btRigidBody* rb in vec_rigidBodies) {

				if (rb->m_rigidBody != 0) { // Objects can be added to the scene and not be initialized yet. Check to see if the bullet rigid body exists.
					rb->state->setIsColliding(rb->isCollision());
					if (rb->m_rigidBody->getInvMass() != 0) {
						sBoundingBox boundingBox = rb->state->getBoundingBox();
						btVector3 btVec = rb->m_rigidBody->getWorldTransform().getOrigin();
						glm::vec3 positionResult = glm::vec3(btVec[0], btVec[1], btVec[2]);
						//positionResult.y = 1.0f;
						//rb->state->setPosition(positionResult);
						btQuaternion rot = rb->m_rigidBody->getWorldTransform().getRotation();

						glm::mat4 transform;

						glm::quat quat;
						quat.x = rot.getX();
						quat.y = rot.getY();
						quat.z = rot.getZ();
						quat.w = rot.getW();


						boundingBox.rotation = quat;
						rb->state->setBoundingBox(boundingBox);

						transform = glm::mat4_cast(quat);
						transform[3] = glm::vec4(positionResult, 1.0f);
						rb->state->setTransform(transform);
					}
				}
			}
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

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Cleans up resources. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void cPhysicsEngine::cleanup()
	{

		//Remove constraints from the dynamics world before the corresponding rigidbodies are deleted. 
		for each(_btRigidBody* rb in vec_rigidBodies) {
			for (int nc = 0; nc < rb->m_rigidBody->getNumConstraintRefs(); nc++) {
				{
					btTypedConstraint * constraint = rb->m_rigidBody->getConstraintRef(nc);
					rb->m_rigidBody->removeConstraintRef(constraint); // Remove rigidbody's internal reference to constraint
					g_btWorld->m_btWorld->removeConstraint(constraint); // Remove constraint from world
					delete constraint; // Delete the pointer we have created

				}
			}
		}

	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads physics component. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
	/// <param name="state">			[in,out] If non-null, the state. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

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
			// These details will include the collision mask for the specific object and the collision masks of objects that it collides with. 
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
			worldTransform.setOrigin(btVector3(offset.x, offset.y, offset.z));

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
			
						btCollisionShape* collisionShapeTerrain = new btConvexTriangleMeshShape(m_map_MeshNameToTriangleMesh[rb->state->getMeshName()]);
						btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); 
						if (isDynamic)
							collisionShapeTerrain->calculateLocalInertia(mass, localInertia);
					
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShapeTerrain, localInertia);
					
						rb->m_rigidBody = new btRigidBody(rbInfo);

						rb->m_rigidBody->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f)); // Keep the doors from falling over..
						rb->m_rigidBody->setRestitution(0.0f);

			

						// Arc door 1
						btHingeConstraint* hinge = new btHingeConstraint(*rb->m_rigidBody, btVector3(rb->state->getBoundingBox().scale.x, 0, 0), btVector3(0, 1, 0), true);

						//hinge->enableAngularMotor(true, 2, 10.0);
						//hinge->setMotorTarget(1, 1.1);
						hinge->setOverrideNumSolverIterations(1);
						//hinge->enableMotor(true);

						g_btWorld->m_btWorld->addConstraint(hinge);

						break;
					}
					case 1: {
		
						btCollisionShape* collisionShapeTerrain = new btConvexTriangleMeshShape(m_map_MeshNameToTriangleMesh[rb->state->getMeshName()]);
						btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin()));
						if (isDynamic)
							collisionShapeTerrain->calculateLocalInertia(mass, localInertia);
					
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShapeTerrain, localInertia);
					
						rb->m_rigidBody = new btRigidBody(rbInfo);

						rb->m_rigidBody->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f)); // Keep the doors from falling over..
						rb->m_rigidBody->setRestitution(0.0f);
						// Arc door 2
						btHingeConstraint* hinge = new btHingeConstraint(*rb->m_rigidBody, btVector3(-rb->state->getBoundingBox().scale.x, 0, 0), btVector3(0, 1, 0), true);
						g_btWorld->m_btWorld->addConstraint(hinge);
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
					
						g_btWorld->m_btWorld->addConstraint(springConstraint, true);
						springConstraint->enableSpring(0, true);
						springConstraint->setStiffness(0, 10.0f);
						springConstraint->setDamping(0, 0.01f);
						springConstraint->enableSpring(5, true);
						springConstraint->setStiffness(5, 10.0f);
						springConstraint->setDamping(0, 0.01f);
						springConstraint->setEquilibriumPoint();
						break;
					}
					//case 3:
					//{
					//	// BOX SHAPES HERE AND PLANE ;)
					//	btBoxShape* bs = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));
					//
					//	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin() )); // TODO: cleanup
					//	if (isDynamic)
					//		bs->calculateLocalInertia(mass, localInertia);
					//
					//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bs, localInertia);
					//
					//	rb->m_rigidBody = new btRigidBody(rbInfo);
					//	// This is a plane. (Box with 0 scale on one axis..)
					//	btTransform frameInB;
					//	frameInB = btTransform::getIdentity();
					//	rb->m_rigidBody->setAngularFactor(btVector3(0, 1.0, 0));
					//	btSliderConstraint* sliderConstraint = new btSliderConstraint(*rb->m_rigidBody, frameInB, true);
					//	sliderConstraint->setLowerLinLimit(-30.0F);
					//	sliderConstraint->setUpperLinLimit(30.0F);
					//
					//	sliderConstraint->setLowerAngLimit(-SIMD_PI / 3.0F);
					//	sliderConstraint->setUpperAngLimit(SIMD_PI / 3.0F);
					//
					//	s_cPhysicsEngine->impl()->m_btWorld->m_btWorld->addConstraint(sliderConstraint, true);
					//	break;
					//}
					case 4:
					{
						// CONE SHAPE HERE
						btCollisionShape* collisionShapeTerrain = new btConvexTriangleMeshShape(m_map_MeshNameToTriangleMesh[rb->state->getMeshName()]);

						//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
						btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
						if (isDynamic)
							collisionShapeTerrain->calculateLocalInertia(mass, localInertia);
					
						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShapeTerrain, localInertia);
					
						rb->m_rigidBody = new btRigidBody(rbInfo);
						rb->m_rigidBody->setFriction(0.5f); 
						btVector3 constraintPivot(btVector3(0.0f, 10.0f, 0.0f));
						btTypedConstraint* p2p = new btPoint2PointConstraint(*rb->m_rigidBody, constraintPivot);
						g_btWorld->m_btWorld->addConstraint(p2p, true);
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

					if (m_map_MeshNameToTriangleMesh[rb->state->getMeshName()] != 0)
					{
						btCollisionShape* collisionShapeTerrain = new btBvhTriangleMeshShape( m_map_MeshNameToTriangleMesh[rb->state->getMeshName()], true, true);
						sBoundingBox bb = rb->state->getBoundingBox(); // For debugging
						btVector3 localScaling = collisionShapeTerrain->getLocalScaling();

					    // if (isDynamic)
						//	collisionShapeTerrain->calculateLocalInertia(mass, localInertia);

						btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin()));
						// NOTE: 0.0f mass == static  && btVector3(0,0,0) == inertia
						btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(0.0f, motionState, collisionShapeTerrain, localInertia);
						btRigidBody* rigidBodyTerrain = new btRigidBody(rigidBodyConstructionInfo);
						rigidBodyTerrain->setFriction(btScalar(0.9));

						rb->m_rigidBody = rigidBodyTerrain;
					}
					else {
						// BOX SHAPES HERE AND PLANE ;)
						btBoxShape* collisionShapeTerrain = new btBoxShape(btVector3(boxHalfWidth.x / 2, boxHalfWidth.y / 2, boxHalfWidth.z / 2));
						btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin())); // TODO: cleanup
						if (isDynamic)
							collisionShapeTerrain->calculateLocalInertia(mass, localInertia);

						btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShapeTerrain, localInertia);

						rb->m_rigidBody = new btRigidBody(rbInfo);
					}
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
			g_btWorld->m_btWorld->addRigidBody(rb->m_rigidBody, BIT(collisionMask), collisionFilterResult);
			rb->m_rigidBody->setUserPointer(rb);
		}
		{
			btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();
			triangleMeshTerrain->addTriangle(
				btVector3(32.0f, 32.0f, -50.0f), btVector3(-32.0f, 0.0f, -0.0f),
				btVector3(-32.0f, 32.0f, -50.0f)
			);
			triangleMeshTerrain->addTriangle(
				btVector3(32.0f, 32.0f, -50.0f), btVector3(32.0f, 0.0f, -0.0f),
				btVector3(-32.0f, 0.0f, -0.0f)
			);
			int numTri = triangleMeshTerrain->getNumTriangles();
			btCollisionShape* collisionShapeTerrain = new btBvhTriangleMeshShape(triangleMeshTerrain, true);
			btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -64)));

			btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(0.0f, motionState, collisionShapeTerrain, btVector3(0, 0, 0));
			btRigidBody* rigidBodyTerrain = new btRigidBody(rigidBodyConstructionInfo);
			rigidBodyTerrain->setFriction(btScalar(0.9));

			g_pDebugRenderer->createCube(glm::vec3(-32.0f, 32.0f, -114.0f), glm::vec3(1.0f), -1, glm::vec3(1.0f));
			g_pDebugRenderer->createCube(glm::vec3(32.0f, 32.0f, -114.0f), glm::vec3(1.0f), -1, glm::vec3(1.0f));
			g_pDebugRenderer->createCube(glm::vec3(-32.0f, 0.0f, -64.0f), glm::vec3(1.0f), -1, glm::vec3(1.0f));
			g_pDebugRenderer->createCube(glm::vec3(32.0f, 0.0f, -64.0f), glm::vec3(1.0f), -1, glm::vec3(1.0f));


			int collisionFilterResult = 0;
			collisionFilterResult |= BIT(1);
			collisionFilterResult |= BIT(3);

			int collisionFilter = BIT(0);
			g_btWorld->m_btWorld->addRigidBody(rigidBodyTerrain, collisionFilter, collisionFilterResult);
		}
		return true;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Initializes the game state handle. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="pGameState">	[in,out] If non-null, state of the game. </param>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API void cPhysicsEngine::initializeGameStateHandle(iGameState * pGameState)
	{
		g_pGameState = pGameState;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Initializes the world handle. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="pWorld">	[in,out] If non-null, the world. </param>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API void cPhysicsEngine::initializeWorldHandle(iWorld * pWorld)
	{
		g_pWorld = pWorld;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Initializes the debug renderer handle. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="pDebugRenderer">	[in,out] If non-null, the debug renderer. </param>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API void cPhysicsEngine::initializeDebugRendererHandle(iDebugRenderer * pDebugRenderer)
	{
		g_pDebugRenderer = pDebugRenderer;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cPhysicsEngine. </returns>
	///-------------------------------------------------------------------------------------------------

	cPhysicsEngine * cPhysicsEngine::instance() {
		if (!s_cPhysicsEngine)
		{
			printf("Physics Engine Initialized\n");
			//TODO: Run initialize() for shapes
			s_cPhysicsEngine = new cPhysicsEngine();
		//sSpinLock* pSpinLock = new sSpinLock();
		//	pSpinLock->lock = 0; // Create a lock for vec rigid bodies
		//	s_cPhysicsEngine->g_pLock = pSpinLock;

			DWORD myThreadID;
			HANDLE myHandle = CreateThread(NULL, 0, // stack size
				(LPTHREAD_START_ROUTINE)&PhysicsEngine::cPhysicsEngine::physicsThread, reinterpret_cast<void*>(s_cPhysicsEngine), 0, &myThreadID);
		}
		return s_cPhysicsEngine;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Adds the physics object with specified 'state'. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="position">	The position. </param>
	/// <param name="state">   	[in,out] If non-null, the state. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API bool cPhysicsEngine::addPhysicsObject(glm::vec3 position, iState * state)
	{
		//PhysicsEngine::cPhysicsEngine::instance()->gLock(0);
		_btRigidBody * rb = new _btRigidBody();
		state->registerComponentXMLDataCallback(std::function<std::string() >(std::bind(&_btRigidBody::saveToXMLNode, rb)));

		rb->state = state;
		state->setPosition(position);

		rb->setCollisionMask(5);
		rb->setCollisionFilter(1);

		glm::vec3 colShapePos = state->getBoundingBox().position;

		btTransform worldTransform;
		worldTransform.setIdentity();
		worldTransform.setOrigin(btVector3(position.x, position.y, position.z));

		btScalar mass = 0.0f;

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);
		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		glm::vec3 boxHalfWidth = state->getBoundingBox().scale;
		if (m_map_MeshNameToTriangleMesh[rb->state->getMeshName()] != 0)
		{
			btCollisionShape* collisionShapeTerrain = new btBvhTriangleMeshShape(m_map_MeshNameToTriangleMesh[rb->state->getMeshName()], true, true);
			sBoundingBox bb = rb->state->getBoundingBox(); // For debugging
			btVector3 localScaling = collisionShapeTerrain->getLocalScaling();

			// if (isDynamic)
			//	collisionShapeTerrain->calculateLocalInertia(mass, localInertia);

			btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), worldTransform.getOrigin()));
			// NOTE: 0.0f mass == static  && btVector3(0,0,0) == inertia
			btRigidBody::btRigidBodyConstructionInfo rigidBodyConstructionInfo(0.0f, motionState, collisionShapeTerrain, localInertia);
			btRigidBody* rigidBodyTerrain = new btRigidBody(rigidBodyConstructionInfo);
			rigidBodyTerrain->setFriction(btScalar(0.9));

			rb->m_rigidBody = rigidBodyTerrain;

		}
		else {


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
		g_btWorld->m_btWorld->addRigidBody(rb->m_rigidBody, BIT(5), collisionFilterResult);
		rb->m_rigidBody->setUserPointer(rb);



		vec_rigidBodies.push_back(rb);
		//map_rigidBodies[rb->m_rigidBody->getUserIndex()] = rb;
		//PhysicsEngine::cPhysicsEngine::instance()->gUnlock(0);
		return true;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates a convex hull. </summary>
	///
	/// <remarks>	Richard, 4/13/2017. </remarks>
	///
	/// <param name="meshName">			Name of the mesh. </param>
	/// <param name="triangleFaces">	[in,out] If non-null, the triangle faces. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API bool cPhysicsEngine::generateConvexHull(std::string meshName, std::vector<sTriangleFace*> triangleFaces)
	{

		btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();
		//btConvexHullShape* shape = new btConvexHullShape();// (const btScalar*)(&(v.xyzw[0])), glmesh->m_numvertices, sizeof(GLInstanceVertex));

		for each(sTriangleFace* triFace in triangleFaces)
		{
			triangleMeshTerrain->addTriangle(
				btVector3(triFace->v1.x, triFace->v1.y, triFace->v1.z),
				btVector3(triFace->v2.x, triFace->v2.y, triFace->v2.z),
				btVector3(triFace->v3.x, triFace->v3.y, triFace->v3.z)
			);
			//shape->addPoint(btVector3(triFace->v1.x, triFace->v1.y, triFace->v1.z));
			//shape->addPoint(btVector3(triFace->v2.x, triFace->v2.y, triFace->v2.z));
			//shape->addPoint(btVector3(triFace->v3.x, triFace->v3.y, triFace->v3.z));
		}
		int numTri = triangleMeshTerrain->getNumTriangles();
		//m_map_MeshNameToTriangleMesh[meshName] = new btBvhTriangleMeshShape(triangleMeshTerrain, true);

		return true;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generates the physics mesh. </summary>
	///
	/// <remarks>	Richard, 4/13/2017. </remarks>
	///
	/// <param name="meshName">   	Name of the mesh. </param>
	/// <param name="indices">	  	[in,out] If non-null, the indices. </param>
	/// <param name="vertices">   	[in,out] If non-null, the vertices. </param>
	/// <param name="numVertices">	Number of vertices. </param>
	/// <param name="numIndices"> 	Number of indices. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API bool cPhysicsEngine::generatePhysicsMesh(std::string meshName, unsigned int * indices, sMeshVertex * vertices, const unsigned int numVertices, const unsigned int numIndices)
	{
	//	gLock(0);
		btTriangleMesh* triangleMeshTerrain = new btTriangleMesh();
		printf(std::to_string(numVertices).c_str());
		printf(meshName.c_str());
	
		triangleMeshTerrain->preallocateVertices(numVertices);
		triangleMeshTerrain->preallocateIndices(numIndices);

			//printf(std::to_string( vertices[0].Position.x).c_str());
			for (unsigned int index = 0; index < numVertices; index++)
			{
				triangleMeshTerrain->findOrAddVertex(
						btVector3(
							vertices[index].Position.x,
							vertices[index].Position.y,
							vertices[index].Position.z
						),
					false);
			}


		for (int index = 0; index < numIndices; index += 3)
		{
			triangleMeshTerrain->addTriangleIndices(
				indices[index],
				indices[index + 1],
				indices[index + 2]);
		}
		m_map_MeshNameToTriangleMesh[meshName] = triangleMeshTerrain;
		// Cast to btBvhTriangleMeshShape((m_map_MeshNameToTriangleMesh[rb->state->getMeshName()], true); for static objects
		// Cast to btConvexTriangleMeshShape((m_map_MeshNameToTriangleMesh[rb->state->getMeshName()]); for dynamic objects
		return true;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Clears the game objects and resets the bullet world. </summary>
	///
	/// <remarks>	Richard, 4/15/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	PhysicsEngine_API void cPhysicsEngine::clearGameObjects()
	{


		vec_rigidBodies.clear();

		if (PhysicsEngine::cPhysicsEngine::instance()->impl() != nullptr)
		{
			if (g_btWorld != nullptr)
			{	
				//delete this->impl()->m_btWorld->m_btWorld;
			g_btWorld->~_btWorld();
			g_btWorld = new _btWorld();
			}
		}

	}
	PhysicsEngine_API void cPhysicsEngine::removeObjectsAtSelection()
	{
		
	}
}

