#ifndef _cSoundManager_HG_
#define _cSoundManager_HG_
#include "externals.h"
#include <string>

#include <iostream>
#include <stdio.h>
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cSoundManager.h - Sound Manager -------------------------*- C++ -*-===//
Description: Manages sounds!
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

// Loads textures into OpenGL texture units
// Maps texture with textureID look up

class cSoundManager {
  static cSoundManager *s_cSoundManager;

public:
  static cSoundManager *instance();
  bool loadSound(rapidxml::xml_node<> *soundNode);
  void update();
  void playSound(std::string soundName, int channelID, FMOD_VECTOR channelPos,
                 FMOD_VECTOR channelVel);
  void init();

private:
  cSoundManager() {
  } // Constructor is private therefore a new instance can not be made
    // externally. Only available to members or friends of this class..
  ~cSoundManager() {}
  // Not defined to prevent copying of the only instance of the class.
  cSoundManager(const cSoundManager &); // Disallow copy constructor
  cSoundManager &operator=(const cSoundManager &textureManager) {
  } // Disallow assignment operator
  FMOD::ChannelGroup *groupA, *groupB, *masterGroup;
  unsigned int version;
  void *extradriverdata = 0;
};

#endif