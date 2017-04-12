#ifndef _cRenderManager_HG_
#define _cRenderManager_HG_
#include "stdafx.h"






// TODO: Move FBO stuff to its own header file..

///-------------------------------------------------------------------------------------------------
/// <summary>	Information about the fbo. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class iFBOInfo {
public:
	std::string name;
	bool colorTexture, // Whether or not this FBO has a Texture at GL_COLOR_ATTACHMENT0
		depthBuffer,   // Whether or not this FBO has a depth attachment
		stencilBuffer; // Whether or not this FBO has a stencil attachment
	std::string description; // Short description/label for this FBO. Used in debugging
	GLuint framebufferID; // ID of the Framebuffer
	GLuint colorTextureID; // TODO: There could be more than one of these! 
	GLuint depthBufferID;
	GLuint stencilBufferID;
	int width, height;
	virtual void renderSceneToFBO() = 0;
	virtual void createFrameBuffer() = 0;
};


// Frame buffer objects(FBOs) are not actually buffers, they are more like managers. They contain attachments(actual buffers) to read or write to..

/* Standard 2D framebuffer object information.
* Serves as an intermediate framebuffer for multi-sampled framebuffers.
* This is necessary because the shader can only interpret 2D samplers as
* textures.
*/

class cFBOInfo : public iFBOInfo {
public:

	virtual void renderSceneToFBO() override;
	virtual void createFrameBuffer() override;
};

/* The multi-sampled FBO has an additional framebuffer.
* One framebuffer for drawing in multi-sampled context, and another for reading from in the standard texture2D sampler format.
* NOTE: The shader can not sample from a multi-sampled texture. Therefore the draw process requires an extra step.
*       Since multi-sampled images contain more information than a normal 2D sample we must resolve the multi-sampled texture
*       via glBlitFramebuffer which copies a region from one frameuffer into another resolving the multi-sampled texture.
*/
class cMSFBOInfo : public iFBOInfo {
public:
	// Multi-sampled color buffer
	GLuint msFramebufferID;
	GLuint msColorTexture;
	GLuint msRenderbufferID;
	virtual void renderSceneToFBO() override;
	virtual void createFrameBuffer() override;
};


// Used primary for managing OpenGL FBOs (FrameBuffer Objects)
class cRenderManager {
	/// <summary>	Manager for render. </summary>
	static cRenderManager *s_cRenderManager;

public:

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the instance. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cRenderManager. </returns>
	///-------------------------------------------------------------------------------------------------

	static cRenderManager *instance();
	std::map<std::string, iFBOInfo*> map_NameToFBOInfo;

	GLuint createFrameBufferObject(std::string name, int width, int height, bool multisampled);
	bool renderSceneToFBO(std::string name);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Renders the skybox. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void renderTheSkybox();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Renders the scene. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void renderScene();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Bind the buffers. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

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