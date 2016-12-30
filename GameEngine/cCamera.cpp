#include "cCamera.h"
#include "global.h"
#include <sstream>
/**
File:
Author(s):
Name: Richard Mills-Laursen
Role(s): Student
E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca

Name: Michael Feeney
E-mail: MFeeney@FanshaweC.on.ca
Role(s): Coordinator & Professor
Course(s): INFO-6044-01 - Game Engine Framework/Patterns
INFO-6028-01 - Graphics 1
INFO-6019-01 - Physics & Simulation 1


Name: Ben McIlmoyle
E-Mail: B_McIlmoyle@FanshaweOnline.ca
Role: Professor
Course: INFO-6016-01 - Network Programming

Name: James Lucas
E-Mail: J_Lucas@FanshaweOnline.ca
Role: Professor
Course: INFO-6017-01 - Artificial Intelligence

Name: Oscar Lara
E-Mail: O_Lara@FanshaweOnline.ca
Role: Professor
Course: INFO-6046-01 - Media Fundamentals

Status: Version 1.7 Alpha
(c) Copyright(s): Fanshawe College

Description:

*/
cCamera::cCamera() {
  mWidth = 800;
  mHeight = 600;
  mZoomSpeed = 200.f;
  mMinDistance = 100.f;
  mDistance = 300.0f;
  mMaxDistance = 10000.f;
  mOffset = glm::vec3(0.f, 7.f, -6.f);
}
cCamera::~cCamera() {}
//                 _    ____               _              _    _
//                 __  __         _          _
//     __ _   ___ | |_ |  _ \  _ __  ___  (_)  ___   ___ | |_ (_)  ___   _ __  |
//     \/  |  __ _ | |_  _ __ (_)__  __
//    / _` | / _ \| __|| |_) || '__|/ _ \ | | / _ \ / __|| __|| | / _ \ | '_ \ |
//    |\/| | / _` || __|| '__|| |\ \/ /
//   | (_| ||  __/| |_ |  __/ | |  | (_) || ||  __/| (__ | |_ | || (_) || | | ||
//   |  | || (_| || |_ | |   | | >  <
//    \__, | \___| \__||_|    |_|   \___/_/ | \___| \___| \__||_| \___/ |_| |_||_|  |_| \__,_| \__||_|   |_|/_/\_\
//    |___/                             |__/
void cCamera::getProjectionMatrix(glm::mat4 &projOut) {
  // set up the projection matrix
  projOut =
      glm::perspective(1.0f, (float)mWidth / (float)mHeight, 0.1f, 100000.f);
}
//                 _  __     __ _                  __  __         _          _
//     __ _   ___ | |_\ \   / /(_)  ___ __      __|  \/  |  __ _ | |_  _ __
//     (_)__  __
//    / _` | / _ \| __|\ \ / / | | / _ \\ \ /\ / /| |\/| | / _` || __|| '__|| |\
//    \/ /
//   | (_| ||  __/| |_  \ V /  | ||  __/ \ V  V / | |  | || (_| || |_ | |   | |
//   >  <
//    \__, | \___| \__|  \_/   |_| \___|  \_/\_/  |_|  |_| \__,_| \__||_|   |_|/_/\_\
//    |___/
void cCamera::getViewMatrix(glm::mat4 &viewOut) { viewOut = mViewMatrix; }
//                 _    _____              ____              _  _    _
//     __ _   ___ | |_ | ____|_   _   ___ |  _ \  ___   ___ (_)| |_ (_)  ___   _
//     __
//    / _` | / _ \| __||  _| | | | | / _ \| |_) |/ _ \ / __|| || __|| | / _ \ | '_ \ 
//   | (_| ||  __/| |_ | |___| |_| ||  __/|  __/| (_) |\__ \| || |_ | || (_) || | | |
//    \__, | \___| \__||_____|\__, | \___||_|    \___/ |___/|_| \__||_| \___/
//    |_| |_|
//    |___/                   |___/
void cCamera::getEyePosition(glm::vec4 &eyeOut) {
  eyeOut.x = mViewMatrix[3].x;
  eyeOut.y = mViewMatrix[3].y;
  eyeOut.z = mViewMatrix[3].z;
  eyeOut.w = 1.f;
}
glm::vec4 cCamera::getEyePosition() { return mViewMatrix[3]; }
//              _             _                  ____             _
//   __      __(_) _ __    __| |  ___ __      __|  _ \  ___  ___ (_) ____ ___
//   \ \ /\ / /| || '_ \  / _` | / _ \\ \ /\ / /| |_) |/ _ \/ __|| ||_  // _ \
//    \ V  V / | || | | || (_| || (_) |\ V  V / |  _ <|  __/\__ \| | / /|  __/
//     \_/\_/  |_||_| |_| \__,_| \___/  \_/\_/  |_| \_\\___||___/|_|/___|\___|
//
void cCamera::windowResize(int width, int height) {
  mWidth = width;
  mHeight = height;
}

void cCamera::loadDataFromXML(rapidxml::xml_node<> *entityNode) {
  this->mTargetTranform = glm::translate(
      glm::mat4(),
      glm::vec3(std::stof(entityNode->first_attribute("x")->value()),
                std::stof(entityNode->first_attribute("y")->value()),
                std::stof(entityNode->first_attribute("z")->value())));

  this->mOffset =
      glm::vec3(std::stof(entityNode->first_attribute("offsetX")->value()),
                std::stof(entityNode->first_attribute("offsetY")->value()),
                std::stof(entityNode->first_attribute("offsetZ")->value()));
}

//                      _         _
//    _   _  _ __    __| |  __ _ | |_  ___
//   | | | || '_ \  / _` | / _` || __|/ _ \
//   | |_| || |_) || (_| || (_| || |_|  __/
//    \__,_|| .__/  \__,_| \__,_| \__|\___|
//          |_|
void cCamera::update(float dt) {
  bool pressUp = glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS;
  bool pressDown = glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS;
  if (pressDown != pressUp) {
    float zoomSpeed = mZoomSpeed;
    if (pressUp)
      zoomSpeed *= -1.f;
    mOffset.y += zoomSpeed / 1000 * dt;
    mDistance += zoomSpeed * dt;
    if (mDistance < mMinDistance)
      mDistance = mMinDistance;
    if (mDistance > mMaxDistance)
      mDistance = mMaxDistance;
  }

  bool pressLeft = glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
  bool pressRight = glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;
  if (pressLeft != pressRight) {
    float panSpeed = mZoomSpeed / 100;
    if (pressLeft)
      panSpeed *= -1.f;

    this->mOffset.x += panSpeed * dt;

  } else if (this->mOffset.x > 0.1)
    mOffset.x -= 0.1f;
  else if (this->mOffset.x < -0.1)
    mOffset.x += 0.1f;

  // glm::vec3 direction;
  //
  // bool pressD = glfwGetKey(gWindow, GLFW_KEY_D) == GLFW_PRESS;
  // bool pressA = glfwGetKey(gWindow, GLFW_KEY_A) == GLFW_PRESS;
  // if (pressD != pressA)
  //{
  //
  //	if (pressD)
  //		direction.x = -1.f;
  //	else
  //		direction.x = 1.f;
  //}
  //
  // bool pressW = glfwGetKey(gWindow, GLFW_KEY_W) == GLFW_PRESS;
  // bool pressS = glfwGetKey(gWindow, GLFW_KEY_S) == GLFW_PRESS;
  // if (pressW != pressS)
  //{
  //
  //	if (pressW)
  //		direction.z = 10.f;
  //	else
  //		direction.z = -10.f;
  //}
  //
  // bool pressQ = glfwGetKey(gWindow, GLFW_KEY_Q) == GLFW_PRESS;
  // bool pressE = glfwGetKey(gWindow, GLFW_KEY_E) == GLFW_PRESS;
  // if (pressQ != pressE)
  //{
  //
  //	if (pressQ)
  //		direction.y = 1.f;
  //	else
  //		direction.y = -1.f;
  //}
  //
  //
  //
  // glm::vec3 offsetDir;
  // bool pressF = glfwGetKey(gWindow, GLFW_KEY_F) == GLFW_PRESS;
  // bool pressH = glfwGetKey(gWindow, GLFW_KEY_H) == GLFW_PRESS;
  // if (pressF != pressH)
  //{
  //
  //	if (pressF)
  //		offsetDir.x = -.01f;
  //	else
  //		offsetDir.x = .01f;
  //}
  //
  // bool pressR = glfwGetKey(gWindow, GLFW_KEY_R) == GLFW_PRESS;
  // bool pressY = glfwGetKey(gWindow, GLFW_KEY_Y) == GLFW_PRESS;
  // if (pressR != pressY)
  //{
  //
  //	if (pressR)
  //		offsetDir.y = -.01f;
  //	else
  //		offsetDir.y = .01f;
  //}
  //
  // bool pressT = glfwGetKey(gWindow, GLFW_KEY_T) == GLFW_PRESS;
  // bool pressG = glfwGetKey(gWindow, GLFW_KEY_G) == GLFW_PRESS;
  // if (pressT != pressG)
  //{
  //
  //	if (pressT)
  //		offsetDir.z = -.01f;
  //	else
  //		offsetDir.z = .01f;
  //}
  //
  //
  // this->mOffset += offsetDir;
  // this->mOffset = normalize(mOffset);
  //
  // this->mTargetTranform = glm::translate(mTargetTranform, direction);
  // std::stringstream title;
  // title << "Camera: "  << mTargetTranform[3].x << "x " <<
  // mTargetTranform[3].y << "y "<< mTargetTranform[3].z << "z " << "Camera
  // Offset: " << mOffset.x << "x " << mOffset.y << "y " << mOffset.z << "z " <<
  // gPartOfTitle;
  // glfwSetWindowTitle(gWindow, title.str().c_str());

  mOffset = glm::clamp(mOffset, glm::vec3(-90.0f, 1.0f, -10.0f),
                       glm::vec3(90.0f, 40.0f, 10.0f));
  updateView();

  bool pressNPPlus = glfwGetKey(gWindow, GLFW_KEY_KP_ADD) == GLFW_PRESS;

  if (pressNPPlus) {

    glm::vec3 up(0.f, 1.f, 0.f);
    mDistance = 300.f;
    // set up the view matrix
    glm::vec3 target(mTargetTranform[3].x, 100, mTargetTranform[3].z);
    glm::vec4 offset(0, 11, -6, 0.f);
    offset = mTargetTranform * offset;
    offset = glm::normalize(offset) * mDistance;
    glm::vec3 eye(target.x + offset.x, 0, target.z + offset.z);

    mViewMatrix = glm::lookAtRH(eye, target, up);
  }
}
//                      _         _       __     __ _
//    _   _  _ __    __| |  __ _ | |_  ___\ \   / /(_)  ___ __      __
//   | | | || '_ \  / _` | / _` || __|/ _ \\ \ / / | | / _ \\ \ /\ / /
//   | |_| || |_) || (_| || (_| || |_|  __/ \ V /  | ||  __/ \ V  V /
//    \__,_|| .__/  \__,_| \__,_| \__|\___|  \_/   |_| \___|  \_/\_/
//          |_|
void cCamera::updateView() {
  glm::vec3 up(0.f, 1.f, 0.f);

  // set up the view matrix
  glm::vec3 target(mTargetTranform[3].x, mTargetTranform[3].y,
                   mTargetTranform[3].z);
  glm::vec4 offset(mOffset.x, mOffset.y, mOffset.z, 0.f);
  offset = mTargetTranform * offset;
  offset = glm::normalize(offset) * mDistance;
  glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);

  mViewMatrix = glm::lookAtRH(eye, target, up);
}

void cCamera::setTargetTransform(glm::mat4 &targetTransform) {
  mTargetTranform = targetTransform;
}

void cCamera::loadFromXML(rapidxml::xml_node<> *cameraNode) {
  this->mOffset.x = std::stof(cameraNode->first_attribute("offsetX")->value());
  this->mOffset.y = std::stof(cameraNode->first_attribute("offsetY")->value());
  this->mOffset.z = std::stof(cameraNode->first_attribute("offsetZ")->value());
}
