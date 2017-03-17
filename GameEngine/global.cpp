#include "stdafx.h"
#include "global.h"
GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = 0;
cEntityManager *cEntityManager::s_cEntityManager =
0; // Allocating pointer to static instance of cMeshManager (singleton)
cEntityManager *g_pEntityManager = cEntityManager::instance();
ComponentEngine::cComponentEngine * g_pComponentEngine = 0;
PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = 0;

SoundEngine::cSoundEngine * g_pSoundEngine = 0;
AIEngine::cAIEngine * g_pAIEngine = 0;
cGameState* g_pGameState = 0;
//iDebugRenderer * g_pDebugRenderer = 0;
//cWorld* g_pWorld = 0;