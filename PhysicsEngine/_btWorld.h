#pragma once
#include "stdafx.h"
#include "iPhysicsWorld.h"
#include "_btRigidBody.h"
namespace PhysicsEngine {
	class _btWorld : public iPhysicsWorld {
		friend class _btRigidBody;
	private:// Todo: create getters and setters
	public:
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

		static bool contact_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
	};

}