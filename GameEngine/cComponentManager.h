#ifndef _cComponent_HG_
#define _cComponent_HG_
#include "cComponentDetails.h"
#include "externals.h"
#include <ctype.h>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
//#elif defined _WIN32
//# include <windows.h>
//#endif
//#if !(defined __LP64__ || defined __LLP64__) || defined _WIN32 && !defined
//_WIN64
// we are compiling for a 32-bit system
//#else
// we are compiling for a 64-bit system
//#endif

/**
 *       __  __ __                   ______                           ______               _            
 *      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___ 
 *     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
 *    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
 *    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/ 
 *                                                                              /____/                  
//===-- cComponentManager.h - Manages components ----------------*- C++ -*-===//
Description: Contains the base component class definition. Provides access to the component registry.
//===----------------------------------------------------------------------===//
Author(s):
 Name: Richard Mills-Laursen
 Role(s): Student
 E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca
 
 Name: Michael Feeney
 E-mail: MFeeney@FanshaweC.on.ca
 Role(s): Coordinator & Professor
 Course(s): 
  INFO-6044-01 - Game Engine Framework/Patterns
  INFO-6028-01 - Graphics 1
  INFO-6019-01 - Physics & Simulation 1
  INFO-6045-01 - Animation
  INFO-6023-01 - Game Algorithms & Gems
  INFO-6020-01 - Graphics 2

 Name: Ben McIlmoyle
 E-Mail: B_McIlmoyle@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6016-01 - Network Programming
 
 Name: James Lucas
 E-Mail: J_Lucas@FanshaweOnline.ca
 Role: Professor
 Course(s): 
  INFO-6017-01 - Artificial Intelligence
  INFO-6022-01 - Physics & Simulation 2

 Name: Oscar Lara
 E-Mail: O_Lara@FanshaweOnline.ca
 Role: Professor
 Course(s): 
  INFO-6046-01 - Media Fundamentals
  INFO-6025-01 - Configuration & Deployment

 Name: L. Gustafson
 E-Mail: L_Gustafson@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/
class cComponent {

public:
  int componentID;

  glm::mat4 m_Transform = glm::mat4(); // Should be private..
  glm::vec3 velocity;
  glm::vec3 acceleration;
  glm::vec3 maxVelocity;
  glm::vec3 minVelocity;
  float lastTime; // Used in every update function
  std::vector<std::shared_ptr<cComponent>>
      m_vec_pComponents; // Components that belong to this component..

  cComponent();
  virtual ~cComponent();
  virtual cComponent *getType() = 0;
  virtual void update() = 0;
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode) = 0;
  void setTransform(const glm::mat4 transform) {
    this->m_Transform = transform;
  }

  glm::mat4 getTransform() { return this->m_Transform; }
  glm::mat4 getOrientation() {
    glm::mat4 tempMatrix = m_Transform;
    // Set position of matrix to the origin
    tempMatrix[3].x = 0.0f;
    tempMatrix[3].y = 0.0f;
    tempMatrix[3].z = 0.0f;
    return glm::mat4_cast(glm::quat_cast(tempMatrix));
  }
  virtual void command(std::string method,
                       std::string value) = 0; // Command pattern.
                                               // Enables components to communicate with other components
                                               // without knowing anything about their abstract members
};

namespace cComponentManager {
static std::map<int, cComponent *> map_ComponentIDToComponent;

cComponent *create_Component(const std::string &name);

void delete_Component(const cComponent *component);
static std::vector<std::shared_ptr<cComponent>>
loadFromXML(rapidxml::xml_node<> *entityNode) {
  std::vector<std::shared_ptr<cComponent>> vec_TempComponents;
  for (rapidxml::xml_node<> *cComponent_node = entityNode->first_node();
       cComponent_node; cComponent_node = cComponent_node->next_sibling()) {
    std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
        cComponentManager::create_Component(cComponent_node->name()));
    tempComponent->loadFromXML(cComponent_node);
    vec_TempComponents.push_back(tempComponent);
  }
  return vec_TempComponents;
}
};

// Backslash token must be followed by new line character..
#define REGISTER_COMPONENT(COMPONENT_TYPE, COMPONENT_NAME)                     \
namespace cComponentManager {                                                  \
    namespace cComponentDetails {                                              \
    namespace {                                                                \
    template <class T> class RegisterComponent;                                \
                                                                               \
    template <> class RegisterComponent<COMPONENT_TYPE> {                      \
      static const ::cComponentManager::cComponentDetails::ComponentEntry<     \
          COMPONENT_TYPE> &reg;                                                \
    };                                                                         \
                                                                               \
    const ::cComponentManager::cComponentDetails::ComponentEntry<              \
        COMPONENT_TYPE> &RegisterComponent<COMPONENT_TYPE>::reg =              \
        ::cComponentManager::cComponentDetails::ComponentEntry<                \
            COMPONENT_TYPE>::instance(COMPONENT_NAME);                         \
    }                                                                          \
    }                                                                          \
  }
#endif