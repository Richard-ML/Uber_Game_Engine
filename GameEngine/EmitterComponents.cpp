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
  //	enum eDSP_Types {
  //		FMOD_DSP_TYPE_LOWPASS,            /* This unit filters sound using a
  //high quality, resonant lowpass filter algorithm but consumes more CPU time.
  //*/
  //		FMOD_DSP_TYPE_HIGHPASS,           /* This unit filters sound using a
  //resonant highpass filter algorithm. */
  //		FMOD_DSP_TYPE_ECHO,               /* This unit produces an echo on
  //the sound and fades out at the desired rate. */
  //		FMOD_DSP_TYPE_FLANGE,             /* This unit produces a flange
  //effect on the sound. */
  //		FMOD_DSP_TYPE_CHORUS,             /* This unit produces a chorus
  //effect on the sound. */
  //		FMOD_DSP_TYPE_DISTORTION,         /* This unit distorts the sound.
  //*/
  //		FMOD_DSP_TYPE_PITCHSHIFT,         /* This unit bends the pitch of a
  //sound without changing the speed of playback. */
  //		FMOD_DSP_TYPE_TREMOLO             /* This unit produces a tremolo /
  //chopper effect on the sound. */
  //	} e_DSPs;

  std::vector<std::shared_ptr<cComponent>>
      m_vec_pComponents; // Components that belong to this component..
  // bool bDSPFilters[sizeof(eDSP_Types)];

  FMOD::Channel *channel;
  int channelID;
  std::string soundName;
};
REGISTER_COMPONENT(SoundEmitterComponent, "SoundEmitterComponent")

SoundEmitterComponent::~SoundEmitterComponent() {}

cComponent *SoundEmitterComponent::getType() { return nullptr; }

void SoundEmitterComponent::update() {

  // gFMODResult = gSoundSystem->playSound(map_gSounds[this->soundName], NULL,
  // true, &gChannels.at(this->channelID));
  // if (gChannels.at(this->channelID)->isPlaying(&isPlaying) == FMOD_OK &&
  // isPlaying)
  //{
  // gFMODResult = gSoundSystem->playSound(map_gSounds[this->soundName], NULL,
  // false, &gChannels.at(this->channelID));

  //
  //	gFMODResult = gChannels.at(this->channelID)->setVolume(gVolume);
  //	gSoundSystem->update();
  // gFMODResult = gChannels.at(this->channelID)->setPitch(gPitch);
  // gFMODResult = gChannels.at(this->channelID)->setPan(gBalance);
  ///
  //}
}

bool SoundEmitterComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
  this->channelID = gChannels.size();

  gChannels.push_back(this->channel);
  this->soundName = componentNode->first_attribute("soundName")->value();
  this->m_Transform = glm::translate(
      glm::mat4(),
      glm::vec3(std::stof(componentNode->first_attribute("offsetX")->value()),
                std::stof(componentNode->first_attribute("offsetY")->value()),
                std::stof(componentNode->first_attribute("offsetZ")->value())));
  this->setTransform(m_Transform);

  glm::mat4 matrix = this->getTransform();
  FMOD_VECTOR chanPos{matrix[3].x, matrix[3].y, matrix[3].z};
  FMOD_VECTOR chanVel{0.0f, 0.0f, 0.0f};
  g_pSoundManager->playSound(this->soundName, this->channelID, chanPos,
                             chanVel);
  return true;
}
