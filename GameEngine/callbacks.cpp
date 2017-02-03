#include "global.h"
#include <sstream>

void callback_windowResize(GLFWwindow *window, int width, int height) {
  if (width != 0 && height != 0)
    gCamera->windowResize(width, height);
}

void callback_WindowClose(GLFWwindow *window) {
  glfwSetWindowShouldClose(window, 1);
}

void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
  static double lasttime; // = glfwGetTime();

  if (lasttime != 0.0) {
    double currentTime = glfwGetTime();
    double elapsedTime = lasttime - currentTime;

    if (std::abs(elapsedTime) >= 3)
      lasttime = 0;
  }
  if (action == 1)
    switch (key) {
    case GLFW_KEY_X:
      // g_pEntityManager->dropBomb();
      break;
    case GLFW_KEY_KP_4:
      g_bool_DebugShapes = !g_bool_DebugShapes;
      break;
	  
	case GLFW_KEY_O:
		if (curSphereIndex == -24)
			curSphereIndex = -15;
		else
			curSphereIndex--;
		break;

	case GLFW_KEY_P:
		if (curSphereIndex == -15)
			curSphereIndex = -24;
		else
			curSphereIndex++;
		break;
    case GLFW_KEY_KP_2:
      g_bool_ToggleSkybox = !g_bool_ToggleSkybox;
      break;
    // case GLFW_KEY_DOWN:
    //	break;
    // case GLFW_KEY_LEFT:
    //	break;
    // case GLFW_KEY_RIGHT:
    //	break;
    case GLFW_KEY_KP_1:
      g_bool_ToggleLights = !g_bool_ToggleLights;
      break;
    case GLFW_KEY_KP_3:
      g_bool_ToggleTextures = !g_bool_ToggleTextures;
      break;
    }
	
}