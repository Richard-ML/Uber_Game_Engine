#pragma once
#include "stdafx.h"

class cGameState : public iGameState {
private:
	eGameState m_gameState;
	eDifficulty m_difficulty = CASUAL;
	struct sSpinLock {
		LOCK lock;
	}m_lock[2]; // 2 locks. One should be used per variable
	virtual void lock(int varNum) {
#if !defined(SKIP_LOCKING)  
		while (_InterlockedExchange(&m_lock[varNum].lock, LOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)
#endif  
	}

	virtual void unlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedExchange(&m_lock[varNum].lock, UNLOCKED);
#endif  
	}
public:
	// Inherited via iGameState

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets game state. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="gameState">	State of the game. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void setGameState(eGameState gameState) override {
		lock(0);
		m_gameState = gameState;
		unlock(0);
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Returns GameState Loading, Saving, etc. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	GameState. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual eGameState getGameState() override {
		eGameState gameState;
		lock(0);
		gameState = m_gameState;
		unlock(0);
		return gameState;
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Returns the current game difficulty. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Game Difficulty. </returns>
	///-------------------------------------------------------------------------------------------------

	virtual eDifficulty getDifficulty() {
		eDifficulty difficulty;
		lock(1);
		difficulty = m_difficulty;
		unlock(1);
		return difficulty;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets the game difficulty. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="difficulty">	Game Difficulty. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void setDifficulty(eDifficulty difficulty) {
		lock(1);
		m_difficulty = difficulty;
		unlock(1);
	}
};