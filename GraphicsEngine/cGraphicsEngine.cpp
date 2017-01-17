#include "stdafx.h"  
#include "cGraphicsEngine.h"

// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace GraphicsEngine {

	class cGraphicsEngine_Impl : public cGraphicsEngine {
		// Boilerplate
		friend class cGraphicsEngine;

	public:

	};
	inline const cGraphicsEngine_Impl *cGraphicsEngine::impl() const {
		return static_cast<const cGraphicsEngine_Impl *>(this);
	}
	inline cGraphicsEngine_Impl *cGraphicsEngine::impl() {
		return static_cast<cGraphicsEngine_Impl *>(this);
	}
	// Initialize vectors

	cGraphicsEngine * cGraphicsEngine::instance() {
		if (!s_cGraphicsEngine)
		{
			s_pMeshManager = cMeshManager::instance();
			s_pShaderManager = cShaderManager::instance();
			s_pTextureManager = cTextureManager::instance();

			s_cGraphicsEngine = new cGraphicsEngine();
			fprintf(stderr, "Graphics Engine Initialized\n");
		}
		return s_cGraphicsEngine;
	}

	GLFWwindow * cGraphicsEngine::createWindow()
	{
	
		// Initialize GLFW
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
		// Open a window and create its OpenGL context
		gWindow =
			glfwCreateWindow(mode->width, mode->height, "Final Project!", NULL, NULL);
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
	
		return gWindow;
	}

}