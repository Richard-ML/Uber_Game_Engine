#define GLM_FORCE_SWIZZLE
#include "cComponentManager.h"
#include "global.h"

class SoundEmitterComponent : public cComponent {
public:
  virtual ~SoundEmitterComponent();
  virtual cComponent *getType();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  virtual void command(std::string method, std::string value) {}

protected:
  std::vector<std::shared_ptr<cComponent>>
      m_vec_pComponents; // Components that belong to this component..
};
REGISTER_COMPONENT(SoundEmitterComponent, "SoundEmitterComponent")

SoundEmitterComponent::~SoundEmitterComponent() {}

cComponent *SoundEmitterComponent::getType() { return nullptr; }

void SoundEmitterComponent::update() {
	// TODO: If position has changed let the Sound engine know...
}

bool SoundEmitterComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
 std::string soundName = componentNode->first_attribute("soundName")->value();
  this->m_Transform = glm::translate(
      glm::mat4(),
      glm::vec3(std::stof(componentNode->first_attribute("offsetX")->value()),
                std::stof(componentNode->first_attribute("offsetY")->value()),
                std::stof(componentNode->first_attribute("offsetZ")->value())));
// TODO: Use g_pSoundEngine interface to create the sound! 
  return true;
}
