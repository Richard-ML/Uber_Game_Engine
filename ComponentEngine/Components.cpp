#include "stdafx.h"
#include "ComponentManager.h"
#include "cComponentEngine.h"
#include "global.h"


class AIComponent : public cComponent {
public:
	virtual ~AIComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(AIComponent, "AIComponent")

AIComponent::~AIComponent() {
}

bool AIComponent::loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID) {
	// Dispatch to corresponding engine to create.. 
	iState* state = ComponentEngine::cComponentEngine::instance()->subcribeToState(stateNodeID);
	g_pAIEngine->loadAIComponent(componentNode, state);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class PhysicsComponent : public cComponent {
public:
	virtual ~PhysicsComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(PhysicsComponent, "PhysicsComponent")

PhysicsComponent::~PhysicsComponent() { }


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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class RenderableComponent : public cComponent {
public:
	virtual ~RenderableComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID);
};
REGISTER_COMPONENT(RenderableComponent, "RenderableComponent")

RenderableComponent::~RenderableComponent() { }


bool RenderableComponent::loadFromXML(
	rapidxml::xml_node<> *componentNode, std::string stateNodeID) {
	iState* state = ComponentEngine::cComponentEngine::instance()->subcribeToState(stateNodeID);
	g_pGraphicsEngine->loadRenderableComponent(componentNode, state);
	// Dispatch to corresponding engine to create.. 
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//class LuaComponent : public cComponent {
//public:
//	LuaComponent();
//	virtual ~LuaComponent();
//	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
//	virtual void command(std::string method, std::string value) {}
//};
//REGISTER_COMPONENT(LuaComponent, "LuaComponent")
