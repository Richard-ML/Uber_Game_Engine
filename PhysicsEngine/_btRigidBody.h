#ifndef __btRigidBody_HG_
#define __btRigidBody_HG_
#include "iRigidBody.h"
#include "_btWorld.h"
#include "stdafx.h"
#ifdef PhysicsEngine_EXPORTS
#define PhysicsEngine_API __declspec(dllexport)
#else
#define PhysicsEngine_API __declspec(dllimport)
#endif // PhysicsEngine_EXPORTS
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cRigidBody.h - Rigid Body Information -------------------*- C++ -*-===//
Description: Primary class for rigid body objects.
//===----------------------------------------------------------------------===//
Author(s):
Name: Richard Mills-Laursen
Role(s): Student
E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca

Name: Michael Feeney
E-mail: MFeeney@FanshaweC.ca
Role(s): Coordinator & Professor
Course(s):
INFO-6044-01 - Game Engine Framework/Patterns
INFO-6028-01 - Graphics 1
INFO-6019-01 - Physics & Simulation 1
INFO-6045-01 - Animation
INFO-6023-01 - Game Algorithms & Gems
INFO-6020-01 - Graphics 2

Name: Ben McIlmoyle
E-Mail: B_McIlmoyle@FanshaweOnline.ca
Role: Professor
Course: INFO-6016-01 - Network Programming

Name: James Lucas
E-Mail: J_Lucas@FanshaweOnline.ca
Role: Professor
Course(s):
INFO-6017-01 - Artificial Intelligence
INFO-6022-01 - Physics & Simulation 2

Name: Oscar Lara
E-Mail: O_Lara@FanshaweOnline.ca
Role: Professor
Course(s):
INFO-6046-01 - Media Fundamentals
INFO-6025-01 - Configuration & Deployment

Name: Lucas Gustafson
E-Mail: L_Gustafson@FanshaweOnline.ca
Role: Professor
Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/
namespace PhysicsEngine {
	class _btRigidBody : public iRigidBody {
		friend class _btWorld;
		friend class _btCollisionShape;
		// PROXY DATA-MEMBERS
		bool isColliding;

	public:
		_btRigidBody() {}
		~_btRigidBody();
		virtual PhysicsEngine_API void getOrientation(glm::mat4& orientation);
		//virtual void getTransform(glm::mat4& transform);
		virtual PhysicsEngine_API void getPosition(glm::vec3& position);
		virtual PhysicsEngine_API void setPosition(const glm::vec3& position);
		virtual PhysicsEngine_API void getVelocity(glm::vec3& velocity);
		virtual PhysicsEngine_API void setVelocity(const glm::vec3& velocity);
		virtual PhysicsEngine_API void getMass(float& mass);
		virtual PhysicsEngine_API void setMass(const float& mass);
		virtual PhysicsEngine_API void getAcceleration(glm::vec3& acceleration);
		virtual PhysicsEngine_API void setAcceleration(const glm::vec3& acceleration);
		virtual PhysicsEngine_API void applyForce(const glm::vec3 velocity);
		virtual PhysicsEngine_API bool isCollision();
		virtual PhysicsEngine_API void setCollision(bool collision);
		virtual PhysicsEngine_API int getCollisionMask();
		virtual PhysicsEngine_API void setCollisionMask(int collisionMask);

		virtual PhysicsEngine_API int getCollisionFilter();
		virtual PhysicsEngine_API void setCollisionFilter(int collisionMask);
	//private:
		btRigidBody* m_rigidBody;

	};
}
#endif