#ifndef _iWorld_HG_
#define _iWorld_HG_
#include "cRigidBody.h"
#include "stdafx.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class cCollisionObject;
	class iWorld {
	public:
		inline virtual void step(float deltaTime) = 0;
		//virtual PhysicsEngine_API void addRigidBody(cRigidBody* rigidBody) = 0;
		virtual void addCollisionObject(iRigidBody* object) = 0;
	};

}
#endif