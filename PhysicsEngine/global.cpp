#include "stdafx.h"
#include "global.h"
#include "cSoftBodyManager.h"
//**************** Initialize Managers ****************
cSoftBodyManager *cSoftBodyManager::s_cSoftBodyManager =
0; // Allocating pointer to static instance of cSoftBodyManager (singleton)
cSoftBodyManager *g_pSoftBodyManager = cSoftBodyManager::instance();