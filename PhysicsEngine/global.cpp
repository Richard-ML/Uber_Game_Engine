#include "stdafx.h"
#include "global.h"

//**************** Initialize Managers ****************
iGameState * g_pGameState = 0;
iDebugRenderer * g_pDebugRenderer = 0;
iWorld* g_pWorld = 0;

sSpinLock * g_pLock = 0;