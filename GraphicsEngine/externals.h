#pragma once

#define GLM_FORCE_SWIZZLE
#include "..\Include\glm\glm\glm.hpp"
#include "..\Include\glm\glm\gtc\matrix_transform.hpp"
#include "..\Include\glm\glm\gtc\random.hpp"
#include "..\Include\glm\glm\gtc\type_ptr.hpp"
#include "..\Include\glm\glm\gtx\euler_angles.hpp"

// GLFW needs these two guys..
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include "..\Include\glew\include\GL\glew.h"
#include "..\Include\glfw\include\GLFW\glfw3.h"