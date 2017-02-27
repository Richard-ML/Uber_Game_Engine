#pragma once
#include "stdafx.h"
#include "iState.h"
#include "cPlayerControlComponent.h"
#include "cLightManager.h"

// TODO: Move this
class cMesh {
public:
	// TODO: Offset ect.
	std::string meshName;
	bool toggleOutline;
	void saveToXMLNode(rapidxml::xml_node<>& componentNode) {
		printf("Worked2!");
		// TODO create xml node append attributes.. then append the new node to the componentNode
	}
};
class cGraphicsObject {
public:

 	iState* pState;
	std::vector<cMesh*> vec_meshes;
	std::vector<cPlayerControlComponent*> vec_playerControllers;
	// Whatever other internal graphics stuff you want!
	std::vector<cLight*> vec_lights; // Lights that belong to this object..
	void saveToXMLNode(rapidxml::xml_node<>& componentNode) {
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

	}
};