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

	glm::vec3 mOffset;
	float mZoomSpeed;
	float mDistance;
	float mMaxDistance;
	float mMinDistance;
	glm::mat4 mViewMatrix;
};
#endif