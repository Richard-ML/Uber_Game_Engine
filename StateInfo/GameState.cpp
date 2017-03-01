#include "stdafx.h"
#include "GameState.h"
namespace GameState {



	cGameState *cGameState::s_cGameState =
		0; // Allocating pointer to static instance of cMeshManager (singleton)
	class cGameState_Impl : public cGameState {
		// Boilerplate
		friend class c
			;
		static cGameState *s_cGameState;
	};
	inline const cGameState_Impl *cGameState::impl() const {
		return static_cast<const cGameState_Impl *>(this);
	}
	inline cGameState_Impl *cGameState::impl() {
		return static_cast<cGameState_Impl *>(this);
	}

	cGameState * cGameState::instance()
	{
		if (!s_cGameState)
		{
			printf("Game State Initialized\n");
			s_cGameState = new cGameState();

		}
		return s_cGameState;
	}
}