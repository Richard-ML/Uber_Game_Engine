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
	if (glm::abs(m_zoom.pendingDistanceOffset > 1))
	{
		float zoomDistance = glm::sign(m_zoom.pendingDistanceOffset) * dt * m_zoom.rate;
		m_zoom.distance += zoomDistance;
	}
	else {
		m_zoom.distance += m_zoom.pendingDistanceOffset;
		m_zoom.pendingDistanceOffset = 0.0f;
	}

	bool pressUp = glfwGetKey(gWindow, GLFW_KEY_UP) == GLFW_PRESS;
	bool pressDown = glfwGetKey(gWindow, GLFW_KEY_DOWN) == GLFW_PRESS;
	if (pressDown != pressUp) {
		if (pressDown)
		{
			m_rotation.pitchOffset += m_rotation.sensitivity.y;
		}
		else
		{
			m_rotation.pitchOffset -= m_rotation.sensitivity.y;
		}
	}

	bool pressLeft = glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool pressRight = glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;
	if (pressLeft != pressRight) {
		if (pressLeft)
		{
			m_rotation.yawOffset += m_rotation.sensitivity.x;
		}
		else 
		{
			m_rotation.yawOffset -= m_rotation.sensitivity.x;
		}
	}
	updateView();
}

void cCamera::updateView() {
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraPosition;
	glm::vec3 cameraForward;
	m_rotation.pitchOffset = glm::clamp(m_rotation.pitchOffset, -89.0f + m_rotation.pitch, -5.0f + m_rotation.pitch);
	float pitch = glm::clamp( m_rotation.pitch + m_rotation.pitchOffset, -89.0f, -5.0f);
	float yaw = m_rotation.yaw + m_rotation.yawOffset;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraForward = glm::normalize(front);

	cameraPosition = glm::vec3(mTargetTranform[3]) - cameraForward * m_zoom.distance;

	mViewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraForward, up);

}

void cCamera::setTargetTransform(glm::mat4 &targetTransform) {
	mTargetTranform = targetTransform;
	
	float sinePitch, cosinePitch, sineYaw, cosineYaw;
	sinePitch = -mTargetTranform[2][0];
	cosinePitch = sqrt(1 - sinePitch * sinePitch);
	if (abs(cosinePitch) > glm::epsilon<float>())
	{
		sineYaw = mTargetTranform[1][0] / cosinePitch;
		cosineYaw = mTargetTranform[0][0] / cosinePitch;
	}
	else
	{
		sineYaw = 0;
		cosineYaw = 1;
	}
	float pi = glm::pi<float>();
	m_rotation.yaw = atan2(sineYaw, cosineYaw) * 180 / pi;
	m_rotation.pitch = atan2(sinePitch, cosinePitch) * 180 / pi;
}

void cCamera::loadFromXML(rapidxml::xml_node<> *cameraNode) {

}
