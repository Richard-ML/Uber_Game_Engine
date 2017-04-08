#pragma once 
#include "externals.h"
// Managers
extern iGameState * g_pGameState;
extern iDebugRenderer* g_pDebugRenderer;
extern iWorld* g_pWorld;




//NOTE: Position shares lock with transform since both variables must be updated
struct sSpinLock {
	//volatile LONG isLocked = 0; // 0 unlocked : 1 locked
	LOCK lock;
};

// Lock used when an rigid-body is being created from another thread.
// vec_rigidbodies must be locked to prevent incompatible iterators within the Physics thread.
extern sSpinLock * g_pLock;