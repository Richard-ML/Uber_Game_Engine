#include "stdafx.h"
#include "global.h"
void callback_windowResize(GLFWwindow *window, int width, int height) {
	//if (width != 0 && height != 0)
		//gCamera->windowResize(width, height);
}

void callback_WindowClose(GLFWwindow *window) {
	glfwSetWindowShouldClose(window, 1);
}

void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
	int mods) {

	if (action == 1)
		switch (key) {
		case GLFW_KEY_X:
			// g_pEntityManager->dropBomb();
			break;
		}

}
