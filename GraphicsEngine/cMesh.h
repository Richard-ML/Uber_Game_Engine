#ifndef _cMesh_HG_
#define _cMesh_HG_
#include "externals.h"
#include "cMeshManager.h"
#include "stdafx.h"
#ifdef GraphicsEngine_EXPORTS
#define GraphicsEngine_API __declspec(dllexport)
#else
#define GraphicsEngine_API __declspec(dllimport)
#endif // GraphicsEngine_EXPORTS
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cMesh.h - Mesh Information ------------------------------*- C++ -*-===//
Description: Contains mesh description
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
class GraphicsEngine_API cMesh {
public:
	cMeshEntry *m_pMeshEntry = NULL;
	glm::vec4 *m_pColor = NULL;
	float alpha;
	bool isWireframe = false;
	bool isEnabled = true;
	glm::mat4 m_pBodyOffset; // Used to enable object to have multiple child
							 // meshes at different positions.
	glm::mat4 m_curTransform;
	glm::mat4 getOrientation() {
		glm::mat4 tempMatrix = m_pBodyOffset;
		// Set position of matrix to the origin
		tempMatrix[3].x = 0.0f;
		tempMatrix[3].y = 0.0f;
		tempMatrix[3].z = 0.0f;
		return glm::mat4_cast(glm::conjugate(glm::quat_cast(tempMatrix)));
	}
	glm::vec3 scale = glm::vec3(1.0f);
};
#endif