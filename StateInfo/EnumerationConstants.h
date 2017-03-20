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

enum eGameState { GAMESTATE_PAUSED = 0, GAMESTATE_LOADING, GAMESTATE_SAVING, GAMESTATE_EXIT, GAMESTATE_RUNNING };
enum eDifficulty { CASUAL = 0, HARDCORE };
struct sAABB {
	glm::vec3 scale;
	glm::vec3 position;
};
enum eActiveWorldTileSelection { FLOOR, WALL_LEFT, WALL_RIGHT, WALL_FORWARD, WALL_BACKWARD };

#endif