#include "stdafx.h"
#include "global.h"

// Window information
GLFWwindow * gWindow = 0;
std::string gWindowTitle = "";
cCamera *gCamera = new cCamera(); // Here for now..

// Core Shader:
GLuint gProgramID = 0;
int gUniformId_NumTextures = 0;
GLuint gUniformId_Texture0 = 0;
GLuint gUniformId_Texture1 = 0;
GLuint gUniformId_Texture2 = 0;
GLuint gUniformId_Texture3 = 0;

GLuint gUniformId_NumLights = 0;

GLuint gUniformId_ModelMatrix = 0;
GLuint gUniformId_ModelMatrixOrientation = 0;
GLuint gUniformId_ViewMatrix = 0;
GLuint gUniformId_PojectionMatrix = 0;
GLuint gUniformId_Alpha = 0;
GLuint gUniformId_ModelColor = 0;
GLuint gUniformId_EyePosition = 0;

GLuint gUniformId_Toggle_Lights = 0;
GLuint gUniformId_Toggle_Textures = 0;


//**************** Initialize Managers ****************
cMeshManager *cMeshManager::s_cMeshManager =
0; // Allocating pointer to static instance of cMeshManager (singleton)
cMeshManager *g_pMeshManager = cMeshManager::instance();
cTextureManager *cTextureManager::s_cTextureManager =
0; // Allocating pointer to static instance of cTextureManager (singleton)
cTextureManager *g_pTextureManager = cTextureManager::instance();
cShaderManager *cShaderManager::s_cShaderManager =
0; // Allocating pointer to static instance of cShaderManager (singleton)
cShaderManager *g_pShaderManager = cShaderManager::instance();



// Uniform IDs
GLuint gVertexBufferID = 0;
GLuint gTexCoordBufferID = 0;
GLuint gNormalBufferID = 0;
GLuint gTangentBufferID = 0;
GLuint gTextureInfoBufferID = 0;
GLuint gIndexBufferID = 0;
// Sky-box Shader:
GLuint gUniformId_ToggleSkyboxTextures = 0;
bool gToggle_Skybox = 1;
GLuint gSkyboxShaderID = 0;
GLuint gSkyboxVMID = 0;
GLuint gUniformId_SamplerCube = 0;
GLuint gCubeVAO = 0;


// Global toggles
bool g_bool_toggleLights = false;
bool g_bool_toggleTextures = true;


// TODO: Move this 
