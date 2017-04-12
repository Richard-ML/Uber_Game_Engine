#ifndef _cMeshManager_HG_
#define _cMeshManager_HG_
#include "stdafx.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	A mesh entry. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct cMeshEntry {
	int NumgIndices;
	int NumgVertices;
	int BaseIndex;
	int BaseVertex;
};
/* Mesh Information */

///-------------------------------------------------------------------------------------------------
/// <summary>	A triangle face. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct triFace {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	A mesh vertex. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct cMeshVertex {
	glm::vec4 Position;
	glm::vec4 TexCoord;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::uvec4 textureUnits; // TODO: Switch to uvec4 (GL_UNSIGNED_INT) or
							 // (GL_UNSIGNED_BYTE)
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Manager for meshes. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cMeshManager {
	static cMeshManager *s_cMeshManager;
	// Boilerplate
	friend class cMeshManager_Impl; // The PIMPL idiom aka Compilation Firewall

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a const cMeshManager_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	const cMeshManager_Impl *impl() const;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cMeshManager_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	cMeshManager_Impl *impl();

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cMeshManager. </returns>
	///-------------------------------------------------------------------------------------------------

	static cMeshManager *instance();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads mesh file into gl buffer. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="name"> 	The name. </param>
	/// <param name="path"> 	Full pathname of the file. </param>
	/// <param name="scale">	The scale. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	bool loadMeshFileIntoGLBuffer(std::string name, const char *path,
		float scale);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads fbx mesh file into gl buffer. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="name"> 	The name. </param>
	/// <param name="path"> 	Full pathname of the file. </param>
	/// <param name="scale">	The scale. </param>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	bool loadFBXMeshFileIntoGLBuffer(std::string name, const char *path, float scale);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads the world. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void loadWorld();

	// Could be in global.. 
	std::vector<std::vector<triFace *>> meshFaces;
	std::vector<unsigned int> indices;
	std::vector<cMeshVertex> vertices;

	std::map<std::string, sBoundingBox> m_MapMeshNameToAABB;
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
