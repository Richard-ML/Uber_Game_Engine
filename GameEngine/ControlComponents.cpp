#include "cComponentManager.h"
#include "global.h"
#include <sstream>


// TODO: Move AI logic to AIEngine.. :P ....................................................................................
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
  int m_TargetID;
  eAIState m_eAIState;
  eAIMood m_eAIMODD;
  float m_AttentionRadius;
  float m_AverageSpeed;
};
REGISTER_COMPONENT(AIControlComponent, "AIControlComponent")

AIControlComponent::~AIControlComponent() {
  m_vec_pComponents.clear(); // TODO: An entity should have a vec of shared pointers to components.. Automagic cleanup..
}

cComponent *AIControlComponent::getType() { return nullptr; }

void AIControlComponent::update() {
	// TODO: Get AI data including orientation.. 
	// Apply forces using physics engine interface.. 
	// Update animation related information using the GraphicsEngine interface..


	// Physics Control Component Should contain
	// AI Control Component which should contain
	// Renderable Component..

  for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
           this->m_vec_pComponents.begin();
       iter != this->m_vec_pComponents.end(); iter++) {

    (*iter)->setTransform(this->getTransform());
    (*iter)->update();
  }

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

//glm::mat4 tempTransform = this->getTransform();
//  std::stringstream ssTitle;
//  ssTitle
//      << "Player Position: " << tempTransform[3].x << "x " << tempTransform[3].y
//      << "y " << tempTransform[3].z << "z "
//      << "Player_Spotlight(1) Player_DirectionalLight(2) Player_PointLight(3) "
//      << " Look up (NumPad +) Zoom (Up/Down) " << gPartOfTitle;
//  glfwSetWindowTitle(gWindow, ssTitle.str().c_str());
//  gPartOfTitle = "";
  for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
           this->m_vec_pComponents.begin();
       iter != this->m_vec_pComponents.end(); iter++) {
    (*iter)->setTransform(this->getTransform());
    (*iter)->update();
  }
  gCamera->setTargetTransform(this->getTransform());

  gCamera->update(deltaTime);
  glm::mat4 matrix = this->getTransform();
  lastTime = currentTime;
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
private:
	float radius;
};
REGISTER_COMPONENT(CollisionControlComponent, "CollisionControlComponent")

CollisionControlComponent::~CollisionControlComponent() {}

cComponent *CollisionControlComponent::getType() { return nullptr; }

CollisionControlComponent::CollisionControlComponent() {}

void CollisionControlComponent::update() {

  glm::vec3 position; 
  this->m_pRigidBody->getPosition(position);

  this->m_Transform[3] = glm::vec4(position, this->m_Transform[3].w);
  this->setTransform(this->m_Transform);
  for (std::vector<std::shared_ptr<cComponent>>::iterator iter = this->m_vec_pComponents.begin(); iter != this->m_vec_pComponents.end(); iter++)
  {
	  (*iter)->update();
  }

  if (this->componentID == curSphereIndex)
  {
	  bool pressW = (glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS);
	  bool pressS = (glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS);
	  if (pressW != pressS) {
		  float direction = pressW ? direction = 1.0f : direction = -1.0f;
		  m_pRigidBody->applyForce(  glm::vec3(0, 0, direction * 10.0f));
	  }
	  bool pressA = (glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS);
	  bool pressD = (glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS);
	  if (pressA != pressD) {
		  float direction = pressA ? direction = 1.0f : direction = -1.0f;
		  m_pRigidBody->applyForce(glm::vec3(direction * 10.0f, 0, 0));

	  }

	  gCamera->setTargetTransform(this->getTransform());
	  gCamera->update(0.1f);
  }
}

bool CollisionControlComponent::loadFromXML(
    rapidxml::xml_node<> *componentNode) {

	this->setTransform(glm::translate(
		this->getTransform(),
		glm::vec3(
			std::stof(componentNode->first_attribute("offsetX")->value()),
			std::stof(componentNode->first_attribute("offsetY")->value()),
			std::stof(componentNode->first_attribute("offsetZ")->value()))));
		this->m_Transform = glm::rotate(
			this->m_Transform,
			std::stof(componentNode->first_attribute("preRotX")->value()),
			glm::vec3(0.0f, 1.0f, 0.0f));

		this->m_Transform = glm::rotate(
			this->m_Transform,
			std::stof(componentNode->first_attribute("preRotY")->value()),
			glm::vec3(1.0f, 0.0f, 0.0f));

		this->m_Transform = glm::rotate(
			this->m_Transform,
			std::stof(componentNode->first_attribute("preRotZ")->value()),
			glm::vec3(0.0f, 0.0f, 1.0f));


        std::string objectType = componentNode->first_attribute("type")->value();
	// Get child renderable components.. (Sphere model || Plane Model)
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


	PhysicsEngine::sRigidBody rbInfo;
	rbInfo.position = glm::vec3(this->m_Transform[3][0], this->m_Transform[3][1], this->m_Transform[3][2]);
	rbInfo.mass = std::stof(componentNode->first_attribute("mass")
		->value()); // Get the mass from the XML file..
					// (CollisionControlComponent) node
	rbInfo.velocity = glm::ballRand(
		500.0f); // Generate initial velocity in random direction..
	
	PhysicsEngine::iCollisionShape* shape;
	if (objectType == "plane")
	{
		shape = g_pPhysicsEngine->createCollisionShape(PhysicsEngine::eShapeType::PLANE);
		shape->setNormal(glm::vec3(this->m_Transform[1][0], this->m_Transform[1][1], this->m_Transform[1][2]));
	}
	else
	{
		shape = g_pPhysicsEngine->createCollisionShape(PhysicsEngine::eShapeType::SPHERE);
		shape->setRadius(rbInfo.mass);
        }

	PhysicsEngine::iRigidBody *rb =
		g_pPhysicsEngine->createRigidBody(shape, rbInfo); // Get the physics engine to
											// create a rigid body!

	g_pPhysicsEngine->addRigidBodyToWorld(rb); // Add the rigid body to the world with object type..

	this->componentID = atoi( componentNode->first_attribute("componentID")->value());
	this->m_pRigidBody = rb;

	return true;
}
