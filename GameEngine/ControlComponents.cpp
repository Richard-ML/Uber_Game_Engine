#include "cComponentManager.h"
#include "global.h"
#include <sstream>

class AIControlComponent : public cComponent {
public:
  virtual ~AIControlComponent();
  virtual cComponent *getType();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  virtual void command(std::string method, std::string value) {
    if (method == "setFlockID") {
      flockTargetID = std::stoi(value);
    }
  }

protected:
  enum eAIState {
    STATE_WANDER, // OUTSIDE OF ATTENTION RADIUS
    STATE_EVADE,  // INSIDE ATTENTION RADIUS AND TARGET IS FACING THEM
    STATE_FLEE,   // NOT IMPL
    STATE_SEEK,   // If Curious
    STATE_PURSUE, // If Angry
    STATE_FLOCK
  };
  enum eAIMood {
    MOOD_ANGRY,
    MOOD_CURIOUS,
    MOOD_FLOCK,
    MOOD_FLOCKMASTER

  };
  int flockTargetID = 0;
  std::vector<cMesh *> m_vec_pMeshes;
  int m_TargetID;
  glm::vec4 stateColor = glm::vec4(1.0f);
  void draw();
  eAIState m_eAIState;
  eAIMood m_eAIMODD;
  float m_AttentionRadius;
  float m_AverageSpeed;
};
REGISTER_COMPONENT(AIControlComponent, "AIControlComponent")

AIControlComponent::~AIControlComponent() {
  m_vec_pComponents.clear();
  for (std::vector<cMesh *>::iterator meshIter = m_vec_pMeshes.begin();
       meshIter != m_vec_pMeshes.end(); meshIter++) {
    delete *meshIter;
  }
}

cComponent *AIControlComponent::getType() { return nullptr; }

void AIControlComponent::update() {
  glm::mat4 tempEyeMatrix = this->getTransform();
  glm::vec3 eyePos =
      glm::vec3(tempEyeMatrix[3].x, tempEyeMatrix[3].y, tempEyeMatrix[3].z);
  glm::mat4 tempTargetMatrix =
      cComponentManager::map_ComponentIDToComponent[m_TargetID]->getTransform();
  glm::vec3 targetPos = glm::vec3(tempTargetMatrix[3].x, tempTargetMatrix[3].y,
                                  tempTargetMatrix[3].z);
  float distanceEyeToTarget = glm::distance(targetPos, eyePos);
  float direction = 1.0f;
  glm::quat eyeOrientation;
  if (distanceEyeToTarget <= this->m_AttentionRadius &&
      this->m_eAIMODD != eAIMood::MOOD_FLOCKMASTER) {
    glm::vec3 eyeToTargetDifference =
        glm::vec3(tempEyeMatrix[3].x, tempEyeMatrix[3].y, tempEyeMatrix[3].z) -
        glm::vec3(tempTargetMatrix[3].x, tempTargetMatrix[3].y,
                  tempTargetMatrix[3].z);
    glm::vec3 playerForward = glm::vec3(
        tempTargetMatrix[2].x, tempTargetMatrix[2].y, tempTargetMatrix[2].z);
    float targetToEyeDot = glm::dot(eyeToTargetDifference, playerForward);

    if (targetToEyeDot > 0) {
      // Target is facing me..

      if (this->m_eAIMODD == eAIMood::MOOD_ANGRY) {
        this->stateColor = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f); // Orange
      } else if (this->m_eAIMODD == eAIMood::MOOD_CURIOUS) {
        this->stateColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f); // Purple
      }

      // Evade logic
      glm::quat targetOrientation = glm::quat_cast(tempTargetMatrix);
      glm::vec3 targetForward = targetOrientation * glm::vec3(0.0f, 0.0f, 1.0f);
      glm::vec3 rotationAxis = glm::cross(targetForward, eyeToTargetDifference);
      float yAxis = rotationAxis.y > 0.f ? 1.f : -1.f;
      rotationAxis = glm::vec3(0.f, yAxis, 0.f);
      eyeOrientation = glm::rotate(targetOrientation, (90.0f * 3.14f / 180.0f),
                                   rotationAxis);
    } else {
      // Target is looking away from me..

      if (this->m_eAIMODD == eAIMood::MOOD_ANGRY) {
        this->stateColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // Pursuit Logic
        tempTargetMatrix =
            glm::translate(tempTargetMatrix, glm::vec3(0.0f, 0.0f, 30.0f));
        targetPos = glm::vec3(tempTargetMatrix[3].x, tempTargetMatrix[3].y,
                              tempTargetMatrix[3].z);
        eyeOrientation = glm::conjugate( // Changes sign of x y and z
            glm::quat_cast( // Cast mat4x4 into a quaternion so that we can
                            // easily make changes to the entities orientation
                glm::lookAt(targetPos, eyePos,
                            glm::vec3(0.0f, 1.0f, 0.0f)))); // Make the current
                                                            // entity face the
                                                            // players position.
                                                            // From position
                                                            // (0.0f, 0.0f,
                                                            // 0.0f)

      } else if (this->m_eAIMODD == eAIMood::MOOD_CURIOUS) {
        eyeOrientation = glm::conjugate( // Changes sign of x y and z
            glm::quat_cast( // Cast mat4x4 into a quaternion so that we can
                            // easily make changes to the entities orientation
                glm::lookAt(targetPos, eyePos,
                            glm::vec3(0.0f, 1.0f, 0.0f)))); // Make the current
                                                            // entity face the
                                                            // players position.
                                                            // From position
                                                            // (0.0f, 0.0f,
                                                            // 0.0f)

        this->stateColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        // Approach Logic
        if (distanceEyeToTarget < this->m_AttentionRadius / 2.0f)
          direction = -1.f;
      }
    }
    glm::mat4 eyeResult = glm::mat4_cast(eyeOrientation);
    eyeResult[3].x = eyePos.x;
    eyeResult[3].y = eyePos.y;
    eyeResult[3].z = eyePos.z;

    this->setTransform(eyeResult);
    this->setTransform(glm::translate(
        this->getTransform(),
        glm::vec3(0.0f, 0.0f,
                  direction * glm::linearRand(0.0f, this->m_AverageSpeed))));

  } else {
    if (this->m_eAIMODD == eAIMood::MOOD_ANGRY) {
      this->stateColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    } else if (this->m_eAIMODD == eAIMood::MOOD_CURIOUS) {
      this->stateColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    } else if (this->m_eAIMODD ==
               eAIMood::MOOD_FLOCK) // || this->m_eAIMODD ==
                                    // eAIMood::MOOD_FLOCKMASTER)
    {

      glm::vec3 eyePos2 =
          glm::vec3(this->getTransform()[3].x, this->getTransform()[3].y,
                    this->getTransform()[3].z);
      glm::mat4 tempTargetMatrix2 =
          cComponentManager::map_ComponentIDToComponent[this->flockTargetID]
              ->getTransform();
      tempTargetMatrix2 =
          glm::translate(tempTargetMatrix2, glm::vec3(0.0f, 0.0f, 30.0f));
      glm::vec3 targetPos2 =
          glm::vec3(tempTargetMatrix2[3].x, tempTargetMatrix2[3].y,
                    tempTargetMatrix2[3].z);
      glm::mat4 eyeResult2 =
          glm::mat4_cast(glm::conjugate( // Changes sign of x y and z
              glm::quat_cast( // Cast mat4x4 into a quaternion so that we can
                              // easily make changes to the entities orientation
                  glm::lookAt(targetPos2, eyePos2,
                              glm::vec3(0.0f, 1.0f, 0.0f))))); // Make the
                                                               // current entity
                                                               // face the
                                                               // players
                                                               // position. From
                                                               // position
                                                               // (0.0f, 0.0f,
                                                               // 0.0f)

      eyeResult2[3].x = eyePos2.x;
      eyeResult2[3].y = eyePos2.y;
      eyeResult2[3].z = eyePos2.z;
      float distanceEyeToTarget2 = std::abs(glm::distance(targetPos2, eyePos2));
      if (distanceEyeToTarget2 > 200)
        this->setTransform(eyeResult2);
    } else if (this->m_eAIMODD == eAIMood::MOOD_FLOCKMASTER) {
      //	this->setTransform(glm::rotate(this->getTransform(),
      //glm::linearRand(-0.4f, 0.2f), glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    this->setTransform(glm::rotate(this->getTransform(),
                                   glm::linearRand(-0.1f, 0.1f),
                                   glm::vec3(0.0f, 1.0f, 0.0f)));
    this->setTransform(glm::translate(
        this->getTransform(),
        glm::vec3(0.0f, 0.0f, glm::linearRand(0.0f, this->m_AverageSpeed))));
  }

  if (this->m_Transform[3].x > g_pAreaInfo->maxPos.x)
    this->m_Transform[3].x = g_pAreaInfo->maxPos.x;

  if (this->m_Transform[3].y > g_pAreaInfo->maxPos.y)
    this->m_Transform[3].y = g_pAreaInfo->maxPos.y;

  if (this->m_Transform[3].z > g_pAreaInfo->maxPos.z) {
    this->m_Transform[3].z = g_pAreaInfo->maxPos.z;
    // glm::vec3 newPos = glm::linearRand(g_pAreaInfo->minPos,
    // g_pAreaInfo->maxPos);
    // this->m_Transform[3].x = newPos.x;
    // this->m_Transform[3].z = newPos.z;
  }

  if (this->m_Transform[3].x < g_pAreaInfo->minPos.x)
    this->m_Transform[3].x = g_pAreaInfo->minPos.x;

  if (this->m_Transform[3].y < g_pAreaInfo->minPos.y)
    this->m_Transform[3].y = g_pAreaInfo->minPos.y;

  if (this->m_Transform[3].z < g_pAreaInfo->minPos.z)
    this->m_Transform[3].z = g_pAreaInfo->minPos.z;

  //	this->m_Transform[3].z = g_pAreaInfo->minPos.z;

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
    (*iter)->m_pColor =
        &this->stateColor; // I know this only needs to be done once..
  }

  for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
           this->m_vec_pComponents.begin();
       iter != this->m_vec_pComponents.end(); iter++) {

    (*iter)->setTransform(this->getTransform());
    (*iter)->update();
  }
  this->draw();
}

bool AIControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
  rapidxml::xml_attribute<char> *att =
      componentNode->first_attribute("componentID");
  if (att != nullptr)
    this->componentID =
        atoi(componentNode->first_attribute("componentID")->value());
  else {
    this->componentID = gNextComponentID;
    gNextComponentID++;
  }

  rapidxml::xml_attribute<char> *att2 =
      componentNode->first_attribute("flockTargetID");
  if (att2 != nullptr)
    this->flockTargetID =
        atoi(componentNode->first_attribute("flockTargetID")->value());

  cComponentManager::map_ComponentIDToComponent[this->componentID] = this;
  this->setTransform(glm::translate(
      this->getTransform(),
      glm::vec3(
          std::stof(componentNode->first_attribute("offsetX")->value()),
          std::stof(componentNode->first_attribute("offsetY")->value()),
          std::stof(componentNode->first_attribute("offsetZ")->value()))));
  this->m_TargetID =
      std::stoi(componentNode->first_attribute("targetComponentID")->value());

  std::string initialMood = componentNode->first_attribute("mood")->value();
  if (initialMood == "angry")
    this->m_eAIMODD = eAIMood::MOOD_ANGRY;
  else if (initialMood == "curious")
    this->m_eAIMODD = eAIMood::MOOD_CURIOUS;
  else if (initialMood == "flock")
    this->m_eAIMODD = eAIMood::MOOD_FLOCK;
  else if (initialMood == "flockMaster")
    this->m_eAIMODD = eAIMood::MOOD_FLOCKMASTER;
  this->m_AttentionRadius =
      std::stof(componentNode->first_attribute("attentionRadius")->value());

  this->m_AverageSpeed =
      std::stof(componentNode->first_attribute("averageSpeed")->value());

  for (rapidxml::xml_node<> *cMeshComponent_node =
           componentNode->first_node("MeshComponent");
       cMeshComponent_node;
       cMeshComponent_node =
           cMeshComponent_node->next_sibling("MeshComponent")) {
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
            scale *
                std::stof(
                    cMeshComponent_node->first_attribute("offsetZ")->value())));
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
    *tempMesh->m_pColor = glm::vec4(
        std::stof(cMeshComponent_node->first_attribute("r")->value()),
        std::stof(cMeshComponent_node->first_attribute("g")->value()),
        std::stof(cMeshComponent_node->first_attribute("b")->value()), 1.0f);
    tempMesh->alpha =
        std::stof(cMeshComponent_node->first_attribute("alpha")->value());
    tempMesh->isEnabled = true;
    (std::stoi(cMeshComponent_node->first_attribute("isWireframe")->value()) ==
     1)
        ? tempMesh->isWireframe = true
        : tempMesh->isWireframe = false;
    tempMesh->m_pMeshEntry =
        &g_pMeshManager->m_MapMeshNameTocMeshEntry
             [cMeshComponent_node->first_attribute("meshName")->value()];
    if (tempMesh->alpha < 1.0f) // Transparent meshes are drawn in the main loop
      gTransparentMeshes.push_back(tempMesh);
    this->m_vec_pMeshes.push_back(tempMesh);
  }

  for (rapidxml::xml_node<> *cComponent_node = componentNode->first_node();
       cComponent_node; cComponent_node = cComponent_node->next_sibling()) {
    std::string name = cComponent_node->name();
    if (name != "MeshComponent") {
      std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
          cComponentManager::create_Component(name));
      tempComponent->loadFromXML(cComponent_node);
      this->m_vec_pComponents.push_back(tempComponent);
    }
  }

  return true;
}

void AIControlComponent::draw() {

  // if (this->entity()->isActive != false)
  //{
  for (std::vector<cMesh *>::iterator iter = this->m_vec_pMeshes.begin();
       iter != this->m_vec_pMeshes.end(); iter++) {
    if ((*iter)->alpha >= 1.0f) // Transparent meshes are drawn in the main loop
    {

      glEnable(GL_BLEND);
      // glBlendEquation(GL_FUNC_SUBTRACT);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_DEPTH_TEST);
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
      glUniform1f(gUniformId_Alpha, (*iter)->alpha);
      glUniform4fv(gUniformId_ModelColor, 1,
                   glm::value_ptr(*(*iter)->m_pColor));
      glDrawElementsBaseVertex(
          GL_TRIANGLES, (*iter)->m_pMeshEntry->NumgIndices, GL_UNSIGNED_INT,
          (void *)(sizeof(unsigned int) * (*iter)->m_pMeshEntry->BaseIndex),
          (*iter)->m_pMeshEntry->BaseVertex);
    }
  }
}

class PlayerControlComponent : public cComponent {
public:
  virtual ~PlayerControlComponent();
  virtual cComponent *getType();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  virtual void command(std::string method, std::string value) {}

protected:
  float m_Speed;
};
REGISTER_COMPONENT(PlayerControlComponent, "PlayerControlComponent")

PlayerControlComponent::~PlayerControlComponent() { m_vec_pComponents.clear(); }

cComponent *PlayerControlComponent::getType() { return nullptr; }

void PlayerControlComponent::update() {

  //*this->m_Transform = glm::translate(*this->m_Transform, glm::vec3(0.0f,
  //0.0f, 1.0f));
  // this->setTransform(this->m_Transform);

  static double lastTime = glfwGetTime();
  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);
  if (deltaTime == 0.f) {
    lastTime = currentTime;
    return;
  }

  glm::vec3 up(0.f, 1.f, 0.f);
  std::string activeLight = "";
  (glfwGetKey(gWindow, GLFW_KEY_1) == GLFW_PRESS)
      ? activeLight = "SpotLight"
      : (glfwGetKey(gWindow, GLFW_KEY_2) == GLFW_PRESS)
            ? activeLight = "DirectionalLight"
            : (glfwGetKey(gWindow, GLFW_KEY_3) == GLFW_PRESS)
                  ? activeLight = "PointLight"
                  : activeLight = ""; // MAHAHAHAHAHHAH
  if (activeLight != "")
                for each(std::shared_ptr<cComponent> component in
                           m_vec_pComponents)(*component)
                      .command("SetActiveLight", activeLight);

  if (cheatsEnabled) {
    bool pressQ = (glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_PRESS);
    bool pressE = (glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS);
    if (pressQ != pressE) {
      float direction = 0.0f;
      pressQ ? direction -= 1.0f : direction += 1.0f;
      this->setTransform(glm::translate(this->getTransform(),
                                        glm::vec3(0.0f, direction, 0.0f)));
    }
  }
  bool pressW = (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS);
  bool pressS = (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS);
  if (pressW != pressS) {
    float direction = pressW ? direction = 1.0f : direction = -1.0f;
    this->setTransform(glm::translate(
        this->getTransform(), glm::vec3(0, 0, direction * this->m_Speed)));
  }
  bool pressA = (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS);
  bool pressD = (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS);
  if (pressA != pressD) {
    float spinSpeed = 1.0f;
    float speed = pressA ? spinSpeed : -spinSpeed;
    this->setTransform(
        glm::rotate(this->getTransform(), speed * deltaTime, up));
  }
  glm::mat4 tempTransform = this->getTransform();
  glm::vec3 playerPos =
      glm::vec3(tempTransform[3].x, tempTransform[3].y, tempTransform[3].z);
  // glm::clamp(playerPos, g_pAreaInfo->minPos, g_pAreaInfo->maxPos);

  // tempTransform[3].x = playerPos.x;
  // tempTransform[3].y = playerPos.y;
  // tempTransform[3].z = playerPos.z;

  if (this->m_Transform[3].x > g_pAreaInfo->maxPos.x)
    this->m_Transform[3].x = g_pAreaInfo->minPos.x;

  if (this->m_Transform[3].y > g_pAreaInfo->maxPos.y)
    this->m_Transform[3].y = g_pAreaInfo->maxPos.y;

  if (this->m_Transform[3].z > g_pAreaInfo->maxPos.z) {
    glm::vec3 newPos =
        glm::linearRand(g_pAreaInfo->minPos, g_pAreaInfo->maxPos);
    this->m_Transform[3].x = newPos.x;
    this->m_Transform[3].z = newPos.z;
  }

  if (this->m_Transform[3].x < g_pAreaInfo->minPos.x)
    this->m_Transform[3].x = g_pAreaInfo->maxPos.x;

  if (this->m_Transform[3].y < g_pAreaInfo->minPos.y)
    this->m_Transform[3].y = g_pAreaInfo->minPos.y;

  if (this->m_Transform[3].z < g_pAreaInfo->minPos.z)
    this->m_Transform[3].z = g_pAreaInfo->minPos.z;

  std::stringstream ssTitle;

  ssTitle
      << "Player Position: " << tempTransform[3].x << "x " << tempTransform[3].y
      << "y " << tempTransform[3].z << "z "
      << "Player_Spotlight(1) Player_DirectionalLight(2) Player_PointLight(3) "
      << " Look up (NumPad +) Zoom (Up/Down) " << gPartOfTitle;
  glfwSetWindowTitle(gWindow, ssTitle.str().c_str());
  gPartOfTitle = "";
  for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
           this->m_vec_pComponents.begin();
       iter != this->m_vec_pComponents.end(); iter++) {
    (*iter)->setTransform(this->getTransform());
    (*iter)->update();
  }
  gCamera->setTargetTransform(this->getTransform());

  gCamera->update(deltaTime);
  glm::mat4 matrix = this->getTransform();
  // Set listener position to the players new position..
  FMOD_VECTOR lisPos{matrix[3].x, matrix[3].y + 30, matrix[3].z};
  // FMOD_VECTOR lisVel{this->entity()->velocity.x, this->entity()->velocity.y,
  // this->entity()->velocity.z };
  FMOD_VECTOR lisVel{0.0f, 0.0f, 0.0f};
  glm::mat4 forward = this->getOrientation();
  FMOD_VECTOR lisForward{forward[2].x, forward[2].y, forward[2].z};
  FMOD_VECTOR lisUp{0.0f, 1.0f, 0.0f};
  ::gFMODResult = ::gSoundSystem->set3DListenerAttributes(0, &lisPos, &lisVel,
                                                          &lisForward, &lisUp);
  //	gSoundSystem->update();
  lastTime = currentTime;
  playerTrans = this->m_Transform;
}

bool PlayerControlComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
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
  this->setTransform(glm::translate(
      this->getTransform(),
      glm::vec3(
          std::stof(componentNode->first_attribute("offsetX")->value()),
          std::stof(componentNode->first_attribute("offsetY")->value()),
          std::stof(componentNode->first_attribute("offsetZ")->value()))));
  this->m_Speed = std::stof(componentNode->first_attribute("speed")->value());
  for (rapidxml::xml_node<> *cRenderingComponent_node =
           componentNode->first_node("RenderingComponent");
       cRenderingComponent_node;
       cRenderingComponent_node =
           cRenderingComponent_node->next_sibling("RenderingComponent")) {
    std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
        cComponentManager::create_Component("RenderingComponent"));
    tempComponent->loadFromXML(cRenderingComponent_node);
    this->m_vec_pComponents.push_back(tempComponent);
  }
  for (rapidxml::xml_node<> *cCollisionComponent_node =
           componentNode->first_node("CollisionControlComponent");
       cCollisionComponent_node;
       cCollisionComponent_node = cCollisionComponent_node->next_sibling(
           "CollisionControlComponent")) {
    std::shared_ptr<cComponent> tempComponent = std::shared_ptr<cComponent>(
        cComponentManager::create_Component("CollisionControlComponent"));
    tempComponent->loadFromXML(cCollisionComponent_node);
    this->m_vec_pComponents.push_back(tempComponent);
  }
  return true;
}

class CollisionControlComponent : public cComponent {
public:
  virtual ~CollisionControlComponent();
  virtual cComponent *getType();
  CollisionControlComponent();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  virtual void command(std::string method, std::string value) {}

protected:
  float m_Speed;
  int cellCol;
  int cellRow;
  void draw();
};
REGISTER_COMPONENT(CollisionControlComponent, "CollisionControlComponent")

CollisionControlComponent::~CollisionControlComponent() {}

void CollisionControlComponent::draw() {
  glEnable(GL_BLEND);
  // glBlendEquation(GL_FUNC_SUBTRACT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  // Turn off backface culling
  // Enable "wireframe" polygon mode
  glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing you
                                   // can pass here
                GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
  glDisable(GL_CULL_FACE);

  glUseProgram(gProgramID);
  glUniform4fv(gUniformId_ModelColor, 1,
               glm::value_ptr(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
  glm::mat4 mat = glm::mat4();
  mat = glm::translate(
      mat, glm::vec3(g_pAreaInfo->minPos.x, 0.0f, g_pAreaInfo->minPos.z));
  mat = glm::translate(
      mat, glm::vec3(glm::vec3(gVoxelSize * cellCol + (gVoxelSize * 0.5f), 0, gVoxelSize * cellRow + (gVoxelSize * 0.5f))));
  mat = glm::scale(mat, glm::vec3(gVoxelSize * 1.0f));

  glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE, glm::value_ptr(mat));
  glDrawElementsBaseVertex(
      GL_TRIANGLES,
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].NumgIndices,
      GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
               g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseIndex),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseVertex);
}
cComponent *CollisionControlComponent::getType() { return nullptr; }

CollisionControlComponent::CollisionControlComponent() {}

// Collision stuff from Ch5 ..

float scalarTriple(glm::vec3 pq, glm::vec3 pc, glm::vec3 pb) {
  return glm::dot(pq, glm::cross(pc, pb));
}
// Given line pq and ccw triangle abc, return whether line pierces triangle. If
// so, also return the barycentric coordinates (u,v,w) of the intersection point
int IntersectLineTriangle(glm::vec3 p, glm::vec3 q, glm::vec3 a, glm::vec3 b,
                          glm::vec3 c, float &u, float &v, float &w) {
  glm::vec3 pq = q - p;
  glm::vec3 pa = a - p;
  glm::vec3 pb = b - p;
  glm::vec3 pc = c - p;
  // Test if pq is inside the edges bc, ca and ab. Done by testing
  // that the signed tetrahedral volumes, computed using scalar triple
  // products, are all positive
  u = scalarTriple(pq, pc, pb);
  if (u < 0.0f)
    return 0;
  v = scalarTriple(pq, pa, pc);
  if (v < 0.0f)
    return 0;
  w = scalarTriple(pq, pb, pa);
  if (w < 0.0f)
    return 0;

  // Compute the barycentric coordinates (u, v, w) determining the
  // intersection point r, r = u*a + v*b + w*c
  float denom = 1.0f / (u + v + w);
  u *= denom;
  v *= denom;
  w *= denom; // w = 1.0f - u - v;
  return 1;
}

// Given segment pq and triangle abc, returns whether segment intersects
// triangle and if so, also returns the barycentric coordinates (u,v,w)
// of the intersection point
bool IntersectSegmentTriangle(glm::vec3 p, glm::vec3 q, glm::vec3 a,
                              glm::vec3 b, glm::vec3 c, float &u, float &v,
                              float &w, float &t) {
  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 qp = p - q;

  // Compute triangle normal. Can be precalculated or cached if
  // intersecting multiple segments against the same triangle
  glm::vec3 n = glm::cross(ab, ac);

  // Compute denominator d. If d <= 0, segment is parallel to or points
  // away from triangle, so exit early
  float d = glm::dot(qp, n);
  if (d <= 0.0f)
    return 0;

  // Compute intersection t value of pq with plane of triangle. A ray
  // intersects if 0 <= t. Segment intersects if 0 <= t <= 1. Delay
  // dividing by d until intersection has been found to pierce triangle
  glm::vec3 ap = p - a;
  t = glm::dot(ap, n);
  if (t < 0.0f)
    return 0;
  if (t > d)
    return 0; // For segment; exclude this code line for a ray test

  // Compute barycentric coordinate components and test if within bounds
  glm::vec3 e = glm::cross(qp, ap);
  v = glm::dot(ac, e);
  if (v < 0.0f || v > d)
    return 0;
  w = -glm::dot(ab, e);
  if (w < 0.0f || v + w > d)
    return 0;

  // Segment/ray intersects triangle. Perform delayed division and
  // compute the last barycentric coordinate component
  float ood = 1.0f / d;
  t *= ood;
  v *= ood;
  w *= ood;
  u = 1.0f - v - w;
  return 1;
}

float SqDistPointSegment(glm::vec3 center, glm::vec3 segA, glm::vec3 segB) {
  glm::vec3 centerA = segA - center;
  glm::vec3 centerB = segB - center;
  glm::vec3 ab = segB - segA;

  float e = glm::dot(centerB, centerA);
  // if segB projects outside of centerA
  if (e < 0.0f)
    return glm::dot(centerA, centerA);
  float f = glm::dot(ab, ab);
  if (e >= f)
    return glm::dot(ab, ab);
  // if c projects onto centerA
  return glm::dot(centerB, centerB) - e * e / f;
}

bool intersects(AABB a, AABB b) {
  return (a.center.x - a.halfWidths.x <= b.center.x + b.halfWidths.x &&
          a.center.x + a.halfWidths.x >= b.center.x - b.halfWidths.x) &&
         (a.center.y - a.halfWidths.y <= b.center.y + b.halfWidths.y &&
          a.center.y + a.halfWidths.y >= b.center.y - b.halfWidths.y) &&
         (a.center.z - a.halfWidths.z <= b.center.z + b.halfWidths.z &&
          a.center.z + a.halfWidths.z >= b.center.z - b.halfWidths.z);
}

void CollisionControlComponent::update() {

  AABB currentAABB = AABB(glm::vec3(m_Transform[3]) + glm::vec3(-30),
                          glm::vec3(m_Transform[3]) + glm::vec3(30), 0,
                          playerMeshFaces.size(), "Skeleton");

  for (int VOXEL_ROW = 0; VOXEL_ROW < gNumZCells; VOXEL_ROW++) {
    float rowOffset = glm::distance(
        (float)(gVoxelSize * VOXEL_ROW + (gVoxelSize * 0.5f)),
        glm::distance(g_pAreaInfo->minPos.z, this->getTransform()[3].z));
    if (std::abs(rowOffset) < (gVoxelSize / 2))
      for (int VOXEL_COL = 0; VOXEL_COL < gNumXCells; VOXEL_COL++) {
        // AABBs can reside in multiple cells
        float colOffset = glm::distance(
            (float)(gVoxelSize * VOXEL_COL + (gVoxelSize * 0.5f)),
            glm::distance(g_pAreaInfo->minPos.x, this->getTransform()[3].x));
        if (std::abs(colOffset) < (gVoxelSize / 2)) {
          cellCol = VOXEL_COL;
          cellRow = VOXEL_ROW;
        }
      }
  }

  gPartOfTitle += "  ----> Cell ROW:" + std::to_string(cellRow) + " Cell Col:" +
                  std::to_string(cellCol) + " <-----  ";

  std::pair<std::multimap<std::pair<int, int>, AABB *>::iterator,
            std::multimap<std::pair<int, int>, AABB *>::iterator>
      ret;
  ret = g_multimap_VoxelGrid.equal_range(
      std::make_pair((int)cellRow, (int)cellCol));

  // std::cout << (int)cellCol << " " << (int)cellRow << " =>";
  // Draw Physics Debug Info
  bool lineCollision[3] = {0, 0, 0};

  for (std::multimap<std::pair<int, int>, AABB *>::iterator iter = ret.first;
       iter != ret.second; ++iter) {
    glUseProgram(g_Physics_ProgramID);

    glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing you
                                     // can pass here
                  GL_LINE);
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));

    // glDisable(GL_DEPTH_TEST);
    glUniformMatrix4fv(gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
                       glm::value_ptr(glm::scale(
                           glm::translate(glm::mat4(), iter->second->center),
                           iter->second->halfWidths * glm::vec3(2))));
    glUniformMatrix4fv(gUniformId_Physics_ModelMatrixOrientation, 1, GL_FALSE,
                       glm::value_ptr(glm::mat4(1.0f)));
    glUniform1f(gUniformId_Physics_Alpha, 1.0f);

    bool insideOfAABB = intersects(currentAABB, *iter->second);

    if (insideOfAABB == true) {
      glUniform4fv(gUniformId_Physics_ModelColor, 1,
                   glm::value_ptr(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)));
      for (int nc = iter->second->offsetToTriFaces;
           nc <= iter->second->offsetToTriFaces + iter->second->numFaces;
           nc++) {
        triFace *it = gMeshFaces[17].at(nc);
        bool collision;
        glm::vec3 p2;
        glm::vec3 p1;
        glm::mat4 tempTransform =
            cComponentManager::map_ComponentIDToComponent[-2]->getTransform();
        float u = 0, v = 0, w = 0, t = 0;

        // X AXIS
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CW OR CCW..
        if (it->v1.x > tempTransform[3].x) {
          p1 = glm::vec3(tempTransform[3].x - 25, tempTransform[3].y + 25,
                         tempTransform[3].z);
          p2 = glm::vec3(tempTransform[3].x + 25, tempTransform[3].y + 25,
                         tempTransform[3].z);
        } else {
          p2 = glm::vec3(tempTransform[3].x - 25, tempTransform[3].y + 25,
                         tempTransform[3].z);
          p1 = glm::vec3(tempTransform[3].x + 25, tempTransform[3].y + 25,
                         tempTransform[3].z);
        }
        u = 0, v = 0, w = 0, t = 0;

        // Check if line segment 1 intercepts a triangle inside of the current
        // voxel cell
        // bool collision = IntersectLineTriangle(p1, p2, it->second->v1,
        // it->second->v2, it->second->v3, u, v, w);
        collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3, u,
                                             v, w, t);
        if (collision == true)
          lineCollision[2] = true;

        while (collision == true) {
          if (it->v1.x > tempTransform[3].x) {
            tempTransform[3].x -= 0.1f;
            p1 = glm::vec3(tempTransform[3].x - 25, tempTransform[3].y + 25,
                           tempTransform[3].z);
            p2 = glm::vec3(tempTransform[3].x + 25, tempTransform[3].y + 25,
                           tempTransform[3].z);
          } else {
            tempTransform[3].x += 0.1f;
            p2 = glm::vec3(tempTransform[3].x - 25, tempTransform[3].y + 25,
                           tempTransform[3].z);
            p1 = glm::vec3(tempTransform[3].x + 25, tempTransform[3].y + 25,
                           tempTransform[3].z);
          }
          collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3,
                                               u, v, w, t);
        }

        // Y AXIS
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CW OR CCW..
        if (it->v1.y > tempTransform[3].y) {
          p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y - 15,
                         tempTransform[3].z);
          p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 15,
                         tempTransform[3].z);
        } else {
          p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y - 15,
                         tempTransform[3].z);
          p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 15,
                         tempTransform[3].z);
        }
        u = 0, v = 0, w = 0, t = 0;

        // Check if line segment 1 intercepts a triangle inside of the current
        // voxel cell
        // bool collision = IntersectLineTriangle(p1, p2, it->second->v1,
        // it->second->v2, it->second->v3, u, v, w);
        collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3, u,
                                             v, w, t);
        if (collision == true)
          lineCollision[1] = true;

        while (collision == true) {
          if (it->v1.y > tempTransform[3].y) {
            tempTransform[3].y -= 0.1f;
            p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y - 15,
                           tempTransform[3].z);
            p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 15,
                           tempTransform[3].z);
          } else {
            tempTransform[3].y += 0.1f;
            p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y - 15,
                           tempTransform[3].z);
            p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 15,
                           tempTransform[3].z);
          }
          collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3,
                                               u, v, w, t);
        }

        // Z AXIS
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // CW OR CCW..
        if (it->v1.z > tempTransform[3].z) {
          p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                         tempTransform[3].z - 25);
          p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                         tempTransform[3].z + 25);
        } else {
          p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                         tempTransform[3].z - 25);
          p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                         tempTransform[3].z + 25);
        }
        u = 0, v = 0, w = 0, t = 0;

        // Check if line segment 1 intercepts a triangle inside of the current
        // voxel cell
        // bool collision = IntersectLineTriangle(p1, p2, it->second->v1,
        // it->second->v2, it->second->v3, u, v, w);
        collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3, u,
                                             v, w, t);
        if (collision == true)
          lineCollision[0] = true;

        while (collision == true) {
          if (it->v1.z > tempTransform[3].z) {
            if (it->v1.z > tempTransform[3].z)
              tempTransform[3].z -= 0.1f;
            else
              tempTransform[3].z += 0.1f;
            p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                           tempTransform[3].z - 25);
            p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                           tempTransform[3].z + 25);
          } else {
            if (it->v1.z > tempTransform[3].z)
              tempTransform[3].z -= 0.1f;
            else
              tempTransform[3].z += 0.1f;
            p2 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                           tempTransform[3].z - 25);
            p1 = glm::vec3(tempTransform[3].x, tempTransform[3].y + 25,
                           tempTransform[3].z + 25);
          }
          collision = IntersectSegmentTriangle(p1, p2, it->v1, it->v2, it->v3,
                                               u, v, w, t);
        }

        // Update Player's Position
        cComponentManager::map_ComponentIDToComponent[-2]->setTransform(
            tempTransform);
      }
    }
    glDrawElementsBaseVertex(
        GL_LINES, 32, GL_UNSIGNED_INT,
        (void *)(sizeof(unsigned int) *
                 g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseIndex),
        g_pMeshManager->m_MapMeshNameTocMeshEntry["Cube"].BaseVertex);
  }

  glUseProgram(g_Physics_ProgramID);

  glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
  glEnable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing you
                                   // can pass here
                GL_LINE);

  // glDisable(GL_DEPTH_TEST);
  glUniformMatrix4fv(
      gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
      glm::value_ptr(glm::scale(
          glm::translate(glm::mat4(), glm::vec3(this->getTransform()[3].x,
                                                this->getTransform()[3].y + 15,
                                                this->getTransform()[3].z)),
          glm::vec3(25.0f))));
  glUniformMatrix4fv(gUniformId_Physics_ModelMatrixOrientation, 1, GL_FALSE,
                     glm::value_ptr(glm::mat4(1.0f)));
  glUniform1f(gUniformId_Physics_Alpha, 1.0f);
  glUniform4fv(gUniformId_Physics_ModelColor, 1,
               glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  if (lineCollision[0])
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
               g_pMeshManager->m_MapMeshNameTocMeshEntry["X-Axis"].BaseIndex),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["X-Axis"].BaseVertex);

  glUniform4fv(gUniformId_Physics_ModelColor, 1,
               glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  if (lineCollision[1])
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
                   g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"]
                       .BaseIndex +
               12),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"].BaseVertex + 12);

  glUniform4fv(gUniformId_Physics_ModelColor, 1,
               glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  if (lineCollision[2])
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
               g_pMeshManager->m_MapMeshNameTocMeshEntry["Z-Axis"].BaseIndex),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Z-Axis"].BaseVertex);
  // Extra lines to pin-point collisions
  glUniform1f(gUniformId_Physics_Alpha, 0.2f);
  glUniformMatrix4fv(
      gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
      glm::value_ptr(glm::scale(
          glm::translate(glm::mat4(), glm::vec3(this->getTransform()[3].x + 25,
                                                this->getTransform()[3].y + 15,
                                                this->getTransform()[3].z)),
          glm::vec3(30.0f))));

  glUniform4fv(gUniformId_Physics_ModelColor, 1,
               glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  if (lineCollision[2])
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
                   g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"]
                       .BaseIndex +
               12),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"].BaseVertex + 12);
  glUniformMatrix4fv(
      gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
      glm::value_ptr(glm::scale(
          glm::translate(glm::mat4(), glm::vec3(this->getTransform()[3].x - 25,
                                                this->getTransform()[3].y + 15,
                                                this->getTransform()[3].z)),
          glm::vec3(30.0f))));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
                   g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"]
                       .BaseIndex +
               12),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"].BaseVertex + 12);

  glUniform4fv(gUniformId_Physics_ModelColor, 1,
               glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
  if (lineCollision[0])
    glUniform4fv(gUniformId_Physics_ModelColor, 1,
                 glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
  glUniformMatrix4fv(
      gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
      glm::value_ptr(
          glm::scale(glm::translate(glm::mat4(),
                                    glm::vec3(this->getTransform()[3].x,
                                              this->getTransform()[3].y + 15,
                                              this->getTransform()[3].z - 25)),
                     glm::vec3(30.0f))));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
                   g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"]
                       .BaseIndex +
               12),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"].BaseVertex + 12);
  glUniformMatrix4fv(
      gUniformId_Physics_ModelMatrix, 1, GL_FALSE,
      glm::value_ptr(
          glm::scale(glm::translate(glm::mat4(),
                                    glm::vec3(this->getTransform()[3].x,
                                              this->getTransform()[3].y + 15,
                                              this->getTransform()[3].z + 25)),
                     glm::vec3(30.0f))));

  glDrawElementsBaseVertex(
      GL_LINES, 2, GL_UNSIGNED_INT,
      (void *)(sizeof(unsigned int) *
                   g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"]
                       .BaseIndex +
               12),
      g_pMeshManager->m_MapMeshNameTocMeshEntry["Y-Axis"].BaseVertex + 12);

  // END OF Extra lines to pin-point collisions
  glUseProgram(gProgramID);
  //
  //
  //
  // this->AABB = this->m_Transform;
  this->draw();
}

bool CollisionControlComponent::loadFromXML(
    rapidxml::xml_node<> *componentNode) {
  return true;
}
