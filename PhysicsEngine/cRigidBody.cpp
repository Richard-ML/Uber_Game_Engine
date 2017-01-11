#include "stdafx.h"
#include "cRigidBody.h"

PhysicsEngine::cRigidBody::cRigidBody(cCollisionObject * object)
{

}

PhysicsEngine::cRigidBody::~cRigidBody()
{
}

glm::mat4 PhysicsEngine::cRigidBody::getTransform()
{
	glm::mat4 matrix = glm::translate(matrix, m_position);
	matrix *= glm::eulerAngleXYZ(m_angle.x, m_angle.y, m_angle.z);
	return matrix;
}
