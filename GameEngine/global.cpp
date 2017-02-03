#include "global.h"
#include <iostream>

GLFWwindow * gWindow = 0;
GLuint gSkyboxShaderID = 0;
GLuint gSkyboxVMID = 0;
cCamera *gCamera = new cCamera();
std::vector<cMesh *> gTransparentMeshes;
int gNextPointLightID = 0;
int gNextSpotLightID = 0;
int gNextDirectionalLightID = 0;

int gVoxelSize = 128;
std::map<std::string, GLuint> gMap_TextureNameToMipmapLevel;
std::vector<cLight *> gLights;
bool g_bool_DebugShapes = true;
GLuint cubeVAO = 0; 
GLuint gUniformId_SamplerCube = 0;
GLuint gUniformId_ToggleTextures = 0;

std::multimap<std::pair<int, int>, AABB *> g_multimap_VoxelGrid;
std::vector<std::vector<std::vector<glm::vec3>>> g_vec_WorldTiles;
std::map<std::tuple<float, float, float>, cMeshEntry *> g_map_RGBToMesh;
int gNextComponentID = 0;
int gEntityNextID = 0;
int gNextChannelID = 0;
GLuint gVertexBufferID = 0;
GLuint gTexCoordBufferID = 0;
GLuint gNormalBufferID = 0;
GLuint gTangentBufferID = 0;
GLuint gIndexBufferID = 0;
GLuint gTextureInfoBufferID = 0;
std::vector<cMeshVertex> gVertices;
std::vector<unsigned int> gIndices;
std::vector<std::vector<triFace *>> gMeshFaces;
std::vector<triFace *> playerMeshFaces;

std::vector<AABB *> gAABBs;

float gVolume = 1.0f;
float gPitch = 1.0f;
float gBalance = 0.0f;

int curSphereIndex = -15;
std::string gPartOfTitle = "";


GLuint gProgramID = 0;
GLuint gUniformId_ModelMatrix = 0;
GLuint gUniformId_ModelMatrixOrientation = 0;
GLuint gUniformId_ViewMatrix = 0;
GLuint gUniformId_PojectionMatrix = 0;
GLuint gUniformId_Alpha = 0;
GLuint gUniformId_ModelColor = 0;
GLuint gUniformId_EyePosition = 0;

GLuint g_Physics_ProgramID = 0;
GLuint gUniformId_Physics_ModelMatrix = 0;
GLuint gUniformId_Physics_ModelMatrixOrientation = 0;
GLuint gUniformId_Physics_ViewMatrix = 0;
GLuint gUniformId_Physics_PojectionMatrix = 0;
GLuint gUniformId_Physics_Alpha = 0;
GLuint gUniformId_Physics_ModelColor = 0;
GLuint gUniformId_Physics_EyePosition = 0;

GLuint gUniformId_NumLights = 0;

GLuint gUniformId_Toggle_Lights = 0;
GLuint gUniformId_Toggle_Textures = 0;
GLuint gUniformId_ToggleSkyboxTextures = 0;
GLuint gUniformId_Toggle_Skybox = 0;
bool g_bool_ToggleLights = 1;
bool g_bool_ToggleTextures = 1;
bool g_bool_ToggleSkybox = 1;

std::vector<GLuint> gUniform_TextureID;
GLuint gNextTextureID = 0;
std::map<std::string, GLuint> g_pMapTextureNameToID;

int gUniformId_NumTextures = 0;
GLuint gUniformId_Texture0 = 0;
GLuint gUniformId_Texture1 = 0;
GLuint gUniformId_Texture2 = 0;
GLuint gUniformId_Texture3 = 0;

float gDistanceX;
float gDistanceZ;
int gNumXCells;
int gNumZCells;

float deltaTime = 0.1f;
//**************** Initialize Utilities ****************
cXML_Utils *cXML_Utils::s_cXML_Utils =
    0; // Allocating pointer to static instance of cXML_Utils (singleton)
cXML_Utils *g_pXML_Utils = cXML_Utils::instance();

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
cEntityManager *cEntityManager::s_cEntityManager =
    0; // Allocating pointer to static instance of cEntityManager (singleton)
cEntityManager *g_pEntityManager = cEntityManager::instance();

cPathNodeGrid *g_pPathFindingManager = 0;
// TODO: Sound Engine... Same design as PhysicsEngine..
PhysicsEngine::cPhysicsEngine *g_pPhysicsEngine = PhysicsEngine::cPhysicsEngine::instance();
//GraphicsEngine::cGraphicsEngine *g_pGraphicsEngine = GraphicsEngine::cGraphicsEngine::instance();