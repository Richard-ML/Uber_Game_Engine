#ifndef _externals_HG_
#define _externals_HG_
#include <limits>
// glfw needs these guys ;)
#include <stdio.h>
#include <stdlib.h>
//#define GLM_MESSAGES
#include <glm\glm.hpp>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\random.hpp>
#include <glm\gtc\type_ptr.hpp>
// include glew first
#define GLEW_STATIC
#include <GL\glew.h>
// include glfw
#include <../fmod/fmod/fmod.hpp>
#include <../fmod/fmod/fmod_errors.h> // I will fix this later. In a rush at the moment.
#include <../rapidxml/rapidxml.hpp>
#include <GLFW\glfw3.h>

#include <FreeImage.h>
#include "cPhysicsEngine.h"  
#include "cTestEngine.h"
//#include "cGraphicsEngine.h"  
#define _CRT_SECURE_NO_WARNINGS
//#define RAPIDXML_NO_EXCEPTIONS

/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- externals.h - External Dependencies ---------------------*- C++ -*-===//
Description: Includes external libraries in a specific order.
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

inline void Set(glm::vec2 &v, float x, float y) {
  v.x = x;
  v.y = y;
}
inline void Set(glm::vec3 &v, float x, float y, float z) {
  v.x = x;
  v.y = y;
  v.z = z;
}
inline void Set(glm::vec4 &v, float x, float y, float z, float w) {
  v.x = x;
  v.y = y;
  v.z = z;
  v.w = w;
}
inline void SetMax(glm::vec3 &v, float x, float y, float z) {
  if (x > v.x)
    v.x = x;
  if (y > v.y)
    v.y = y;
  if (z > v.z)
    v.z = z;
}
inline void SetMin(glm::vec3 &v, float x, float y, float z) {
  if (x < v.x)
    v.x = x;
  if (y < v.y)
    v.y = y;
  if (z < v.z)
    v.z = z;
}
inline void SetMax(glm::vec3 &v) {
  //	v.x = std::numeric_limits<float>::max();
  //	v.y = std::numeric_limits<float>::max();
  //	v.z = std::numeric_limits<float>::max();
}
inline void SetMin(glm::vec3 &v) {
  //	v.x = std::numeric_limits<float>::min();
  //	v.y = std::numeric_limits<float>::min();
  //	v.z = std::numeric_limits<float>::min();
}
#endif