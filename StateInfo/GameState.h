#pragma once
 #include "stdafx.h"
 #include <vector>
 #include "EnumerationConstants.h"
//Change state data (used by core state manager to dispatch updates to each
// engine which implements the same interface!

///-------------------------------------------------------------------------------------------------
/// <summary>	A game state. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class iGameState {
	public:

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets game state. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="gameState">	State of the game. </param>
		///-------------------------------------------------------------------------------------------------

		virtual void setGameState(eGameState gameState) = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets game state. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	The game state. </returns>
		///-------------------------------------------------------------------------------------------------

		virtual eGameState getGameState() = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the difficulty. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <returns>	The difficulty. </returns>
		///-------------------------------------------------------------------------------------------------

		virtual eDifficulty getDifficulty() = 0;

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets the difficulty. </summary>
		///
		/// <remarks>	Richard, 4/12/2017. </remarks>
		///
		/// <param name="difficulty">	The difficulty. </param>
		///-------------------------------------------------------------------------------------------------

		virtual void setDifficulty(eDifficulty difficulty) = 0;
		
};