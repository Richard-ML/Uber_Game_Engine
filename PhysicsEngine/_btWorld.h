#pragma once
#include "stdafx.h"
#include "iWorld.h"
#include "_btRigidBody.h"
namespace PhysicsEngine {
	class _btWorld : public iWorld {
		friend class _btRigidBody;
	private:
		btDiscreteDynamicsWorld* m_btWorld = 0;
		btDefaultCollisionConfiguration* m_collisionConfiguration = 0;
		btCollisionDispatcher* m_dispatcher = 0;
		btBroadphaseInterface* m_overlappingPairCache = 0;
		btSequentialImpulseConstraintSolver* m_solver = 0;
	public:
		_btWorld();
		virtual ~_btWorld();
		// Inherited via iWorld
		virtual void step(float deltaTime) override;
		virtual void addCollisionObject(iRigidBody * object) override;
	};

}