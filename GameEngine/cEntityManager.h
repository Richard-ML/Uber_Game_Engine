#ifndef _cEntityManager_HG_
#define _cEntityManager_HG_
#include <fstream>
#include <string>
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cEntityManager.h - Manages game entities ----------------*- C++ -*-===//
Description: Encapsulates entities, and provides simplified access to their common functionality.
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
 
 Name: L. Gustafson
 E-Mail: L_Gustafson@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/
class cEntityManager_Impl; // Purpose: Encapsulate private member variables and
                           // internal routines. Reduces make-time,
                           // compile-time, and the Fragile Binary Interface
                           // Problem.

class cEntityManager {
  static cEntityManager *s_cEntityManager;
  // Boilerplate
  friend class cEntityManager_Impl; // The PIMPL idiom aka Compilation Firewall
  const cEntityManager_Impl *impl() const;
  cEntityManager_Impl *impl();

public:
  int loadGameFromXML(std::string filename);
  static cEntityManager *instance();
  void drawEntites() const;
  void createSpiderAtPos(float x, float y, float z);

private:
  cEntityManager() {
  } // Constructor is private therefore a new instance can not be made
    // externally. Only available to members or friends of this class..
  ~cEntityManager() {}
  // Not defined to prevent copying of the only instance of the class.
  cEntityManager(const cEntityManager &); // Disallow copy constructor
  cEntityManager &operator=(const cEntityManager &entityManager) {
  } // Disallow assignment operator
};
#endif