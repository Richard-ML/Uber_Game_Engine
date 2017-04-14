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
	std::vector<std::vector<sTriangleFace *>> meshFaces;
	std::vector<unsigned int> indices;
	std::vector<sMeshVertex> vertices;

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
