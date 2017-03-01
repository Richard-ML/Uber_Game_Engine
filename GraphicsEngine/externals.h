#pragma once

#define GLM_FORCE_SWIZZLE
#include "..\Include\glm\glm\glm.hpp"
#include "..\Include\glm\glm\gtc\matrix_transform.hpp"
#include "..\Include\glm\glm\gtc\random.hpp"
#include "..\Include\glm\glm\gtc\type_ptr.hpp"
#include "..\Include\glm\glm\gtx\euler_angles.hpp"

#include "..\Include\Assimp\assimp\Importer.hpp"
#include "..\Include\Assimp\assimp\postprocess.h"
#include "..\Include\Assimp\assimp\scene.h"

#include "..\Include\rapidxml\rapidxml.hpp"
#include "..\Include\FreeImage\FreeImage.h"

// GLFW needs these two guys..
#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GLFW\glfw3.h>
#include "..\Include\glew\include\GL\glew.h"
#include "..\Include\glfw\include\GLFW\glfw3.h"
#include "..\StateInfo\State.h"
#include "..\StateInfo\GameState.h"