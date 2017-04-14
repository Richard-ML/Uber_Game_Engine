#pragma once
//#define GLM_FORCE_SWIZZLE
#include "..\Include\glm\glm\glm.hpp"
			 
#include "..\Include\glm\glm\gtc\matrix_transform.hpp"
#include "..\Include\glm\glm\gtc\random.hpp"
#include "..\Include\glm\glm\gtc\type_ptr.hpp"
#include "..\Include\glm\glm\gtx\euler_angles.hpp"

#include "..\Include\Bullet\src\btBulletDynamicsCommon.h"
#include "..\Include\Bullet\src\btBulletCollisionCommon.h"
#include "..\Include\Bullet\src\LinearMath\btVector3.h"
#include "..\Include\Bullet\src\LinearMath\btAlignedObjectArray.h"
//#include "..\Include\Bullet\src\BulletCollision\CollisionShapes\btShapeHull.h"
//#include "..\Include\Bullet\src\BulletCollision\BroadphaseCollision\btOverlappingPairCache.h"
//#include "..\Include\Bullet\src\BulletCollision\CollisionDispatch\btGhostObject.h"


//#include <LinearMath\btVector3.h>
//#include <LinearMath\btAlignedObjectArray.h>
//#include <btBulletDynamicsCommon.h>
//#include <btBulletCollisionCommon.h>
//#include <BulletCollision/CollisionDispatch/btGhostObject.h>


#include "..\Include\rapidxml\rapidxml.hpp"
#include "..\StateInfo\State.h"
#include "..\StateInfo\GameState.h"

#include "..\Include\Assimp\assimp\Importer.hpp"
#include "..\Include\Assimp\assimp\postprocess.h"
#include "..\Include\Assimp\assimp\scene.h"
#include "..\StateInfo\DebugRenderer.h"
#include "..\StateInfo\World.h"