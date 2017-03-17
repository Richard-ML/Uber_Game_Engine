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
GLuint gUniformId_MSTexture0 = 0;

GLuint gUniformId_Toggle_NormalAndSpecularMaps = 0;

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

std::vector<cGraphicsObject*> g_vec_pGraphicObjects;

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

cLightManager* cLightManager::s_cLightManager = 0;
cLightManager* g_pLightManager = cLightManager::instance();

cRenderManager* cRenderManager::s_cRenderManager = 0;
cRenderManager* g_pRenderManager = cRenderManager::instance();

iGameState * g_pGameState = 0;
//iDebugRenderer * g_pDebugRenderer = 0;

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

int gWindowWidth = 0;
int gWindowHeight = 0;


// Global toggles
bool g_bool_toggleLights = true;
bool g_bool_toggleTextures = true;
bool g_bool_toggleStencil = false;
bool g_bool_toggleScissor = false;

// Global Player control components
std::vector<cPlayerControlComponent> g_vec_playerControlComponents;