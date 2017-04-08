#pragma once 
// Managers
extern iGameState * g_pGameState;
extern iDebugRenderer* g_pDebugRenderer;
extern iWorld* g_pWorld;




//NOTE: Position shares lock with transform since both variables must be updated
struct sSpinLock {
	//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
	LOCK lock;
};

extern sSpinLock * g_pLock;