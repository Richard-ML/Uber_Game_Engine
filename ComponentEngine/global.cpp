#include "stdafx.h"
#include "global.h"

int gNextComponentID = 0;
PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
SoundEngine::cSoundEngine * g_pSoundEngine = SoundEngine::cSoundEngine::instance();
AIEngine::cAIEngine * g_pAIEngine = AIEngine::cAIEngine::instance();