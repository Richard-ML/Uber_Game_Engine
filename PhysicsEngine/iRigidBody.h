#ifndef _iRigidBody_HG_
#define _iRigidBody_HG_
#include "stdafx.h"
#include "cCollisionObject.h"
#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
namespace PhysicsEngine {
	// Constructor info for the rigid body
	struct sRigidBody {
		int motionState;
		glm::vec3 turnVelocity;
		glm::vec3 velocity;
		float mass;
		glm::mat4 transform;
		glm::vec3 acceleration;
		eCollisionFlags collisionFlag;
		bool collision;
		glm::vec3 position;
	};

	class iRigidBody: public cCollisionObject {
	public:
		virtual ~iRigidBody() {}
		virtual void getOrientation(glm::mat4& orientation) = 0;
		//virtual void getTransform(glm::mat4& transform) = 0;
		virtual PhysicsEngine_API void getPosition(glm::vec3& position) = 0;
		virtual PhysicsEngine_API void setPosition(const glm::vec3& position) = 0;
		virtual PhysicsEngine_API void getVelocity(glm::vec3& velocity) = 0;
		virtual PhysicsEngine_API void setVelocity(const glm::vec3& velocity) = 0;
		virtual PhysicsEngine_API void getMass(float& mass) = 0;
		virtual PhysicsEngine_API void setMass(const float& mass) = 0;
		virtual PhysicsEngine_API void getAcceleration(glm::vec3& acceleration) = 0;
		virtual PhysicsEngine_API void setAcceleration(const glm::vec3& acceleration) = 0;
		virtual PhysicsEngine_API void applyForce(const glm::vec3 velocity) = 0;
		virtual PhysicsEngine_API void isCollision(bool& collision) = 0;
		virtual PhysicsEngine_API void setCollision(bool& collision) = 0;

	protected:
		iRigidBody() {}
	private:
		iRigidBody(const iRigidBody& other) {}
		iRigidBody& operator=(const iRigidBody& other) { return *this; }
	};
}
#endif