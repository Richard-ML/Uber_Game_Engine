#ifndef _global_HG_
#define _global_HG_
#include "cCamera.h"
#include "externals.h"
#include "cLight.h"
#include "cMesh.h"
#include "cTexture.h"

// Utilities
#include "cXML_Utils.h"

// Managers
#include "cWorld.h"
#include "cEntityManager.h"
#include "cMeshManager.h"
#include "cPathFindingManager.h"
#include "cShaderManager.h"
#include "cSoundManager.h"
#include "cTextureManager.h"

/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- global.h - Global ---------------------------------------*- C++ -*-===//
Description: Stores information about globally scoped attributes and routines.
//===----------------------------------------------------------------------===//
Author(s):
 Name: Richard Mills-Laursen
 Role(s): Student
 E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca
 
 Name: Michael Feeney
 E-mail: MFeeney@FanshaweC.ca
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
 
 Name: Lucas Gustafson
 E-Mail: L_Gustafson@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/

extern GLFWwindow *gWindow;
extern cCamera *gCamera;
extern std::vector<cMesh *> gTransparentMeshes;
extern std::vector<cLight *> gLights;
////////////////////////////////////////////////////////////////
// This will be replaced by the soundEmiterComponent when I get around to it..               
extern FMOD::System *gSoundSystem;         
extern FMOD_RESULT gFMODResult;            
extern float gVolume;                      
extern std::vector<FMOD::Sound *> gSounds; 
extern std::vector<FMOD::Channel *>
    gChannels; // Channels define the position of the sound		
extern std::map<std::string, FMOD::Sound *>
    map_gSounds; // This will be inside the sound manager
////////////////////////////////////////////////////////////////

extern int gEntityNextID;
extern int gNextMeshID;
extern int gNextSoundID;
extern int gNextChannelID;
extern int gNextComponentID;

/* Mesh Information */
struct triFace {
  glm::vec3 v1;
  glm::vec3 v2;
  glm::vec3 v3;
};
extern std::vector<std::vector<triFace *>> gMeshFaces;
extern std::vector<triFace *> playerMeshFaces;
extern std::vector<unsigned int> gIndices;
extern std::vector<cMeshVertex> gVertices;

/* Texture Information */
static int gCurrentMipmapLevel;
extern std::vector<GLuint> gUniform_TextureID;
extern GLuint gNextTextureID;
extern std::map<std::string, GLuint> g_pMapTextureNameToID;
extern std::map<std::string, GLuint> gMap_TextureNameToMipmapLevel;

/* Booleans */
extern bool g_bool_ToggleLights;
extern bool g_bool_ToggleTextures;
extern bool g_bool_ToggleSkybox;
extern bool g_bool_DebugShapes; // TODO: toggle debug shapes

/* Uniform Variable Information */
// Core Shader:
extern GLuint gProgramID;
extern int gUniformId_NumTextures;
extern GLuint gUniformId_Texture0;
extern GLuint gUniformId_Texture1;
extern GLuint gUniformId_Texture2;
extern GLuint gUniformId_Texture3;

extern GLuint gUniformId_NumLights;

extern GLuint gUniformId_ModelMatrix;
extern GLuint gUniformId_ModelMatrixOrientation;
extern GLuint gUniformId_ViewMatrix;
extern GLuint gUniformId_PojectionMatrix;
extern GLuint gUniformId_Alpha;
extern GLuint gUniformId_ModelColor;
extern GLuint gUniformId_EyePosition;

extern GLuint gUniformId_Toggle_Lights;
extern GLuint gUniformId_Toggle_Textures;

extern GLuint gVertexBufferID;
extern GLuint gTexCoordBufferID;
extern GLuint gNormalBufferID;
extern GLuint gTangentBufferID;
extern GLuint gTextureInfoBufferID;
extern GLuint gIndexBufferID;

// Sky-box Shader:
extern GLuint gUniformId_ToggleSkyboxTextures;
extern GLuint gUniformId_Toggle_Skybox;
extern GLuint gSkyboxShaderID;
extern GLuint gSkyboxVMID;
extern GLuint gUniformId_SamplerCube;
extern GLuint gUniformId_ToggleTextures;
extern GLuint cubeVAO; // , cubeVBO;
extern GLuint skyboxVAO, skyboxVBO;
// Physics Shader:
extern GLuint g_Physics_ProgramID;

extern GLuint gUniformId_Physics_ModelMatrix;
extern GLuint gUniformId_Physics_ModelMatrixOrientation;
extern GLuint gUniformId_Physics_ViewMatrix;
extern GLuint gUniformId_Physics_PojectionMatrix;
extern GLuint gUniformId_Physics_Alpha;
extern GLuint gUniformId_Physics_ModelColor;
extern GLuint gUniformId_Physics_EyePosition;

/* Utilities */
extern cXML_Utils *g_pXML_Utils;

/* Managers */
extern cWorld *g_pAreaInfo;
extern cMeshManager *g_pMeshManager;
extern cTextureManager *g_pTextureManager;
extern cShaderManager *g_pShaderManager;
extern cEntityManager *g_pEntityManager;
extern cSoundManager *g_pSoundManager;
extern cPathNodeGrid *g_pPathFindingManager;
extern PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine;

/* Callback Functions */
void callback_windowResize(GLFWwindow *window, int width, int height);
void callback_WindowClose(GLFWwindow *window);
void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
                       int mods);

/* Misc */
extern bool cheatsEnabled;
extern glm::vec3 gravityForce;
extern glm::mat4 playerTrans;
extern float gDistanceX;
extern float gDistanceZ;
extern int gNumXCells;
extern int gNumZCells;
extern std::string gPartOfTitle;

/* Tile Information */
extern std::vector<std::vector<std::vector<glm::vec3>>> g_vec_WorldTiles;
extern std::map<std::tuple<float, float, float>, cMeshEntry *> g_map_RGBToMesh;

/* BSP Tree */
struct AABB {
public:
  AABB(glm::vec3 minExtent, glm::vec3 maxExtent, int offsetToTriFaces,
       int numFaces, std::string nameOfMesh) {

    this->halfWidths =
        glm::abs(maxExtent - minExtent) * glm::vec3(0.5f); // Extent computation
    this->center = maxExtent - halfWidths; // Center position of AABB
    this->offsetToTriFaces = offsetToTriFaces;
    this->numFaces = numFaces;
    this->nameOfMesh =
        nameOfMesh; // Triangle mesh faces inside of the mesh's AABB
  };
  AABB(){};
  glm::vec3 center;
  int offsetToTriFaces;
  int numFaces;
  glm::vec3 halfWidths;
  std::string nameOfMesh;
  // std::vector<triFace*> triangleFaces;
};
extern std::multimap<std::pair<int, int>, AABB *> g_multimap_VoxelGrid;
extern std::vector<AABB *> gAABBs;
extern int gVoxelSize;
#endif