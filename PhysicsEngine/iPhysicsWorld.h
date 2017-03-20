#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_
#include "cRigidBody.h"
#include "stdafx.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class cCollisionObject; // TODO remove this
	class iPhysicsWorld {
	public:
		inline virtual void step(float deltaTime) = 0;
		//virtual PhysicsEngine_API void addRigidBody(cRigidBody* rigidBody) = 0;
		virtual void addCollisionObject(iRigidBody* object) = 0;
	};

}
#endif