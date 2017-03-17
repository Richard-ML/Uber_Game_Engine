#pragma once
 #include "stdafx.h"
 #include <vector>
 #include "EnumerationConstants.h"
//Change state data (used by core state manager to dispatch updates to each
// engine which implements the same interface!
class iGameState {
	public:
		virtual void setGameState(eGameState gameState) = 0;
		virtual eGameState getGameState() = 0;
		virtual eDifficulty getDifficulty() = 0;
		virtual void setDifficulty(eDifficulty difficulty) = 0;
		
};