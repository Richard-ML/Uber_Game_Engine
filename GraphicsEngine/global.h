#pragma once
#include "stdafx.h"
#include "cTextureManager.h"
#include "cMeshManager.h"
#include "cShaderManager.h"
#include "cCamera.h"


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
extern bool gToggle_Skybox;
extern GLuint gSkyboxShaderID;
extern GLuint gSkyboxVMID;
extern GLuint gUniformId_SamplerCube;
extern GLuint gCubeVAO;


// Managers
extern cMeshManager *g_pMeshManager;
extern cTextureManager *g_pTextureManager;
extern cShaderManager *g_pShaderManager;



