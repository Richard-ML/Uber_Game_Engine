#include "stdafx.h"
#include "cWorld.h"


void PhysicsEngine::cWorld::step(float deltaTime)
{
	m_rigidBodies.at(0)->setMass(112.2f); // TEST
}

void PhysicsEngine::cWorld::addRigidBody(cRigidBody * rigidBody)
{
	//if (!rigidBody) return; // If nullptr return...
	if (std::find(m_rigidBodies.begin(), m_rigidBodies.end(), rigidBody) == m_rigidBodies.end())
		m_rigidBodies.push_back(rigidBody);
	
}
namespace PhysicsEngine{
std::vector<cRigidBody*> cWorld::m_rigidBodies;
}