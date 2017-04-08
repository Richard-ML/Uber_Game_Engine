#ifndef _iPhysicsWorld_HG_
#define _iPhysicsWorld_HG_
#include "stdafx.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class iPhysicsWorld {
	public:
		inline virtual void step(float deltaTime) = 0;
	};

}
#endif