#pragma once
#include "stdafx.h"
#include "EnumerationConstants.h"
// Used by all Engines to render debug info
class iDebugRenderer {
public:
	//spawn primitives with specific lifetimes, etc... 
	virtual void createCube(glm::vec3 position, glm::vec3 scale, float duration, glm::vec3 color) = 0;
};
