#include "cCamera.h"
#include "stdafx.h"
#include "global.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	Default constructor. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

cCamera::cCamera() {
	m_width = 800;
	m_height = 600;
}
cCamera::~cCamera() {}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets projection matrix. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="projOut">	[in,out] The projection. </param>
///-------------------------------------------------------------------------------------------------

void cCamera::getProjectionMatrix(glm::mat4 &projOut) {
	// set up the projection matrix
	projOut =
		glm::perspective(1.0f,(float)m_width / (float)m_height, 0.1f, 100000.f);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets view matrix. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="viewOut">	[in,out] The view out. </param>
///-------------------------------------------------------------------------------------------------

void cCamera::getViewMatrix(glm::mat4 &viewOut) { viewOut = m_viewMatrix; }

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets eye position. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="eyeOut">	[in,out] The eye out. </param>
///-------------------------------------------------------------------------------------------------

void cCamera::getEyePosition(glm::vec4 &eyeOut) {
	eyeOut.x = m_viewMatrix[3].x;
	eyeOut.y = m_viewMatrix[3].y;
	eyeOut.z = m_viewMatrix[3].z;
	eyeOut.w = 1.f;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets eye position. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	The eye position. </returns>
///-------------------------------------------------------------------------------------------------

glm::vec4 cCamera::getEyePosition() { return m_viewMatrix[3]; }

void cCamera::windowResize(int width, int height) {
	m_width = width;
	m_height = height;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Updates based on the given delta time. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="dt">	The delta time. </param>
///-------------------------------------------------------------------------------------------------

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
	if (pressDown != pressUp)
		if (pressDown)
			m_rotation.pendingRotation.y -= m_rotation.sensitivity.y;
		else
			m_rotation.pendingRotation.y += m_rotation.sensitivity.y;

	bool pressLeft = glfwGetKey(gWindow, GLFW_KEY_LEFT) == GLFW_PRESS;
	bool pressRight = glfwGetKey(gWindow, GLFW_KEY_RIGHT) == GLFW_PRESS;
	if (pressLeft != pressRight)
		if (pressLeft)
			m_rotation.pendingRotation.x -= m_rotation.sensitivity.x;
		else 
			m_rotation.pendingRotation.x += m_rotation.sensitivity.x;

	updateView();
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Updates the view. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

void cCamera::updateView() {
	// NOTE: The camera does not currently inherit the target's orientation as its base orientation. 
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 targetPosition = glm::vec3(m_targetTransform[3]);
	targetPosition.y += 7.5f;
	m_rotation.pendingRotation.y = glm::clamp(m_rotation.pendingRotation.y, 0.0f, 3.14f);
	glm::mat4 rotation = glm::yawPitchRoll(m_rotation.pendingRotation.x, m_rotation.pendingRotation.y, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, -m_zoom.distance);
	translation = glm::vec3(rotation * glm::vec4(translation, 0.0f));
	glm::vec3 position = targetPosition + translation;
	glm::vec3 forward = glm::normalize(targetPosition - position);
	up = glm::vec3(rotation * glm::vec4(up, 0.0f));
	glm::vec3 right = glm::cross(forward, up);
	m_viewMatrix = glm::lookAt(position, targetPosition, up);
	/////////////////////////////
	glm::mat4 temp;
	getProjectionMatrix(temp);
	m_viewIdentityMatrix *= glm::mat3(temp);
	m_eyePosition = position;
	m_viewIdentityMatrix[0] = forward;
	m_viewIdentityMatrix[2] = glm::normalize(glm::cross(forward, up));
	m_viewIdentityMatrix[1] = glm::normalize(glm::cross(m_viewIdentityMatrix[2], forward));

}

///-------------------------------------------------------------------------------------------------
/// <summary>	Sets target transform. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="targetTransform">	[in,out] Target transform. </param>
///-------------------------------------------------------------------------------------------------

void cCamera::setTargetTransform(glm::mat4 &targetTransform) {
	m_targetTransform = targetTransform;
	m_viewMatrix = glm::lookAtRH(glm::vec3( m_viewMatrix[3] ), glm::vec3( targetTransform[3] ), glm::vec3(0.0f, 1.0f, 0.0f));
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="cameraNode">	[in,out] If non-null, the camera node. </param>
///-------------------------------------------------------------------------------------------------

void cCamera::loadFromXML(rapidxml::xml_node<> *cameraNode) {

}
