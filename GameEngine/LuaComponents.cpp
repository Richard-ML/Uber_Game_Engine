extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include "cComponentManager.h"
#include "externals.h"
#include "global.h"
#include <cerrno>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
typedef int (*lua_CFunction)(lua_State *L);
struct locationTrigger {
  glm::vec3 position;
  float radius;
  // TODO: Trigger callback -- For now I am just displaying that the trigger was
  // triggered. :P
};
static std::vector<locationTrigger> locationTriggers;

class LuaComponent : public cComponent {
public:
  LuaComponent();
  virtual ~LuaComponent();
  virtual cComponent *getType();
  virtual void update();
  virtual bool loadFromXML(rapidxml::xml_node<> *componentNode);
  virtual void command(std::string method, std::string value) {}

  // Methods called from Lua

  static int l_SetComponentState(
      lua_State *L); // Retrieve component's information based on componentID
  static int l_GetComponentState(
      lua_State *L); // Set component's information based on componentID
  static int
  l_Wander(lua_State *L); // Called by AI that have the wander state active
  static int
  l_Hunt(lua_State *L); // Called by AI that are hunting another entity
  static int l_MoveToInSeconds(lua_State *L); // Simple motion tween
  static int l_MoveTo(lua_State *L);          // Another simple motion tween
  static int l_OrientTo(lua_State *L);
  static int l_SetCameraPosition(lua_State *L);
  static int l_SetCameraOffset(lua_State *L);
  static int l_AddLocationTrigger(lua_State *L);
  // static void moveTo(int componentID, glm::vec3 destination, glm::vec3
  // maxVelocity);

  // End of methods called from Lua

  // Miscellaneous functions that belong in a utilities class
  void load_Script(std::string scriptName, const char *scriptPath);
  void RunScriptImmediately(std::string script);
  std::string m_decodeLuaErrorToString(int error);
  void delete_Script(std::string name);

  // Core Lua component data-members and methods

  std::map<std::string, std::string> m_mapScripts;
  lua_State *m_pLuaState;
  lua_State *get_LuaState();
};
REGISTER_COMPONENT(LuaComponent, "LuaComponent")

/* Used to load a script from a .lua file */
std::string get_file_contents(const char *filename) {
  std::ifstream theFile(filename); // load the xml data into the buffer
  std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
                            std::istreambuf_iterator<char>());
  bufData.push_back('\0');
  return std::string(bufData.begin(), bufData.end());
}

LuaComponent::LuaComponent() {

  this->setTransform(glm::mat4());
  this->acceleration = glm::vec3(12.f);
  this->velocity = glm::vec3(0.0f);
  this->maxVelocity = glm::vec3(30.0f);
  this->minVelocity = glm::vec3(-3.0f);

  this->m_pLuaState = luaL_newstate();
  luaL_openlibs(this->m_pLuaState); // Lua v5.3.3

  // Core component information exchange methods
  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_SetComponentState);
  lua_setglobal(this->m_pLuaState, "setComponentState");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_GetComponentState);
  lua_setglobal(this->m_pLuaState, "getComponentState");

  // Simple movement methods
  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_MoveTo);
  lua_setglobal(this->m_pLuaState, "moveTo");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_MoveToInSeconds);
  lua_setglobal(this->m_pLuaState, "moveToInSeconds");

  // Complex movement methods

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_Wander);
  lua_setglobal(this->m_pLuaState, "wander");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_Hunt);
  lua_setglobal(this->m_pLuaState, "hunt"); // <-- Follow target

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_OrientTo);
  lua_setglobal(this->m_pLuaState, "orientTo");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_SetCameraPosition);
  lua_setglobal(this->m_pLuaState, "setCamera");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_SetCameraOffset);
  lua_setglobal(this->m_pLuaState, "setCameraOffset");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_AddLocationTrigger);
  lua_setglobal(this->m_pLuaState, "addLocationTrigger");

  RunScriptImmediately("");
  this->lastTime = (float)glfwGetTime();
}

LuaComponent::~LuaComponent() {
  lua_close(this->m_pLuaState);
  return;
}

cComponent *LuaComponent::getType() {
  return nullptr; // This is not being used yet. But it's here to foreshadow an
                  // adaptable framework
}

// Call all the active scripts that are loaded
void LuaComponent::update() {
  float deltaTime = (float)glfwGetTime() - lastTime;
  //	std::cout << "cLuaBrain::Update() called" << std::endl;
  for (std::map<std::string /*name*/, std::string /*source*/>::iterator
           itScript = this->m_mapScripts.begin();
       itScript != this->m_mapScripts.end(); itScript++) {
    std::string curLuaScript = itScript->second;

    int error = luaL_loadstring(this->m_pLuaState, curLuaScript.c_str());

    if (error != 0 /*no error*/) {
      std::cout << "-------------------------------------------------------"
                << std::endl;
      std::cout << "Error running Lua script: ";
      std::cout << itScript->first << std::endl;
      std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
      std::cout << "-------------------------------------------------------"
                << std::endl;
      continue;
    }

    // execute funtion in "protected mode", where problems are
    //  caught and placed on the stack for investigation
    error = lua_pcall(
        this->m_pLuaState, /* lua state */
        0,  /* nargs: number of arguments pushed onto the lua stack */
        0,  /* nresults: number of results that should be on stack at end*/
        0); /* errfunc: location, in stack, of error function.
            if 0, results are on top of stack. */
    if (error != 0 /*no error*/) {
      std::cout << "Lua: There was an error..." << std::endl;
      std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

      std::string luaError;
      // Get error information from top of stack (-1 is top)
      luaError.append(lua_tostring(this->m_pLuaState, -1));

      // Make error message a little more clear
      std::cout << "-------------------------------------------------------"
                << std::endl;
      std::cout << "Error running Lua script: ";
      std::cout << itScript->first << std::endl;
      std::cout << luaError << std::endl;
      std::cout << "-------------------------------------------------------"
                << std::endl;
      // We passed zero (0) as errfunc, so error is on stack)
      lua_pop(this->m_pLuaState, 1); /* pop error message from the stack */

      continue;
    }
  }
  this->m_Transform[3] = glm::vec4(glm::vec3(this->m_Transform[3]) +
                                       (this->velocity * glm::vec3(deltaTime)),
                                   this->m_Transform[3].w);
        for each(locationTrigger trigger in locationTriggers) {
            if (glm::distance(glm::vec3(this->m_Transform[3]),
                              trigger.position) < trigger.radius)
              std::cout << "Component " << componentID
                        << " triggered location trigger at "
                        << trigger.position.x << "x " << trigger.position.y
                        << "y " << trigger.position.z << "z \n";
          }
        for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
                 this->m_vec_pComponents.begin();
             iter != this->m_vec_pComponents.end(); iter++) {
          (*iter)->setTransform(this->getTransform());
          (*iter)->update();
        }
        this->lastTime = (float)glfwGetTime();
        // this->m_Transform = *curTransform;
        return;
}

// Runs a script, but doesn't save it (originally used to set the ObjectID)
void LuaComponent::RunScriptImmediately(std::string script) {
  std::stringstream ssLuaScript;
  ssLuaScript << "ObjectID = " << 0;

  int error = luaL_loadstring(this->m_pLuaState, ssLuaScript.str().c_str());

  if (error != 0 /*no error*/) {
    std::cout << "-------------------------------------------------------"
              << std::endl;
    std::cout << "Error running Lua script: ";
    std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
    std::cout << "-------------------------------------------------------"
              << std::endl;
    // return;
  }

  // execute function in "protected mode", where problems are
  //  caught and placed on the stack for investigation
  error = lua_pcall(
      this->m_pLuaState, /* lua state */
      0,  /* nargs: number of arguments pushed onto the lua stack */
      0,  /* nresults: number of results that should be on stack at end*/
      0); /* errfunc: location, in stack, of error function.
          if 0, results are on top of stack. */
  if (error != 0 /*no error*/) {
    std::cout << "Lua: There was an error..." << std::endl;
    std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

    std::string luaError;
    // Get error information from top of stack (-1 is top)
    luaError.append(lua_tostring(this->m_pLuaState, -1));

    // Make error message a little more clear
    std::cout << "-------------------------------------------------------"
              << std::endl;
    std::cout << "Error running Lua script: ";
    std::cout << luaError << std::endl;
    std::cout << "-------------------------------------------------------"
              << std::endl;
    // We passed zero (0) as errfunc, so error is on stack)
    lua_pop(this->m_pLuaState, 1); /* pop error message from the stack */
  }

  return;
}

bool LuaComponent::loadFromXML(rapidxml::xml_node<> *componentNode) {
  this->componentID = gNextComponentID;
  gNextComponentID++;
  cComponentManager::map_ComponentIDToComponent[this->componentID] = this;
  this->setTransform(glm::translate(
      glm::mat4(),
      glm::vec3(
          std::stof(componentNode->first_attribute("offsetX")->value()),
          std::stof(componentNode->first_attribute("offsetY")->value()),
          std::stof(componentNode->first_attribute("offsetZ")->value()))));

  for (rapidxml::xml_node<> *LuaScript_node =
           componentNode->first_node("LuaScript");
       LuaScript_node;
       LuaScript_node = LuaScript_node->next_sibling("LuaScript")) {
    this->load_Script(LuaScript_node->first_attribute("name")->value(),
                      LuaScript_node->first_attribute("path")->value());
  }
  for (rapidxml::xml_node<> *LuaScript_node =
           componentNode->first_node("RenderingComponent");
       LuaScript_node;
       LuaScript_node = LuaScript_node->next_sibling("RenderingComponent")) {
    std::string name = LuaScript_node->name();
    std::shared_ptr<cComponent> tempComponent =
        std::shared_ptr<cComponent>(cComponentManager::create_Component(name));
    tempComponent->loadFromXML(LuaScript_node);
    this->m_vec_pComponents.push_back(tempComponent);
  }

  return true;
}

void LuaComponent::load_Script(std::string scriptName, const char *scriptPath) {
  std::stringstream ss;
  ss << "componentID =" << this->componentID << " \n"
     << get_file_contents(scriptPath);

  this->m_mapScripts[scriptName] = ss.str();
  // std::cout << this->m_mapScripts[scriptName];
  return;
}

void LuaComponent::delete_Script(std::string scriptName) {
  this->m_mapScripts.erase(scriptName);
  return;
}

lua_State *LuaComponent::get_LuaState() { return this->m_pLuaState; }

std::string LuaComponent::m_decodeLuaErrorToString(int error) {
  switch (error) {
  case 0:
    return "Lua: no error";
    break;
  case LUA_ERRSYNTAX:
    return "Lua: syntax error";
    break;
  case LUA_ERRMEM:
    return "Lua: memory allocation error";
    break;
  case LUA_ERRRUN:
    return "Lua: Runtime error";
    break;
  case LUA_ERRERR:
    return "Lua: Error while running the error handler function";
    break;
  } // switch ( error )

  // Who knows what this error is?
  return "Lua: UNKNOWN error";
}

// Called by Lua
// Passes object ID, new velocity, etc.
// Returns valid (true or false)
// Passes:
// - position (xyz)
// - velocity (xyz)
// called "setObjectState" in lua
int LuaComponent::l_SetComponentState(lua_State *L) {
  int componentID = (int)lua_tonumber(L, 1); /* get argument */
  cComponent *currentCommponent =
      cComponentManager::map_ComponentIDToComponent[componentID];

  // Object ID is valid
  // Get the values that lua pushed and update object
  currentCommponent->m_Transform[3].x =
      (float)lua_tonumber(L, 2); /* get argument */
  currentCommponent->m_Transform[3].y =
      (float)lua_tonumber(L, 3); /* get argument */
  currentCommponent->m_Transform[3].z =
      (float)lua_tonumber(L, 4);                             /* get argument */
  currentCommponent->velocity.x = (float)lua_tonumber(L, 5); /* get argument */
  currentCommponent->velocity.y = (float)lua_tonumber(L, 6); /* get argument */
  currentCommponent->velocity.z = (float)lua_tonumber(L, 7); /* get argument */

  lua_pushboolean(L, true); // index is OK

  return 1; // There were 7 things on the stack
}

// Passes object ID
// Returns valid (true or false)
// - position (xyz)
// - velocity (xyz)
// called "getObjectState" in lua
int LuaComponent::l_GetComponentState(lua_State *L) {
  int componentID = (int)lua_tonumber(L, 1); /* get argument */
  cComponent *currentCommponent =
      cComponentManager::map_ComponentIDToComponent[componentID];

  // Object ID is valid
  lua_pushboolean(L, true); // index is OK
  lua_pushnumber(L, currentCommponent->m_Transform[3].x);
  lua_pushnumber(L, currentCommponent->m_Transform[3].y);
  lua_pushnumber(L, currentCommponent->m_Transform[3].z);
  lua_pushnumber(L, currentCommponent->velocity.x);
  lua_pushnumber(L, currentCommponent->velocity.y);
  lua_pushnumber(L, currentCommponent->velocity.z);
  lua_pushnumber(L, currentCommponent->acceleration.x);
  lua_pushnumber(L, currentCommponent->acceleration.y);
  lua_pushnumber(L, currentCommponent->acceleration.z);
  lua_pushnumber(L, currentCommponent->maxVelocity.x);
  lua_pushnumber(L, currentCommponent->maxVelocity.y);
  lua_pushnumber(L, currentCommponent->maxVelocity.z);

  return 13; // There were 7 things on the stack
}

int LuaComponent::l_Wander(lua_State *L) {
  int componentID = (int)lua_tonumber(L, 1);
  float curTargetX, curTargetY, curTargetZ;
  curTargetX = (float)lua_tonumber(L, 2);
  curTargetY = (float)lua_tonumber(L, 3);
  // curTargetY = lua_tonumber(L, 1);
  curTargetZ = (float)lua_tonumber(L, 4);
  glm::mat4 currentTransform =
      cComponentManager::map_ComponentIDToComponent[componentID]
          ->getTransform();

  glm::vec3 curDestPositionOffset =
      glm::vec3(currentTransform[3]) -
      glm::vec3(curTargetX, curTargetY, curTargetZ);
  if ((std::abs(curDestPositionOffset.x) < 1 &&
       std::abs(curDestPositionOffset.z) < 1) ||
      (curTargetX == 0 && curTargetZ == 0)) {
    // Determine current cell position.
    int col = (int)glm::round(currentTransform[3].z / 128);
    int row = (int)glm::round(currentTransform[3].x / 128);
    col = glm::clamp(col, 0, 511);
    row = glm::clamp(row, 0, 511);
    glm::vec3 newTarget =
        g_pPathFindingManager->getRandomAdjacentTilePos(row, col);
    curTargetX = newTarget.x;
    curTargetY = newTarget.y;
    curTargetZ = newTarget.z;
    curDestPositionOffset = glm::vec3(currentTransform[3]) -
                            glm::vec3(curTargetX, curTargetY, curTargetZ);
  }
  // TODO: Setup moveTo : l_MoveTo(L) accepts current Lua state
  lua_pushnumber(L, curTargetX);
  lua_pushnumber(L, curTargetY);
  lua_pushnumber(L, curTargetZ);
  return 3;
}
int LuaComponent::l_Hunt(lua_State *L) {

  int currentComponentID = (int)lua_tonumber(L, 1);
  glm::mat4 currentTransform =
      cComponentManager::map_ComponentIDToComponent[currentComponentID]
          ->getTransform();
  int targetComponentID = (int)lua_tonumber(L, 2);
  glm::mat4 targetTransform =
      playerTrans; // cComponentManager::map_ComponentIDToComponent[targetComponentID];

  bool inSight = glm::distance(currentTransform[3], targetTransform[3]) < 600;

  if (glm::distance(currentTransform[3], targetTransform[3]) < 100) {
    // GAME OVER
    // std::cout << "--- GAME OVER --- \nThe spider bit you!" << std::endl;
    // system("pause");
  }

  lua_pushboolean(L, inSight);

  lua_pushnumber(L, targetTransform[3].x);
  lua_pushnumber(L, targetTransform[3].y);
  lua_pushnumber(L, targetTransform[3].z);

  return 4;
}

int LuaComponent::l_MoveToInSeconds(lua_State *L) {

  // int componentID = (int)lua_tonumber(L, 1); // Get the current component's
  // ID
  //	glm::mat4 currentTransform =
  //cComponentManager::map_ComponentIDToComponent[currentComponentID]->getTransform();
  //// Retrieve the current component's transform matrix

  glm::vec3 waypoint;
  // float timeInSeconds;

  // waypoint = glm::vec3((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3),
  // (float)lua_tonumber(L, 4)); // Retrieve the first three values and assign
  // them to a vec 3 (waypoint)/(destination)
  // timeInSeconds = (float)lua_tonumber(L, 4);

  return 3;
}

int LuaComponent::l_MoveTo(lua_State *L) {
  int numArgs = lua_gettop(L); // Returns the index of the top element in the
                               // stack. Because indices start at 1, this result
                               // is equal to the number of elements in the
                               // stack (and so 0 means an empty stack).
  if (numArgs == 4 || numArgs == 7) {
    int currentComponentID =
        (int)lua_tonumber(L, 1); // Get the current component's ID
    glm::vec3 waypoint =
        glm::vec3((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3),
                  (float)lua_tonumber(L, 4)); // Retrieve the next three values
                                              // and assign them to a vec 3
                                              // (waypoint)/(destination)
    if (numArgs == 7) // The last three arguments are optional since the object
                      // already has its own velocity information defined
    {
      glm::vec3 maxVelocity =
          glm::vec3((float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6),
                    (float)lua_tonumber(L, 7)); // Retrieve the next three
                                                // values and assign them to a
                                                // vec 3 (maxVelocity)
      // std::thread(moveTo, currentComponentID, waypoint,
      // maxVelocity).detach(); // Non-blocking fire and forget // #include
      // <future> required

      cComponent *curComponent = cComponentManager::map_ComponentIDToComponent
          [currentComponentID]; // Retrieve the current component's data

      float currentDistanceX =
          glm::distance(curComponent->m_Transform[3].x, waypoint.x);
      float currentDistanceY =
          glm::distance(curComponent->m_Transform[3].y, waypoint.y);
      float currentDistanceZ =
          glm::distance(curComponent->m_Transform[3].z, waypoint.z);
      float currentDistance =
          glm::distance(glm::vec3(curComponent->m_Transform[3]), waypoint);
      // static float lastTime = glfwGetTime();
      if (currentDistance >= 3) {
        float deltaTime = (float)glfwGetTime() - curComponent->lastTime;
        glm::vec3 acceleration =
            curComponent->acceleration * glm::vec3(deltaTime);
        glm::vec3 direction;
        // Direction computation
        if (curComponent->m_Transform[3].x > waypoint.x)
          acceleration.x *= -1;
        else
          acceleration.x *= 1;

        if (curComponent->m_Transform[3].y > waypoint.y)
          acceleration.y *= -1;
        else
          acceleration.y *= 1;

        if (curComponent->m_Transform[3].z > waypoint.z)
          acceleration.z *= -1;
        else
          acceleration.z *= 1;

        // d = vt + (1/2)at^2 is this the formula I should be using?
        float distanceToStopX =
            (glm::abs(curComponent->velocity.x * curComponent->velocity.x) /
             glm::abs(acceleration.x / 2));
        float distanceToStopY =
            (glm::abs(curComponent->velocity.y * curComponent->velocity.y) /
             glm::abs(acceleration.y / 2));
        float distanceToStopZ =
            (glm::abs(curComponent->velocity.z * curComponent->velocity.z) /
             glm::abs(acceleration.z / 2));
        if (currentDistanceX >= 1.0f)
          if (distanceToStopX >= currentDistanceX) // Time to slow down!
          {
            curComponent->velocity.x -= acceleration.x;
          } else // Speed up towards target location
          {
            curComponent->velocity.x += acceleration.x;
          }
        else
          curComponent->velocity.x = 0.0f;
        if (currentDistanceY >= 1.0f)
          if (distanceToStopY >= currentDistanceY) {
            curComponent->velocity.y -= acceleration.y;
          } else // Speed up towards target location
          {
            curComponent->velocity.y += acceleration.y;
          }
        else
          curComponent->velocity.y = 0.0f;
        if (currentDistanceZ >= 1.0f)
          if (distanceToStopZ >= currentDistanceZ) {
            curComponent->velocity.z -= acceleration.z;
          } else // Speed up towards target location
          {
            curComponent->velocity.z += acceleration.z;
          }
        else
          curComponent->velocity.z = 0.0f;

        // Clamp the values based on max velocity
        curComponent->velocity = glm::clamp(
            curComponent->velocity,

            curComponent->minVelocity, // Should be zero since object will
                                       // always be moving towards an object
                                       // since acceleration is not currently a
                                       // method arguement
            maxVelocity);
        lua_pushboolean(L, false); // still traveling to destination
      } else {
        curComponent->m_Transform[3] = glm::vec4(
            waypoint,
            curComponent->m_Transform[3]
                .w); // Make sure the component is at the excat destination
        curComponent->velocity = glm::vec3(0.0f);
        lua_pushboolean(L, true); // arrived at waypoint
      }
    }

  } else {
    return luaL_error(L, "expecting exactly 4 or 7 arguments");
  }
  return 1;
}

int LuaComponent::l_OrientTo(lua_State *L) {
  int numArgs = lua_gettop(L); // Returns the index of the top element in the
                               // stack. Because indices start at 1, this result
                               // is equal to the number of elements in the
                               // stack (and so 0 means an empty stack).
  if (numArgs == 4 || numArgs == 7) {
    int currentComponentID =
        (int)lua_tonumber(L, 1); // Get the current component's ID
    glm::vec3 waypoint =
        glm::vec3((float)lua_tonumber(L, 2), (float)lua_tonumber(L, 3),
                  (float)lua_tonumber(L, 4)); // Retrieve the next three values
                                              // and assign them to a vec 3
                                              // (waypoint)/(destination)
    if (numArgs == 7) // The last three arguments are optional since the object
                      // already has its own velocity information defined
    {
      glm::vec3 maxVelocity =
          glm::vec3((float)lua_tonumber(L, 5), (float)lua_tonumber(L, 6),
                    (float)lua_tonumber(L, 7)); // Retrieve the next three
                                                // values and assign them to a
                                                // vec 3 (maxVelocity)
                                                // //std::thread(moveTo,
                                                // currentComponentID, waypoint,
                                                // maxVelocity).detach(); //
                                                // Non-blocking fire and forget
                                                // // #include <future> required
      cComponent *curComponent = cComponentManager::map_ComponentIDToComponent
          [currentComponentID]; // Retrieve the current component's data
      double deltaTime = glfwGetTime() - curComponent->lastTime;
      glm::mat4 targetOrientation =
          glm::lookAt(glm::vec3(curComponent->m_Transform[3]), waypoint,
                      glm::vec3(0.0f, 1.0f, 0.0));

      glm::quat targetQuat = glm::quat_cast(targetOrientation);
      glm::quat currentQuat = glm::quat_cast(curComponent->m_Transform);

      glm::quat difference = targetQuat * glm::inverse(currentQuat);
      // if(difference .x != 0)
      //		curComponent->m_Transform =
      //glm::rotate(curComponent->m_Transform, maxVelocity.x, glm::vec3(-1.0f,
      //0.0f, 0.0f));

      if (difference.y != 0)
        curComponent->m_Transform =
            glm::rotate(curComponent->m_Transform, maxVelocity.y,
                        glm::vec3(0.0f, -1.0f, 0.0f));

      // if (difference .z != 0)
      //		curComponent->m_Transform =
      //glm::rotate(curComponent->m_Transform, maxVelocity.z, glm::vec3(0.0f,
      //0.0f, -1.0f));

      float distance = glm::distance(
          glm::vec3(difference.x, difference.y, difference.z), glm::vec3(0.0f));
      if (glm::abs(distance) <= 0.01) {
        difference = targetQuat * glm::inverse(currentQuat);
        // curComponent->m_Transform = glm::rotate(curComponent->m_Transform,
        // difference.x, glm::vec3(-1.0f, 0.0f, 0.0f));
        curComponent->m_Transform =
            glm::rotate(curComponent->m_Transform, difference.y,
                        glm::vec3(0.0f, -1.0f, 0.0f));
        // curComponent->m_Transform = glm::rotate(curComponent->m_Transform,
        // difference.z, glm::vec3(0.0f, 0.0f, -1.0f));
        lua_pushboolean(L, true); // done rotating to destination
      } else {
        lua_pushboolean(L, false); // still rotating to destination
      }
      // float pi = glm::pi<float>();
    }

  } else {
    return luaL_error(L, "expecting exactly 4 or 7 arguments");
  }

  // Saving this as a reference for later :)
  // mat4 lookAt(vec3 position, vec3 target, vec3 up) {
  //	vec3 front = normalize(target - position);
  //	vec3 right = normalize(cross(up, front));
  //	up = cross(front, right);
  //	return mat4(right.x, up.x, front.x, position.x,
  //		right.y, up.y, front.y, position.y,
  //		right.z, up.z, front.z, position.z,
  //		0, 0, 0, 1);
  //}

  return 1;
}
int LuaComponent::l_SetCameraOffset(lua_State *L) {
  // Get the values that lua pushed and update object
  int componentID = (int)lua_tonumber(L, 1); /* get argument */
  float x = (float)lua_tonumber(L, 2);       /* get argument */
  float y = (float)lua_tonumber(L, 3);       /* get argument */
  float z = (float)lua_tonumber(L, 4);       /* get argument */
  float distance = (float)lua_tonumber(L, 5);

  gCamera->mOffset = glm::vec3(x, y, z);
  gCamera->mDistance = distance;

  return 1; // There were 7 things on the stack
}

int LuaComponent::l_AddLocationTrigger(lua_State *L) {
  locationTrigger locTrig;

  locTrig.position.x = (float)lua_tonumber(L, 1); /* get argument */
  locTrig.position.y = (float)lua_tonumber(L, 2); /* get argument */
  locTrig.position.z = (float)lua_tonumber(L, 3); /* get argument */
  locTrig.radius = (float)lua_tonumber(L, 4);     /* get argument */
  // locTrig.halfwidth.x = (float)lua_tonumber(L, 4);				/* get argument
  // */
  // locTrig.halfwidth.y = (float)lua_tonumber(L, 5);				/* get argument
  // */
  // locTrig.halfwidth.z = (float)lua_tonumber(L, 6);				/* get argument
  // */

  locationTriggers.push_back(locTrig);
  return 0;
}

int LuaComponent::l_SetCameraPosition(lua_State *L) {
  int componentID = (int)lua_tonumber(L, 1); /* get argument */

  gCamera->setTargetTransform(
      cComponentManager::map_ComponentIDToComponent[componentID]->m_Transform);
  gCamera->update(0.0);
  gCamera->mOffset = glm::vec3(0.0038f, 4.65f, -6.f);
  gCamera->mDistance = 200.0f;

  return 1; // There were 7 things on the stack
}