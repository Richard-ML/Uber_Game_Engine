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
	virtual void loadDataFromXML(rapidxml::xml_node<> *entityNode);
	void update(float dt);
	void setTargetTransform(glm::mat4 &targetTransform);
	void loadFromXML(rapidxml::xml_node<> *cameraNode);
	void updateView();

	int mWidth;
	int mHeight;
	glm::mat4 mTargetTranform;
	glm::mat4 mViewMatrix;
	


	struct {
		float yaw = 0.0f;
		float pitch = 0.0f;
		float yawOffset = 90.0f;
		float pitchOffset = -45.0f;
		glm::vec2 sensitivity = glm::vec3(1.0f);
	}m_rotation;
	struct {
		float pendingDistanceOffset = 0.0f;
		float distance = 50.0f;
		float rate = 1000.0f;
		float minDistance = 30.0f;
		float maxDistance = 90.0f;
	}m_zoom;

};
#endif