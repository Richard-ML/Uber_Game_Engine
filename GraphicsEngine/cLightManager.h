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
	/// <summary>	The matrix. </summary>
	glm::mat4 matrix;
	/// <summary>	The offset. </summary>
	glm::vec3 offset;
	/// <summary>	The direction. </summary>
	glm::vec3 direction;
	/// <summary>	The direction offset. </summary>
	glm::mat4 directionOffset;
	/// <summary>	The type flag. </summary>
	int typeFlag;
	/// <summary>	True if this object is enabled. </summary>
	bool isEnabled;
	/// <summary>	The cone angle. </summary>
	float coneAngle;
	/// <summary>	The ambient. </summary>
	glm::vec3 ambient;
	/// <summary>	The diffuse. </summary>
	glm::vec3 diffuse;
	/// <summary>	The specular. </summary>
	glm::vec3 specular;
	/// <summary>	The specular power. </summary>
	GLfloat specularPower;
	/// <summary>	The attenuation constant. </summary>
	float attenConst; 
	/// <summary>	The linear attenuation. </summary>
	float attenLinear;
	/// <summary>	The quadratic attenuation. </summary>
	float attenQuad; 
	/// <summary>	Identifier for the light. </summary>
	unsigned int lightID;
	/// <summary>	The uniform identifier type flag. </summary>
	GLuint gUniformId_TypeFlag;
	/// <summary>	The uniform identifier is enabled. </summary>
	GLuint gUniformId_IsEnabled;
	/// <summary>	The uniform identifier position. </summary>
	GLuint gUniformId_Position;
	/// <summary>	The uniform identifier direction. </summary>
	GLuint gUniformId_Direction;
	/// <summary>	The uniform identifier cone angle. </summary>
	GLuint gUniformId_ConeAngle;
	/// <summary>	The uniform identifier ambient. </summary>
	GLuint gUniformId_Ambient;
	/// <summary>	The uniform identifier diffuse. </summary>
	GLuint gUniformId_Diffuse;
	/// <summary>	The uniform identifier specular. </summary>
	GLuint gUniformId_Specular;
	/// <summary>	The uniform identifier specular power. </summary>
	GLuint gUniformId_SpecularPower;
	/// <summary>	The uniform identifier attenuation constant. </summary>
	GLuint gUniformId_AttenuationConst;
	/// <summary>	The uniform identifier attenuation linear. </summary>
	GLuint gUniformId_AttenuationLinear;
	/// <summary>	The uniform identifier attenuation quad. </summary>
	GLuint gUniformId_AttenuationQuad;
	/// <summary>	The uniform identifier color. </summary>
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