#ifndef _cCamera_HG_
#define _cCamera_HG_
#include "stdafx.h"

class cCamera {
public:
	cCamera();
	~cCamera();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets projection matrix. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="projOut">	[in,out] The project out. </param>
	///-------------------------------------------------------------------------------------------------

	void getProjectionMatrix(glm::mat4 &projOut);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets view matrix. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="viewOut">	[in,out] The view out. </param>
	///-------------------------------------------------------------------------------------------------

	void getViewMatrix(glm::mat4 &viewOut);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets eye position. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="eyeOut">	[in,out] The eye out. </param>
	///-------------------------------------------------------------------------------------------------

	void getEyePosition(glm::vec4 &eyeOut);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Gets eye position. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	The eye position. </returns>
	///-------------------------------------------------------------------------------------------------

	glm::vec4 getEyePosition();

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Window resize. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="width"> 	The width. </param>
	/// <param name="height">	The height. </param>
	///-------------------------------------------------------------------------------------------------

	void windowResize(int width, int height);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Updates based on the given delta time. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="dt">	The delta time. </param>
	///-------------------------------------------------------------------------------------------------

	void update(float dt);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Sets target transform. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="targetTransform">	[in,out] Target transform. </param>
	///-------------------------------------------------------------------------------------------------

	void setTargetTransform(glm::mat4 &targetTransform);

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Loads from XML. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="cameraNode">	[in,out] If non-null, the camera node. </param>
	///-------------------------------------------------------------------------------------------------

	void loadFromXML(rapidxml::xml_node<> *cameraNode); //TODO: Currently the basic initial camera settings are hard-coded in this header file. Load this information from XML.

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Updates the view. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	void updateView();

	/// <summary>	The width. </summary>
	int m_width;
	/// <summary>	The height. </summary>
	int m_height;
	/// <summary>	Target transform. </summary>
	glm::mat4 m_targetTransform;
	/// <summary>	The view matrix. </summary>
	glm::mat4 m_viewMatrix;
	/// <summary>	The camera forward. </summary>
	glm::vec3 m_cameraForward;
	/// <summary>	The view identity matrix. </summary>
	glm::mat3 m_viewIdentityMatrix;
	/// <summary>	The eye position. </summary>
	glm::vec3 m_eyePosition;

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
		float rate = 6.0f;
		float minDistance = 2.0f; // How close the camera can be to its target.
		float maxDistance = 400.0f; // How far away the camera can be from its target.
	}m_zoom;


};
#endif