#include "global.h"
#include <sstream>

//                _  _  _                   _                  _             _
//                ____             _
//     ___  __ _ | || || |__    __ _   ___ | | __   __      __(_) _ __    __| |
//     ___ __      __|  _ \  ___  ___ (_) ____ ___
//    / __|/ _` || || || '_ \  / _` | / __|| |/ /   \ \ /\ / /| || '_ \  / _` | / _ \\ \ /\ / /| |_) |/ _ \/ __|| ||_  // _ \
//   | (__| (_| || || || |_) || (_| || (__ |   <     \ V  V / | || | | || (_| || (_) |\ V  V / |  _ <|  __/\__ \| | / /|  __/
//    \___|\__,_||_||_||_.__/  \__,_| \___||_|\_\_____\_/\_/  |_||_| |_| \__,_|
//    \___/  \_/\_/  |_| \_\\___||___/|_|/___|\___|
//                                              |_____|
void callback_windowResize(GLFWwindow *window, int width, int height) {
  if (width != 0 && height != 0)
    gCamera->windowResize(width, height);
}
//                _  _  _                   _      __        __ _             _
//                ____  _
//     ___  __ _ | || || |__    __ _   ___ | | __  \ \      / /(_) _ __    __| |
//     ___ __      __ / ___|| |  ___   ___   ___
//    / __|/ _` || || || '_ \  / _` | / __|| |/ /   \ \ /\ / / | || '_ \  / _` | / _ \\ \ /\ / /| |    | | / _ \ / __| / _ \
//   | (__| (_| || || || |_) || (_| || (__ |   <     \ V  V /  | || | | || (_| || (_) |\ V  V / | |___ | || (_) |\__ \|  __/
//    \___|\__,_||_||_||_.__/  \__,_| \___||_|\_\_____\_/\_/   |_||_| |_| \__,_|
//    \___/  \_/\_/   \____||_| \___/ |___/ \___|
//                                              |_____|
void callback_WindowClose(GLFWwindow *window) {
  glfwSetWindowShouldClose(window, 1);
}
//                _  _  _                   _          _  __             ____
//     ___  __ _ | || || |__    __ _   ___ | | __     | |/ / ___  _   _ |  _ \
//     _ __  ___  ___  ___
//    / __|/ _` || || || '_ \  / _` | / __|| |/ /     | ' / / _ \| | | || |_) ||
//    '__|/ _ \/ __|/ __|
//   | (__| (_| || || || |_) || (_| || (__ |   <      | . \|  __/| |_| ||  __/ | |  |  __/\__ \\__ \
//    \___|\__,_||_||_||_.__/  \__,_| \___||_|\_\_____|_|\_\\___| \__, ||_|    |_|   \___||___/|___/
//                                              |_____|           |___/
void callback_KeyPress(GLFWwindow *window, int key, int scancode, int action,
                       int mods) {
  // 3 seconds
  //'i' 'd' 'k' 'f' 'a'

  static double lasttime; // = glfwGetTime();

  static bool bI, bD, bK, bF, bA;
  if (lasttime != 0.0) {
    double currentTime = glfwGetTime();
    double elapsedTime = lasttime - currentTime;

    if (std::abs(elapsedTime) >= 3)
      lasttime = 0;
  }
  if (action == 1)
    switch (key) {
    case GLFW_KEY_X:
      // g_pEntityManager->dropBomb();
      break;

    case GLFW_KEY_KP_4:
      g_bool_DebugShapes = !g_bool_DebugShapes;
      break;

    case GLFW_KEY_I:
      bI = true;
      bD = false;
      bK = false;
      bF = false;
      bA = false;
      lasttime = glfwGetTime();
      break;

    case GLFW_KEY_D:
      if (bI == true)
        bD = true;
      else {
        bI = false;
        bD = false;
        bK = false;
        bF = false;
        bA = false;
        lasttime = 0.0;
      }
      break;
    case GLFW_KEY_K:
      if (bD == true)
        bK = true;
      else {
        bI = false;
        bD = false;
        bK = false;
        bF = false;
        bA = false;
        lasttime = 0.0;
      }
      break;
    case GLFW_KEY_F:
      if (bK == true)
        bF = true;
      else {
        bI = false;
        bD = false;
        bK = false;
        bF = false;
        bA = false;
        lasttime = 0.0;
      }
      break;
    case GLFW_KEY_A:
      if (bF == true) {
        bD = true;
        std::cout << "Cheats enabled! Press Q and E to fly!";
        cheatsEnabled = true;
      } else {
        bI = false;
        bD = false;
        bK = false;
        bF = false;
        bA = false;
        lasttime = 0.0;
      }
      break;
    // case GLFW_KEY_KP_6:
    //	gVec_Translate.x = -0.01f;
    //	break;
    // case GLFW_KEY_KP_8:
    //	gVec_Translate.z = 0.01f;
    //	break;
    case GLFW_KEY_KP_2:
      g_bool_ToggleSkybox = !g_bool_ToggleSkybox;
      break;
    // case GLFW_KEY_KP_7:
    //	gVec_Translate.y = 0.01f;
    //	break;
    // case GLFW_KEY_KP_9:
    //	gVec_Translate.y = -0.01f;
    //	break;
    //	//Rotation
    // case GLFW_KEY_UP:
    //	gVec_Rotate.x = 0.01f;
    //	rotated = true;
    //	break;
    // case GLFW_KEY_DOWN:
    //	gVec_Rotate.x = -0.01f;
    //	rotated = true;
    //	break;
    // case GLFW_KEY_LEFT:
    //	gVec_Rotate.z = 0.01f;
    //	rotated = true;
    //	break;
    // case GLFW_KEY_RIGHT:
    //	gVec_Rotate.z = -0.01f;
    //	rotated = true;
    //	break;
    case GLFW_KEY_KP_1:
      g_bool_ToggleLights = !g_bool_ToggleLights;
      break;
    case GLFW_KEY_KP_3:
      g_bool_ToggleTextures = !g_bool_ToggleTextures;
      break;

      //}
      // if (rotated) {
      //	gVec_Rotate = normalize(gVec_Rotate);
      //	gModifyEntity = glm::rotate(gModifyEntity, gVec_Rotate.x,
      //glm::vec3(0.0f, 0.0f, 0.01f));
      //	gModifyEntity = glm::rotate(gModifyEntity, gVec_Rotate.y,
      //glm::vec3(0.0f, 0.01f, 0.0f));
      //	gModifyEntity = glm::rotate(gModifyEntity, gVec_Rotate.z,
      //glm::vec3(0.01f, 0.0f, 0.0f));
      //	gVec_Rotate = glm::vec3(0.0f);
      //}
      // gModifyEntity = glm::translate(gModifyEntity, gVec_Translate);
      // gVec_Translate = glm::vec3(0.0f);
      // std::stringstream title2;
      // title2 << "Entity: " << gModifyEntity[3].x << "x " <<
      // gModifyEntity[3].y << "y " << gModifyEntity[3].z << "z " << "Entity
      // Forward: " << gModifyEntity[2].x << "x " << gModifyEntity[2].y << "y "
      // << gModifyEntity[2].z << "z ";
      //::gPartOfTitle = title2.str();
    }
}