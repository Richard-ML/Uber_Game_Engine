#ifndef _cMeshManager_HG_
#define _cMeshManager_HG_
#include "stdafx.h"
#include "externals.h"
#include <map>
#include <vector>
#include "cGraphicsEngine.h"
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
//===-- cMeshManager.h - Manages meshes -------------------------*- C++ -*-===//
Description: Provides functionality to load meshes into OpenGL buffers.
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
struct GraphicsEngine_API cMeshEntry {
	int NumgIndices;
	int NumgVertices;
	int BaseIndex;
	int BaseVertex;
};

struct GraphicsEngine_API cMeshVertex {
	glm::vec4 Position;
	glm::vec4 TexCoord;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::uvec4 textureUnits; // TODO: Switch to uvec4 (GL_UNSIGNED_INT) or
							 // (GL_UNSIGNED_BYTE)
};
class GraphicsEngine_API cMeshManager {
	static cMeshManager *s_cMeshManager;
	// Boilerplate
	friend class cMeshManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cMeshManager_Impl *impl() const;
	cMeshManager_Impl *impl();

public:

	static cMeshManager *instance();
	bool loadMeshFileIntoGLBuffer(const char *path, cMeshEntry &entryOut,
		float scale, bool isStationary);
	void loadWorldTiles(cMeshEntry &entryOut);

	// Map... aka "dictionary" "look up table"
	std::map<std::string, cMeshEntry> m_MapMeshNameTocMeshEntry;

private:
	cMeshManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cMeshManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cMeshManager(const cMeshManager &); // Disallow copy constructor
	cMeshManager &operator=(const cMeshManager &meshManager) {
	} // Disallow assignment operator
};

#endif
