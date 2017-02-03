#ifndef _cCollisionObject_HG_
#define _cCollisionObject_HG_
#include "stdafx.h"
#include "iCollisionShape.h"
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
	enum eCollisionFlags
	{
		STATIC_OBJECT = 1,
		KINEMATIC_OBJECT = 2,
		NO_CONTACT_RESPONSE = 4,
		CUSTOM_MATERIAL_CALLBACK = 8,//this allows per-triangle material (friction/restitution)
		CHARACTER_OBJECT = 16,
	};

	enum	eCollisionObjectTypes
	{
		COLLISION_OBJECT = 1,
		RIGID_BODY = 2,
		///GHOST_OBJECT keeps track of all objects overlapping its AABB and that pass its collision filter
		///It is useful for collision sensors, explosion objects, character controller etc.
		GHOST_OBJECT = 4,
		SOFT_BODY = 8,
	};
	class cCollisionObject {
	private:
		iCollisionShape*		m_collisionShape;
		eCollisionFlags			m_collisionFlags;
		eCollisionObjectTypes   m_collisionObjectType;
		void*					m_userPointer;
		//iBroadphaseProxy*		m_broadphaseHandle;
	public:
		
		//Getters
		
		//Setters
		virtual void setCollisionShape(iCollisionShape* collisionShape) { m_collisionShape = collisionShape; }

		// Getters
		inline const iCollisionShape*	getCollisionShape() const { return m_collisionShape; }
		inline iCollisionShape*	getCollisionShape(){ return m_collisionShape; }
		inline eCollisionFlags	getCollisionFlag() const { return m_collisionFlags; }
		inline void* getUserPointer() { return m_userPointer;  }
		//Setters
		void setCollisionFlags(eCollisionFlags flags) { m_collisionFlags = flags; }
		void setUserPointer(void* userPointer) { m_userPointer = userPointer;  }
	};
}
#endif