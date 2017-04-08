#ifndef _iRigidBody_HG_
#define _iRigidBody_HG_
#include "stdafx.h"
#include <sstream>
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
		bool collision;
		glm::vec3 position;
		glm::quat orientation; 
		glm::mat4 rotMatrix;
	};

	class iRigidBody {
	public:
		iState* state;
		// TEMP VARIABLES///////////////////////
		int collisionMask = 0;
		int collisionFilter;
		std::vector<int> collisionFilters;
		int hingeID = 0;
		float mass = 0;
		///////////////////////////////////////
		virtual ~iRigidBody() {}
		virtual PhysicsEngine_API void getOrientation(glm::mat4& orientation) = 0;
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
		virtual PhysicsEngine_API bool isCollision() = 0;
		virtual PhysicsEngine_API void setCollision(bool collision) = 0;

		virtual PhysicsEngine_API int getCollisionMask() = 0;
		virtual PhysicsEngine_API void setCollisionMask(int collisionMask) = 0;

		virtual PhysicsEngine_API int getCollisionFilter() = 0;
		virtual PhysicsEngine_API void setCollisionFilter(int collisionMask) = 0;
		std::string saveToXMLNode() {
			std::string componentNode;
			glm::vec3 position;
			this->state->getPosition(position);
			componentNode = "<PhysicsComponent>";
			componentNode += "<RigidBody ";
			componentNode += "offsetX=\"";
			//glm::to_string()
			std::stringstream string;
			string.precision(6);
			string << position.x <<
				"\" " << "offsetY=\"" <<
				position.y << "\" " << "offsetZ=\""
				<< position.z << "\""
				<< " mass=\"" << this->mass << "\""

				<< " hingeConstraint=\"" << this->hingeID << "\" "
				<< ">"
				"<CollisionInfo collisionMask=\"" << collisionMask
				<< "\" >";

			for each(int filter in collisionFilters)
			{
				string << "<CollidesWith collisionMask=\"" << filter << "\" />";
			}
			string 
				<< "</CollisionInfo>"
				<< "</RigidBody>"
				<< "</PhysicsComponent>";

			componentNode += string.str();
			return componentNode;
		}
	protected:
		iRigidBody() {}
	private:
		iRigidBody(const iRigidBody& other) {}
		iRigidBody& operator=(const iRigidBody& other) { return *this; }
	};
}
#endif