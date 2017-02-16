#ifndef _cMeshManager_HG_
#define _cMeshManager_HG_
#include "stdafx.h"
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
class cMeshManager {
	static cMeshManager *s_cMeshManager;
	// Boilerplate
	friend class cMeshManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cMeshManager_Impl *impl() const;
	cMeshManager_Impl *impl();

public:
	static cMeshManager *instance();

	bool loadMeshFileIntoGLBuffer(std::string name, const char *path,
		float scale);
	void loadWorld();

	// Could be in global.. 
	std::vector<std::vector<triFace *>> meshFaces;
	std::vector<unsigned int> indices;
	std::vector<cMeshVertex> vertices;



	// Map... aka "dictionary" "look up table"
	std::map<std::string, cMeshEntry> m_MapMeshNameTocMeshEntry;
	std::map<glm::vec3, std::string> m_mapRGBToMeshName;
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
