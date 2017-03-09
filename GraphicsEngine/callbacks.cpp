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
			g_pGameState->setGameState(GAMESTATE_LOADING);
		}
		break;
		case GLFW_KEY_P:
		{
			g_pGameState->setGameState(GAMESTATE_SAVING);
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


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Ben check this out!! TAG : BenIs1337
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		g_pDebugRenderer->createCube(glm::vec3(gCamera->m_viewMatrix[3].x, gCamera->m_viewMatrix[3].y, gCamera->m_viewMatrix[3].z), glm::vec3(20.0f), 10, glm::vec3(0.3f));



	// OR if there is lag. From the main loop or update methods check the mouse button state..
	//int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	//if (state == GLFW_PRESS)
	//	stuff
}
