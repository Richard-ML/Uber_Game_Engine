#include "cCamera.h"
#include "stdafx.h"
#include "global.h"
cCamera::cCamera() {
	mWidth = 800;
	mHeight = 600;
}
cCamera::~cCamera() {}

void cCamera::getProjectionMatrix(glm::mat4 &projOut) {
	// set up the projection matrix
	projOut =
		glm::perspective(1.0f, (float)mWidth / (float)mHeight, 0.1f, 100000.f);
}

void cCamera::getViewMatrix(glm::mat4 &viewOut) { viewOut = mViewMatrix; }

void cCamera::getEyePosition(glm::vec4 &eyeOut) {
	eyeOut.x = mViewMatrix[3].x;
	eyeOut.y = mViewMatrix[3].y;
	eyeOut.z = mViewMatrix[3].z;
	eyeOut.w = 1.f;
}
glm::vec4 cCamera::getEyePosition() { return mViewMatrix[3]; }

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
}

void cCamera::update(float dt) {
	if(m_zoom.pendingDistanceOffset != 0.0f)
	if (glm::abs(m_zoom.pendingDistanceOffset > m_zoom.rate))
	{
		float zoomDistance = glm::sign(m_zoom.pendingDistanceOffset) * dt * m_zoom.rate;
		m_zoom.distance += zoomDistance;
	}
	else {
		m_zoom.distance += m_zoom.pendingDistanceOffset;
		m_zoom.pendingDistanceOffset = 0.0f;
	}

	if (m_zoom.distance > m_zoom.maxDistance || m_zoom.distance < m_zoom.minDistance)
	{
		m_zoom.distance = glm::clamp(m_zoom.distance, m_zoom.minDistance, m_zoom.maxDistance);
		m_zoom.pendingDistanceOffset = 0.0f;
	}


	bool pressUp = glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS;
	bool pressDown = glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS;
	if (pressDown != pressUp) {
		if (pressDown)
		{
			m_rotation.pendingRotation.y -= m_rotation.sensitivity.y;
		}
		else
		{
			m_rotation.pendingRotation.y += m_rotation.sensitivity.y;
		}
	}

	bool pressLeft = glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool pressRight = glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;
	if (pressLeft != pressRight) {
		if (pressLeft)
		{
			m_rotation.pendingRotation.x -= m_rotation.sensitivity.x;
		}
		else 
		{
			m_rotation.pendingRotation.x += m_rotation.sensitivity.x;
		}
	}
	updateView();
}

void cCamera::updateView() {
	// NOTE: The camera does not inherit the target's orientation as its base orientation. 
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 targetPosition = glm::vec3(mTargetTranform[3]);
	glm::mat4 rotation = glm::yawPitchRoll(m_rotation.pendingRotation.x, m_rotation.pendingRotation.y, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, -m_zoom.distance);
	translation = glm::vec3(rotation * glm::vec4(translation, 0.0f));
	glm::vec3 position = targetPosition + translation;
	glm::vec3 forward = glm::normalize(targetPosition - position);
	up = glm::vec3(rotation * glm::vec4(up, 0.0f));
	glm::vec3 right = glm::cross(forward, up);
	mViewMatrix = glm::lookAt(position, targetPosition, up);
}

void cCamera::setTargetTransform(glm::mat4 &targetTransform) {
	mTargetTranform = targetTransform;
	mViewMatrix = glm::lookAtRH(glm::vec3( mViewMatrix[3] ), glm::vec3( targetTransform[3] ), glm::vec3(0.0f, 1.0f, 0.0f));
}

void cCamera::loadFromXML(rapidxml::xml_node<> *cameraNode) {

}
