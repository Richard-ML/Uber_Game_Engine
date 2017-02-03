#include "stdafx.h"
#include "cRigidBody.h"

PhysicsEngine::cRigidBody::cRigidBody(const sRigidBody & rigidBody)
{
	m_rigidBody = rigidBody;
}

PhysicsEngine::cRigidBody::~cRigidBody()
{
}

void PhysicsEngine::cRigidBody::getOrientation(glm::mat4 & orientation)
{
	glm::mat3 RotationMatrix = glm::transpose(glm::lookAt(glm::vec3(0.0f), glm::normalize(this->m_rigidBody.velocity), glm::vec3(0.0f, 1.0f, 0.0f)));
	orientation = RotationMatrix;
}

void PhysicsEngine::cRigidBody::getPosition(glm::vec3 & position)
{
	position = m_rigidBody.position;
}

void PhysicsEngine::cRigidBody::setPosition(const glm::vec3 & position)
{
	m_rigidBody.position = position;
}

void PhysicsEngine::cRigidBody::getVelocity(glm::vec3 & velocity)
{
	velocity = m_rigidBody.velocity;
}

void PhysicsEngine::cRigidBody::setVelocity(const glm::vec3 & velocity)
{
	m_rigidBody.velocity = velocity;
}

void PhysicsEngine::cRigidBody::getMass(float & mass)
{
	mass = m_rigidBody.mass;
}

void PhysicsEngine::cRigidBody::setMass(const float & mass)
{
	m_rigidBody.mass = mass;
}

void PhysicsEngine::cRigidBody::getAcceleration(glm::vec3 & acceleration)
{
	acceleration = m_rigidBody.acceleration;
}

void PhysicsEngine::cRigidBody::setAcceleration(const glm::vec3 & acceleration)
{
	m_rigidBody.acceleration = acceleration;
}

PhysicsEngine_API void PhysicsEngine::cRigidBody::applyForce(const glm::vec3 velocity)
{
	this->m_rigidBody.velocity += velocity;
}

PhysicsEngine_API void PhysicsEngine::cRigidBody::isCollision( bool& collision)
{
	collision = this->m_rigidBody.collision;
}

PhysicsEngine_API void PhysicsEngine::cRigidBody::setCollision(bool & collision)
{
	this->m_rigidBody.collision = collision;
}

//void PhysicsEngine::cRigidBody::translate(glm::vec3 & translation)
//{
//}

//glm::mat4 PhysicsEngine::cRigidBody::getTransform()
//{
//	glm::mat4 matrix = glm::translate(matrix, m_position);
//	matrix *= glm::eulerAngleXYZ(m_angle.x, m_angle.y, m_angle.z);
//	return matrix;
//}
