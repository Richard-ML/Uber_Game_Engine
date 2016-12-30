#ifndef _cLight_HG_
#define _cLight_HG_
#include "externals.h"
#include "cMesh.h"
#include <vector>
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cLight.h - Light Information ----------------------------*- C++ -*-===//
Description: Contains light description
//===----------------------------------------------------------------------===//
Author(s):
 Name: Richard Mills-Laursen
 Role(s): Student
 E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca
 
 Name: Michael Feeney
 E-mail: MFeeney@FanshaweC.on.ca
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
 
 Name: L. Gustafson
 E-Mail: L_Gustafson@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/

class cLight {
public:
  glm::mat4 matrix;
  glm::vec3 offset;
  glm::vec3 direction;
  glm::mat4 directionOffset;
  int typeFlag;
  bool isEnabled;
  float coneAngle;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  GLfloat specularPower;
  float attenConst;  // = 0.0f;
  float attenLinear; // = 0.1f;
  float attenQuad;   // = 0.01f;
  unsigned int lightID;
  GLuint gUniformId_TypeFlag;
  GLuint gUniformId_IsEnabled;
  GLuint gUniformId_Position;
  GLuint gUniformId_Direction;
  GLuint gUniformId_ConeAngle;
  GLuint gUniformId_Ambient;
  GLuint gUniformId_Diffuse;
  GLuint gUniformId_Specular;
  GLuint gUniformId_SpecularPower;
  GLuint gUniformId_AttenuationConst;
  GLuint gUniformId_AttenuationLinear;
  GLuint gUniformId_AttenuationQuad;
  GLuint gUniformId_Color;
  // Debug spheres
  std::vector<cMesh *> vec_DebugShapes;
};
#endif