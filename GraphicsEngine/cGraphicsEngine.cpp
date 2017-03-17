#include "stdafx.h"
#include "global.h"
#include "cGraphicsEngine.h"
#include <ctime>
#include <chrono>
// Forward deceleration of local functions
void initializeGLFW();
void createTheBuffers();
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace GraphicsEngine {
	cGraphicsEngine *cGraphicsEngine::s_cGraphicsEngine =
		0; // Allocating pointer to static instance of cGraphicsEngine (singleton)

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
			// NOTE: Since the graphics context resides on this thread all gl related processes can not be called directly from another thread

			// TODO: Set initial window title via external configuration file.. (.xml/.json)
			gWindowTitle = "Uber Game Engine";

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
	GraphicsEngine_API void cGraphicsEngine::loadTextures(rapidxml::xml_node<>* texturesNode)
	{
		for (rapidxml::xml_node<> *texture_node = texturesNode->first_node("Texture");
			texture_node; texture_node = texture_node->next_sibling()) {
			g_pTextureManager->loadTexture(texture_node);
		}
		return;
	}
	GraphicsEngine_API bool cGraphicsEngine::loadRenderableComponent(rapidxml::xml_node<>* componentNode, iState* state)
	{
		for (rapidxml::xml_node<> *cRenderableComponentEntry_node = componentNode->first_node("Mesh");
			cRenderableComponentEntry_node; cRenderableComponentEntry_node = cRenderableComponentEntry_node->next_sibling("Mesh")) {
			// load the mesh buffers
			cGraphicsObject* graphicsObject = new cGraphicsObject();
			graphicsObject->meshName = cRenderableComponentEntry_node->first_attribute("name")->value(); // TODO: Offset scale rotation etc..
			graphicsObject->pState = state;
			graphicsObject->pState->setScale(1.0f);
			graphicsObject->toggleOutline = std::string(cRenderableComponentEntry_node->first_attribute("outline")->value()) == "true";
			g_vec_pGraphicObjects.push_back(graphicsObject);

		}
		for (rapidxml::xml_node<> *cRenderableComponentEntry_node = componentNode->first_node("Light");
			cRenderableComponentEntry_node; cRenderableComponentEntry_node = cRenderableComponentEntry_node->next_sibling("Light")) {
			// load the mesh buffers
			g_pLightManager->loadLightFromXML(cRenderableComponentEntry_node);
			g_pLightManager->vecLights.back()->state = state;
		}

		for (rapidxml::xml_node<> *cRenderableComponentEntry_node = componentNode->first_node("PlayerControlComponent");
			cRenderableComponentEntry_node; cRenderableComponentEntry_node = cRenderableComponentEntry_node->next_sibling("PlayerControlComponent")) {
			cPlayerControlComponent controlComponent;
			controlComponent.pState = state;
			g_vec_playerControlComponents.push_back(controlComponent);
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

	GraphicsEngine_API bool cGraphicsEngine::loadFramebufferObjects(rapidxml::xml_node<>* framebuffersNode)
	{
		for (rapidxml::xml_node<> *cFBO_node = framebuffersNode->first_node("FrameBufferObject");
			cFBO_node; cFBO_node = cFBO_node->next_sibling()) {
			bool isMultiSampled = std::string(cFBO_node->first_attribute("isMultiSampled")->value()) == "true";
			g_pRenderManager->createFrameBufferObject(cFBO_node->first_attribute("name")->value(),
				gWindowWidth, gWindowHeight, isMultiSampled);
				//std::stoi(cFBO_node->first_attribute("width")->value()), 
				//std::stoi(cFBO_node->first_attribute("height")->value()), isMultiSampled) ;
		}


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
		deltaTime = deltaTime * 1000.0f;

		/////////////////////////////////////////////////////////////////////////////////////////
		// TODO: Create input manager that gets updated each tick //////////////////////////////
		// TAG: 001X
		bool pressW = glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS;
		bool pressS = glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS;
		bool pressA = glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS;
		bool pressD = glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS;
		if (pressW != pressS)
		{
			// TODO: Add velocity based on delta time. 
			glm::vec3 translation = glm::vec3(0.0f, 0.0f, 1.28f);
			if (pressS)
				translation *= -1.0f;
			for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
			{
				glm::mat4 tempTrans = glm::translate(controlComponent.pState->getTransform(), translation);
				// PROJECT CODE: Position restriction
				//tempTrans[3] = glm::clamp(tempTrans[3], glm::vec4(-1000.0f, 0.0f, -1000.0f, 0.0f), glm::vec4(1000.0f, 1.0f, -5.0f, 1.0f));
				controlComponent.pState->setTransform(tempTrans);
				gCamera->setTargetTransform(tempTrans);
			}
		}

		if (pressA != pressD) {
			glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f);
			if (pressD)
				axis *= -1.0f;
			// TODO: Add turn velocity based on delta time. 
			glm::mat4 rotMatrix = glm::rotate(glm::mat4(), 0.157f, axis);

			for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
			{
				glm::mat4 tempTrans = controlComponent.pState->getTransform();
				tempTrans *= rotMatrix;
				controlComponent.pState->setTransform(tempTrans);
				gCamera->setTargetTransform(controlComponent.pState->getTransform());
			}

		}

		// TODO: Fix this so that it does not just set the camera target transform to the last player control component in the vector
		gCamera->setTargetTransform(g_vec_playerControlComponents.back().pState->getTransform());
		/////////////////////////////////////////////////////////////////////////////////////////

		// Do graphics stuff!
		glfwSetWindowTitle(gWindow, gWindowTitle.c_str());
		gCamera->update(deltaTime);
		glEnable(GL_TEXTURE_2D);
		// Set shader model. Does this really make a difference?
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);

		// Clear the screen..
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Use primary rendering shader
		glUseProgram(gProgramID);
		glUniform1i(gUniformId_Toggle_Lights, g_bool_toggleLights);
		glUniform1i(gUniformId_Toggle_Textures, g_bool_toggleTextures);

		if (g_bool_toggleLights)
			g_pLightManager->updateLightUniforms();
		
		// Render Scene to FBO
		g_pRenderManager->renderSceneToFBO("Portal");
		
		// Render objects
		g_pRenderManager->renderScene();

		// Swap buffers..
		glfwSwapBuffers(gWindow);
		glfwPollEvents();

		return;
	}
	GraphicsEngine_API void cGraphicsEngine::initializeGameStateHandle(iGameState * pGameState)
	{
		g_pGameState = pGameState;
	}
}
void initializeGLFW() {
	gWindowTitle = "Single-threaded GraphicsEngine Window! The invisible eye watches.. WASD to move & Arrows control camera rotation around player";
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("pause");
	}
	//glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 16);
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

	
	// Open a window and create its OpenGL context
	
	gWindowHeight = mode->height;
	gWindowWidth = mode->width;

	gWindow =
		glfwCreateWindow(mode->width, mode->height, gWindowTitle.c_str(), NULL, NULL);
	if (gWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. Intel GPUs are not 3.3 compatible.\n");
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
	if (!g_pShaderManager->setupTheShaders()) {
		std::cout << "Oh no! The shaders didn't load!!" << std::endl;
		system("pause");
	}

	printf("Information:\n");
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("\n");
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

  g_pRenderManager->bindTheBuffers();
}

