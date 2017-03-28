#include "stdafx.h"
#include "_btRigidBody.h"
PhysicsEngine::_btRigidBody::_btRigidBody(const sRigidBody & rigidBody, iCollisionShape* collisionShape)
{
	_btCollisionShape* colShape = dynamic_cast<_btCollisionShape*> (collisionShape);

	btTransform worldTransform;
	worldTransform.setIdentity();
	//m_rigidBody.translate(btVector3( rigidBody.position.x, rigidBody.position.y, rigidBody.position.z));
    worldTransform.setOrigin(btVector3(rigidBody.position.x, rigidBody.position.y, rigidBody.position.z));

	btScalar mass = rigidBody.mass;

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	

	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (isDynamic)
		colShape->m_collisionShape->calculateLocalInertia(mass, localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(rigidBody.position.x, rigidBody.position.y, rigidBody.position.z))); // TODO: cleanup
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, colShape->m_collisionShape, localInertia);
	m_rigidBody = new btRigidBody(rbInfo);
	m_rigidBody->setRestitution(0.80f);
	m_rigidBody->setLinearVelocity(btVector3(rigidBody.velocity.x, rigidBody.velocity.y, rigidBody.velocity.z));
	m_rigidBody->activate(true);
	m_rigidBody->forceActivationState(DISABLE_DEACTIVATION);
	switch (collisionShape->getShapeType())
	{
	case eShapeType::UNKNOWN:



		break;

	case eShapeType::SPHERE:
	{
		float inertiaTensor = glm::pow((0.4f*mass*collisionShape->getRadius()), 2.0f);
		m_rigidBody->setMassProps(mass, btVector3(inertiaTensor, inertiaTensor, inertiaTensor));
		m_rigidBody->setFriction(0.8f);
		m_rigidBody->setRollingFriction(.5f);
		m_rigidBody->setSpinningFriction(0.5f);
		//m_rigidBody->setRestitution(0.5f);
		m_rigidBody->setAnisotropicFriction(m_rigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	}
		break;
	case eShapeType::PLANE:
		m_rigidBody->setFriction(0.8f);
		//m_rigidBody->setRestitution(0.5f);
		break;
	}




}

PhysicsEngine::_btRigidBody::~_btRigidBody()
{
	delete m_rigidBody;
}

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

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getPosition(glm::vec3 & position)
{
	btVector3 worldPos = m_rigidBody->getWorldTransform().getOrigin();

	glm::vec3 pos(worldPos.getX(), worldPos.getY(), worldPos.getZ());
	position = pos;
	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setPosition(const glm::vec3 & position)
{
	btTransform worldTransform = m_rigidBody->getWorldTransform();
	worldTransform.setIdentity();
	//m_rigidBody.translate(btVector3( rigidBody.position.x, rigidBody.position.y, rigidBody.position.z));
	worldTransform.setOrigin(btVector3(position.x, position.y, position.z));
	m_rigidBody->setWorldTransform(worldTransform);

	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getVelocity(glm::vec3 & velocity)
{
	btVector3 vel = m_rigidBody->getLinearVelocity();
	glm::vec3 linVel(vel.getX(), vel.getY(), vel.getZ());
	velocity = linVel;
	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setVelocity(const glm::vec3 & velocity)
{
	m_rigidBody->setLinearVelocity(btVector3( velocity.x, velocity.y, velocity.z ));
	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getMass(float & mass)
{
	mass = std::abs(m_rigidBody->getInvMass());
	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setMass(const float & mass)
{

	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::getAcceleration(glm::vec3 & acceleration)
{
	
	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setAcceleration(const glm::vec3 & acceleration)
{

	return;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::applyForce(const glm::vec3 velocity)
{
	//m_rigidBody->forceActivationState(ACTIVE_TAG); // Wake up the object if it is sleeping
	//m_rigidBody->clearForces();
	m_rigidBody->applyCentralImpulse(btVector3(velocity.x * 10, velocity.y * 10, velocity.z * 10));
	//m_rigidBody->applyForce(btVector3(velocity.x * 100, velocity.y * 100, velocity.z * 100), m_rigidBody->getWorldTransform().getOrigin()); // TODO : Fix this
	return;
}

PhysicsEngine_API bool PhysicsEngine::_btRigidBody::isCollision()
{
	//return m_rigidBody.getCollisionFlags() != eCollisionFlags;
	return isColliding;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollision(bool collision)
{
	isColliding = collision;
	return;
}

PhysicsEngine_API int PhysicsEngine::_btRigidBody::getCollisionMask()
{
	return this->collisionMask;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollisionMask(int collisionMask)
{
	this->collisionMask = collisionFilter;
	return;
}

PhysicsEngine_API int PhysicsEngine::_btRigidBody::getCollisionFilter()
{
	return this->collisionFilter;
}

PhysicsEngine_API void PhysicsEngine::_btRigidBody::setCollisionFilter(int collisionMask)
{
	this->collisionFilter = collisionMask;
}
