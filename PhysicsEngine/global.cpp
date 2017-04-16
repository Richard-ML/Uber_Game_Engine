#include "stdafx.h"
#include "global.h"

//**************** Initialize Managers ****************
iGameState * g_pGameState = 0;
iDebugRenderer * g_pDebugRenderer = 0;
iWorld* g_pWorld = 0;
PhysicsEngine::_btWorld* g_btWorld = 0;
//**************** Initialize Locks *******************
//sSpinLock * g_pLock = 0;