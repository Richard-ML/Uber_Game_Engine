
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
#include <list>

extern "C" {

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>


	// TODO: Lua script should only call TWO c/c++ functions! UPDATEREQUIRED.. 
	// BaseController.lua acts as a script interface.. It creates a metatable.. Of routine information.. 

	// Each row is a stage.. each cell is a routine & related parameters. C++ simply interprets said metatable into some type of
	// list.. Then dispatches corresponding functions with said arguments... WAAAAY FAAAAAAAAASSSSSSTER then letting lua bottle-neck
	// the shit out of everything with its MANY c++ function calls.. The two languages do not work well together that way when time is
	// crucial

	// The parameters will a list of function queue elements containing a string which maps to a C++ function and its list of arguments. 

	// http://rubenlaguna.com/wp/2012/12/09/accessing-cpp-objects-from-lua/
	// Still looking for optimal solution for exchanging lists from LUA to C. 
	// The parameters will a list of function queue elements containing a string which maps to a C++ function and its list of arguments. 
	// Push elements from LUA 
	//static int l_queue_push(lua_State *L) {
		//assert(lua_gettop(L) == 2); // confirm that the number of arguments is 2 
		//std::list<int> **ud = static_cast<std::list<int> **>(luaL_checkudata(L, 1, "ListMT")); // The first argument is a list
		//int v = luaL_checkint(L, 2); // seconds argument is the integer to be pushed to the std::list<int>
		//(*ud)->push_back(v); // perform the push on C++ object through the pointer stored in user data
		//return 0;
	//}
}

typedef int (*lua_CFunction)(lua_State *L);
struct locationTrigger {
  glm::vec3 position;
  float radius;
  // TODO: Trigger callback -- For now I am just displaying that the trigger was
  // triggered. :P These triggers should be ghost objects inside of the physics engine!!
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
   // End of methods called from LUA

  //Wander // Called by AI that have the wander state active
  //Hunt // Called by AI that are hunting another entity
  //SetCameraPosition(lua_State *L);
  //SetCameraOffset(lua_State *L);
  //AddLocationTrigger(lua_State *L); // Physics engine!

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
  std::ifstream theFile(filename); // load the XML data into the buffer
  std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
                            std::istreambuf_iterator<char>());
  bufData.push_back('\0');
  return std::string(bufData.begin(), bufData.end());
}

LuaComponent::LuaComponent() {
  this->m_pLuaState = luaL_newstate();
  luaL_openlibs(this->m_pLuaState); // LUA v5.3.3

  // Core component information exchange methods
  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_SetComponentState);
  lua_setglobal(this->m_pLuaState, "setComponentState");

  lua_pushcfunction(this->m_pLuaState, LuaComponent::l_GetComponentState);
  lua_setglobal(this->m_pLuaState, "getComponentState");


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
        this->m_pLuaState, /* LUA state */
        0,  /* nargs: number of arguments pushed onto the LUA stack */
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
      std::cout << "Error running LUA script: ";
      std::cout << itScript->first << std::endl;
      std::cout << luaError << std::endl;
      std::cout << "-------------------------------------------------------"
                << std::endl;
      // We passed zero (0) as errfunc, so error is on stack)
      lua_pop(this->m_pLuaState, 1); /* pop error message from the stack */

      continue;
    }
  }
        for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
                 this->m_vec_pComponents.begin();
             iter != this->m_vec_pComponents.end(); iter++) {
          (*iter)->setTransform(this->getTransform());
          (*iter)->update();
        }
        this->lastTime = (float)glfwGetTime();
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
    std::cout << "Error running LUA script: ";
    std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
    std::cout << "-------------------------------------------------------"
              << std::endl;
    // return;
  }

  // execute function in "protected mode", where problems are
  //  caught and placed on the stack for investigation
  error = lua_pcall(
      this->m_pLuaState, /* LUA state */
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
    return "LUA: no error";
    break;
  case LUA_ERRSYNTAX:
    return "LUA: syntax error";
    break;
  case LUA_ERRMEM:
    return "LUA: memory allocation error";
    break;
  case LUA_ERRRUN:
    return "LUA: Runtime error";
    break;
  case LUA_ERRERR:
    return "LUA: Error while running the error handler function";
    break;
  } // switch ( error )

  // Who knows what this error is?
  return "LUA: UNKNOWN error";
}

// Called by LUA
// Passes object ID, new velocity, etc.
// Returns valid (true or false)
// Passes:
// - position (xyz)
// - velocity (xyz)
// called "setObjectState" in LUA
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


  lua_pushboolean(L, true); // index is OK

  return 1; // There were 7 things on the stack
}

// Passes object ID
// Returns valid (true or false)
// - position (xyz)
// - velocity (xyz)
// called "getObjectState" in LUA
int LuaComponent::l_GetComponentState(lua_State *L) {
  int componentID = (int)lua_tonumber(L, 1); /* get argument */
  cComponent *currentCommponent =
      cComponentManager::map_ComponentIDToComponent[componentID];

  // Object ID is valid
  lua_pushboolean(L, true); // index is OK
 // lua_pushnumber(L, currentCommponent->m_Transform[3].x);

  return 13; // There were 7 things on the stack
}