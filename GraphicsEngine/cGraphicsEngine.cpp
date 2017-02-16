#include "stdafx.h"
#include "global.h"
#include "cGraphicsEngine.h"
#include <ctime>
#include <chrono>
// Forward deceleration of local functions
void initializeGLFW();
void createTheBuffers();
void bindTheBuffers();
bool setupTheShader();
void renderSkybox();
void renderScene();

// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace GraphicsEngine {
	cGraphicsEngine *cGraphicsEngine::s_cGraphicsEngine =
		0; // Allocating pointer to static instance of cGraphicsEngine (singleton)
	std::vector<cGraphicsObject*> cGraphicsEngine::m_vec_pGraphicObjects;
	class cGraphicsEngine_Impl : public cGraphicsEngine {
		// Boilerplate
		friend class cGraphicsEngine;
	};
	inline const cGraphicsEngine_Impl *cGraphicsEngine::impl() const {
		return static_cast<const cGraphicsEngine_Impl *>(this);
	}
	inline cGraphicsEngine_Impl *cGraphicsEngine::impl() {
		return static_cast<cGraphicsEngine_Impl *>(this);
	}

	_declspec(dllexport) cGraphicsEngine * cGraphicsEngine::instance()
	{
		if (!s_cGraphicsEngine)
		{
			printf("Graphics Engine Initialized\n");
			s_cGraphicsEngine = new cGraphicsEngine();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&GraphicsEngine::cGraphicsEngine::graphicsThread, reinterpret_cast<void*>(s_cGraphicsEngine), 0, &myThreadID);
			// NOTE: Since the graphics context resides on this thread all gl related processes can not be called directly from outside of this engine

			// TODO: Set initial window title via external configuration file.. (.xml/.json)
			gWindowTitle = "Single-threaded GraphicsEngine Window!";

			//One time setup stuff goes here!!
			///////////////////////////////////////////////////////////////////////
			// Initialize GLFW
			initializeGLFW();
		}
		return s_cGraphicsEngine;
	}

	GraphicsEngine_API void cGraphicsEngine::loadCubemaps(rapidxml::xml_node<>* cubemapsNode)
	{
		for (rapidxml::xml_node<> *cubemap_node = cubemapsNode->first_node("Cubemap");
			cubemap_node; cubemap_node = cubemap_node->next_sibling()) {
			g_pTextureManager->loadCubeMap(cubemap_node);
		}
		return;
	}
	GraphicsEngine_API void cGraphicsEngine::loadMipmapTextures(rapidxml::xml_node<>* mipmapTexturesNode)
	{
		for (rapidxml::xml_node<> *mipmap_node = mipmapTexturesNode->first_node("MipmapTexture");
			mipmap_node; mipmap_node = mipmap_node->next_sibling()) {
			g_pTextureManager->loadTextureMipmap(mipmap_node);
		}
		return;
	}
	GraphicsEngine_API bool cGraphicsEngine::loadRenderableComponent(rapidxml::xml_node<>* componentNode, iState* state)
	{
		for (rapidxml::xml_node<> *cRenderableComponentEntry_node = componentNode->first_node("Mesh");
			cRenderableComponentEntry_node; cRenderableComponentEntry_node = cRenderableComponentEntry_node->next_sibling()) {
			// load the mesh buffers
			cGraphicsObject* graphicsObject = new cGraphicsObject();
			graphicsObject->meshName = cRenderableComponentEntry_node->first_attribute("name")->value(); // TODO: Offset scale rotation etc..
			graphicsObject->pState = state;
			m_vec_pGraphicObjects.push_back(graphicsObject);
		}

		// Create base object that contains iState*
		printf("Graphics object created!\n");

		return true;
	}
	GraphicsEngine_API bool cGraphicsEngine::loadMeshes(rapidxml::xml_node<> *meshesNode) {
		for (rapidxml::xml_node<> *cMeshEntry_node = meshesNode->first_node("Mesh");
			cMeshEntry_node; cMeshEntry_node = cMeshEntry_node->next_sibling()) {
			// load the mesh buffers
			bool success = false;
			std::string meshName = cMeshEntry_node->first_attribute("name")->value();

			//glm::vec3 rgb = glm::vec3(std::stof(cMeshEntry_node->first_attribute("r")->value()), std::stof(cMeshEntry_node->first_attribute("g")->value()), std::stof(cMeshEntry_node->first_attribute("b")->value()));
			//g_pMeshManager->m_mapRGBToMeshName[rgb] = meshName;
			success = g_pMeshManager->loadMeshFileIntoGLBuffer(
				meshName,
				cMeshEntry_node->first_attribute("path")->value(),
				std::stof(cMeshEntry_node->first_attribute("scale")->value()));

			if (!success) {
				std::cout << "We couldn't load a mesh. Please fix the XML File.."
					<< std::endl;
				system("pause");
			}
		}
		printf("Loaded meshes!\n");

		return true;
	}

	GraphicsEngine_API void cGraphicsEngine::update(float deltaTime)
	{
		// TODO: Can't initialize buffers until some mesh data exists. Fix this. See TAG: 001
		if (!buffersInitialized)
		{
			createTheBuffers();
			glGenVertexArrays(1, &gCubeVAO);
			glBindVertexArray(gCubeVAO);
			buffersInitialized = true;
		}
		// Do graphics stuff!
		glfwSetWindowTitle(gWindow, gWindowTitle.c_str());
		gCamera->update(deltaTime);
		glEnable(GL_TEXTURE_2D);
		// Set shader model. Does this really make a difference?
		glShadeModel(GL_SMOOTH);

		// Clear the screen..
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render Skybox
		renderSkybox();

		// Use primary rendering shader
		glUseProgram(gProgramID);
		glUniform1i(gUniformId_Toggle_Lights, g_bool_toggleLights);
		glUniform1i(gUniformId_Toggle_Textures, g_bool_toggleTextures);
		// Render objects
		renderScene();

		// Swap buffers..
		glfwSwapBuffers(gWindow);
		glfwPollEvents();

		return;
	}
}
void initializeGLFW() {
	gWindowTitle = "Single-threaded GraphicsEngine Window!";
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("pause");
	}

	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glEnable(GL_MULTISAMPLE);
	// Open a window and create its OpenGL context
	gWindow =
		glfwCreateWindow(mode->width, mode->height, gWindowTitle.c_str(), NULL, NULL);
	if (gWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, "
			"they are not 3.3 compatible.\n");
		glfwTerminate();
		system("pause");
	}
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		system("pause");
	}

	// Register Callback Functions
	glfwSetWindowSizeCallback(gWindow, callback_windowResize);
	glfwSetWindowCloseCallback(gWindow, callback_WindowClose);
	glfwSetKeyCallback(gWindow, callback_KeyPress);
	glfwSetScrollCallback(gWindow, scroll_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glfwPollEvents();

	//Load the shader
	if (!setupTheShader()) {
		std::cout << "Oh no! The shaders didn't load!!" << std::endl;
		system("pause");
	}
}

void createTheBuffers() {
	glGenVertexArrays(1, &gVertexBufferID);
	glBindVertexArray(gVertexBufferID);

	glGenBuffers(1, &gTexCoordBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);

	glGenBuffers(1, &gNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);

	glGenBuffers(1, &gTangentBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTangentBufferID);

	glGenBuffers(1, &gTextureInfoBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, gTextureInfoBufferID);

	int bytesInVertexArray = g_pMeshManager->vertices.size() * sizeof(cMeshVertex);
	glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &g_pMeshManager->vertices[0], // TAG: 001
		GL_STATIC_DRAW);

	glGenBuffers(1, &gIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

	int bytesInIndexArray = g_pMeshManager->indices.size() * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &g_pMeshManager->indices[0],//  &g_pMeshManager->indices[0]
		GL_STATIC_DRAW);

	bindTheBuffers();
}

void bindTheBuffers() {
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

bool setupTheShader() {
	//Create and compile our GLSL program from the shaders
	gProgramID = ::g_pShaderManager->loadShaders(
		"../GraphicsEngine/Simple.vs.glsl", "../GraphicsEngine/Simple.fs.glsl", "../GraphicsEngine/Simple.gs.glsl");

	if (!gProgramID) {
		std::cout << "Shaders failed to load!!" << std::endl;
		return false;
	}

	// per item
	// Uber shader
	gUniformId_ModelMatrix = glGetUniformLocation(gProgramID, "ModelMatrix");
	gUniformId_ModelMatrixOrientation =
		glGetUniformLocation(gProgramID, "ModelMatrixOrientation");
	gUniformId_Alpha = glGetUniformLocation(gProgramID, "Alpha");
	gUniformId_ModelColor = glGetUniformLocation(gProgramID, "ModelColor");
	gUniformId_NumLights = glGetUniformLocation(gProgramID, "NUM_LIGHTS");

	gUniformId_ViewMatrix = glGetUniformLocation(gProgramID, "ViewMatrix");
	gUniformId_PojectionMatrix =
		glGetUniformLocation(gProgramID, "ProjectionMatrix");
	gUniformId_EyePosition = glGetUniformLocation(gProgramID, "EyePosition");

	gUniformId_Toggle_Lights = glGetUniformLocation(gProgramID, "Toggle_Lights");
	//gUniformId_Toggle_Skybox = glGetUniformLocation(gProgramID, "Toggle_Skybox");
	gUniformId_Toggle_Textures =
		glGetUniformLocation(gProgramID, "Toggle_Textures");

	gUniformId_NumTextures = glGetUniformLocation(gProgramID, "NUM_TEXTURES");
	gUniformId_Texture0 = glGetUniformLocation(gProgramID, "Texture0");
	gUniformId_Texture1 = glGetUniformLocation(gProgramID, "Texture1");
	gUniformId_Texture2 = glGetUniformLocation(gProgramID, "Texture2");
	gUniformId_Texture3 = glGetUniformLocation(gProgramID, "Texture3");

	// Skybox Shader
	::gSkyboxShaderID =
		::g_pShaderManager->loadShaders("../GraphicsEngine/Skybox.vs.glsl", "../GraphicsEngine/Skybox.fs.glsl");
	::gUniformId_ToggleSkyboxTextures =
		glGetUniformLocation(gSkyboxShaderID, "Toggle_Skybox_Textures");
	gSkyboxVMID = glGetUniformLocation(gSkyboxShaderID, "view_matrix");

	gUniformId_SamplerCube = glGetUniformLocation(gSkyboxShaderID, "skybox");
	//glActiveTexture(gUniformId_SamplerCube);

	return true;
}

void renderSkybox() {
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
		glm::mat4 camViewInverse(viewMatrix[0][0], viewMatrix[1][0],
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

void renderScene() {
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

	for each(cGraphicsObject* graphicObject in GraphicsEngine::cGraphicsEngine::m_vec_pGraphicObjects)
	{
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
		glDrawElementsBaseVertex(
			GL_TRIANGLES, g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].NumgIndices, GL_UNSIGNED_INT,
			(void *)(sizeof(unsigned int) *  g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex),
			g_pMeshManager->m_MapMeshNameTocMeshEntry[graphicObject->meshName].BaseIndex);
	}
}