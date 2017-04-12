#include "stdafx.h"
#include "cLightManager.h"
#include "global.h"


// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cLightManager_Impl : public cLightManager {
	// Boilerplate

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Manager for the lights. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///-------------------------------------------------------------------------------------------------

	friend class cLightManager;

public:
	//	static std::map<std::tuple<float, float, float>, aiLight>
	//m_map_RGBToLight;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a const cLightManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline const cLightManager_Impl *cLightManager::impl() const {
	return static_cast<const cLightManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the implementation. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cLightManager_Impl. </returns>
///-------------------------------------------------------------------------------------------------

inline cLightManager_Impl *cLightManager::impl() {
	return static_cast<cLightManager_Impl *>(this);
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the instance. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <returns>	Null if it fails, else a pointer to a cLightManager. </returns>
///-------------------------------------------------------------------------------------------------

cLightManager *cLightManager::instance() {
	if (!s_cLightManager)
		s_cLightManager = new cLightManager();
	return s_cLightManager;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Loads light info from XML. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///
/// <param name="lightNode">	[in,out] If non-null, the light node. </param>
///-------------------------------------------------------------------------------------------------

void cLightManager::loadLightFromXML(rapidxml::xml_node<>* lightNode)
{
	if ((std::string)lightNode->first_attribute("type")->value() ==
		"PointLight") {
		cLight *tempLight = new cLight();
		tempLight->isEnabled = true; // g_pXML_Utils->attributeToBool(
			//lightNode->first_attribute("isEnabled"));

		tempLight->typeFlag = 0;
		tempLight->offset = glm::vec3(
			std::stof(lightNode->first_attribute("offsetX")->value()),
			std::stof(lightNode->first_attribute("offsetY")->value()),
			std::stof(lightNode->first_attribute("offsetZ")->value()));
		tempLight->diffuse = glm::vec3(
			std::stof(lightNode->first_attribute("diffuseR")->value()),
			std::stof(lightNode->first_attribute("diffuseG")->value()),
			std::stof(lightNode->first_attribute("diffuseB")->value()));
		tempLight->ambient = glm::vec3(
			std::stof(lightNode->first_attribute("ambientR")->value()),
			std::stof(lightNode->first_attribute("ambientG")->value()),
			std::stof(lightNode->first_attribute("ambientB")->value()));
		tempLight->specular = glm::vec3(
			std::stof(lightNode->first_attribute("specularR")->value()),
			std::stof(lightNode->first_attribute("specularG")->value()),
			std::stof(lightNode->first_attribute("specularB")->value()));

		tempLight->specularPower = std::stof(
			lightNode->first_attribute("specularPower")->value());
		tempLight->attenConst =
			std::stof(lightNode->first_attribute("attenConst")->value());
		tempLight->attenLinear =
			std::stof(lightNode->first_attribute("attenLinear")->value());
		tempLight->attenQuad =
			std::stof(lightNode->first_attribute("attenQuad")->value());
		tempLight->lightID = vecLights.size();
		std::string s12 =
			"lights[" + std::to_string(tempLight->lightID) + "].IsEnabled";
		tempLight->gUniformId_IsEnabled =
			glGetUniformLocation(gProgramID, s12.c_str());
		std::string s11 =
			"lights[" + std::to_string(tempLight->lightID) + "].TypeFlag";
		tempLight->gUniformId_TypeFlag =
			glGetUniformLocation(gProgramID, s11.c_str());
		std::string s1 =
			"lights[" + std::to_string(tempLight->lightID) + "].Position";
		tempLight->gUniformId_Position =
			glGetUniformLocation(gProgramID, s1.c_str());
		std::string s2 =
			"lights[" + std::to_string(tempLight->lightID) + "].Diffuse";
		tempLight->gUniformId_Diffuse =
			glGetUniformLocation(gProgramID, s2.c_str());
		std::string s3 =
			"lights[" + std::to_string(tempLight->lightID) + "].Specular";
		tempLight->gUniformId_Specular =
			glGetUniformLocation(gProgramID, s3.c_str());
		std::string s4 =
			"lights[" + std::to_string(tempLight->lightID) + "].Ambient";
		tempLight->gUniformId_Ambient =
			glGetUniformLocation(gProgramID, s4.c_str());
		std::string s5 =
			"lights[" + std::to_string(tempLight->lightID) + "].SpecularPower";
		tempLight->gUniformId_SpecularPower =
			glGetUniformLocation(gProgramID, s5.c_str());
		std::string s6 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationConst";
		tempLight->gUniformId_AttenuationConst =
			glGetUniformLocation(gProgramID, s6.c_str());
		std::string s7 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationLinear";
		tempLight->gUniformId_AttenuationLinear =
			glGetUniformLocation(gProgramID, s7.c_str());
		std::string s8 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationQuad";
		tempLight->gUniformId_AttenuationQuad =
			glGetUniformLocation(gProgramID, s8.c_str());
		vecLights.push_back(tempLight);
	}
	else
	if ((std::string)lightNode->first_attribute("type")
		->value() == "DirectionalLight") {
		cLight *tempLight = new cLight();
		//tempLight->isEnabled = g_pXML_Utils->attributeToBool(
		//	lightNode->first_attribute("isEnabled"));
		tempLight->isEnabled = true;
		tempLight->typeFlag = 1;
		glm::vec3 target = glm::vec3(
			std::stof(lightNode->first_attribute("directionX")->value()),
			std::stof(lightNode->first_attribute("directionY")->value()),
			std::stof(lightNode->first_attribute("directionZ")->value()));
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		tempLight->directionOffset =
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), target, up);
		tempLight->offset = glm::vec3(
			std::stof(lightNode->first_attribute("offsetX")->value()),
			std::stof(lightNode->first_attribute("offsetY")->value()),
			std::stof(lightNode->first_attribute("offsetZ")->value()));
		tempLight->diffuse = glm::vec3(
			std::stof(lightNode->first_attribute("diffuseR")->value()),
			std::stof(lightNode->first_attribute("diffuseG")->value()),
			std::stof(lightNode->first_attribute("diffuseB")->value()));
		tempLight->ambient = glm::vec3(
			std::stof(lightNode->first_attribute("ambientR")->value()),
			std::stof(lightNode->first_attribute("ambientG")->value()),
			std::stof(lightNode->first_attribute("ambientB")->value()));
		tempLight->specular = glm::vec3(
			std::stof(lightNode->first_attribute("specularR")->value()),
			std::stof(lightNode->first_attribute("specularG")->value()),
			std::stof(lightNode->first_attribute("specularB")->value()));

		tempLight->specularPower = std::stof(
			lightNode->first_attribute("specularPower")->value());
		tempLight->attenConst =
			std::stof(lightNode->first_attribute("attenConst")->value());
		tempLight->attenLinear =
			std::stof(lightNode->first_attribute("attenLinear")->value());

		tempLight->attenQuad =
			std::stof(lightNode->first_attribute("attenQuad")->value());
		tempLight->lightID = vecLights.size();
		std::string s12 =
			"lights[" + std::to_string(tempLight->lightID) + "].IsEnabled";
		tempLight->gUniformId_IsEnabled =
			glGetUniformLocation(gProgramID, s12.c_str());
		std::string s11 =
			"lights[" + std::to_string(tempLight->lightID) + "].TypeFlag";
		tempLight->gUniformId_TypeFlag =
			glGetUniformLocation(gProgramID, s11.c_str());
		std::string s1 =
			"lights[" + std::to_string(tempLight->lightID) + "].Position";
		tempLight->gUniformId_Position =
			glGetUniformLocation(gProgramID, s1.c_str());
		std::string s9 =
			"lights[" + std::to_string(tempLight->lightID) + "].Direction";
		tempLight->gUniformId_Direction =
			glGetUniformLocation(gProgramID, s9.c_str());
		std::string s2 =
			"lights[" + std::to_string(tempLight->lightID) + "].Diffuse";
		tempLight->gUniformId_Diffuse =
			glGetUniformLocation(gProgramID, s2.c_str());
		std::string s3 =
			"lights[" + std::to_string(tempLight->lightID) + "].Specular";
		tempLight->gUniformId_Specular =
			glGetUniformLocation(gProgramID, s3.c_str());
		std::string s4 =
			"lights[" + std::to_string(tempLight->lightID) + "].Ambient";
		tempLight->gUniformId_Ambient =
			glGetUniformLocation(gProgramID, s4.c_str());
		std::string s5 =
			"lights[" + std::to_string(tempLight->lightID) + "].SpecularPower";
		tempLight->gUniformId_SpecularPower =
			glGetUniformLocation(gProgramID, s5.c_str());
		std::string s6 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationConst";
		tempLight->gUniformId_AttenuationConst =
			glGetUniformLocation(gProgramID, s6.c_str());
		std::string s7 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationLinear";
		tempLight->gUniformId_AttenuationLinear =
			glGetUniformLocation(gProgramID, s7.c_str());
		std::string s8 = "lights[" + std::to_string(tempLight->lightID) +
			"].AttenuationQuad";
		tempLight->gUniformId_AttenuationQuad =
			glGetUniformLocation(gProgramID, s8.c_str());

		vecLights.push_back(tempLight);
	}
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Updates the light uniforms. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

void cLightManager::updateLightUniforms() {

	glUniform1i(gUniformId_Toggle_Lights, g_bool_toggleLights);
	glUniform1i(gUniformId_NumLights, g_pLightManager->vecLights.size());
	for (auto iter = g_pLightManager->vecLights.begin(); iter != g_pLightManager->vecLights.end(); iter++) {
		glUniform1i((*iter)->gUniformId_IsEnabled, (*iter)->isEnabled);
		glUniform1i((*iter)->gUniformId_TypeFlag, (*iter)->typeFlag);
		glUniform3fv((*iter)->gUniformId_Position, 1,
			glm::value_ptr((*iter)->state->getPosition() + (*iter)->offset));
		glUniform3fv((*iter)->gUniformId_Direction, 1,
			glm::value_ptr((*iter)->direction));
		glUniform1f((*iter)->gUniformId_ConeAngle, (*iter)->coneAngle);
		glUniform3fv((*iter)->gUniformId_Ambient, 1,
			glm::value_ptr((*iter)->ambient));
		glUniform3fv((*iter)->gUniformId_Diffuse, 1,
			glm::value_ptr((*iter)->diffuse));
		glUniform3fv((*iter)->gUniformId_Specular, 1,
			glm::value_ptr((*iter)->specular));
		glUniform1f((*iter)->gUniformId_SpecularPower, (*iter)->specularPower);
		glUniform1f((*iter)->gUniformId_AttenuationConst, (*iter)->attenConst);
		glUniform1f((*iter)->gUniformId_AttenuationLinear, (*iter)->attenLinear);
		glUniform1f((*iter)->gUniformId_AttenuationQuad, (*iter)->attenQuad);
	}

}