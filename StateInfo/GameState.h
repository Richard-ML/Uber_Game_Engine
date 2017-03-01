#pragma once
// Hack for now.. TODO: Something better than this
#include <functional>   // std::function, std::negate
#include <vector>
namespace GameState {
	class cGameState_Impl;
	class cGameState {
	private:
		static cGameState *s_cGameState;
		// Boilerplate
		friend class cGameState_Impl; // The PIMPL idiom aka Compilation Firewall
		const cGameState_Impl *impl() const;
		cGameState_Impl *impl();
		cGameState() {}; // Constructor is private therefore a new instance can not be made
							  // externally. Only available to members or friends of this class..
		~cGameState() {
			// TODO: Clean up resources..
		}
		// Not defined to prevent copying of the only instance of the class.
		cGameState(const cGameState &) {}; // Disallow copy constructor
		cGameState &operator=(const cGameState &GraphicsManager) {
		} // Disallow assignment operator
	public:
		bool _loadRequested;
		int _difficulty;
		bool _saveRequested;

		static   cGameState *instance();

		static void loadGame(int difficulty) {
			s_cGameState->_difficulty = difficulty;
			s_cGameState->_loadRequested = true;
		}
		static void saveGame() {
			s_cGameState->_saveRequested = true;
		}
	};


}