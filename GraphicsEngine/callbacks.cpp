#include "stdafx.h"
#include "global.h"
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
	//if (action == 1)
	//	switch (key) {
	//	case GLFW_KEY_W:
	//		for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
	//		{
	//			glm::mat4 tempTrans = glm::translate(controlComponent.pState->getTransform(), glm::vec3(0.0f, 0.0f, 1.0f));
	//			controlComponent.pState->setTransform(tempTrans);
	//			gCamera->setTargetTransform(tempTrans);
	//		}
	//		break;
	//	case GLFW_KEY_S:
	//		for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
	//		{
	//			glm::mat4 tempTrans = glm::translate(controlComponent.pState->getTransform(), glm::vec3(0.0f,0.0f,-1.0f));
	//			controlComponent.pState->setTransform(tempTrans);
	//			gCamera->setTargetTransform(tempTrans);
	//		}
	//		break;
	//	case GLFW_KEY_A:
	//		for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
	//		{
	//			glm::mat4 rotMatrix;
	//			glm::mat4 tempTrans = controlComponent.pState->getTransform();
	//			rotMatrix = glm::rotate(glm::mat4(), 0.1f, glm::vec3(0.0f,1.0f,0.0f));
	//			tempTrans *= rotMatrix;
	//			controlComponent.pState->setTransform(tempTrans);
	//			gCamera->setTargetTransform(controlComponent.pState->getTransform());
	//		}
	//		break;
	//	case GLFW_KEY_D:
	//		for each (cPlayerControlComponent controlComponent in g_vec_playerControlComponents)
	//		{
	//			glm::mat4 rotMatrix;
	//			glm::mat4 tempTrans = controlComponent.pState->getTransform();
	//			rotMatrix = glm::rotate(glm::mat4(), 0.1f, glm::vec3(0.0f, -1.0f, 0.0f));
	//			tempTrans *= rotMatrix;
	//			controlComponent.pState->setTransform(tempTrans);
	//			gCamera->setTargetTransform(controlComponent.pState->getTransform());
	//		}
	//		break;
	//	}






}
void callback_Error(int, const char* error)
{
	std::cout << "GLFW Error: " << error << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Scrolling: backwards = 1 || forwards = -1
	gCamera->m_zoom.pendingDistanceOffset -= (float) yoffset;
}