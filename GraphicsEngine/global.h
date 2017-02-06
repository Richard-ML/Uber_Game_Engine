#pragma once
#include "stdafx.h"

// Window information
extern GLFWwindow *gWindow;
extern std::string gWindowTitle;

// Shader Information
extern GLuint gProgramID;

/* Callback Functions */
void callback_windowResize(GLFWwindow *window, int width, int height);
void callback_WindowClose(GLFWwindow *window);
void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
	int mods);
