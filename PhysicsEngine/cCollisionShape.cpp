#include "stdafx.h"  
#include "cPhysicsEngine.h"
namespace PhysicsEngine {
	static  void getBoundingSphere(glm::vec3 min, glm::vec3 max, glm::vec3& center, glm::vec3& radius)
	{
		radius = glm::length(max - min) * glm::vec3(0.5f);
		center = (min + max) * glm::vec3(0.5f);
	}
}