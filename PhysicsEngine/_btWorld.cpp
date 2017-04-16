#include "stdafx.h"
#include "_btWorld.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Destructor. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

PhysicsEngine::_btWorld::~_btWorld()
{
		delete m_btWorld;
		delete m_collisionConfiguration;
		delete m_dispatcher;
		delete m_overlappingPairCache;
		delete m_solver;
}



///-------------------------------------------------------------------------------------------------
/// <summary>	Steps the simulation based on given delta time </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="deltaTime">	The delta time. </param>
///-------------------------------------------------------------------------------------------------

void PhysicsEngine::_btWorld::step(float deltaTime)
{
	//printf("STEP!");
	m_btWorld->stepSimulation(deltaTime, 7);
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// Custom collision callback. When two objects collide this method is triggered and the is
/// colliding flag is set to true in the iState interface.
/// </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="cp">		  	[in,out] The cp. </param>
/// <param name="colObj0Wrap">	The col object 0 wrap. </param>
/// <param name="partId0">	  	The part identifier 0. </param>
/// <param name="index0">	  	The index 0. </param>
/// <param name="colObj1Wrap">	The col object 1 wrap. </param>
/// <param name="partId1">	  	The first part identifier. </param>
/// <param name="index1">	  	The first index. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool PhysicsEngine::_btWorld::contact_callback(btManifoldPoint & cp, const btCollisionObjectWrapper * colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper * colObj1Wrap, int partId1, int index1)
{
	_btRigidBody* rb1 = reinterpret_cast<_btRigidBody*>(colObj0Wrap->getCollisionObject()->getUserPointer());
	if(rb1 != 0)
	rb1->setCollision(true);

	_btRigidBody* rb2 = reinterpret_cast<_btRigidBody*>(colObj1Wrap->getCollisionObject()->getUserPointer());
	if (rb2 != 0)
	rb2->setCollision(true);
	return true;
}

