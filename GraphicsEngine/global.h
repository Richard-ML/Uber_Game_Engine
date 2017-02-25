#pragma once
#include "stdafx.h"
#include "cTextureManager.h"
#include "cMeshManager.h"
#include "cShaderManager.h"
#include "cCamera.h"
#include "cLightManager.h"
#include "cRenderManager.h"
#include "cGraphicsObject.h"
#include "cPlayerControlComponent.h"
// Window information
extern GLFWwindow *gWindow;
extern std::string gWindowTitle;

extern cCamera *gCamera; // Here for now..

// Shader Information
extern GLuint gProgramID;

/* Callback Functions */
void callback_windowResize(GLFWwindow *window, int width, int height);
void callback_WindowClose(GLFWwindow *window);
void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
	int mods);
void callback_Error(int, const char* error);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



extern 	std::vector<cGraphicsObject*> g_vec_pGraphicObjects;



// Uniform IDs
extern GLuint gVertexBufferID;
extern GLuint gTexCoordBufferID;
extern GLuint gNormalBufferID;
extern GLuint gTangentBufferID;
extern GLuint gTextureInfoBufferID;
extern GLuint gIndexBufferID;

// TODO: Remove unused
// Core Shader:
extern GLuint gProgramID;
extern int gUniformId_NumTextures;
extern GLuint gUniformId_Texture0;
extern GLuint gUniformId_Texture1;
extern GLuint gUniformId_Texture2;
extern GLuint gUniformId_Texture3;

extern GLuint gUniformId_MSTexture0;
extern GLuint gUniformId_Toggle_NormalAndSpecularMaps;

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


extern int gWindowWidth;
extern int gWindowHeight;


// Sky-box Shader:
extern GLuint gUniformId_ToggleSkyboxTextures;
extern bool gToggle_Skybox;
extern GLuint gSkyboxShaderID;
extern GLuint gSkyboxVMID;
extern GLuint gUniformId_SamplerCube;
extern GLuint gCubeVAO;


// Managers
extern cMeshManager *g_pMeshManager;
extern cTextureManager *g_pTextureManager;
extern cShaderManager *g_pShaderManager;
extern cLightManager *g_pLightManager;
extern cRenderManager *g_pRenderManager;




// Global toggles
extern bool g_bool_toggleLights;
extern bool g_bool_toggleTextures;

// Global Player control components
extern std::vector<cPlayerControlComponent> g_vec_playerControlComponents;