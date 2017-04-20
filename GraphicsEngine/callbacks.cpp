#include "stdafx.h"
#include "global.h"
#include "cGraphicsEngine.h"

///-------------------------------------------------------------------------------------------------
/// <summary>
/// Updates global window information when the window is resized. Updates camera's properties
/// with new values.
/// </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="window">	window handle[in,out]. </param>
/// <param name="width"> 	width </param>
/// <param name="height">   height </param>
///-------------------------------------------------------------------------------------------------

void callback_windowResize(GLFWwindow *window, int width, int height) {
	if (width != 0 && height != 0)
	{
		gWindowWidth = width;
		gWindowHeight = height;
		gCamera->windowResize(width, height);

	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>
/// Tells the application that it's time to exit when the application's window is exited. Unloads
/// GLFW resources.
/// </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="window">	[in,out] GLFW window handle. </param>
///-------------------------------------------------------------------------------------------------

void callback_WindowClose(GLFWwindow *window) {
	glfwSetWindowShouldClose(window, 1);

	// TODO: Something better than just this
	g_pGameState->setGameState(GAMESTATE_EXIT);

	glfwTerminate();
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Key press event handler. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="window">  	[in,out] If non-null, the window. </param>
/// <param name="key">	   	The key. </param>
/// <param name="scancode">	The scancode. </param>
/// <param name="action">  	The action. </param>
/// <param name="mods">	   	The mods. </param>
///-------------------------------------------------------------------------------------------------

void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
	int mods) {
	// NOTE: Does not work for buttons being held-down. See TAG: 001X
	if (action == 1) {
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

		case GLFW_KEY_NUM_LOCK:
		{
			g_bool_toggleWorldEditMode = !g_bool_toggleWorldEditMode;
		}
		break;
		}
		if (g_bool_toggleWorldEditMode) {
			switch (key) {
			case GLFW_KEY_0:
			{
				g_pWorld->setActiveSelectionMode(FLOOR);
			}
			break;

			case GLFW_KEY_9:
			{
				g_pWorld->setActiveSelectionMode(WALL_LEFT);
			}
			break;
			case GLFW_KEY_8:
			{
				g_pWorld->setActiveSelectionMode(WALL_RIGHT);
			}
			break;

			case GLFW_KEY_7:
			{
				g_pWorld->setActiveSelectionMode(WALL_FORWARD);
			}
			break;

			case GLFW_KEY_6:
			{
				g_pWorld->setActiveSelectionMode(WALL_BACKWARD);
			}
			break;
			case GLFW_KEY_SPACE:
			{
				g_pWorld->clearSelection();
			}
			break;

			case GLFW_KEY_ENTER:
			{
				g_pWorld->generateObjectAtSlection();
			}
			break;

			case GLFW_KEY_ESCAPE:
			{
				glfwSetWindowShouldClose(window, 1);

				// TODO: Something better than just this
				g_pGameState->setGameState(GAMESTATE_EXIT);

				glfwTerminate();
			}
			break;


			case GLFW_KEY_Q:
			{
				gToggle_Skybox = !gToggle_Skybox;
				g_bool_toggleDebugShapes = !g_bool_toggleDebugShapes;
			}
			break;

			case GLFW_KEY_DELETE:
			{
				g_pWorld->removeObjectsAtSelection();
			}
			break;
			}
		}
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	VERY simple error handler. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
/// <param name="parameter1">	The first parameter. </param>
/// <param name="error">	 	The error. </param>
///-------------------------------------------------------------------------------------------------

void callback_Error(int, const char* error)
{
	std::cout << "GLFW Error: " << error << std::endl;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Mouse wheel event handler. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="window"> 	[in,out] If non-null, the window. </param>
/// <param name="xoffset">	The xoffset. </param>
/// <param name="yoffset">	The yoffset. </param>
///-------------------------------------------------------------------------------------------------

void callback_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// Scrolling: backwards = 1 || forwards = -1
	//gCamera->m_zoom.pendingDistanceOffset -= (float) yoffset * gCamera->m_zoom.rate; // TODO: Clearly the camera's properties are not private.. Fix this.
	gCamera->m_zoom.distance -= (float)yoffset * gCamera->m_zoom.rate;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Mouse button event handler. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>

///-------------------------------------------------------------------------------------------------

///-------------------------------------------------------------------------------------------------
/// <summary>	Callback mouse button. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="window">	[in,out] If non-null, the window. </param>
/// <param name="button">	The button. </param>
/// <param name="action">	The action. </param>
/// <param name="mods">  	The mods. </param>
///-------------------------------------------------------------------------------------------------

void callback_mouse_button(GLFWwindow* window, int button, int action, int mods)
{
	if (g_bool_toggleWorldEditMode) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		if (action == GLFW_PRESS)
		{
			glm::vec3 cursorDirection;
			cursorDirection.x = 1.0f;
			cursorDirection.y = (float)(((gWindowHeight / 2) - ypos) / (gWindowHeight / 2));
			cursorDirection.z = (float)((xpos - (gWindowWidth / 2)) / (gWindowHeight / 2));

			float fov = std::tan(0.5f);
			cursorDirection.y *= fov;

			cursorDirection.z *= fov;


			cursorDirection = gCamera->m_viewIdentityMatrix * glm::vec4(cursorDirection, 0);

			cursorDirection = glm::normalize(cursorDirection);
			glm::vec3 up(0.0f, 1.0f, 0.0f);

			float d = glm::dot(up, glm::vec3(0.0f));
			float h1 = glm::dot(up, gCamera->m_eyePosition) - d;
			float h2 = -glm::dot(up, cursorDirection);
			float t = h1 / h2;

			glm::vec3 result = cursorDirection * t + gCamera->m_eyePosition;
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				g_pWorld->selectTileAtPosition(result);
			else
				if (button == GLFW_MOUSE_BUTTON_LEFT)
					g_pWorld->deselectTileAtPosition(result);
			//g_pDebugRenderer->createCube(result - glm::vec3(32.0f, 0.0f,32.0f), glm::vec3(64.0f, 6.0f, 64.0f), 10, glm::vec3(1.0f));
		}
	}
}
