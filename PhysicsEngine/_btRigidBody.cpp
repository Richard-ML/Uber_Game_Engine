#include "stdafx.h"
#include "_btRigidBody.h"

PhysicsEngine::_btRigidBody::~_btRigidBody()
{
	delete m_rigidBody;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets orientation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="orientation">	[in,out] The orientation. </param>
///-------------------------------------------------------------------------------------------------

void PhysicsEngine::_btRigidBody::getOrientation(glm::mat4 & orientation)
{
	if (this->m_rigidBody->getCollisionShape()->getName() != "STATICPLANE")
	{
	
	btTransform trans;
	glm::mat4 result;
	m_rigidBody->getMotionState()->getWorldTransform(trans);
	//trans = trans.getIdentity();
	trans.getOpenGLMatrix(&result[0][0]);

	orientation = result;
}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets position. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="position">	[in,out] The position. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getPosition(glm::vec3 & position)
{
	btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();

	glm::vec3 pos(worldPos.getX(), worldPos.getY(), worldPos.getZ());
	position = pos;
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets position. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="position">	The position. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setPosition(const glm::vec3 & position)
{
	btTransform worldTransform = m_rigidBody->getWorldTransform();
	worldTransform.setIdentity();
	//m_rigidBody.translate(btVector3( rigidBody.position.x, rigidBody.position.y, rigidBody.position.z));
	worldTransform.setOrigin(btVector3(position.x, position.y, position.z));
	m_rigidBody->setWorldTransform(worldTransform);

	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets velocity. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="velocity">	[in,out] The velocity. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getVelocity(glm::vec3 & velocity)
{
	btVector3 vel = m_rigidBody->getLinearVelocity();
	glm::vec3 linVel(vel.getX(), vel.getY(), vel.getZ());
	velocity = linVel;
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets velocity. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="velocity">	The velocity. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setVelocity(const glm::vec3 & velocity)
{
	m_rigidBody->setLinearVelocity(btVector3( velocity.x, velocity.y, velocity.z ));
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the mass. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="mass">	[in,out] The mass. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getMass(float & mass)
{
	mass = std::abs(m_rigidBody->getInvMass());
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets the mass. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="mass">	The mass. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setMass(const float & mass)
{

	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets acceleration. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="acceleration">	[in,out] The acceleration. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getAcceleration(glm::vec3 & acceleration)
{
	
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets acceleration. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="acceleration">	The acceleration. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setAcceleration(const glm::vec3 & acceleration)
{

	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Applies the force described by velocity. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="velocity">	The velocity. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::applyForce(const glm::vec3 velocity)
{
	//m_rigidBody->forceActivationState(ACTIVE_TAG); // Wake up the object if it is sleeping
	//m_rigidBody->clearForces();
	m_rigidBody->applyCentralImpulse(btVector3(velocity.x * 10, velocity.y * 10, velocity.z * 10));
	//m_rigidBody->applyForce(btVector3(velocity.x * 100, velocity.y * 100, velocity.z * 100), m_rigidBody->getWorldTransform().getOrigin()); // TODO : Fix this
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Query if this object is colliding with another object. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	True if collision, false if not. </returns>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API bool PhysicsEngine::_btRigidBody::isCollision()
{
	//return m_rigidBody.getCollisionFlags() != eCollisionFlags;
	return isColliding;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets collision status. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="collision">	True to collision. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollision(bool collision)
{
	isColliding = collision;
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets collision mask. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	The collision mask. </returns>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API int PhysicsEngine::_btRigidBody::getCollisionMask()
{
	return this->collisionMask;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets collision mask. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="collisionMask">	The collision mask. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollisionMask(int collisionMask)
{
	this->collisionMask = collisionMask;
	return;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets collision filter. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	The collision filter. </returns>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API int PhysicsEngine::_btRigidBody::getCollisionFilter()
{
	return this->collisionFilter;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets collision filter. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="collisionMask">	The collision mask. </param>
///-------------------------------------------------------------------------------------------------

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollisionFilter(int collisionMask)
{
	this->collisionFilter = collisionMask;
}
