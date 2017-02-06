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
			DWORD myThreadID;
			HANDLE myHandle = CreateThread(NULL, 0, // stack size
			(LPTHREAD_START_ROUTINE)&GraphicsEngine::cGraphicsEngine::graphicsThread, reinterpret_cast<void*>(s_cGraphicsEngine), 0, &myThreadID);
		}
		return s_cGraphicsEngine;
	}

	DWORD cGraphicsEngine::graphicsThread(void *lpParam) {
		// TODO: Set initial window title via external configuration file.. (.xml/.json)
		gWindowTitle = "Multi-threaded GraphicsEngine Window!";


		// Initialize GLFW
		initializeGLFW();

		std::chrono::high_resolution_clock::time_point lastTime =
			std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime;
		cGraphicsEngine *physicsEngine =
			reinterpret_cast<cGraphicsEngine *>(lpParam);
		do {
			std::chrono::high_resolution_clock::time_point t2 =
				std::chrono::high_resolution_clock::now();
			deltaTime =
				std::chrono::duration_cast<std::chrono::duration<float>>(
					std::chrono::high_resolution_clock::now() -
					lastTime); // Get the time that as passed

			// CORE RENDERING ROUTINE --- START

			// Use primary rendering shader
			//glUseProgram(gProgramID);

			// Set shader model. Does this really make a difference?
			glShadeModel(GL_SMOOTH);

			glm::vec3 randomColor = glm::abs(glm::ballRand(255.0f));
			// Clear the screen..
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			// Render Skybox
			//renderSkybox();


			// Render objects
			//renderScene();

			// Swap buffers..
			glfwSwapBuffers(gWindow);
			glfwPollEvents();
			
			// CORE RENDERING ROUTINE --- END
			lastTime = std::chrono::high_resolution_clock::now();

			// Aim for 60FPS
			// 1000 / 60 = ~16.6 milliseconds
			float msTimePassed = deltaTime.count() / 1000.0f;
			if (msTimePassed >= 16.6f)
				Sleep(1); // Free the thread
			else
				Sleep(16.6f - msTimePassed); // Sleep for 16.6 ms
		} while (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			!glfwWindowShouldClose(gWindow));


		// TODO: Clean up resources..
		glfwTerminate();


		return 0;
	}
}

void initializeGLFW() {
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
	//}

}


