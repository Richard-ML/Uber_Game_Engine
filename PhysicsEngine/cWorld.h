#ifndef _cWorld_HG_
#define _cWorld_HG_
#include "iWorld.h"
#include <vector>
#include "stdafx.h"
#include "cCollisionObject.h"

#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	class cCollisionObject;
	class cWorld: public iWorld {
	public:
		void step(float deltaTime);
		//virtual PhysicsEngine_API void addRigidBody(cRigidBody* rigidBody)
		void addCollisionObject(iRigidBody* object);
	private:
		//static std::vector<cRigidBody*> m_rigidBodies;
	 std::vector<iRigidBody *> m_objects;
	};


}
#endif