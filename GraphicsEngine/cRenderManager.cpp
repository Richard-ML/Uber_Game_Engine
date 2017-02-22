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
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	gCamera->getProjectionMatrix(projectionMatrix);
	gCamera->getViewMatrix(viewMatrix);

	bindTheBuffers();
	glUseProgram(gProgramID);


		for (int ncW = 0; ncW < 128; ncW++)
		{
			for (int ncD = 0; ncD < 128; ncD++)
			{
				float offsetX = (-1024.0f + 12.79f * (float)ncW);
				float offsetZ = (-1024.0f + 12.79f * (float)ncD);

				// per frame uniforms
				glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
					glm::value_ptr(projectionMatrix));
				glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
					glm::value_ptr(viewMatrix));


				glm::vec4 eye4;
				gCamera->getEyePosition(eye4);
				glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));

				glEnable(GL_BLEND);
				// glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_DEPTH_TEST);

				glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
				glEnable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
												 // you can pass here
					GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL

				glm::mat4 transform;
				transform[3].x = offsetX;
				transform[3].z = offsetZ;

				// TODO: Scale in object..
				glUniformMatrix4fv(
					gUniformId_ModelMatrix, 1, GL_FALSE,
					glm::value_ptr(glm::scale(transform, glm::vec3(1.0f))));
				glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
					glm::value_ptr(glm::mat4()));
				glUniform4fv(gUniformId_ModelColor, 1,
					glm::value_ptr(glm::vec4(1.0f)));

				glUniform1f(gUniformId_Alpha, 1.0f);
				glDrawElementsBaseVertex(
					GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry["GrassTile"].NumgIndices, GL_UNSIGNED_INT,
					(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry["GrassTile"].BaseIndex),
					g_pMeshManager->m_MapMeshNameTocMeshEntry["GrassTile"].BaseIndex);
			}
		}

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
			graphicObject->pState->setScale(1.0f);
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
	sFBOInfo * fboInfo = map_NameToFBOInfo[name];
	if (fboInfo == nullptr)
		return false;


	//TODO: Remove this test code TAG: 1003
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, g_pRenderManager->map_NameToFBOInfo["Portal"]->frameBufferID);
	
	glm::mat4 view = gCamera->mViewMatrix;
	
	








	// do this only when portal created?
	glm::vec3 portal_view_anormal = glm::normalize((glm::vec3(gCamera->mViewMatrix[3]) -glm::vec3(10.0f,0.0f,10.0f)));
	//Vector3DF portal_cam_normal = (portal_cam_to[i] - portal_cam_from[i]).Normalize();
	
	glm::vec3 portal_cam_normal = glm::normalize((glm::vec3(10.0f, 0.0f, 15.0f) - glm::vec3(10.0f, 0.0f, 10.0f)));
	float portal_angle = glm::degrees( glm::acos(glm::dot(portal_view_anormal, portal_cam_normal)));

	//	printf("angle between view and cam: %f\n", portal_angle);

	glm::vec3 portal_cross = glm::cross(portal_cam_normal, portal_view_anormal);

	//glTranslatef(portal_view_from[i].x, 0, portal_view_from[i].z);
	gCamera->mViewMatrix[3] = glm::vec4(-glm::vec3(10.0f, 0.0f, 10.0f), 1.0f);

	//glRotatef(portal_angle, portal_cross.x, portal_cross.y, portal_cross.z);
	gCamera->mViewMatrix = glm::rotate(gCamera->mViewMatrix, -portal_angle, portal_cross);

	// go to portal cam location
	//glTranslatef(-portal_cam_from[i].x, 0, -portal_cam_from[i].z);


	//glTranslatef(-portal_view_from[other(i)].x, 0, -portal_view_from[other(i)].z);






































	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glClearBufferfv(GL_COLOR, 0, glm::value_ptr(glm::vec3(0.0f, 120.0f, 0.0f)));
	glClearBufferfv(GL_DEPTH, 0, glm::value_ptr(glm::vec3(0.0f, 120.0f, 0.0f)));
	static const  GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLenum e = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	switch (e) {

	case GL_FRAMEBUFFER_UNDEFINED:
		printf("FBO Undefined\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf("FBO Incomplete Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf("FBO Missing Attachment\n");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf("FBO Incomplete Draw Buffer\n");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf("FBO Unsupported\n");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		printf("FBO OK\n");
		break;
	default:
		printf("FBO Problem?\n");
	}
	
	renderScene();

	// After

	glViewport(0, 0, gWindowWidth, gWindowHeight);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	gCamera->mViewMatrix = view;

	return true;
}


