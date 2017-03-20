#ifndef _cCamera_HG_
#define _cCamera_HG_
#include "stdafx.h"

class cCamera {
public:
	cCamera();
	~cCamera();

	void getProjectionMatrix(glm::mat4 &projOut);
	void getViewMatrix(glm::mat4 &viewOut);
	void getEyePosition(glm::vec4 &eyeOut);
	glm::vec4 getEyePosition();
	void windowResize(int width, int height);
	void update(float dt);
	void setTargetTransform(glm::mat4 &targetTransform);
	void loadFromXML(rapidxml::xml_node<> *cameraNode); //TODO: Currently the basic initial camera settings are hard-coded in this header file. Load this information from XML.
	void updateView();

	int m_width;
	int m_height;
	glm::mat4 m_targetTranform;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_cameraForward;
	

	// NOTE: Roll is not being used currently.
	struct Rotation {
		/* Rotation applied to the camera. (yaw, pitch, roll) in radians.
		 *  Optionally this rotation could be applied in addition to the camera-target's orientation.
		 */
		glm::vec3 pendingRotation = glm::vec3(0.0f, 0.57f, 0.0f);
		// The speed at which rotation is incremented/decremented. (yaw, pitch, roll)
		glm::vec3 sensitivity = glm::vec3(0.05f);
	}m_rotation;
	struct Zoom {
		// Zoom distance that will be applied speeds up to @rate * delta-time
		float pendingDistanceOffset = 0.0f;
		// Current zoom distance
		float distance = 50.0f;
		// The speed at which zooming distance is incremented/decremented based on delta-time.
		float rate = 1000.0f;
		float minDistance = 1.0f; // How close the camera can be to its target.
		float maxDistance = 90.0f; // How far away the camera can be from its target.
	}m_zoom;


};
#endif