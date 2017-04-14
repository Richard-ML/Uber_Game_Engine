#ifndef _EnumerationConstants_HG_
#define _EnumerationConstants_HG_
#include "stdafx.h"
 #include <Windows.h>
 #include <intrin.h>
 #include <thread>
 #include <memory>
#include <vector>
//https://msdn.microsoft.com/en-us/library/ttk2z1ws.aspx
#pragma intrinsic(_InterlockedCompareExchange, _InterlockedExchange)
 enum { UNLOCKED = 0, LOCKED = 1 };
typedef volatile LONG LOCK;

///-------------------------------------------------------------------------------------------------
/// <summary>	Values that represent game state. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

enum eGameState { GAMESTATE_PAUSED = 0, GAMESTATE_LOADING, GAMESTATE_SAVING, GAMESTATE_EXIT, GAMESTATE_RUNNING };

///-------------------------------------------------------------------------------------------------
/// <summary>	Values that represent difficulties. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

enum eDifficulty { CASUAL = 0, HARDCORE };

///-------------------------------------------------------------------------------------------------
/// <summary>	A bounding box. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct sBoundingBox {
	glm::vec3 scale;
	glm::vec3 position;
	glm::quat rotation;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	A Axis Aligned Bounding Box. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct sAABB {
	glm::vec3 scale;
	glm::vec3 position;
};
/* Mesh Information */

///-------------------------------------------------------------------------------------------------
/// <summary>	A mesh vertex. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct sMeshVertex {
	glm::vec4 Position;
	glm::vec4 TexCoord;
	glm::vec4 Normal;
	glm::vec4 Tangent;
	glm::uvec4 textureUnits; // TODO: Switch to uvec4 (GL_UNSIGNED_INT) or
							 // (GL_UNSIGNED_BYTE)
};
///-------------------------------------------------------------------------------------------------
/// <summary>	A triangle face. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

struct sTriangleFace {
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Values that represent active world tile selections. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

enum eActiveWorldTileSelection { FLOOR, WALL_LEFT, WALL_RIGHT, WALL_FORWARD, WALL_BACKWARD };

#endif