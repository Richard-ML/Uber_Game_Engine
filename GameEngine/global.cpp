#include "stdafx.h"
#include "global.h"
cEntityManager *cEntityManager::s_cEntityManager =
0; // Allocating pointer to static instance of cMeshManager (singleton)
cEntityManager *g_pEntityManager = cEntityManager::instance();
PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
SoundEngine::cSoundEngine * g_pSoundEngine = SoundEngine::cSoundEngine::instance();
AIEngine::cAIEngine * g_pAIEngine = AIEngine::cAIEngine::instance();