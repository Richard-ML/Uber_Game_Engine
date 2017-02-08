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
// Sky-box Shader:
extern GLuint gUniformId_ToggleSkyboxTextures;
extern bool gToggle_Skybox;
extern GLuint gSkyboxShaderID;
extern GLuint gSkyboxVMID;
extern GLuint gUniformId_SamplerCube;


// Managers
extern cMeshManager *g_pMeshManager;
extern cTextureManager *g_pTextureManager;
extern cShaderManager *g_pShaderManager;