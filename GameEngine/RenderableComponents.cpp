#define GLM_FORCE_SWIZZLE
#include "cComponentManager.h"

#include "global.h"

// THE FOLLOWING CODE WAS CREATED BY:
// Name: Michael Feeney
// E-mail : MFeeney@FanshaweC.ca
#include "windows.h"
#include <stdlib.h> // srand, rand
class lightMath {
public:
  lightMath(){};
  // http://stackoverflow.com/questions/686353/c-random-float-number-generation
  float getRandFloat(float LO, float HI) {
    float r3 =
        LO +
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
    return r3;
  }

  float calcApproxDistFromAtten(float targetLightLevel, float attenConst,
                                float attenLinear, float attenQuad,
                                float accuracy, float infiniteDistance,
                                unsigned int maxIterations = 50) {
    const float DEFAULTINFINITEDISTANCE = 10000.0f;
    // static
    const float DEFAULDIFFUSEACCURACYTHRESHOLD = 0.001f;

    // See if the accuracy being set it too big for the targetLightLevel, unless
    // targetLightLevel is actually zero (0.0f)
    // If it's actually zero, then adjusting the accuracy to a tenth of zero
    // would give... wait for it...
    //	zero, and we would max out the iterations
    if (targetLightLevel != 0.0f) {
      if ((accuracy * 10.0f) >=
          targetLightLevel * 10.0f) { // Adjust the accuracy by a hundredth
        accuracy = targetLightLevel / 10.0f;
      }
    } // if ( targetLightLevel != 0.0f )

    float targetLightLevelLow = targetLightLevel - accuracy;
    // if ( targetLightLevelLow <= 0.0f )	{ targetLightLevel = 0.0f; }
    float targetLightLevelHigh = targetLightLevel + accuracy;

    // See if we're getting a value at infinite. i.e. at 'infinite distance', is
    // the light level too high already
    // if ( calcDiffuseFromAttenByDistance( DEFAULTINFINITEDISTANCE, accuracy )
    // > targetLightLevelHigh )
    float diffuseCalculated = calcDiffuseFromAttenByDistance(
        DEFAULTINFINITEDISTANCE, attenConst, attenLinear, attenQuad);

    if (diffuseCalculated > targetLightLevelHigh) { // Yes, so we can never get
                                                    // down to this light level
      return DEFAULTINFINITEDISTANCE;
    }

    // There is a light level somewhere between a distance of 0.0 to
    // DEFAULTINFINITEDISTANCE
    // Set initial guesses
    float distanceGuessLow = 0.0f;
    float distanceGuessHigh = DEFAULTINFINITEDISTANCE;

    unsigned int iterationCount = 0;
    while (iterationCount < maxIterations) {
      // Pick a distance between the high and low
      float curDistanceGuess =
          ((distanceGuessHigh - distanceGuessLow) / 2.0f) + distanceGuessLow;
      //
      float curDiffuseAtGuessDistance = calcDiffuseFromAttenByDistance(
          curDistanceGuess, attenConst, attenLinear, attenQuad);
      // Could be three possibilities: too low, too high, or in between
      if (curDiffuseAtGuessDistance <
          targetLightLevelLow) { // Light is too dark, so distance is to HIGH.
                                 // Reduce and guess again.
        distanceGuessHigh =
            curDistanceGuess; // Lower the high limit for the guesses
      } else if (curDiffuseAtGuessDistance >
                 targetLightLevelHigh) { // Light is too bright, so distance is
                                         // to LOW. Increase and guess again
        distanceGuessLow = curDistanceGuess;
      } else { // Nailed it! Light level is within range, so return this
               // distance
        return curDistanceGuess;
      }

      iterationCount++;

    } // while ( iterationCount < maxIterations )
      // If we are here, then we ran out of iterations.
      // Pick a distance between the low and high
    float distance = (distanceGuessHigh - distanceGuessLow) / 2.0f;

    return distance;
  }

  // const float CLightDescription::DEFAULTZEROTHRESHOLD = 0.0001f;

  float calcDiffuseFromAttenByDistance(float distance, float attenConst,
                                       float attenLinear, float attenQuad,
                                       float zeroThreshold = 0.0001f) {
    const float DEFAULTZEROTHRESHOLD = 0.0001f;

    float diffuse = 1.0f; // Assume full brightness

    float denominator =
        attenConst + attenLinear * distance + attenQuad * distance * distance;
    // Is this zero (we don't want a divide by zero, do we)?
    if (denominator <= zeroThreshold) { // Let's just say it's zero, shall we?
      diffuse = 1.0f;
    } else {
      float atten = 1.0f / denominator;
      diffuse *= atten;
      if (diffuse > 1.0f) {
        diffuse = 1.0f;
      }
    } // if ( denominator <= zeroThreshold )
    return diffuse;
  }
};
// END OF COPIED CODE
class RenderingComponent : public cComponent {
public:
  virtual ~RenderingComponent();
  virtual cComponent *getType();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  void draw();
  virtual void command(std::string method, std::string value) {
    if (method == "SetActiveLight") {
                        for each(cLight * light in m_vec_pLights) {
                            if (value == "SpotLight") {
                              if (light->typeFlag == 2)
                                light->isEnabled = true;
                              else
                                light->isEnabled = false;
                            } else if (value == "DirectionalLight") {
                              if (light->typeFlag == 1)
                                light->isEnabled = true;
                              else
                                light->isEnabled = false;
                            } else if (value == "PointLight") {
                              if (light->typeFlag == 0)
                                light->isEnabled = true;
                              else
                                light->isEnabled = false;
                            }
                          }
    }
  }

protected:
  std::vector<cMesh *> m_vec_pMeshes;
  std::vector<cLight *>
      m_vec_pLights; // Each rendering component can have light(s)
  std::vector<cTexture *> m_vec_pTextures;
};
REGISTER_COMPONENT(RenderingComponent, "RenderingComponent")

RenderingComponent::~RenderingComponent() {
  m_vec_pComponents.clear();
  for (std::vector<cMesh *>::iterator meshIter = m_vec_pMeshes.begin();
       meshIter != m_vec_pMeshes.end(); meshIter++) {
    delete *meshIter;
  }
  for (std::vector<cLight *>::iterator lightIter = m_vec_pLights.begin();
       lightIter != m_vec_pLights.end(); lightIter++)
    delete *lightIter;
}

cComponent *RenderingComponent::getType() {
  return this; // return entire object for now..
}

void RenderingComponent::update() {
  for (std::vector<cMesh *>::iterator iter = this->m_vec_pMeshes.begin();
       iter != this->m_vec_pMeshes.end(); iter++) {
    // Current mesh is being controlled so update it's position to the
    // controller's position..
    glm::mat4 translationMatrix = this->getTransform();
    glm::mat4 rotationMatrix = (*iter)->getOrientation();

    (*iter)->m_curTransform = glm::translate(
        translationMatrix,
        glm::vec3((*iter)->m_pBodyOffset[3].x, (*iter)->m_pBodyOffset[3].y,
                  (*iter)->m_pBodyOffset[3].z));
    (*iter)->m_curTransform *= rotationMatrix;
  }
  for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
           this->m_vec_pComponents.begin();
       iter != this->m_vec_pComponents.end(); iter++) {
    (*iter)->setTransform(this->getTransform());
    (*iter)->update();
  }
  this->draw();
}

bool RenderingComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
  rapidxml::xml_attribute<char> *att =
      componentNode->first_attribute("componentID");
  if (att != nullptr)
    this->componentID =
        atoi(componentNode->first_attribute("componentID")->value());
  else {
    this->componentID = gNextComponentID;
    gNextComponentID++;
  }
  cComponentManager::map_ComponentIDToComponent[this->componentID] = this;
  for (rapidxml::xml_node<> *cTextureComponent_node =
           componentNode->first_node("TextureComponent");
       cTextureComponent_node;
       cTextureComponent_node =
           cTextureComponent_node->next_sibling("TextureComponent")) {

    cTexture *tempTexture = new cTexture();
    tempTexture->textureID =
        &g_pMapTextureNameToID[cTextureComponent_node->first_attribute("name")
                                   ->value()];
    this->m_vec_pTextures.push_back(tempTexture);
  }
  for (rapidxml::xml_node<> *cMeshComponent_node =
           componentNode->first_node("MeshComponent");
       cMeshComponent_node;
       cMeshComponent_node = cMeshComponent_node->next_sibling()) {
    if ((std::string)cMeshComponent_node->name() == "MeshComponent") {
      cMesh *tempMesh = new cMesh();

      float scale =
          std::stof(cMeshComponent_node->first_attribute("scale")->value());
      tempMesh->scale = glm::vec3(scale);
      tempMesh->m_pBodyOffset = glm::translate(
          tempMesh->m_pBodyOffset,
          glm::vec3(
              scale *
                  std::stof(
                      cMeshComponent_node->first_attribute("offsetX")->value()),
              scale *
                  std::stof(
                      cMeshComponent_node->first_attribute("offsetY")->value()),
              scale * std::stof(cMeshComponent_node->first_attribute("offsetZ")
                                    ->value())));

      tempMesh->m_pBodyOffset = glm::rotate(
          tempMesh->m_pBodyOffset,
          std::stof(cMeshComponent_node->first_attribute("preRotX")->value()),
          glm::vec3(0.0f, 1.0f, 0.0f));
      // this->entity()->matrix = glm::rotate(this->entity()->matrix,
      // std::stof(entityNode->first_attribute("preRotY")->value()),
      // glm::vec3(1.0f, 0.0f, 0.0f));
      // this->entity()->matrix = glm::rotate(this->entity()->matrix,
      // std::stof(entityNode->first_attribute("preRotZ")->value()),
      // glm::vec3(0.0f, 0.0f, 1.0f));
      tempMesh->m_pColor = new glm::vec4();
      *tempMesh->m_pColor = *tempMesh->m_pColor = glm::vec4(
          std::stof(cMeshComponent_node->first_attribute("r")->value()),
          std::stof(cMeshComponent_node->first_attribute("g")->value()),
          std::stof(cMeshComponent_node->first_attribute("b")->value()), 1.0f);
      tempMesh->alpha =
          std::stof(cMeshComponent_node->first_attribute("alpha")->value());

      tempMesh->isWireframe = g_pXML_Utils->attributeToBool(
          cMeshComponent_node->first_attribute("isWireframe"));
      tempMesh->m_pMeshEntry =
          &g_pMeshManager->m_MapMeshNameTocMeshEntry
               [cMeshComponent_node->first_attribute("meshName")->value()];
      tempMesh->isEnabled = true;
      if (tempMesh->alpha < 1.0f)
        gTransparentMeshes.push_back(tempMesh);

      this->m_vec_pMeshes.push_back(tempMesh);
    }
  }

  for (rapidxml::xml_node<> *cLightComponent = componentNode->first_node();
       cLightComponent; cLightComponent = cLightComponent->next_sibling()) {
    if ((std::string)cLightComponent->name() == "LightComponent") {
      if ((std::string)cLightComponent->first_attribute("type")->value() ==
          "PointLight") {
        cLight *tempLight = new cLight();
        tempLight->isEnabled = g_pXML_Utils->attributeToBool(
            cLightComponent->first_attribute("isEnabled"));

        tempLight->typeFlag = 0;
        tempLight->offset = glm::vec3(
            std::stof(cLightComponent->first_attribute("offsetX")->value()),
            std::stof(cLightComponent->first_attribute("offsetY")->value()),
            std::stof(cLightComponent->first_attribute("offsetZ")->value()));
        tempLight->diffuse = glm::vec3(
            std::stof(cLightComponent->first_attribute("diffuseR")->value()),
            std::stof(cLightComponent->first_attribute("diffuseG")->value()),
            std::stof(cLightComponent->first_attribute("diffuseB")->value()));
        tempLight->ambient = glm::vec3(
            std::stof(cLightComponent->first_attribute("ambientR")->value()),
            std::stof(cLightComponent->first_attribute("ambientG")->value()),
            std::stof(cLightComponent->first_attribute("ambientB")->value()));
        tempLight->specular = glm::vec3(
            std::stof(cLightComponent->first_attribute("specularR")->value()),
            std::stof(cLightComponent->first_attribute("specularG")->value()),
            std::stof(cLightComponent->first_attribute("specularB")->value()));

        tempLight->specularPower = std::stof(
            cLightComponent->first_attribute("specularPower")->value());
        tempLight->attenConst =
            std::stof(cLightComponent->first_attribute("attenConst")->value());
        tempLight->attenLinear =
            std::stof(cLightComponent->first_attribute("attenLinear")->value());
        tempLight->attenQuad =
            std::stof(cLightComponent->first_attribute("attenQuad")->value());
        tempLight->lightID = gLights.size();
        std::string s12 =
            "lights[" + std::to_string(tempLight->lightID) + "].IsEnabled";
        tempLight->gUniformId_IsEnabled =
            glGetUniformLocation(gProgramID, s12.c_str());
        std::string s11 =
            "lights[" + std::to_string(tempLight->lightID) + "].TypeFlag";
        tempLight->gUniformId_TypeFlag =
            glGetUniformLocation(gProgramID, s11.c_str());
        std::string s1 =
            "lights[" + std::to_string(tempLight->lightID) + "].Position";
        tempLight->gUniformId_Position =
            glGetUniformLocation(gProgramID, s1.c_str());
        std::string s2 =
            "lights[" + std::to_string(tempLight->lightID) + "].Diffuse";
        tempLight->gUniformId_Diffuse =
            glGetUniformLocation(gProgramID, s2.c_str());
        std::string s3 =
            "lights[" + std::to_string(tempLight->lightID) + "].Specular";
        tempLight->gUniformId_Specular =
            glGetUniformLocation(gProgramID, s3.c_str());
        std::string s4 =
            "lights[" + std::to_string(tempLight->lightID) + "].Ambient";
        tempLight->gUniformId_Ambient =
            glGetUniformLocation(gProgramID, s4.c_str());
        std::string s5 =
            "lights[" + std::to_string(tempLight->lightID) + "].SpecularPower";
        tempLight->gUniformId_SpecularPower =
            glGetUniformLocation(gProgramID, s5.c_str());
        std::string s6 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationConst";
        tempLight->gUniformId_AttenuationConst =
            glGetUniformLocation(gProgramID, s6.c_str());
        std::string s7 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationLinear";
        tempLight->gUniformId_AttenuationLinear =
            glGetUniformLocation(gProgramID, s7.c_str());
        std::string s8 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationQuad";
        tempLight->gUniformId_AttenuationQuad =
            glGetUniformLocation(gProgramID, s8.c_str());

        cMesh *tempMesh = new cMesh();

        tempMesh->m_pMeshEntry =
            &g_pMeshManager->m_MapMeshNameTocMeshEntry["Sphere"];
        tempMesh->isWireframe = true;
        // tempMesh->m_pBodyOffset += this->m_Transform;
        tempMesh->m_pBodyOffset = tempMesh->m_pBodyOffset + tempLight->matrix;

        lightMath *tempLightMath = new lightMath();
        float distanceAt50Percent = tempLightMath->calcApproxDistFromAtten(
            0.5f, // desired brightness
            tempLight->attenConst, tempLight->attenLinear, tempLight->attenQuad,
            0.001f,     // Accuracy (how close to 0.75 are we getting
            100000.0f); // "Infinite" distance (so give up)
        // DrawDebugBall(glm::vec3(::lightPos.x, ::lightPos.y, ::lightPos.z),
        //	glm::vec3(1.0f, 0.0f, 0.0f), distanceAt75Percent);

        tempMesh->scale = glm::vec3(distanceAt50Percent);

        tempMesh->m_pColor = new glm::vec4();
        *tempMesh->m_pColor = *tempMesh->m_pColor =
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        tempMesh->alpha = 0.2f;
        if (tempMesh->alpha < 1.0f)
          gTransparentMeshes.push_back(tempMesh);

        tempLight->vec_DebugShapes.push_back(tempMesh);

        gLights.push_back(tempLight);
        this->m_vec_pLights.push_back(tempLight);
        delete tempLightMath;
      } else if ((std::string)cLightComponent->first_attribute("type")
                     ->value() == "SpotLight") {
        cLight *tempLight = new cLight();
        tempLight->isEnabled = g_pXML_Utils->attributeToBool(
            cLightComponent->first_attribute("isEnabled"));
        tempLight->typeFlag = 2;
        tempLight->offset = glm::vec3(
            std::stof(cLightComponent->first_attribute("offsetX")->value()),
            std::stof(cLightComponent->first_attribute("offsetY")->value()),
            std::stof(cLightComponent->first_attribute("offsetZ")->value()));

        glm::vec3 target = glm::vec3(
            std::stof(cLightComponent->first_attribute("directionX")->value()),
            std::stof(cLightComponent->first_attribute("directionY")->value()),
            std::stof(cLightComponent->first_attribute("directionZ")->value()));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        tempLight->directionOffset =
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), target, up);
        tempLight->coneAngle =
            std::stof(cLightComponent->first_attribute("coneAngle")->value());
        tempLight->specularPower = std::stof(
            cLightComponent->first_attribute("specularPower")->value());
        tempLight->diffuse = glm::vec3(
            std::stof(cLightComponent->first_attribute("diffuseR")->value()),
            std::stof(cLightComponent->first_attribute("diffuseG")->value()),
            std::stof(cLightComponent->first_attribute("diffuseB")->value()));
        tempLight->ambient = glm::vec3(
            std::stof(cLightComponent->first_attribute("ambientR")->value()),
            std::stof(cLightComponent->first_attribute("ambientG")->value()),
            std::stof(cLightComponent->first_attribute("ambientB")->value()));
        tempLight->specular = glm::vec3(
            std::stof(cLightComponent->first_attribute("specularR")->value()),
            std::stof(cLightComponent->first_attribute("specularG")->value()),
            std::stof(cLightComponent->first_attribute("specularB")->value()));

        tempLight->attenConst =
            std::stof(cLightComponent->first_attribute("attenConst")->value());
        tempLight->attenLinear =
            std::stof(cLightComponent->first_attribute("attenLinear")->value());
        tempLight->attenQuad =
            std::stof(cLightComponent->first_attribute("attenQuad")->value());
        tempLight->lightID = gLights.size();
        std::string s12 =
            "lights[" + std::to_string(tempLight->lightID) + "].IsEnabled";
        tempLight->gUniformId_IsEnabled =
            glGetUniformLocation(gProgramID, s12.c_str());
        std::string s11 =
            "lights[" + std::to_string(tempLight->lightID) + "].TypeFlag";
        tempLight->gUniformId_TypeFlag =
            glGetUniformLocation(gProgramID, s11.c_str());
        std::string s1 =
            "lights[" + std::to_string(tempLight->lightID) + "].Position";
        tempLight->gUniformId_Position =
            glGetUniformLocation(gProgramID, s1.c_str());
        std::string s9 =
            "lights[" + std::to_string(tempLight->lightID) + "].Direction";
        tempLight->gUniformId_Direction =
            glGetUniformLocation(gProgramID, s9.c_str());
        std::string s10 =
            "lights[" + std::to_string(tempLight->lightID) + "].ConeAngle";
        tempLight->gUniformId_ConeAngle =
            glGetUniformLocation(gProgramID, s10.c_str());
        std::string s2 =
            "lights[" + std::to_string(tempLight->lightID) + "].Diffuse";
        tempLight->gUniformId_Diffuse =
            glGetUniformLocation(gProgramID, s2.c_str());
        std::string s3 =
            "lights[" + std::to_string(tempLight->lightID) + "].Specular";
        tempLight->gUniformId_Specular =
            glGetUniformLocation(gProgramID, s3.c_str());
        std::string s4 =
            "lights[" + std::to_string(tempLight->lightID) + "].Ambient";
        tempLight->gUniformId_Ambient =
            glGetUniformLocation(gProgramID, s4.c_str());
        std::string s5 =
            "lights[" + std::to_string(tempLight->lightID) + "].SpecularPower";
        tempLight->gUniformId_SpecularPower =
            glGetUniformLocation(gProgramID, s5.c_str());
        std::string s6 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationConst";
        tempLight->gUniformId_AttenuationConst =
            glGetUniformLocation(gProgramID, s6.c_str());
        std::string s7 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationLinear";
        tempLight->gUniformId_AttenuationLinear =
            glGetUniformLocation(gProgramID, s7.c_str());
        std::string s8 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationQuad";
        tempLight->gUniformId_AttenuationQuad =
            glGetUniformLocation(gProgramID, s8.c_str());

        // cMesh* tempMesh = new cMesh();
        //
        // tempMesh->m_pMeshEntry =
        // &g_pMeshManager->m_MapMeshNameTocMeshEntry["Sphere"];
        // tempMesh->isWireframe = true;
        // tempMesh->m_pBodyOffset += this->m_Transform;
        //
        // lightMath * tempLightMath = new lightMath();
        // float distanceAt10Percent = tempLightMath->calcApproxDistFromAtten(
        //	0.50f, // desired brightness
        //	tempLight->attenConst, tempLight->attenLinear,
        //tempLight->attenQuad,
        //	0.001f, // Accuracy (how close to 0.75 are we getting
        //	100000.0f);  // "Infinite" distance (so give up)
        //				 //DrawDebugBall(glm::vec3(::lightPos.x,
        //::lightPos.y, ::lightPos.z),
        //				 //	glm::vec3(1.0f, 0.0f, 0.0f),
        //distanceAt75Percent);
        //
        // tempMesh->scale = glm::vec3(distanceAt10Percent);
        //
        // tempMesh->m_pColor = new glm::vec4();
        //*tempMesh->m_pColor =
        //	*tempMesh->m_pColor =
        //	glm::vec4(1.0f,
        //		0.0f,
        //		0.0f,
        //		1.0f);
        // tempMesh->alpha = 0.2f;
        // if (tempMesh->alpha < 1.0f)
        //	gTransparentMeshes.push_back(tempMesh);
        //
        // tempLight->vec_DebugShapes.push_back(tempMesh);

        gLights.push_back(tempLight);
        this->m_vec_pLights.push_back(tempLight);
        // delete tempLightMath;
      } else if ((std::string)cLightComponent->first_attribute("type")
                     ->value() == "DirectionalLight") {
        cLight *tempLight = new cLight();
        tempLight->isEnabled = g_pXML_Utils->attributeToBool(
            cLightComponent->first_attribute("isEnabled"));
        tempLight->typeFlag = 1;
        glm::vec3 target = glm::vec3(
            std::stof(cLightComponent->first_attribute("directionX")->value()),
            std::stof(cLightComponent->first_attribute("directionY")->value()),
            std::stof(cLightComponent->first_attribute("directionZ")->value()));
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        tempLight->directionOffset =
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), target, up);
        tempLight->offset = glm::vec3(
            std::stof(cLightComponent->first_attribute("offsetX")->value()),
            std::stof(cLightComponent->first_attribute("offsetY")->value()),
            std::stof(cLightComponent->first_attribute("offsetZ")->value()));
        tempLight->diffuse = glm::vec3(
            std::stof(cLightComponent->first_attribute("diffuseR")->value()),
            std::stof(cLightComponent->first_attribute("diffuseG")->value()),
            std::stof(cLightComponent->first_attribute("diffuseB")->value()));
        tempLight->ambient = glm::vec3(
            std::stof(cLightComponent->first_attribute("ambientR")->value()),
            std::stof(cLightComponent->first_attribute("ambientG")->value()),
            std::stof(cLightComponent->first_attribute("ambientB")->value()));
        tempLight->specular = glm::vec3(
            std::stof(cLightComponent->first_attribute("specularR")->value()),
            std::stof(cLightComponent->first_attribute("specularG")->value()),
            std::stof(cLightComponent->first_attribute("specularB")->value()));

        tempLight->specularPower = std::stof(
            cLightComponent->first_attribute("specularPower")->value());
        tempLight->attenConst =
            std::stof(cLightComponent->first_attribute("attenConst")->value());
        tempLight->attenLinear =
            std::stof(cLightComponent->first_attribute("attenLinear")->value());

        tempLight->attenQuad =
            std::stof(cLightComponent->first_attribute("attenQuad")->value());
        tempLight->lightID = gLights.size();
        std::string s12 =
            "lights[" + std::to_string(tempLight->lightID) + "].IsEnabled";
        tempLight->gUniformId_IsEnabled =
            glGetUniformLocation(gProgramID, s12.c_str());
        std::string s11 =
            "lights[" + std::to_string(tempLight->lightID) + "].TypeFlag";
        tempLight->gUniformId_TypeFlag =
            glGetUniformLocation(gProgramID, s11.c_str());
        std::string s1 =
            "lights[" + std::to_string(tempLight->lightID) + "].Position";
        tempLight->gUniformId_Position =
            glGetUniformLocation(gProgramID, s1.c_str());
        std::string s9 =
            "lights[" + std::to_string(tempLight->lightID) + "].Direction";
        tempLight->gUniformId_Direction =
            glGetUniformLocation(gProgramID, s9.c_str());
        std::string s2 =
            "lights[" + std::to_string(tempLight->lightID) + "].Diffuse";
        tempLight->gUniformId_Diffuse =
            glGetUniformLocation(gProgramID, s2.c_str());
        std::string s3 =
            "lights[" + std::to_string(tempLight->lightID) + "].Specular";
        tempLight->gUniformId_Specular =
            glGetUniformLocation(gProgramID, s3.c_str());
        std::string s4 =
            "lights[" + std::to_string(tempLight->lightID) + "].Ambient";
        tempLight->gUniformId_Ambient =
            glGetUniformLocation(gProgramID, s4.c_str());
        std::string s5 =
            "lights[" + std::to_string(tempLight->lightID) + "].SpecularPower";
        tempLight->gUniformId_SpecularPower =
            glGetUniformLocation(gProgramID, s5.c_str());
        std::string s6 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationConst";
        tempLight->gUniformId_AttenuationConst =
            glGetUniformLocation(gProgramID, s6.c_str());
        std::string s7 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationLinear";
        tempLight->gUniformId_AttenuationLinear =
            glGetUniformLocation(gProgramID, s7.c_str());
        std::string s8 = "lights[" + std::to_string(tempLight->lightID) +
                         "].AttenuationQuad";
        tempLight->gUniformId_AttenuationQuad =
            glGetUniformLocation(gProgramID, s8.c_str());

        // cMesh* tempMesh = new cMesh();
        //
        // tempMesh->m_pMeshEntry =
        // &g_pMeshManager->m_MapMeshNameTocMeshEntry["Sphere"];
        // tempMesh->isWireframe = true;
        // tempMesh->m_pBodyOffset += this->m_Transform;
        //
        // lightMath * tempLightMath = new lightMath();
        // float distanceAt10Percent = tempLightMath->calcApproxDistFromAtten(
        //	0.50f, // desired brightness
        //	tempLight->attenConst, tempLight->attenLinear,
        //tempLight->attenQuad,
        //	0.001f, // Accuracy (how close to 0.75 are we getting
        //	100000.0f);  // "Infinite" distance (so give up)
        //				 //DrawDebugBall(glm::vec3(::lightPos.x,
        //::lightPos.y, ::lightPos.z),
        //				 //	glm::vec3(1.0f, 0.0f, 0.0f),
        //distanceAt75Percent);
        //
        // tempMesh->scale = glm::vec3(distanceAt10Percent);
        //
        // tempMesh->m_pColor = new glm::vec4();
        //*tempMesh->m_pColor =
        //	*tempMesh->m_pColor =
        //	glm::vec4(1.0f,
        //		0.0f,
        //		0.0f,
        //		1.0f);
        // tempMesh->alpha = 0.2f;
        // if (tempMesh->alpha < 1.0f)
        //	gTransparentMeshes.push_back(tempMesh);
        //
        // tempLight->vec_DebugShapes.push_back(tempMesh);
        //
        gLights.push_back(tempLight);
        this->m_vec_pLights.push_back(tempLight);
        // delete tempLightMath;
      }
    }
  }
  for (rapidxml::xml_node<> *cComponent_node = componentNode->first_node();
       cComponent_node; cComponent_node = cComponent_node->next_sibling()) {
    std::string name = cComponent_node->name();
    if (name == "SoundEmitterComponent") {
      std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
          cComponentManager::create_Component(name));
      tempComponent->loadFromXML(cComponent_node);
      this->m_vec_pComponents.push_back(tempComponent);

    } else if (name == "LuaComponent") {
      std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
          cComponentManager::create_Component(name));
      tempComponent->loadFromXML(cComponent_node);
      this->m_vec_pComponents.push_back(tempComponent);
    }
  }

  return true;
}

void RenderingComponent::draw() {
  // if (this->entity()->isActive != false)
  //{
  for (std::vector<cMesh *>::iterator iter = this->m_vec_pMeshes.begin();
       iter != this->m_vec_pMeshes.end(); iter++) {
    if ((*iter)->alpha >= 1.0f) // Transparent meshes are drawn in the main loop
    {

      // glUseProgram(g_Physics_ProgramID);

      glEnable(GL_BLEND);
      // glBlendEquation(GL_FUNC_ADD);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_DEPTH_TEST);

      // glUniform1i(gUniformId_NumTextures, m_vec_pTextures.size());
      // int curTexture = 0;
      // for each(cTexture* texture in this->m_vec_pTextures)
      //{
      //	std::string text = "Texture" + std::to_string(curTexture);
      //	glUniform1i(glGetUniformLocation(gProgramID, text.c_str()),
      //*texture->textureID);
      //	curTexture++;
      //
      //}

      // glEnable(GL_COLOR_MATERIAL);
      if ((*iter)->isWireframe) {        // Turn off backface culling
                                         // Enable "wireframe" polygon mode
        glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
                                         // you can pass here
                      GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
        glDisable(GL_CULL_FACE);
      } else { // "Regular" rendering:
        // Turn on backface culling
        // Turn polygon mode to solid (Fill)
        glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
                                         // you can pass here
                      GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL
      }
      glUniformMatrix4fv(
          gUniformId_ModelMatrix, 1, GL_FALSE,
          glm::value_ptr(glm::scale((*iter)->m_curTransform, (*iter)->scale)));
      glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
                         glm::value_ptr((*iter)->getOrientation()));
      glUniform4fv(gUniformId_ModelColor, 1,
                   glm::value_ptr(*(*iter)->m_pColor));

      glUniform1f(gUniformId_Alpha, (*iter)->alpha);
      glDrawElementsBaseVertex(
          GL_TRIANGLES, (*iter)->m_pMeshEntry->NumgIndices, GL_UNSIGNED_INT,
          (void *)(sizeof(unsigned int) * (*iter)->m_pMeshEntry->BaseIndex),
          (*iter)->m_pMeshEntry->BaseVertex);
      //		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      //}
    }
  }

  for (std::vector<cLight *>::iterator iter = this->m_vec_pLights.begin();
       iter != this->m_vec_pLights.end(); iter++) {
    (*iter)->matrix = glm::translate(m_Transform, (*iter)->offset);
    (*iter)->direction =
        (this->getOrientation() * (*iter)->directionOffset)[2].xyz;
    if ((*iter)->isEnabled && g_bool_ToggleLights)
      for (std::vector<cMesh *>::iterator iter2 =
               (*iter)->vec_DebugShapes.begin();
           iter2 != (*iter)->vec_DebugShapes.end(); iter2++) {

        (*iter2)->m_curTransform = this->m_Transform;
        (*iter2)->isEnabled = g_bool_DebugShapes;
        if ((*iter2)->alpha >=
            1.0f) // Transparent meshes are drawn in the main loop
        {
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glEnable(GL_DEPTH_TEST);
          if ((*iter2)->isWireframe) {       // Turn off backface culling
                                             // Enable "wireframe" polygon mode
            glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only
                                             // thing you can pass here
                          GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
            glDisable(GL_CULL_FACE);
          } else { // "Regular" rendering:
            // Turn on backface culling
            // Turn polygon mode to solid (Fill)
            glCullFace(
                GL_FRONT_AND_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
            glEnable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only
                                             // thing you can pass here
                          GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL
          }
          glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE,
                             glm::value_ptr((*iter2)->m_curTransform));
          glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
                             glm::value_ptr((*iter2)->getOrientation()));
          glUniform4fv(gUniformId_ModelColor, 1,
                       glm::value_ptr(*(*iter2)->m_pColor));

          glUniform1f(gUniformId_Alpha, (*iter2)->alpha);
          glDrawElementsBaseVertex(
              GL_TRIANGLES, (*iter2)->m_pMeshEntry->NumgIndices,
              GL_UNSIGNED_INT, (void *)(sizeof(unsigned int) *
                                        (*iter2)->m_pMeshEntry->BaseIndex),
              (*iter2)->m_pMeshEntry->BaseVertex);
        }
      }
    else {
      for (std::vector<cMesh *>::iterator iter2 =
               (*iter)->vec_DebugShapes.begin();
           iter2 != (*iter)->vec_DebugShapes.end(); iter2++) {
        (*iter2)->m_curTransform = this->m_Transform;
        (*iter2)->isEnabled = false;
      }
    }
  }
}
