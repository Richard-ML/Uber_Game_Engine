#pragma once
#include "stdafx.h"
#include "iState.h"
#include "cPlayerControlComponent.h"
#include "cLightManager.h"

// TODO: Move this

///-------------------------------------------------------------------------------------------------
/// <summary>	A mesh. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cMesh {
public:
	// TODO: Offset ect.
	std::string meshName;
	bool toggleOutline;
	void saveToXMLNode(std::string& xmlString) {

		xmlString += "<Mesh name=\"";
		xmlString += meshName;
		xmlString += "\" outline=\"";
		xmlString += toggleOutline ? "true" : "false";
		xmlString += "\" />";
	}
};

///-------------------------------------------------------------------------------------------------
/// <summary>	The graphics object. </summary>
///
/// <remarks>	Richard, 4/12/2017. </remarks>
///-------------------------------------------------------------------------------------------------

class cGraphicsObject {
public:

	iState* pState;
	std::vector<cMesh*> vec_meshes;
	std::vector<cPlayerControlComponent*> vec_playerControllers;
	// Whatever other internal graphics stuff you want!
	std::vector<cLight*> vec_lights; // Lights that belong to this object..

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Saves to formatted XML string. </summary>
	///
	/// <remarks>	Richard, 4/12/2017. </remarks>
	///
	/// <returns>	A std::string. </returns>
	///-------------------------------------------------------------------------------------------------

	std::string saveToXMLNode() {
		std::string componentNode = "<RenderableComponent>";
		for each(cMesh* mesh in vec_meshes)
		{
			mesh->saveToXMLNode(componentNode);
		}
		// Why would there be more than one player controller? TODO: fix this
		for each(cPlayerControlComponent* playerController in vec_playerControllers)
		{
			playerController->saveToXMLNode(componentNode);
		}
		for each(cLight* light in vec_lights)
		{
			light->saveToXMLNode(componentNode);
		}
		componentNode += "</RenderableComponent>";
		return componentNode;
	}
};