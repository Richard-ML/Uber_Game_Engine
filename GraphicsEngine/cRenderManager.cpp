#include "stdafx.h"
#include "cRenderManager.h"
#include "global.h"
cRenderManager * cRenderManager::instance()
{
	if (!s_cRenderManager)
		s_cRenderManager = new cRenderManager();
	return s_cRenderManager;
}
void drawFullScreenPlane()
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	static glm::mat4 transform;
	//transform = glm::rotate(glm::mat4(), 3.14f, glm::vec3(1.0f, 0.0f, 0.0f));
	g_pRenderManager->bindTheBuffers();
	gCamera->getProjectionMatrix(projectionMatrix);
	glUseProgram(gProgramID);
	viewMatrix = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	transform[3].z = 1.0f;
	glCullFace(GL_FRONT); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
									 // you can pass here
		GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL

	glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
		glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
		glm::value_ptr(viewMatrix));
	glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(glm::vec4(0.0f)));

	glUniformMatrix4fv(
		gUniformId_ModelMatrix, 1, GL_FALSE,
		glm::value_ptr(glm::scale(transform, glm::vec3(0.55f))));
	glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
		glm::value_ptr(glm::mat4()));
	glUniform4fv(gUniformId_ModelColor, 1,
		glm::value_ptr(glm::vec4(1.0f)));

	glUniform1f(gUniformId_Alpha, 1.0f);

	glDrawElementsBaseVertex(
		GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry["Plane"].NumgIndices, GL_UNSIGNED_INT,
		(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry["Plane"].BaseIndex),
		g_pMeshManager->m_MapMeshNameTocMeshEntry["Plane"].BaseIndex);


	glEnable(GL_DEPTH_TEST);
}
GLuint cRenderManager::createFrameBufferObject(std::string name, int width, int height, bool multisampled)
{
	iFBOInfo* pFBOInfo;
	if (!multisampled)
		pFBOInfo = new cFBOInfo();
	else
		pFBOInfo = new cMSFBOInfo();

	pFBOInfo->name = name;
	pFBOInfo->width = width;
	pFBOInfo->height = height;

	pFBOInfo->createFrameBuffer();
	
	// Add FBO information to the map! 
	map_NameToFBOInfo[name] = pFBOInfo;

	return pFBOInfo->framebufferID;
}

void cRenderManager::renderTheSkybox() {
	//////////////////////////////////////////////////////////
	//                  Render The Skybox                  //
	/////////////////////////////////////////////////////////
	if (gToggle_Skybox) {
		///////////////////////
		// TODO: Day/Night? :P
		static const GLfloat gray[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const GLfloat ones[] = { 1.0f };
		glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
										 // you can pass here
			GL_FILL);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glm::mat4 viewMatrix;
		gCamera->getViewMatrix(viewMatrix);
		glm::vec3 eyePos = gCamera->getEyePosition();
		// ;)
		glm::mat4 camViewInverse(
			viewMatrix[0][0], viewMatrix[1][0],
			viewMatrix[2][0], 0.0f, viewMatrix[0][1],
			viewMatrix[1][1], viewMatrix[2][1], 0.0f,
			viewMatrix[0][2], viewMatrix[1][2],
			viewMatrix[2][2], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

		camViewInverse = glm::inverse(camViewInverse);

		glClearBufferfv(GL_COLOR, 0, gray);
		glClearBufferfv(GL_DEPTH, 0, ones);
		glUseProgram(gSkyboxShaderID);

		glUniform1i(gUniformId_ToggleSkyboxTextures, gToggle_Skybox);

		glUniform1iARB(gUniformId_SamplerCube, g_pTextureManager->mapTextureNameToID["Skybox"]);
		glUniformMatrix4fv(gSkyboxVMID, 1, GL_FALSE,
			glm::value_ptr(camViewInverse));

		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glEnable(GL_DEPTH_TEST);
	}
}

void cRenderManager::renderScene()
{
	// Render Skybox
	renderTheSkybox();

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	gCamera->getProjectionMatrix(projectionMatrix);
	gCamera->getViewMatrix(viewMatrix);

	bindTheBuffers();
	glUseProgram(gProgramID);




		glEnable(GL_DEPTH_TEST);
		for each(cGraphicsObject* graphicObject in g_vec_pGraphicObjects)
		{
			// per frame uniforms
			glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
				glm::value_ptr(projectionMatrix));
			glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
				glm::value_ptr(viewMatrix));
			glm::vec4 eye4;
			gCamera->getEyePosition(eye4);
			glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));
			glEnable(GL_MULTISAMPLE);
			glEnable(GL_BLEND);
			// glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);



			if (graphicObject->toggleOutline) {
				glEnable(GL_DEPTH_TEST);
				glClearStencil(0);
				glClear(GL_STENCIL_BUFFER_BIT);
				// Render the mesh into the stencil buffer.
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}

			// glEnable(GL_COLOR_MATERIAL);
			//if ((*iter)->isWireframe) {        // Turn off backface culling
			//								   // Enable "wireframe" polygon mode
			//	glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
			//									 // you can pass here
			//		GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
			//	glDisable(GL_CULL_FACE);
			//}
			//else { // "Regular" rendering:
			// Turn on backface culling
			// Turn polygon mode to solid (Fill)
			glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
											 // you can pass here
				GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL
								   //}

			glm::mat4 transform;
			graphicObject->pState->getTransform(transform);
			float scale;
			graphicObject->pState->getScale(scale);

			// TODO: Scale in object..
			glUniformMatrix4fv(
				gUniformId_ModelMatrix, 1, GL_FALSE,
				glm::value_ptr(glm::scale(transform, glm::vec3(scale))));
			glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
				glm::value_ptr(glm::mat4()));
			glUniform4fv(gUniformId_ModelColor, 1,
				glm::value_ptr(glm::vec4(1.0f)));

			glUniform1f(gUniformId_Alpha, 1.0f);
			if (graphicObject->meshName == "Portal")
			{
				glBindTexture(GL_TEXTURE_2D, g_pRenderManager->map_NameToFBOInfo["Portal"]->colorTextureID);
				glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, false); // TODO: Add global boolean toggle

				glDrawElementsBaseVertex(
					GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].NumgIndices, GL_UNSIGNED_INT,
					(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex),
					g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex);
				
				glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);
				glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, true);
			}
			else {
				glDrawElementsBaseVertex(
					GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].NumgIndices, GL_UNSIGNED_INT,
					(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex),
					g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex);
			}

			if (graphicObject->toggleOutline)
			{
				glUniform1i(gUniformId_Toggle_Lights, false);
				// Render the wireframe to create outline effect.
				glCullFace(GL_FRONT); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
				glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
												 // you can pass here
					GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL

				glStencilFunc(GL_NOTEQUAL, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


				transform[3] *= 0.995f;
				glUniform1i(gUniformId_Toggle_Textures, 0);
				glUniformMatrix4fv(
					gUniformId_ModelMatrix, 1, GL_FALSE,
					glm::value_ptr(glm::scale(transform, glm::vec3(scale))));
				glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
					glm::value_ptr(glm::mat4()));
				glUniform4fv(gUniformId_ModelColor, 1,
					glm::value_ptr(glm::vec4(0.75f, 0.0f, 0.0f, 0.6f)));

				glUniform1f(gUniformId_Alpha, 0.75f);

				glDrawElementsBaseVertex(
					GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].NumgIndices, GL_UNSIGNED_INT,
					(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex),
					g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex);

				glUniform1i(gUniformId_Toggle_Textures, 1);
				glUniform1i(gUniformId_Toggle_Lights, true);
			}

	}

	if (g_bool_toggleDebugShapes) {
		glUniform1i(gUniformId_Toggle_Textures, false);
		glUniform1i(gUniformId_Toggle_Lights, false);

		glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
			glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
			glm::value_ptr(viewMatrix));
		glm::vec4 eye4;
		gCamera->getEyePosition(eye4);
		glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		// glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		// Enable "wireframe" polygon mode
		glPolygonMode(GL_FRONT_AND_BACK,
			GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
		glDisable(GL_CULL_FACE);

		glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
										 // you can pass here
			GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL

		g_pDebugRenderer->lock(0);
		for each(cDebugRenderer::sDebugShape shape in g_pDebugRenderer->m_vec_debugShapes) {

			glm::mat4 transform;

			transform[3] = glm::vec4(shape.position, 1.0f);

			glUniformMatrix4fv(
				gUniformId_ModelMatrix, 1, GL_FALSE,
				glm::value_ptr(glm::scale(transform, shape.scale)));
			glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
				glm::value_ptr(glm::mat4()));
			glUniform4fv(gUniformId_ModelColor, 1,
				glm::value_ptr(shape.color));
			glUniform1f(gUniformId_Alpha, 0.65f);


			glDrawElementsBaseVertex(
				GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].NumgIndices, GL_UNSIGNED_INT,
				(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseIndex),
				g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseIndex);
		}

		glUniform1i(gUniformId_Toggle_Textures, 1);
		glUniform1i(gUniformId_Toggle_Lights, true);
		g_pDebugRenderer->unlock(0);
	}

}
void cRenderManager::bindTheBuffers()
{
	glBindVertexArray(gVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTextureInfoBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // TexCoord
	glEnableVertexAttribArray(2); // Normal
	glEnableVertexAttribArray(3); // Texture DiffuseID
	glEnableVertexAttribArray(4); // Texture DiffuseID

	glVertexAttribPointer(
		0,        // index or "slot" in the shader
		4,        // Number of variables: vec4 would be 4 32-bit variables
		GL_FLOAT, // Type: vec4 is float
		GL_FALSE, // "normalize" the values (or not)
		sizeof(cMeshVertex), // Number of bytes per vertex (the "stride")
		(GLvoid *)0); // Offset from vertex (position is first, so offset = 0

	int offsetToTexCoordsInBytes = sizeof(((cMeshVertex *)0)->Position);
	glVertexAttribPointer(
		1, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
		(GLvoid *)offsetToTexCoordsInBytes); // Offset in bytes to TexCoords

	int offsetToNormalInBytes =
		offsetToTexCoordsInBytes + sizeof(((cMeshVertex *)0)->TexCoord);
	glVertexAttribPointer(
		2, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
		(GLvoid *)offsetToNormalInBytes); // Offset in bytes to Normal

	int offsetToTangentInBytes =
		offsetToNormalInBytes + sizeof(((cMeshVertex *)0)->Normal);
	glVertexAttribPointer(
		3, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
		(GLvoid *)offsetToTangentInBytes); // Offset in bytes to Tangent

	int offsetToTextureInfoInBytes =
		offsetToTangentInBytes + sizeof(((cMeshVertex *)0)->Tangent);
	glVertexAttribIPointer(
		4, 4, GL_UNSIGNED_INT, sizeof(cMeshVertex),
		(GLvoid *)offsetToTextureInfoInBytes); // Offset in bytes to Texture Info
}


bool cRenderManager::renderSceneToFBO(std::string name)
{
	iFBOInfo * fboInfo = map_NameToFBOInfo[name];
	if (fboInfo == nullptr)
		return false;

	glm::mat4 view = gCamera->m_viewMatrix;

	// PROJECT CODE: different viewpoint.
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 targetPos = g_vec_playerControlComponents.at(0).pState->getPosition();
	float direction = 1.0f;
	if (targetPos.z < 0)
		direction = -1.0f;
	glm::vec3 camPos = glm::vec3(0.0f, 10.0f, direction*3.0f);
	
	targetPos.y += 8.0f;
	glm::vec3 forward = glm::normalize(targetPos - camPos);
	up = glm::vec3(glm::vec4(up, 0.0f));
	glm::vec3 right = glm::cross(forward, up);
	gCamera->m_viewMatrix = glm::lookAt(camPos, targetPos, up);
	/////////////////////////////////////////////

	fboInfo->renderSceneToFBO();
	gCamera->m_viewMatrix = view;
	
	return true;
}

void cFBOInfo::renderSceneToFBO()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebufferID);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec3(0.0f, 120.0f, 0.0f)));
	glClearBufferfv(GL_DEPTH, 0, glm::value_ptr(glm::vec3(0.0f, 120.0f, 0.0f)));
	static const  GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_COMPONENT };
	glDrawBuffers(1, attachments);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	g_pRenderManager->renderScene();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cFBOInfo::createFrameBuffer()
{
	// Create a FBO then bind it..
	glCreateFramebuffers(1, &this->framebufferID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebufferID);

	// Create a color buffer texture for our new FBO
	glGenTextures(1, &this->colorTextureID);
	glBindTexture(GL_TEXTURE_2D, this->colorTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		width,
		height,
		0, GL_RGBA, GL_UNSIGNED_BYTE,
		NULL);

	// Create a depth buffer texture for out new FBO
	glGenTextures(1, &this->depthBufferID);
	glBindTexture(GL_TEXTURE_2D, this->depthBufferID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width,
		height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);

	// Attach color and depth textures to the FBO
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->colorTextureID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthBufferID, 0);

	// Draw to the framebuffers one and only color attachment.
	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);
}

void cMSFBOInfo::renderSceneToFBO()
{
	glViewport(0.0f, 0.0f, this->width, this->height);
	// Draw scene to multi-sampled buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->msFramebufferID);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if (g_bool_toggleScissor) {
		glEnable(GL_SCISSOR_TEST);
		glScissor((float)gWindowWidth / 5.5f, (float)gWindowHeight / 8.5f,
			(float)gWindowWidth / 1.55f, (float)gWindowHeight / 1.2f);
	}
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	static const  GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
	glDrawBuffers(1, attachments);
	if (g_bool_toggleStencil) {
		glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, false);
		glEnable(GL_DEPTH_TEST);

		// Render the mesh into the stencil buffer.
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, g_pTextureManager->mapTextureNameToID["Eye"]);
		drawFullScreenPlane();

		glStencilFunc(GL_GREATER, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glUniform1i(gUniformId_Toggle_NormalAndSpecularMaps, true);
	}

	glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);



	g_pRenderManager->renderScene();

	// Blit the multi-sampled buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->msFramebufferID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->framebufferID);
	glBlitFramebuffer(0, 0, this->width, this->height, 0, 0,
		this->width, this->height, GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); 
	//glDisable(GL_DEPTH_TEST);

	glViewport(0.0f, 0.0f, gWindowWidth, gWindowHeight);

	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_STENCIL_TEST);
}


void cMSFBOInfo::createFrameBuffer()
{
	// Create MS FBO
	glGenFramebuffers(1, &this->msFramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->msFramebufferID);
	// Generate a multi-sampled texture for the multi-sampled framebuffer
	glGenTextures(1, &this->msColorTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->msColorTexture);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, // TODO: Make number of samples non-hardcoded
		GL_RGBA, this->width, this->height, GL_TRUE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->msColorTexture, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0); // Unbind PROBLEM HERE?
	// Create a render buffer for stencil and depth attachments
	GLuint renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, this->width, this->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("ERROR: Framebuffer invalid!\n");
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind

	// TODO: Create other attachments. Depth/Stencil
	GLenum bufferAttachment = GL_RGBA;
	// Create a color buffer texture for the intermediate buffer
	glGenTextures(1, &this->colorTextureID);
	glBindTexture(GL_TEXTURE_2D, this->colorTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, bufferAttachment,
		width,
		height,
		0, bufferAttachment, GL_UNSIGNED_BYTE,
		NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Could be removed..
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // ^

	// Create a depth buffer texture for out new FBO
	glGenTextures(1, &this->depthBufferID);
	glBindTexture(GL_TEXTURE_2D, this->depthBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		width,
		height,
		0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Create intermediate FBO then bind it..
	glGenFramebuffers(1, &this->framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebufferID);
	// Attach color and depth textures to the FBO
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->colorTextureID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthBufferID, 0);
	// Draw to the framebuffers one and only color attachment.
	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };// remove this
	glDrawBuffers(1, draw_buffers);

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (e) {

	case GL_FRAMEBUFFER_UNDEFINED:
		printf("ERROR: FBO Undefined\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("ERROR: FBO Incomplete Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("ERROR: FBO Missing Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("ERROR: FBO Incomplete Draw Buffer\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("ERROR: FBO Unsupported\n");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		printf("FBO VALID\n");
		break;
	default:
		printf("ERROR: Framebuffer invalid!\n");
	}
		

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind
	glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);

}
