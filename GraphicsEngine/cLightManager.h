#ifndef _cLightManager_HG_
#define _cLightManager_HG_
#include "stdafx.h"
class cLight {
public:
	glm::mat4 matrix;
	glm::vec3 offset;
	glm::vec3 direction;
	glm::mat4 directionOffset;
	int typeFlag;
	bool isEnabled;
	float coneAngle;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat specularPower;
	float attenConst;  // = 0.0f;
	float attenLinear; // = 0.1f;
	float attenQuad;   // = 0.01f;
	unsigned int lightID;
	GLuint gUniformId_TypeFlag;
	GLuint gUniformId_IsEnabled;
	GLuint gUniformId_Position;
	GLuint gUniformId_Direction;
	GLuint gUniformId_ConeAngle;
	GLuint gUniformId_Ambient;
	GLuint gUniformId_Diffuse;
	GLuint gUniformId_Specular;
	GLuint gUniformId_SpecularPower;
	GLuint gUniformId_AttenuationConst;
	GLuint gUniformId_AttenuationLinear;
	GLuint gUniformId_AttenuationQuad;
	GLuint gUniformId_Color;
	iState* state; // Base transform information

	void saveToXMLNode(std::string& componentNode) {
		std::stringstream ssXML;
		ssXML << "<Light ";
		switch ( this->typeFlag )
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

		componentNode.append( ssXML.str() );
	}
};
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