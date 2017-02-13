#ifndef _ComponentManager_HG_
#define _ComponentManager_HG_
#include "stdafx.h"
#include "ComponentDetails.h"
	class cComponent { //: public ComponentManager::iComponent {

	public:
		cComponent();
		virtual ~cComponent();
		//virtual cComponent *getType() = 0;
		virtual bool loadFromXML(rapidxml::xml_node<> *componentNode, std::string stateNodeID) = 0;
	};




namespace ComponentManager {


	static std::map<int, cComponent *> map_ComponentIDToComponent;

	cComponent *create_Component(const std::string &name);

	void delete_Component(const cComponent *component);
	static std::vector<std::shared_ptr<cComponent>>
		loadFromXML(rapidxml::xml_node<> *entityNode,
			std::string stateNodeID) // StateNodeID is used in each engine that receives a redirected xml node. 
		// For instance: A renderable component would send some mesh information along with this id so that 
		// the object created in the engine can subscribe the state of the entity
	{

		std::vector<std::shared_ptr<cComponent>> vec_TempComponents;

		for (rapidxml::xml_node<> *cComponent_node = entityNode->first_node();
			cComponent_node; cComponent_node = cComponent_node->next_sibling()) {

			std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
				create_Component(cComponent_node->name()));

			tempComponent->loadFromXML(cComponent_node, stateNodeID);

			vec_TempComponents.push_back(tempComponent);
		}
		return vec_TempComponents;
	}

};

// Backslash token must be followed by new line character..
#define REGISTER_COMPONENT(COMPONENT_TYPE, COMPONENT_NAME)                     \
namespace ComponentManager {                                                   \
    namespace ComponentDetails {                                               \
    namespace {                                                                \
    template <class T> class RegisterComponent;                                \
                                                                               \
    template <> class RegisterComponent<COMPONENT_TYPE> {                      \
      static const ::ComponentManager::ComponentDetails::ComponentEntry<       \
          COMPONENT_TYPE> &reg;                                                \
    };                                                                         \
                                                                               \
    const ::ComponentManager::ComponentDetails::ComponentEntry<                \
        COMPONENT_TYPE> &RegisterComponent<COMPONENT_TYPE>::reg =              \
        ::ComponentManager::ComponentDetails::ComponentEntry<                  \
            COMPONENT_TYPE>::instance(COMPONENT_NAME);                         \
    }                                                                          \
    }                                                                          \
  }

#endif