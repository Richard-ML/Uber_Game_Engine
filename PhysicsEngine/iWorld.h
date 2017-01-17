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
	class cRigidBody;
	class iWorld {
	public:
		virtual PhysicsEngine_API void step(float deltaTime) = 0;
		virtual PhysicsEngine_API void addRigidBody(cRigidBody* rigidBody) = 0;

	};

}
#endif