#ifndef _cLightManager_HG_
#define _cLightManager_HG_
#include "stdafx.h"

///-------------------------------------------------------------------------------------------------
/// <summary>	A light. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cLight {
public:
	// The matrix.
	glm::mat4 matrix;
	// The offset.
	glm::vec3 offset;
	// The direction.
	glm::vec3 direction;
	// The direction offset.
	glm::mat4 directionOffset;
	// The type flag.
	int typeFlag;
	// True if this object is enabled.
	bool isEnabled;
	// The cone angle.
	float coneAngle;
	// The ambient.
	glm::vec3 ambient;
	// The diffuse.
	glm::vec3 diffuse;
	// The specular.
	glm::vec3 specular;
	// The specular power.
	GLfloat specularPower;
	// The attenuation constant.
	float attenConst; 
	// The linear attenuation.
	float attenLinear;
	// The quadratic attenuation.
	float attenQuad; 
	// Identifier for the light.
	unsigned int lightID;
	// The uniform identifier type flag.
	GLuint gUniformId_TypeFlag;
	// The uniform identifier is enabled.
	GLuint gUniformId_IsEnabled;
	// The uniform identifier position.
	GLuint gUniformId_Position;
	// The uniform identifier direction.
	GLuint gUniformId_Direction;
	// The uniform identifier cone angle.
	GLuint gUniformId_ConeAngle;
	// The uniform identifier ambient.
	GLuint gUniformId_Ambient;
	// The uniform identifier diffuse.
	GLuint gUniformId_Diffuse;
	//The uniform identifier specular.
	GLuint gUniformId_Specular;
	// The uniform identifier specular power.
	GLuint gUniformId_SpecularPower;
	// The uniform identifier attenuation constant.
	GLuint gUniformId_AttenuationConst;
	// The uniform identifier attenuation linear.
	GLuint gUniformId_AttenuationLinear;
	// The uniform identifier attenuation quad.
	GLuint gUniformId_AttenuationQuad;
	// The uniform identifier color.
	GLuint gUniformId_Color;
	iState* state; // Base transform information

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Saves light information to XML formatted string. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <param name="componentNode">	[in,out] The component node. </param>
	///-------------------------------------------------------------------------------------------------

	void saveToXMLNode(std::string& componentNode) {
		std::stringstream ssXML;
		ssXML << "<Light ";
		switch (this->typeFlag)
		{
		case 0:
			ssXML << "type=\"" << "PointLight" << "\" ";
			break;
		case 1:
			ssXML << "type=\"" << "DirectionalLight" << "\" ";
			break;
		}

		ssXML << "isEnabled=\"" << this->isEnabled << "\" ";
		ssXML << "coneAngle=\"" << this->coneAngle << "\" ";
		ssXML << "specularPower=\"" << this->specularPower << "\" ";
		ssXML << "directionX=\"" << this->direction.x << "\" ";
		ssXML << "directionY=\"" << this->direction.y << "\" ";
		ssXML << "directionZ=\"" << this->direction.z << "\" ";
		ssXML << "offsetX=\"" << this->offset.x << "\" ";
		ssXML << "offsetY=\"" << this->offset.y << "\" ";
		ssXML << "offsetZ=\"" << this->offset.z << "\" ";
		ssXML << "ambientR=\"" << this->ambient.r << "\" ";
		ssXML << "ambientG=\"" << this->ambient.g << "\" ";
		ssXML << "ambientB=\"" << this->ambient.b << "\" ";
		ssXML << "diffuseR=\"" << this->diffuse.r << "\" ";
		ssXML << "diffuseG=\"" << this->diffuse.g << "\" ";
		ssXML << "diffuseB=\"" << this->diffuse.b << "\" ";
		ssXML << "specularR=\"" << this->specular.r << "\" ";
		ssXML << "specularG=\"" << this->specular.g << "\" ";
		ssXML << "specularB=\"" << this->specular.b << "\" ";
		ssXML << "attenConst=\"" << this->attenConst << "\" ";
		ssXML << "attenLinear=\"" << this->attenLinear << "\" ";
		ssXML << "attenQuad=\"" << this->attenQuad << "\" ";
		ssXML << "/>" << std::endl;

		componentNode.append(ssXML.str());
	}

};

///-------------------------------------------------------------------------------------------------
/// <summary>	Manager for lights. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cLightManager {
	static cLightManager *s_cLightManager;
	// Boilerplate
	friend class cLightManager_Impl; // The PIMPL idiom aka Compilation Firewall
	const cLightManager_Impl *impl() const;
	cLightManager_Impl *impl();

public:
	std::vector<cLight*> vecLights;
	static cLightManager *instance();
	void loadLightFromXML(rapidxml::xml_node<>* lightNode);
	void updateLightUniforms();

private:
	cLightManager() {
	} // Constructor is private therefore a new instance can not be made
	  // externally. Only available to members or friends of this class..
	~cLightManager() {}
	// Not defined to prevent copying of the only instance of the class.
	cLightManager(const cLightManager &); // Disallow copy constructor
	cLightManager &operator=(const cLightManager &meshManager) {
	} // Disallow assignment operator
};


#endif