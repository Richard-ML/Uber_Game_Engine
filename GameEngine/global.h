#pragma once
#include "stdafx.h"
#include "cEntityManager.h"
extern cEntityManager *g_pEntityManager;
extern ComponentEngine::cComponentEngine * g_pComponentEngine;
extern PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine;
extern GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine;
extern SoundEngine::cSoundEngine * g_pSoundEngine;
extern AIEngine::cAIEngine * g_pAIEngine;
extern GameState::cGameState* g_pGameState;