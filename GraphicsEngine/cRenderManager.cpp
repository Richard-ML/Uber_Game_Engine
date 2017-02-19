#include "stdafx.h"
#include "cRenderManager.h"
#include "global.h"
cRenderManager * cRenderManager::instance()
{
	if (!s_cRenderManager)
		s_cRenderManager = new cRenderManager();
	return s_cRenderManager;
}

GLuint cRenderManager::createFrameBufferObject(std::string name, int width, int height)
{
	sFBOInfo* pFBOInfo = new sFBOInfo();
	pFBOInfo->name = name;

	// Create a FBO then bind it..
	glCreateFramebuffers(1, &pFBOInfo->frameBufferID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pFBOInfo->frameBufferID);
	
	// Create a color buffer texture for our new FBO
	glGenTextures(1, &pFBOInfo->colorTextureID);
	glBindTexture(GL_TEXTURE_2D, pFBOInfo->colorTextureID);
	//glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		width,
		height,
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		NULL);

	// Create a depth buffer texture for out new FBO
	glGenTextures(1, &pFBOInfo->depthBufferID);
	glBindTexture(GL_TEXTURE_2D, pFBOInfo->depthBufferID);
	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, width, height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width,
		height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);


	
	// Attach color and depth textures to the FBO
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pFBOInfo->colorTextureID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pFBOInfo->depthBufferID, 0);
	
	// Draw to the framebuffers one and only color attachment.
	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);
	
	// Add FBO information to the map! 
	map_NameToFBOInfo[name] = pFBOInfo;

	// TODO: Something better than this... 
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // Return to initial frame buffer (default one)
	///glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0); // Bind the original texture that contains all of the textures in a mipmap
	return pFBOInfo->frameBufferID;
}

//bool cRenderManager::renderSceneToFBO(std::string name)
//{
//	sFBOInfo * fboInfo = map_NameToFBOInfo[name];
//	if (fboInfo == nullptr)
//		return false;
//
//
//
//	return true;
//}


