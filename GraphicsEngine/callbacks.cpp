#include "stdafx.h"
#include "global.h"
#include "cGraphicsEngine.h"
void callback_windowResize(GLFWwindow *window, int width, int height) {
	if (width != 0 && height != 0)
		gCamera->windowResize(width, height);
}

void callback_WindowClose(GLFWwindow *window) {
	glfwSetWindowShouldClose(window, 1);
	// TODO: Something better than just this
	glfwTerminate();
}

void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
	int mods) {
	// NOTE: Does not work for buttons being held-down. See TAG: 001X
	if (action == 1)
		switch (key) {
		case GLFW_KEY_1:
		{
			GraphicsEngine::cGraphicsEngine::instance()->g_pGameState->loadGame(0);
		}
		break;
		case GLFW_KEY_P:
		{
			GraphicsEngine::cGraphicsEngine::instance()->g_pGameState->saveGame();
		}
		break;
		}
}
void callback_Error(int, const char* error)
{
	std::cout << "GLFW Error: " << error << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Scrolling: backwards = 1 || forwards = -1
	//gCamera->m_zoom.pendingDistanceOffset -= (float) yoffset * gCamera->m_zoom.rate; // TODO: Clearly the camera's properties are not private.. Fix this.
	gCamera->m_zoom.distance -= (float)yoffset * gCamera->m_zoom.rate;
}