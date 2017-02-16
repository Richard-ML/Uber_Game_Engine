#include "stdafx.h"
#include "_btCollisionShape.h"

PhysicsEngine::_btCollisionShape::_btCollisionShape(eShapeType shapeType)
{
	switch (shapeType)
	{
	case PhysicsEngine::UNKNOWN:
		// Such mystery..
		break;
	case PhysicsEngine::SPHERE:
		m_collisionShape = new btSphereShape(0); // Make sure the radius is set later!
		break;
	case PhysicsEngine::PLANE:
		m_collisionShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 0.0f);
		break;
	default:
		//How bout u meet me outside? 
		// HOW-BOU-TAH!
		break;
	}
}

void PhysicsEngine::_btCollisionShape::getBoundingSphere(glm::vec3 &position, float& radius) const
{
	btVector3* _btPosition = new btVector3();
	btScalar* _btRadius = new btScalar();
	m_collisionShape->getBoundingSphere(*_btPosition, *_btRadius);
	glm::vec3 _pos = glm::vec3((float)_btPosition->getX(), (float)_btPosition->getY(), (float)_btPosition->getZ());
	position = _pos;
	radius = *_btRadius;

	delete _btPosition;
	delete _btRadius;
}

void PhysicsEngine::_btCollisionShape::setRadius(float radius)
{
	btSphereShape* colShape = dynamic_cast<btSphereShape*> (m_collisionShape);
	colShape->setUnscaledRadius(radius);

	m_collisionShape = colShape;
	// TODO: What is user tries to set radius of non-sphere shape

}

float PhysicsEngine::_btCollisionShape::getRadius()
{
	if (this->getShapeType() == SPHERE) {
		btSphereShape* colShape = dynamic_cast<btSphereShape*> (m_collisionShape);
		return colShape->getRadius();
	}
	return 0.0f;

}

const char * PhysicsEngine::_btCollisionShape::getName() const
{
	return m_collisionShape->getName();
}

void PhysicsEngine::_btCollisionShape::setNormal(glm::vec3 normal)
{
	m_collisionShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), 0.0f);
	//m_collisionShape = planeShape;
	//bool isnomove = m_collisionShape->isNonMoving();
	//dynamic_cast<btStaticPlaneShape*>(m_collisionShape)->ch
}
