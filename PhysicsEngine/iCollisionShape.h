#ifndef _iCollisionShape_HG_
#define _iCollisionShape_HG_
#include "stdafx.h"
#include <string>
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
//===-- cCollisionShape.h - Collision Shape Information ---------*- C++ -*-===//
Description: Provides an interface for collision shapes that can be shared among cCollisionObject(s).
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
	enum eShapeType {
		UNKNOWN = 0,
		SPHERE,
		PLANE
	};

	class iCollisionShape {
	private:
		void* m_usrPointer; // Pointer to state data that is thread safe..
		eShapeType m_shapeType;

		glm::vec3 m_normal;
	public:
		iCollisionShape() : m_shapeType(UNKNOWN), m_usrPointer(0){ }
		iCollisionShape(eShapeType shapeType) : m_shapeType(shapeType), m_usrPointer(0) { }
		virtual ~iCollisionShape(){ }

		///calculateTemporalAabb calculates the enclosing aabb for the moving object over interval [0..timeStep)
		///result is conservative
		// virtual void calculateTemporalAabb(const glm::mat4& transform, const glm::vec3& linvel, const glm::vec3& angvel, float timeStep, glm::vec3& temporalAabbMin, glm::vec3& temporalAabbMax) const = 0;

		virtual void getAabb(const glm::mat4& transform, glm::vec3& aabbMin, glm::vec3& aabbMax) const {};
		virtual void getBoundingSphere(glm::vec3& radius) const = 0;
		virtual void setRadius(float radius) = 0;
		inline virtual float getRadius() = 0;
		inline  void getBoundingSphere(glm::vec3 min, glm::vec3 max, glm::vec3& center, glm::vec3& radius)
		{
			radius = glm::length(max - min) * glm::vec3(0.5f);
			center = (min + max) * glm::vec3(0.5f);
		}
		inline void	setUserPointer(void*  userPtr) { m_usrPointer = userPtr; }

		inline void*	getUserPointer() const { return m_usrPointer; }
		inline void setNormal(glm::vec3 normal) { m_normal = normal; }
		inline eShapeType getShapeType() const { return m_shapeType; }
		inline virtual glm::vec3 getNormal() { return m_normal; };
		// DEBUG INFO METHODS
		virtual const char*	getName()const = 0;

	};

	class cPlane : public iCollisionShape {
	public:
		virtual void getBoundingSphere( glm::vec3& radius) const {}
		virtual inline const char*	getName()const { return "Plane"; }
		virtual void setRadius(float radius) {}
		inline virtual float getRadius() { return 0.0f; }
		cPlane(): iCollisionShape(eShapeType::PLANE) {  };
	};
	class cSphere : public iCollisionShape {
	private:
		float m_radius;
	public:
		virtual glm::vec3 getNormal() { return glm::vec3(); }
		virtual void getBoundingSphere(glm::vec3& radius) const { radius = radius; }
		virtual inline const char*	getName()const { return "Sphere"; }
		virtual void setRadius(float radius) { m_radius = radius; }
		inline virtual float getRadius() { return m_radius; }
		cSphere() : iCollisionShape(eShapeType::SPHERE) {  };
	};
}
#endif