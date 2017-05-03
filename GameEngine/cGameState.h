#pragma once
#include "stdafx.h"

class cGameState : public iGameState {
private:
	// State of the game.
	eGameState m_gameState;

	eDifficulty m_difficulty = CASUAL;

	///-------------------------------------------------------------------------------------------------
	/// <summary>	A spin lock. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	struct sSpinLock {
		LOCK lock;
	}m_lock[2]; // 2 locks. One should be used per variable

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Locks. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="varNum">	The variable number. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void lock(int varNum) {
#if !defined(SKIP_LOCKING)  
		while (_InterlockedCompareExchange(&m_lock[varNum].lock, LOCKED, UNLOCKED) == UNLOCKED) {
			// spin!  
		}
		// At this point, the lock is acquired. ;)
#endif  
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Unlocks. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="varNum">	The variable number. </param>
	///-------------------------------------------------------------------------------------------------

	virtual void unlock(int varNum) {
#if !defined(SKIP_LOCKING)  
		_InterlockedCompareExchange(&m_lock[varNum].lock, UNLOCKED, LOCKED);
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