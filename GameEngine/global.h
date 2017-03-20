#pragma once
#include "stdafx.h"
#include "cEntityManager.h"
#include "cGameState.h"
extern cEntityManager *g_pEntityManager;
extern ComponentEngine::cComponentEngine * g_pComponentEngine;
extern PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine;
extern GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine;
extern SoundEngine::cSoundEngine * g_pSoundEngine;
extern AIEngine::cAIEngine * g_pAIEngine;
extern cGameState* g_pGameState;
extern iDebugRenderer* g_pDebugRenderer;
//extern cWorld* g_pWorld;