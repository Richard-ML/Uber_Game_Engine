#ifndef _cTextureManager_HG_
#define _cTextureManager_HG_
#include "stdafx.h"  
#include "externals.h"
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cTextureManager.h - Texture Manager ---------------------*- C++ -*-===//
Description: Manages textures!
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

// Loads textures into OpenGL texture units
// Maps texture with textureID look up

class cTextureManager {
	static cTextureManager *s_cTextureManager;

public:
	static cTextureManager *instance();
	void loadTexture(rapidxml::xml_node<> *textureNode);
	void loadTextureMipmap(rapidxml::xml_node<> *textureNode);
	void loadWorldTilesFromImage(rapidxml::xml_node<> *worldNode);
	GLuint loadCubeMap(rapidxml::xml_node<> *cubeNode);

private:
	cTextureManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cTextureManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cTextureManager(const cTextureManager &); // Disallow copy constructor
	cTextureManager &operator=(const cTextureManager &textureManager) {
	} // Disallow assignment operator
};

#endif