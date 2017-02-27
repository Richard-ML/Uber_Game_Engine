#ifndef _cSoftBodyManager_HG_
#define _cSoftBodyManager_HG_
#include "stdafx.h"
// TODO: Change these!
struct cMeshEntry {
	int NumgIndices;
	int NumgVertices;
	int BaseIndex;
	int BaseVertex;
};
/* Mesh Information */
struct triFace {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

struct cMeshVertex {
	glm::vec4 Position;
	glm::vec4 TexCoord;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::uvec4 textureUnits; // TODO: Switch to uvec4 (GL_UNSIGNED_INT) or
							 // (GL_UNSIGNED_BYTE)
};
class cSoftBodyManager {
	static cSoftBodyManager *s_cSoftBodyManager;
	// Boilerplate
	friend class cSoftBodyManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cSoftBodyManager_Impl *impl() const;
	cSoftBodyManager_Impl *impl();

public:
	static cSoftBodyManager *instance();

	//bool loadSoftBodyFileIntoGLBuffer(std::string name, const char *path,
	//	float scale);

private:
	cSoftBodyManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cSoftBodyManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cSoftBodyManager(const cSoftBodyManager &); // Disallow copy constructor
	cSoftBodyManager &operator=(const cSoftBodyManager &meshManager) {
	} // Disallow assignment operator
};

#endif