#ifndef _cRenderManager_HG_
#define _cRenderManager_HG_
#include "stdafx.h"
// Frame buffer objects(FBOs) are not actually buffers, they are more like managers. They contain attachments(actual buffers) to read or write to
struct sFBOInfo {
public:
	std::string name;
	bool colorTexture, // Whether or not this FBO has a Texture at GL_COLOR_ATTACHMENT0
		depthBuffer,   // Whether or not this FBO has a depth attachment
		stencilBuffer; // Whether or not this FBO has a stencil attachment
	std::string description; // Short description/label for this FBO. Used in debugging
	GLuint frameBufferID; // ID of the FrameBuffer
	GLuint colorTextureID; // TODO: There could be more than one of these! 
	GLuint depthBufferID;
};

// Used primary for managing OpenGL FBOs (FrameBuffer Objects)
class cRenderManager {
	static cRenderManager *s_cRenderManager;

public:
	static cRenderManager *instance();
	std::map<std::string, sFBOInfo*> map_NameToFBOInfo;

	GLuint createFrameBufferObject(std::string name, int width, int height);
	bool renderSceneToFBO(std::string name);

	void renderTheSkybox();
	void renderScene();
	void bindTheBuffers();
private:
	cRenderManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cRenderManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cRenderManager(const cRenderManager &); // Disallow copy constructor
	cRenderManager &operator=(const cRenderManager &textureManager) {
	} // Disallow assignment operator
};



#endif