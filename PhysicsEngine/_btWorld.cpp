#include "stdafx.h"
#include "_btWorld.h"
PhysicsEngine::_btWorld::_btWorld()
{
		//TODO: CLEAN UP THESE POINTERS WHEN WE ARE DONE WITH THEM
		///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
	
		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
		gContactAddedCallback = PhysicsEngine::_btWorld::contact_callback;
		///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		m_overlappingPairCache = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		
		m_solver = new btSequentialImpulseConstraintSolver;
	
		m_btWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
		//m_btWorld->getSolverInfo().m_splitImpulse = 0;
		m_btWorld->setGravity(btVector3(0, -1000 ,0));
	

}
PhysicsEngine::_btWorld::~_btWorld()
{
		delete m_btWorld;
		delete m_collisionConfiguration;
		delete m_dispatcher;
		delete m_overlappingPairCache;
		delete m_solver;
}
void PhysicsEngine::_btWorld::step(float deltaTime)
{
	//printf("STEP!");
	m_btWorld->stepSimulation(deltaTime, 7);
}

void PhysicsEngine::_btWorld::addCollisionObject(iRigidBody * object)
{
	_btRigidBody* rb = 	dynamic_cast<_btRigidBody*>(object);
	//if(rb->getCollisionShape()->getShapeType() == eShapeType::PLANE)
	//m_btWorld->addCollisionObject(rb->m_rigidBody);
	//else
	m_btWorld->addRigidBody(rb->m_rigidBody);
}

bool PhysicsEngine::_btWorld::contact_callback(btManifoldPoint & cp, const btCollisionObjectWrapper * colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper * colObj1Wrap, int partId1, int index1)
{
	_btRigidBody* rb1 = reinterpret_cast<_btRigidBody*>(colObj0Wrap->getCollisionObject()->getUserPointer());
	rb1->setCollision(true);

	_btRigidBody* rb2 = reinterpret_cast<_btRigidBody*>(colObj1Wrap->getCollisionObject()->getUserPointer());
	rb2->setCollision(true);
	return false;
}

