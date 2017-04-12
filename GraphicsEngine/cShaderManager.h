#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_
#include "stdafx.h"
class cShaderManager {
	static cShaderManager *s_cShaderManager;

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cShaderManager. </returns>
	///-------------------------------------------------------------------------------------------------

	static cShaderManager *instance();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads the shaders. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="vertex_file_path">  	Full pathname of the vertex file. </param>
	/// <param name="fragment_file_path">	Full pathname of the fragment file. </param>
	///
	/// <returns>	The shaders. </returns>
	///-------------------------------------------------------------------------------------------------

	GLuint loadShaders(const char *vertex_file_path,
		const char *fragment_file_path);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads the shaders. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="vertex_file_path">  	Full pathname of the vertex file. </param>
	/// <param name="fragment_file_path">	Full pathname of the fragment file. </param>
	/// <param name="geometry_file_path">	Full pathname of the geometry file. </param>
	///-------------------------------------------------------------------------------------------------

	GLuint loadShaders(const char *vertex_file_path,
		const char *fragment_file_path,
		const char *geometry_file_path);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets up the shaders. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------

	bool setupTheShaders();

private:
	cShaderManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cShaderManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cShaderManager(const cShaderManager &); // Disallow copy constructor
	cShaderManager &operator=(const cShaderManager &shaderManager) {
	} // Disallow assignment operator
};

#endif