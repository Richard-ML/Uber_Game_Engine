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

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a const cComponentEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline const cComponentEngine_Impl *cComponentEngine::impl() const {
		return static_cast<const cComponentEngine_Impl *>(this);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets the implementation. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	Null if it fails, else a pointer to a cComponentEngine_Impl. </returns>
	///-------------------------------------------------------------------------------------------------

	inline cComponentEngine_Impl *cComponentEngine::impl() {
		return static_cast<cComponentEngine_Impl *>(this);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

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

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads from XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="entityNode"> 	[in,out] If non-null, the entity node. </param>
	/// <param name="stateNodeID">	Identifier for the state node. </param>
	///
	/// <returns>	The data that was generated in an XML formatted string. </returns>
	///-------------------------------------------------------------------------------------------------

	ComponentEngine_API std::vector<std::shared_ptr<cComponent>> cComponentEngine::loadFromXML(rapidxml::xml_node<>* entityNode, std::string stateNodeID)
	{
		return ComponentManager::loadFromXML(entityNode, stateNodeID);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Registers the new entity. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	A std::string. </returns>
	///-------------------------------------------------------------------------------------------------

	ComponentEngine_API std::string cComponentEngine::registerNewEntity()
	{
		return m_pStateManager->registerState();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Subscribe to state. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="stateID">	Identifier for the state. </param>
	///
	/// <returns>	Null if it fails, else a pointer to an iState. </returns>
	///-------------------------------------------------------------------------------------------------

	ComponentEngine_API iState* cComponentEngine::subcribeToState(std::string stateID) {
		return m_pStateManager->subscribe(stateID);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets game entity XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="stateNodeID">	Identifier for the state node. </param>
	///
	/// <returns>	The game entity XML. </returns>
	///-------------------------------------------------------------------------------------------------

	ComponentEngine_API std::string cComponentEngine::getGameEntityXML(std::string stateNodeID)
	{
		// Pass through interface.. 
		std::string xmlStringResult = m_pStateManager->getGameEntityXML(stateNodeID);
		return xmlStringResult;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Clears the state information. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	ComponentEngine_API void cComponentEngine::clearStateInfo()
	{
		m_pStateManager->clearStateInfo();
	}
}