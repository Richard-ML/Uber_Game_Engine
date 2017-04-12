#include "stdafx.h"
#include "ComponentManager.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	Creates a component. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="name">	The name of the component. </param>
///
/// <returns>	Null if it fails, else the new component. </returns>
///-------------------------------------------------------------------------------------------------

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

///-------------------------------------------------------------------------------------------------
/// <summary>	Deletes the component described by name. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="component">	The component. </param>
///-------------------------------------------------------------------------------------------------

void ComponentManager::delete_Component(const cComponent *component) {
	delete component;
} // Component Manager delete_Component

cComponent::cComponent() {}

cComponent::~cComponent() {}