
#define GLM_FORCE_SWIZZLE
#include <iostream>
#include "externals.h"
#include "global.h"
#include <Windows.h>
#include <algorithm>
#include <ctime>
#include <sstream> // "String Stream", used for setting the windows title
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- main.cpp - Entry Point ----------------------------------*- C++ -*-===//
Description: Execution starts here!
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
void createTheBuffers();
void bindTheBuffers();
bool setupTheShader();
void drawScene();


int main() {
	
	PhysicsEngine::cRigidBody* rb;
    rb =  g_pPhysicsEngine->createRigidBody();
	rb->setMass(13.3f);
	float mass;
	rb->getMass(mass);
	g_pPhysicsEngine->world.addRigidBody(rb);

	g_pPhysicsEngine->world.step(0.1f);
	rb->getMass(mass); // Mass has been changed within the world.step method :)


	gCamera = new cCamera();
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		system("pause");
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
		GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	// Open a window and create its OpenGL context
	gWindow =
		glfwCreateWindow(mode->width, mode->height, "Final Project!", NULL, NULL);
	if (gWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, "
			"they are not 3.3 compatible.\n");
		glfwTerminate();
		system("pause");
	}
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		system("pause");
	}
 
  // Register Callback Functions
  glfwSetWindowSizeCallback(gWindow, callback_windowResize);
  glfwSetWindowCloseCallback(gWindow, callback_WindowClose);
  glfwSetKeyCallback(gWindow, callback_KeyPress);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);

  glfwPollEvents();

  // Load the shader
  if (!setupTheShader()) {
    std::cout << "Oh no! The shaders didn't load!!" << std::endl;
    system("pause");
    return -1;
  }
  /////////////////////////
  // Generate a name for the texture
  glGenTextures(1, &gUniformId_Texture0);

  // Now bind it to the context using the GL_TEXTURE_2D binding point
  glBindTexture(GL_TEXTURE_2D, gUniformId_Texture0);

  // Specify the amount of storage we want to use for the texture
  glTexStorage2D(GL_TEXTURE_2D, // 2D texture
                 1,             // 8 mipmap levels
                 GL_RGBA32F,    // 32-bit floating-point RGBA data
                 7680, 7680);   // 512 x 512 texels
  //////////////////////

  // Load game data from XML
  g_pEntityManager->loadGameFromXML(
      "GameAssets.xml"); // Load the data stored in the XML file
  srand((unsigned)time(NULL));

  createTheBuffers();

  // Initialize AStar Path finding Information
  std::cout << "Processing Path-finding data.." << std::endl;
  g_pPathFindingManager =
      new cPathNodeGrid(128.f, 512, 512, glm::vec3(0.0f),
                        glm::vec3(g_pAreaInfo->maxPos.x, 0.0f, 0.0f),
                        glm::vec3(0.0f, 0.0f, g_pAreaInfo->maxPos.z));

  g_pPathFindingManager;

  static float lastTime = (float)glfwGetTime();
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);
  do {
    g_pSoundManager->update();

    std::stringstream tempText;
    tempText << "Lights enabled: " << g_bool_ToggleLights
             << "(KP_1) Skybox enabled: " << g_bool_ToggleSkybox
             << "(KP_2) Textures enabled: " << g_bool_ToggleTextures
             << " (KP_3) Debug-Shapes enabled: " << g_bool_DebugShapes
             << "(KP_4) ";
    gPartOfTitle += tempText.str();
    glUniform1i(gUniformId_Toggle_Lights, g_bool_ToggleLights);
    glUniform1i(gUniformId_Toggle_Textures, g_bool_ToggleTextures);
    // Update Light Values
    glUniform1i(gUniformId_Toggle_Lights, g_bool_ToggleLights);
    glUniform1i(gUniformId_NumLights, gLights.size());
    for (auto iter = gLights.begin(); iter != gLights.end(); iter++) {
      glUniform1i((*iter)->gUniformId_IsEnabled, (*iter)->isEnabled);
      glUniform1i((*iter)->gUniformId_TypeFlag, (*iter)->typeFlag);
      glUniform3fv((*iter)->gUniformId_Position, 1,
                   glm::value_ptr((*iter)->matrix[3]));
      glUniform3fv((*iter)->gUniformId_Direction, 1,
                   glm::value_ptr((*iter)->direction));
      glUniform1f((*iter)->gUniformId_ConeAngle, (*iter)->coneAngle);
      glUniform3fv((*iter)->gUniformId_Ambient, 1,
                   glm::value_ptr((*iter)->ambient));
      glUniform3fv((*iter)->gUniformId_Diffuse, 1,
                   glm::value_ptr((*iter)->diffuse));
      glUniform3fv((*iter)->gUniformId_Specular, 1,
                   glm::value_ptr((*iter)->specular));
      glUniform1f((*iter)->gUniformId_SpecularPower, (*iter)->specularPower);
      glUniform1f((*iter)->gUniformId_AttenuationConst, (*iter)->attenConst);
      glUniform1f((*iter)->gUniformId_AttenuationLinear, (*iter)->attenLinear);
      glUniform1f((*iter)->gUniformId_AttenuationQuad, (*iter)->attenQuad);
    }

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glClearColor(0, 0, 0, 0);
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
    // flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gCamera->update(0.1f);

    //////////////////////////////////////////////////////////
    //                  Render The Skybox                  //
    /////////////////////////////////////////////////////////
    if (g_bool_ToggleSkybox) {
      ///////////////////////
      static const GLfloat gray[] = {0.2f, 0.2f, 0.2f, 1.0f};
      static const GLfloat ones[] = {1.0f};
      glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
      glEnable(GL_CULL_FACE);
      glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only thing
                                       // you can pass here
                    GL_FILL);
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
      float currentTime = (float)glfwGetTime() - lastTime;
      float t = currentTime * 0.1f; // TODO: DeltaTime
      glm::mat4 viewMatrix;
      gCamera->getViewMatrix(viewMatrix);
      glm::vec3 eyePos = gCamera->getEyePosition();
      // ;)
      glm::mat4 camViewInverse(viewMatrix[0][0], viewMatrix[1][0],
                               viewMatrix[2][0], 0.0f, viewMatrix[0][1],
                               viewMatrix[1][1], viewMatrix[2][1], 0.0f,
                               viewMatrix[0][2], viewMatrix[1][2],
                               viewMatrix[2][2], 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);

      camViewInverse = glm::inverse(camViewInverse);
      // gCamera->getViewMatrix(viewMatrix); //glm::lookAt(glm::vec3(15.0f *
      // sinf(t), 0.0f, 15.0f * cosf(t)),
      // glm::vec3(0.0f, 0.0f, 0.0f),
      // glm::vec3(0.0f, 1.0f, 0.0f));
      glClearBufferfv(GL_COLOR, 0, gray);
      glClearBufferfv(GL_DEPTH, 0, ones);
      glUseProgram(gSkyboxShaderID);

      glUniform1i(gUniformId_ToggleSkyboxTextures, g_bool_ToggleTextures);

      glUniform1iARB(gUniformId_SamplerCube, g_pMapTextureNameToID["Skybox"]);
      glUniformMatrix4fv(gSkyboxVMID, 1, GL_FALSE,
                         glm::value_ptr(camViewInverse));

      glDisable(GL_DEPTH_TEST);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      glEnable(GL_DEPTH_TEST);
      ///////////////////////
    }

    // glUniform1i(glGetUniformLocation(gProgramID, "Texture0"),
    // gUniformId_Texture0);
    // glActiveTexture(glGetUniformLocation(gProgramID, "Texture0"));
    /////////////////////////////////////////////////////////
    //                  Render The Scene                  //
    ////////////////////////////////////////////////////////
    bindTheBuffers();
    glUseProgram(gProgramID);

    drawScene();
    // Important to update system
    g_pSoundManager->update();

    // Swap buffers
    glfwSwapBuffers(gWindow);
    glfwPollEvents();
    lastTime = (float)glfwGetTime();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(gWindow));

  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  delete gCamera; // clean and tidy :)
  // Time to take out the trash..
  for (std::vector<FMOD::Sound *>::iterator curSound = gSounds.begin();
       curSound < gSounds.end(); curSound++)
    (*curSound)->release();
  gSoundSystem->close();
  gSoundSystem->release();
  //*************** Delete Managers ***************
  // delete g_pMeshManager;
  // delete g_pShaderManager;
  // delete g_pEntityManager;
  //*************** Delete Entities ***************
  // std::vector<cEntity*>::iterator it = gEntities.begin();
  // for (; it != gEntities.end(); it++)
  //	delete (*it); //Delete the data of each vector element
  // gEntities.clear(); //Clear the vector since all of its elements are now
  // null
  return 0;
}

struct mesh_to_camera_less_than {
  inline bool operator()(cMesh *struct1, cMesh *struct2) {
    glm::vec4 camPos = gCamera->getEyePosition();
    return (glm::distance(glm::vec3(camPos.xyz),
                          glm::vec3(struct1->m_curTransform[3].xyz)) <
            glm::distance(glm::vec3(camPos.xyz),
                          glm::vec3(struct2->m_curTransform[3].xyz)));
  }
};


void drawScene() {
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  gCamera->getProjectionMatrix(projectionMatrix);
  gCamera->getViewMatrix(viewMatrix);

  bindTheBuffers();
  glUseProgram(gProgramID);

  // per frame uniforms
  glUniformMatrix4fv(gUniformId_PojectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(projectionMatrix));
  glUniformMatrix4fv(gUniformId_ViewMatrix, 1, GL_FALSE,
                     glm::value_ptr(viewMatrix));
  glm::vec4 eye4;
  gCamera->getEyePosition(eye4);
  glUniform4fv(gUniformId_EyePosition, 1, glm::value_ptr(eye4));

  glUseProgram(g_Physics_ProgramID);

  // Physics Uniforms
  glUniformMatrix4fv(gUniformId_Physics_ViewMatrix, 1, GL_FALSE,
                     glm::value_ptr(viewMatrix));
  glUniformMatrix4fv(gUniformId_Physics_PojectionMatrix, 1, GL_FALSE,
                     glm::value_ptr(projectionMatrix));
  glUniform4fv(gUniformId_Physics_EyePosition, 1, glm::value_ptr(eye4));

  glUseProgram(gProgramID);

  g_pEntityManager->drawEntites();
  // This needs to be done after all of the entities have been updated, but
  // before they have been drawn..
  std::sort(gTransparentMeshes.begin(), gTransparentMeshes.end(),
            mesh_to_camera_less_than());
  for (std::vector<cMesh *>::iterator iter = gTransparentMeshes.begin();
       iter != gTransparentMeshes.end(); iter++) {
    if ((*iter)->alpha < 1.0f) // Transparent meshes are drawn in the main loop
    {
      if ((*iter)->isEnabled) {

        glEnable(GL_BLEND);
        // glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        if ((*iter)->isWireframe) {        // Turn off backface culling
                                           // Enable "wireframe" polygon mode
          glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only
                                           // thing you can pass here
                        GL_LINE);          // GL_POINT, GL_LINE, or GL_FILL
          glDisable(GL_CULL_FACE);
        } else { // "Regular" rendering:
          // Turn on backface culling
          // Turn polygon mode to solid (Fill)
          glCullFace(GL_BACK); // GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
          glEnable(GL_CULL_FACE);
          glPolygonMode(GL_FRONT_AND_BACK, // GL_FRONT_AND_BACK is the only
                                           // thing you can pass here
                        GL_FILL);          // GL_POINT, GL_LINE, or GL_FILL
        }
        glUniformMatrix4fv(gUniformId_ModelMatrix, 1, GL_FALSE,
                           glm::value_ptr(glm::scale((*iter)->m_curTransform,
                                                     (*iter)->scale)));
        glUniformMatrix4fv(gUniformId_ModelMatrixOrientation, 1, GL_FALSE,
                           glm::value_ptr((*iter)->getOrientation()));
        glUniform4fv(gUniformId_ModelColor, 1,
                     glm::value_ptr(*(*iter)->m_pColor));

        glUniform1f(gUniformId_Alpha, (*iter)->alpha);
        glDrawElementsBaseVertex(
            GL_TRIANGLES, (*iter)->m_pMeshEntry->NumgIndices, GL_UNSIGNED_INT,
            (void *)(sizeof(unsigned int) * (*iter)->m_pMeshEntry->BaseIndex),
            (*iter)->m_pMeshEntry->BaseVertex);

        // glDrawArrays(GL_TRIANGLES,
        //	0, /*start from start of buffer*/
        //	3 /*number of verticies*/);
      }
    } else
      gTransparentMeshes.erase(iter);
  }
}

bool setupTheShader() {
  // Create and compile our GLSL program from the shaders
  gProgramID = ::g_pShaderManager->loadShaders(
      "SimpleShader.vert", "SimpleShader.frag", "SimpleShader.geom");

  if (!gProgramID) {
    std::cout << "Shaders failed to load!!" << std::endl;
    return false;
  }

  // per item
  // Uber shader
  gUniformId_ModelMatrix = glGetUniformLocation(gProgramID, "ModelMatrix");
  gUniformId_ModelMatrixOrientation =
      glGetUniformLocation(gProgramID, "ModelMatrixOrientation");
  gUniformId_Alpha = glGetUniformLocation(gProgramID, "Alpha");
  gUniformId_ModelColor = glGetUniformLocation(gProgramID, "ModelColor");
  gUniformId_NumLights = glGetUniformLocation(gProgramID, "NUM_LIGHTS");

  gUniformId_ViewMatrix = glGetUniformLocation(gProgramID, "ViewMatrix");
  gUniformId_PojectionMatrix =
      glGetUniformLocation(gProgramID, "ProjectionMatrix");
  gUniformId_EyePosition = glGetUniformLocation(gProgramID, "EyePosition");

  gUniformId_Toggle_Lights = glGetUniformLocation(gProgramID, "Toggle_Lights");
  gUniformId_Toggle_Skybox = glGetUniformLocation(gProgramID, "Toggle_Skybox");
  gUniformId_Toggle_Textures =
      glGetUniformLocation(gProgramID, "Toggle_Textures");

  gUniformId_NumTextures = glGetUniformLocation(gProgramID, "NUM_TEXTURES");
  gUniformId_Texture0 = glGetUniformLocation(gProgramID, "Texture0");
  gUniformId_Texture1 = glGetUniformLocation(gProgramID, "Texture1");
  gUniformId_Texture2 = glGetUniformLocation(gProgramID, "Texture2");
  gUniformId_Texture3 = glGetUniformLocation(gProgramID, "Texture3");

  // Physics Shader
  g_Physics_ProgramID = ::g_pShaderManager->loadShaders(
      "Physics.vert", "Physics.frag", "Physics.geom");
  gUniformId_Physics_ModelMatrix =
      glGetUniformLocation(g_Physics_ProgramID, "ModelMatrix");
  gUniformId_Physics_ModelMatrixOrientation =
      glGetUniformLocation(g_Physics_ProgramID, "ModelMatrixOrientation");
  gUniformId_Physics_ViewMatrix =
      glGetUniformLocation(g_Physics_ProgramID, "ViewMatrix");
  gUniformId_Physics_PojectionMatrix =
      glGetUniformLocation(g_Physics_ProgramID, "ProjectionMatrix");
  gUniformId_Physics_Alpha = glGetUniformLocation(g_Physics_ProgramID, "Alpha");
  gUniformId_Physics_ModelColor =
      glGetUniformLocation(g_Physics_ProgramID, "ModelColor");
  gUniformId_Physics_EyePosition =
      glGetUniformLocation(g_Physics_ProgramID, "EyePosition");

  // Skybox Shader
  ::gSkyboxShaderID =
      ::g_pShaderManager->loadShaders("Skybox.vs.glsl", "Skybox.fs.glsl");
  ::gUniformId_ToggleSkyboxTextures =
      glGetUniformLocation(gSkyboxShaderID, "Toggle_Skybox_Textures");
  gSkyboxVMID = glGetUniformLocation(gSkyboxShaderID, "view_matrix");

  gUniformId_SamplerCube = glGetUniformLocation(gSkyboxShaderID, "skybox");
  // glActiveTexture(gUniformId_SamplerCube);

  return true;
}

void createTheBuffers() {
  glGenVertexArrays(1, &gVertexBufferID);
  glBindVertexArray(gVertexBufferID);

  glGenBuffers(1, &gTexCoordBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);

  glGenBuffers(1, &gNormalBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);

  glGenBuffers(1, &gTangentBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTangentBufferID);

  glGenBuffers(1, &gTextureInfoBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTextureInfoBufferID);

  int bytesInVertexArray = gVertices.size() * sizeof(cMeshVertex);
  glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &gVertices[0],
               GL_STATIC_DRAW);

  glGenBuffers(1, &gIndexBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

  int bytesInIndexArray = gIndices.size() * sizeof(GLuint);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &gIndices[0],
               GL_STATIC_DRAW);

  bindTheBuffers();
}

void bindTheBuffers() {
  glBindVertexArray(gVertexBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTexCoordBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gNormalBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTangentBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, gTextureInfoBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBufferID);

  glEnableVertexAttribArray(0); // Position
  glEnableVertexAttribArray(1); // TexCoord
  glEnableVertexAttribArray(2); // Normal
  glEnableVertexAttribArray(3); // Texture DiffuseID
  glEnableVertexAttribArray(4); // Texture DiffuseID

  glVertexAttribPointer(
      0,        // index or "slot" in the shader
      4,        // Number of variables: vec4 would be 4 32-bit variables
      GL_FLOAT, // Type: vec4 is float
      GL_FALSE, // "normalize" the values (or not)
      sizeof(cMeshVertex), // Number of bytes per vertex (the "stride")
      (GLvoid *)0); // Offset from vertex (position is first, so offset = 0

  int offsetToTexCoordsInBytes = sizeof(((cMeshVertex *)0)->Position);
  glVertexAttribPointer(
      1, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
      (GLvoid *)offsetToTexCoordsInBytes); // Offset in bytes to TexCoords

  int offsetToNormalInBytes =
      offsetToTexCoordsInBytes + sizeof(((cMeshVertex *)0)->TexCoord);
  glVertexAttribPointer(
      2, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
      (GLvoid *)offsetToNormalInBytes); // Offset in bytes to Normal

  int offsetToTangentInBytes =
      offsetToNormalInBytes + sizeof(((cMeshVertex *)0)->Normal);
  glVertexAttribPointer(
      3, 4, GL_FLOAT, GL_FALSE, sizeof(cMeshVertex),
      (GLvoid *)offsetToTangentInBytes); // Offset in bytes to Tangent

  int offsetToTextureInfoInBytes =
      offsetToTangentInBytes + sizeof(((cMeshVertex *)0)->Tangent);
  glVertexAttribIPointer(
      4, 4, GL_UNSIGNED_INT, sizeof(cMeshVertex),
      (GLvoid *)offsetToTextureInfoInBytes); // Offset in bytes to Texture Info
}
