#include "cSoundManager.h"
#include "global.h"

cSoundManager *cSoundManager::instance() {
  if (!s_cSoundManager) {
    s_cSoundManager = new cSoundManager();
    gFMODResult = FMOD::System_Create(&gSoundSystem); // Create system object
    gFMODResult = gSoundSystem->getVersion(&s_cSoundManager->version);

    ////Validate version
    if (s_cSoundManager->version < FMOD_VERSION) {
      // std::cout << "FMOD lib version doesn't match header version " <<
      // std::endl;
    }
    gFMODResult = gSoundSystem->init(32, FMOD_INIT_NORMAL, 0); // Initialize
    // gFMODResult = gSoundSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS,
    // &gEffect_dsplowpass);	//(lowpass)A low-pass filter is a filter that
    // passes signals with a frequency lower than a certain cutoff frequency and
    // attenuates signals with frequencies higher than the cutoff frequency
    // gFMODResult = gSoundSystem->createDSPByType(FMOD_DSP_TYPE_HIGHPASS,
    // &gEffect_dsphighpass);  //(highpass)A high-pass filter is an electronic
    // filter that passes signals with a frequency higher than a certain cutoff
    // frequency and attenuates signals with frequencies lower than the cutoff
    // frequency
    // gFMODResult = gSoundSystem->createDSPByType(FMOD_DSP_TYPE_ECHO,
    // &gEffect_dspecho);          //(echo)This unit produces an echo on the
    // sound and fades out at the desired rate
    // gFMODResult = gSoundSystem->createDSPByType(FMOD_DSP_TYPE_FLANGE,
    // &gEffect_dspflange);      //(flange)effect produced by mixing two
    // identical signals together, one signal delayed by a small and gradually
    // changing period
    // FMOD_DSP_TYPE_FLANGE
    //	Param1 : FMOD_DSP_FLANGE_MIX (Type:float) - Percentage of wet signal in
    //mix. 0 to 100. Default = 50.
    //	Param2 : FMOD_DSP_FLANGE_DEPTH (Type:float) - Flange depth (percentage
    //of 40ms delay). 0.01 to 1.0. Default = 1.0.
    //	Param2 : FMOD_DSP_FLANGE_RATE (Type:float) - Flange speed in hz. 0.0 to
    //20.0. Default = 0.1.
    //	gFMODResult = gEffect_dspflange->setParameterFloat(2, 20.f);
  }
  return s_cSoundManager;
}

void cSoundManager::playSound(std::string soundName, int channelID,
                              FMOD_VECTOR channelPos, FMOD_VECTOR channelVel) {
  gFMODResult = gSoundSystem->playSound(map_gSounds[soundName], NULL, false,
                                        &gChannels.at(channelID));
  gFMODResult =
      gChannels.at(channelID)->set3DAttributes(&channelPos, &channelVel);
}

bool cSoundManager::loadSound(rapidxml::xml_node<> *soundNode) {

  std::string path = soundNode->first_attribute("path")->value();

  ////1. spider_crawl.mp3
  FMOD::Sound *tempSound;

  int mode = 0;

  if ((std::string)soundNode->first_attribute("mode")->value() == "FMOD_3D") {
    // It's a 3d sound
    mode = FMOD_3D;
  } else {
    mode = FMOD_2D;
  }

  if (g_pXML_Utils->attributeToBool(soundNode->first_attribute("isStream")))
    gFMODResult = gSoundSystem->createStream(path.c_str(), mode, 0, &tempSound);
  else
    gFMODResult = gSoundSystem->createSound(path.c_str(), mode, 0, &tempSound);
  gFMODResult = tempSound->set3DMinMaxDistance(1.0f, 5000.0f);
  if (g_pXML_Utils->attributeToBool(soundNode->first_attribute("boolLoop")))
    gFMODResult = tempSound->setMode(FMOD_LOOP_NORMAL);
  map_gSounds[soundNode->first_attribute("name")->value()] = tempSound;
  ::gSounds.push_back(tempSound);

  // unsigned int lenbytes = 0;
  // float hz;
  ////Get length in bytes
  // gFMODResult = tempSound->getLength(&lenbytes, FMOD_TIMEUNIT_PCMBYTES);
  //
  // std::cout << "Compressed Sample length in bytes: " << lenbytes << "
  // bytes\n";
  // unsigned int lenms = 0;
  // gFMODResult = tempSound->getLength(&lenms, FMOD_TIMEUNIT_MS);
  // std::cout << "Compressed Sample length in MS: " << lenms << " ms\n";
  // unsigned int lensamples;
  ////Get length in samples
  // gFMODResult = tempSound->getLength(&lensamples, FMOD_TIMEUNIT_PCM);
  // std::cout << "Compressed Sample length in Samples: " << lenms << "
  // samples\n";

  gNextSoundID++;
  // ERRCHECK(result);
  return true;

  //	result = channel[0]->set3DAttributes(&listenerposition, &vel);
}

void cSoundManager::update() {
  // Important to update system
  gFMODResult = ::gSoundSystem->update();
}

void cSoundManager::init() {
  // gFMODResult = sound1->set3DMinMaxDistance(0.5f * UNITS_PER_METER, 5000.0f *
  // UNITS_PER_METER);
}
