#include "stdafx.h"
#include "ComponentManager.h"
#include "cComponentEngine.h"
#include "global.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	An ai control component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class AIControlComponent : public cComponent {
public:
	virtual ~AIControlComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(AIControlComponent, "AIControlComponent")

AIControlComponent::~AIControlComponent() {
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
/// <param name="stateNodeID">  	Identifier for the state node. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool AIControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID) {
	// Dispatch to corresponding engine to create.. 

	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	An input control component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class InputControlComponent : public cComponent {
public:
	virtual ~InputControlComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(InputControlComponent, "InputControlComponent")

InputControlComponent::~InputControlComponent() {}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
/// <param name="stateNodeID">  	Identifier for the state node. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool InputControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID) {

	// Dispatch to corresponding engine to create.. Register a series of listeners? 
	return true;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	The physics component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class PhysicsComponent : public cComponent {
public:
	virtual ~PhysicsComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(PhysicsComponent, "PhysicsComponent")

PhysicsComponent::~PhysicsComponent() { }

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
/// <param name="stateNodeID">  	Identifier for the state node. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool PhysicsComponent::loadFromXML(
	rapidxml::xml_node<> *componentNode, std::string stateNodeID) {
	iState* state = ComponentEngine::cComponentEngine::instance()->subcribeToState(stateNodeID);
	g_pPhysicsEngine->loadPhysicsComponent(componentNode, state);
	// Dispatch to corresponding engine to create.. 
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//class SoundEmitterComponent : public cComponent {
//public:
//	virtual ~SoundEmitterComponent();
//	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
//
//protected:
//	std::vector<std::shared_ptr<cComponent>>
//		m_vec_pComponents; // Components that belong to this component..
//};
//REGISTER_COMPONENT(SoundEmitterComponent, "SoundEmitterComponent")
//
//SoundEmitterComponent::~SoundEmitterComponent() { m_vec_pComponents.clear(); }
//
//bool SoundEmitterComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
//	rapidxml::xml_attribute<char> *att =
//		componentNode->first_attribute("componentID");
//	if (att != nullptr)
//		this->componentID =
//		atoi(componentNode->first_attribute("componentID")->value());
//	else {
//		this->componentID = gNextComponentID;
//		gNextComponentID++;
//	}
//	ComponentManager::map_ComponentIDToComponent[this->componentID] = this;
//
//	// Dispatch to corresponding engine to create.. 
//	return true;
//}

///-------------------------------------------------------------------------------------------------
/// <summary>	A renderable component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class RenderableComponent : public cComponent {
public:
	virtual ~RenderableComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(RenderableComponent, "RenderableComponent")

RenderableComponent::~RenderableComponent() { }

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="componentNode">	[in,out] If non-null, the component node. </param>
/// <param name="stateNodeID">  	Identifier for the state node. </param>
///
/// <returns>	True if it succeeds, false if it fails. </returns>
///-------------------------------------------------------------------------------------------------

bool RenderableComponent::loadFromXML(
	rapidxml::xml_node<> *componentNode, std::string stateNodeID) {
	iState* state = ComponentEngine::cComponentEngine::instance()->subcribeToState(stateNodeID);
	g_pGraphicsEngine->loadRenderableComponent(componentNode, state);
	// Dispatch to corresponding engine to create.. 
	return true;
}


//class LuaComponent : public cComponent {
//public:
//	LuaComponent();
//	virtual ~LuaComponent();
//	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
//	virtual void command(std::string method, std::string value) {}
//};
//REGISTER_COMPONENT(LuaComponent, "LuaComponent")
