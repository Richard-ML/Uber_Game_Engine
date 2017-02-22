#ifndef _cShaderManager_HG_
#define _cShaderManager_HG_
#include "stdafx.h"
class cShaderManager {
	static cShaderManager *s_cShaderManager;

public:
	static cShaderManager *instance();

	GLuint loadShaders(const char *vertex_file_path,
		const char *fragment_file_path);
	GLuint loadShaders(const char *vertex_file_path,
		const char *fragment_file_path,
		const char *geometry_file_path);

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