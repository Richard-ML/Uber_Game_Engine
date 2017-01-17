#ifndef _cTexture_HG_
#define _cTexture_HG_
#include "externals.h"
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
//===-- cTexture.h - Texture Information ------------------------*- C++ -*-===//
Description: Contains texture description
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
class GraphicsEngine_API cTexture {
public:
	GLuint *textureID;
	// TODO: other custom texture properties..

	//"Important restriction for using shader images: underlying texture must have
	//been allocated using “immutable” storage, i.e. via glTexStorage*()-like
	//functions, and not glTexImage2D()."
};
#endif