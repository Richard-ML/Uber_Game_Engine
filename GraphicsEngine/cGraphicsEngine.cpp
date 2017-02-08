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
			gWindowTitle = "Multi-threaded GraphicsEngine Window!";



			//One time setup stuff goes here!! 
			///////////////////////////////////////////////////////////////////////
			// Initialize GLFW
			initializeGLFW();
			//createTheBuffers();
			setupTheShader();

			// Generate a name for the texture
			glGenTextures(1, &g_pTextureManager->uniformId_Texture0);

			// Now bind it to the context using the GL_TEXTURE_2D binding point
			glBindTexture(GL_TEXTURE_2D, g_pTextureManager->uniformId_Texture0);

			// Specify the amount of storage we want to use for the texture
			glTexStorage2D(GL_TEXTURE_2D, // 2D texture
				1,             // 8 mipmap levels
				GL_RGBA32F,    // 32-bit floating-point RGBA data
				7680, 7680);   // 512 x 512 texels
			//////////////////////////////////////////////////////////////////////


		}
		return s_cGraphicsEngine;
	}

	GraphicsEngine_API void cGraphicsEngine::loadCubeMap(rapidxml::xml_node<>* cubeNode)
	{
		g_pTextureManager->loadCubeMap(cubeNode);
		return;
	}
	GraphicsEngine_API void cGraphicsEngine::update(float deltaTime)
	{
		// Do graphics stuff!
		printf("Graphics did stuff!\n");


		glEnable(GL_TEXTURE_2D);
		// Set shader model. Does this really make a difference?
		glShadeModel(GL_SMOOTH);

		// Clear the screen..
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gSkyboxShaderID);
		// Render Skybox
		renderSkybox();

		// Use primary rendering shader
		//glUseProgram(gProgramID);

		// Render objects
		//renderScene();

		// Swap buffers..
		glfwSwapBuffers(gWindow);
		glfwPollEvents();

		return;
	}
}

void initializeGLFW() {

	gWindowTitle = "Multi-threaded GraphicsEngine Window!";
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("pause");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glfwPollEvents();


	// Load the shader
	//if (!setupTheShader()) {
	//	std::cout << "Oh no! The shaders didn't load!!" << std::endl;
	//	system("pause");
	//	return -1;
	//} = true;
	
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
	glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &g_pMeshManager->vertices[0],
		GL_STATIC_DRAW);

	glGenBuffers(1, &gIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

	int bytesInIndexArray = g_pMeshManager->indices.size() * sizeof(GLuint);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &g_pMeshManager->indices[0],
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
	// Create and compile our GLSL program from the shaders
	//gProgramID = ::g_pShaderManager->loadShaders(
	//	"SimpleShader.vert", "SimpleShader.frag", "SimpleShader.geom");

	//if (!gProgramID) {
	//	std::cout << "Shaders failed to load!!" << std::endl;
	//	return false;
	//}

	// per item
	// Uber shader
	//gUniformId_ModelMatrix = glGetUniformLocation(gProgramID, "ModelMatrix");
	//gUniformId_ModelMatrixOrientation =
	//	glGetUniformLocation(gProgramID, "ModelMatrixOrientation");
	//gUniformId_Alpha = glGetUniformLocation(gProgramID, "Alpha");
	//gUniformId_ModelColor = glGetUniformLocation(gProgramID, "ModelColor");
	//gUniformId_NumLights = glGetUniformLocation(gProgramID, "NUM_LIGHTS");
	//
	//gUniformId_ViewMatrix = glGetUniformLocation(gProgramID, "ViewMatrix");
	//gUniformId_PojectionMatrix =
	//	glGetUniformLocation(gProgramID, "ProjectionMatrix");
	//gUniformId_EyePosition = glGetUniformLocation(gProgramID, "EyePosition");
	//
	//gUniformId_Toggle_Lights = glGetUniformLocation(gProgramID, "Toggle_Lights");
	//gUniformId_Toggle_Skybox = glGetUniformLocation(gProgramID, "Toggle_Skybox");
	//gUniformId_Toggle_Textures =
	//	glGetUniformLocation(gProgramID, "Toggle_Textures");
	//
	//gUniformId_NumTextures = glGetUniformLocation(gProgramID, "NUM_TEXTURES");
	//gUniformId_Texture0 = glGetUniformLocation(gProgramID, "Texture0");
	//gUniformId_Texture1 = glGetUniformLocation(gProgramID, "Texture1");
	//gUniformId_Texture2 = glGetUniformLocation(gProgramID, "Texture2");
	//gUniformId_Texture3 = glGetUniformLocation(gProgramID, "Texture3");
	//
	//// Physics Shader
	//g_Physics_ProgramID = ::g_pShaderManager->loadShaders(
	//	"Physics.vert", "Physics.frag", "Physics.geom");
	//gUniformId_Physics_ModelMatrix =
	//	glGetUniformLocation(g_Physics_ProgramID, "ModelMatrix");
	//gUniformId_Physics_ModelMatrixOrientation =
	//	glGetUniformLocation(g_Physics_ProgramID, "ModelMatrixOrientation");
	//gUniformId_Physics_ViewMatrix =
	//	glGetUniformLocation(g_Physics_ProgramID, "ViewMatrix");
	//gUniformId_Physics_PojectionMatrix =
	//	glGetUniformLocation(g_Physics_ProgramID, "ProjectionMatrix");
	//gUniformId_Physics_Alpha = glGetUniformLocation(g_Physics_ProgramID, "Alpha");
	//gUniformId_Physics_ModelColor =
	//	glGetUniformLocation(g_Physics_ProgramID, "ModelColor");
	//gUniformId_Physics_EyePosition =
	//	glGetUniformLocation(g_Physics_ProgramID, "EyePosition");

	// Skybox Shader
	::gSkyboxShaderID =
		::g_pShaderManager->loadShaders("../GraphicsEngine/Skybox.vs.glsl", "../GraphicsEngine/Skybox.fs.glsl");
	//::gUniformId_ToggleSkyboxTextures =
	//	glGetUniformLocation(gSkyboxShaderID, "Toggle_Skybox_Textures");
	gSkyboxVMID = glGetUniformLocation(gSkyboxShaderID, "view_matrix");

	gUniformId_SamplerCube = glGetUniformLocation(gSkyboxShaderID, "skybox");
	glActiveTexture(gUniformId_SamplerCube);

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

		glUniform1i(gUniformId_ToggleSkyboxTextures, gUniformId_ToggleSkyboxTextures);

		glUniform1iARB(gUniformId_SamplerCube, g_pTextureManager->mapTextureNameToID["Skybox"]);
		glUniformMatrix4fv(gSkyboxVMID, 1, GL_FALSE,
			glm::value_ptr(camViewInverse));

		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glEnable(GL_DEPTH_TEST);
	}
}

