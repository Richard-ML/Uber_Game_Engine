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

	///-------------------------------------------------------------------------------------------------
	/// <summary>	A bullet rigid body wrapper. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	class _btRigidBody : public iRigidBody {
		friend class _btWorld;
		friend class _btCollisionShape;
		// PROXY DATA-MEMBERS
		bool isColliding;

	public:
		_btRigidBody() {}
		~_btRigidBody();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the orientation. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="orientation">	[in,out] The orientation. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void getOrientation(glm::mat4& orientation);
		//virtual void getTransform(glm::mat4& transform);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the position. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="position">	[in,out] The position. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void getPosition(glm::vec3& position);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets position. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="position">	The position. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setPosition(const glm::vec3& position);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets velocity. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="velocity">	[in,out] The velocity. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void getVelocity(glm::vec3& velocity);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets velocity. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="velocity">	The velocity. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setVelocity(const glm::vec3& velocity);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the mass. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="mass">	[in,out] The mass. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void getMass(float& mass);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the mass. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="mass">	The mass. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setMass(const float& mass);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the acceleration. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="acceleration">	[in,out] The acceleration. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void getAcceleration(glm::vec3& acceleration);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets acceleration. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="acceleration">	The acceleration. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setAcceleration(const glm::vec3& acceleration);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Applies the force described by velocity. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="velocity">	The velocity. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void applyForce(const glm::vec3 velocity);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Query if this object is collision. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	True if collision, false if not. </returns>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API bool isCollision();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets collision status. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="collision">	True to collision. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setCollision(bool collision);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets collision mask. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	The collision mask. </returns>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API int getCollisionMask();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets collision mask. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="collisionMask">	The collision mask. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setCollisionMask(int collisionMask);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets collision filter. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	The collision filter. </returns>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API int getCollisionFilter();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets collision filter. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="collisionMask">	The collision mask. </param>
		///-------------------------------------------------------------------------------------------------

		virtual PhysicsEngine_API void setCollisionFilter(int collisionMask);
	//private:
		/// <summary>	The actual bullet rigid body. </summary>
		btRigidBody* m_rigidBody;

	};
}
#endif