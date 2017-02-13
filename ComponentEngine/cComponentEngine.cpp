#include "stdafx.h"
#include "stdafx.h"
#include "cComponentEngine.h"
#include <stdio.h>
//#include <stdlib.h>
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
namespace ComponentEngine {
	cComponentEngine *cComponentEngine::s_cComponentEngine =
		0; // Allocating pointer to static instance of cComponentEngine (singleton)
	cStateManager* cComponentEngine::m_pStateManager = 0;
	class cComponentEngine_Impl : public cComponentEngine {
		// Boilerplate
		friend class cComponentEngine;
	};
	inline const cComponentEngine_Impl *cComponentEngine::impl() const {
		return static_cast<const cComponentEngine_Impl *>(this);
	}
	inline cComponentEngine_Impl *cComponentEngine::impl() {
		return static_cast<cComponentEngine_Impl *>(this);
	}

	_declspec(dllexport) cComponentEngine * cComponentEngine::instance()
	{
		if (!s_cComponentEngine)
		{
			printf("Component Engine Initialized\n");
			s_cComponentEngine = new cComponentEngine();
			m_pStateManager = new cStateManager();
			//DWORD myThreadID;
			//HANDLE myHandle = CreateThread(NULL, 0, // stack size
			//(LPTHREAD_START_ROUTINE)&ComponentEngine::cComponentEngine::ComponentThread, reinterpret_cast<void*>(s_cComponentEngine), 0, &myThreadID);

			PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
			GraphicsEngine::cGraphicsEngine * g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();
			SoundEngine::cSoundEngine * g_pSoundEngine = SoundEngine::cSoundEngine::instance();
			AIEngine::cAIEngine * g_pAIEngine = AIEngine::cAIEngine::instance();

		}
		return s_cComponentEngine;
	}

	ComponentEngine_API std::vector<std::shared_ptr<cComponent>> cComponentEngine::loadFromXML(rapidxml::xml_node<>* entityNode, std::string stateNodeID)
	{
		return ComponentManager::loadFromXML(entityNode, stateNodeID);
	}

	ComponentEngine_API std::string cComponentEngine::registerNewEntity()
	{
		return m_pStateManager->registerState();
	}
	ComponentEngine_API iState* cComponentEngine::subcribeToState(std::string stateID) {
		return m_pStateManager->subscribe(stateID);
	}

}