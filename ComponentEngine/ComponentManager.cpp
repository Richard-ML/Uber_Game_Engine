#include "stdafx.h"
#include "ComponentManager.h"
cComponent *ComponentManager::create_Component(const std::string &name) {
	ComponentDetails::map_ComponentRegistry &curRegistry =
		ComponentDetails::getComponentRegistry();
	ComponentDetails::map_ComponentRegistry::iterator it =
		curRegistry.find(name);

	if (it == curRegistry.end()) {
		// No component registered to this name..
		std::cout << "No component has been registered to this name.. \n";
		return nullptr;
	}

	ComponentDetails::func_createComponent func = it->second;
	return func();
} // Component Manager create_Component

void ComponentManager::delete_Component(const cComponent *component) {
	delete component;
} // Component Manager delete_Component

cComponent::cComponent() {}

cComponent::~cComponent() {}