#include "cComponentManager.h"
#include "global.h"
#include <sstream>

cComponent *cComponentManager::create_Component(const std::string &name) {
  cComponentDetails::map_ComponentRegistry &curRegistry =
      cComponentDetails::getComponentRegistry();
  cComponentDetails::map_ComponentRegistry::iterator it =
      curRegistry.find(name);

  if (it == curRegistry.end()) {
    // No component registered to this name..
    std::cout << "No component has been registered to this name.. \n";
    return nullptr;
  }

  cComponentDetails::func_createComponent func = it->second;
  return func();
} // Component Manager create_Component

void cComponentManager::delete_Component(const cComponent *component) {
  delete component;
} // Component Manager delete_Component

cComponent::cComponent() {}

cComponent::~cComponent() {}
