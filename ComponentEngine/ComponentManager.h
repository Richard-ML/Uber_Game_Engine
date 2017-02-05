#pragma once
#include "stdafx.h"
#include "global.h"
#include "iComponent.h"

	class cComponent { //: public ComponentManager::iComponent {

	public:
		int componentID;
		iStateNode* state;

		std::vector<std::shared_ptr<cComponent>>
			m_vec_pComponents; // Components that belong to this component..

		cComponent();
		virtual ~cComponent();
		//virtual cComponent *getType() = 0;
		virtual bool loadFromXML(rapidxml::xml_node<> *componentNode) = 0;
	};

	cComponent::cComponent()
	{
	}
	cComponent::~cComponent()
	{
	}


	namespace ComponentManager {

	namespace ComponentDetails {
		typedef cComponent *(*func_createComponent)();
		typedef std::map<std::string, func_createComponent> map_ComponentRegistry;

		inline map_ComponentRegistry &getComponentRegistry() {
			static map_ComponentRegistry reg;
			return reg;
		}
		template <class T> cComponent *createComponent() { return new T; }

		template <class T> struct ComponentEntry {
		public:
			static ComponentEntry<T> &instance(const std::string &componentName) {
				static ComponentEntry<T> instance(componentName); // Singleton
				return instance;
			}

		private:
			ComponentEntry(const std::string &name) {
				map_ComponentRegistry &reg = getComponentRegistry();
				func_createComponent func = createComponent<T>;

				std::pair<map_ComponentRegistry::iterator, bool> ret =
					reg.insert(map_ComponentRegistry::value_type(name, func));
				if (ret.second == false) {
					// Component already exists with this name..
					printf("COMPONENT ALREADY EXISTS WOOT! \n");
				}
			}
			ComponentEntry(const ComponentEntry<T> &) =
				delete; // Remove copy constructors
			ComponentEntry &operator=(const ComponentEntry<T> &) = delete;
		};
	} // namespace ComponentDetails


	static std::map<int, cComponent *> map_ComponentIDToComponent;

	cComponent *create_Component(const std::string &name);

	void delete_Component(const cComponent *component);
	static std::vector<std::shared_ptr<cComponent>>
		loadFromXML(rapidxml::xml_node<> *entityNode) {
		std::vector<std::shared_ptr<cComponent>> vec_TempComponents;
		for (rapidxml::xml_node<> *cComponent_node = entityNode->first_node();
			cComponent_node; cComponent_node = cComponent_node->next_sibling()) {
			std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
				create_Component(cComponent_node->name()));
			tempComponent->loadFromXML(cComponent_node);
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


namespace ComponentManager {
	cComponent *create_Component(const std::string &name) {
		ComponentDetails::map_ComponentRegistry &curRegistry =
			ComponentDetails::getComponentRegistry();
		ComponentDetails::map_ComponentRegistry::iterator it =
			curRegistry.find(name);

		if (it == curRegistry.end()) {
			// No component registered to this name..
			printf("No component has been registered to this name.. \n");
			return nullptr;
		}

		ComponentDetails::func_createComponent func = it->second;
		return func();
	} // Component Manager create_Component

	void ComponentManager::delete_Component(const cComponent *component) {
		delete component;
	} // Component Manager delete_Component

}