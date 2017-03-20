// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#include "..\Include\glm\glm\glm.hpp"

#include "..\Include\glm\glm\gtc\matrix_transform.hpp"
#include "..\Include\glm\glm\gtc\random.hpp"
#include "..\Include\glm\glm\gtc\type_ptr.hpp"
#include "..\Include\glm\glm\gtx\euler_angles.hpp"
#include "..\StateInfo\State.h"
#include "..\StateInfo\GameState.h"
#include "..\StateInfo\DebugRenderer.h"
#include "..\StateInfo\World.h"