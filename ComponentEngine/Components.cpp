#include "stdafx.h"
#include "ComponentManager.h"
#include "global.h"
#include <sstream>


// TODO: Move AI logic to AIEngine.. :P ....................................................................................
class AIControlComponent : public cComponent {
public:
	virtual ~AIControlComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
};
REGISTER_COMPONENT(AIControlComponent, "AIControlComponent")

AIControlComponent::~AIControlComponent() {
	m_vec_pComponents.clear(); // TODO: An entity should have a vec of shared pointers to components.. Automagic cleanup..
}

bool AIControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
	rapidxml::xml_attribute<char> *att =
		componentNode->first_attribute("componentID");
	if (att != nullptr)
		this->componentID =
		atoi(componentNode->first_attribute("componentID")->value());
	else {
		this->componentID = gNextComponentID;
		gNextComponentID++;
	}

	// Dispatch to corresponding engine to create.. 
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InputControlComponent : public cComponent {
public:
	virtual ~InputControlComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
};
REGISTER_COMPONENT(InputControlComponent, "InputControlComponent")

InputControlComponent::~InputControlComponent() { m_vec_pComponents.clear(); }


bool InputControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
	rapidxml::xml_attribute<char> *att =
		componentNode->first_attribute("componentID");
	if (att != nullptr)
		this->componentID =
		atoi(componentNode->first_attribute("componentID")->value());
	else {
		this->componentID = gNextComponentID;
		gNextComponentID++;
	}
	ComponentManager::map_ComponentIDToComponent[this->componentID] = this;

	// Dispatch to corresponding engine to create.. Register a series of listeners? 
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






class CollisionComponent : public cComponent {
public:
	virtual ~CollisionComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
};
REGISTER_COMPONENT(CollisionComponent, "CollisionComponent")

CollisionComponent::~CollisionComponent() { m_vec_pComponents.clear(); }


bool CollisionComponent::loadFromXML(
	rapidxml::xml_node<> *componentNode) {
	rapidxml::xml_attribute<char> *att =
		componentNode->first_attribute("componentID");
	if (att != nullptr)
		this->componentID =
		atoi(componentNode->first_attribute("componentID")->value());
	else {
		this->componentID = gNextComponentID;
		gNextComponentID++;
	}
	ComponentManager::map_ComponentIDToComponent[this->componentID] = this;

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


class RenderingComponent : public cComponent {
public:
	virtual ~RenderingComponent();
	virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
};
REGISTER_COMPONENT(RenderingComponent, "RenderingComponent")

RenderingComponent::~RenderingComponent() { m_vec_pComponents.clear(); }


bool RenderingComponent::loadFromXML(
	rapidxml::xml_node<> *componentNode) {
	rapidxml::xml_attribute<char> *att =
		componentNode->first_attribute("componentID");
	if (att != nullptr)
		this->componentID =
		atoi(componentNode->first_attribute("componentID")->value());
	else {
		this->componentID = gNextComponentID;
		gNextComponentID++;
	}
	ComponentManager::map_ComponentIDToComponent[this->componentID] = this;

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
