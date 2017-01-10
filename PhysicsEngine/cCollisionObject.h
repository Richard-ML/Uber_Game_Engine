#ifndef _cCollisionObject_HG_
#define _cCollisionObject_HG_
#include "stdafx.h"
#include "cCollisionShape.h"
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
//===-- cCollisionObject.h - Collision Object Information ---------*- C++ -*-===//
Description: Maintains all information that is needed for a collision detection: Object, Transform and AABB proxy. Added to the cCollisionWorld.
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
	class cCollisionObject {
	public:
		struct sAABB {
		public:
			sAABB(glm::vec3 min, glm::vec3 max) {
				this->halfWidths =
					glm::abs(max - min) * glm::vec3(0.5f); // Extent computation
				this->center = max - halfWidths; // Center position of AABB
				this->min = min;
				this->max = max;
			};
			sAABB() {};
			glm::vec3 center;
			glm::vec3 halfWidths;
			glm::vec3 min;
			glm::vec3 max;
		};
	private:
		sAABB m_broadPhase; // Pointer to parent AABB.. 
		glm::mat4 m_transform;
		int m_collisionFlag;
		bool m_disableGravity;
		cCollisionShape* m_collisionShape; // Shape used for narrow phase collision detection. Reuse shapes as much as possible!

	};
}
#endif