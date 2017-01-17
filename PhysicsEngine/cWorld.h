#ifndef _cWorld_HG_
#define _cWorld_HG_
#include "iWorld.h"
#include <vector>
#include "stdafx.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class cRigidBody;
	class cWorld: public iWorld {
	public:
		virtual PhysicsEngine_API void step(float deltaTime);
		virtual PhysicsEngine_API void addRigidBody(cRigidBody* rigidBody);
	private:
		static std::vector<cRigidBody*> m_rigidBodies;
	};


}
#endif