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
		// ^.^ 
		//rapidxml::xml_node<>* meshNode = new rapidxml::xml_node();
		//componentNode.append_node( SOME NODE? )
		// Append attributes to SOME NODE?

		// OR 

		// Create SOME NODE
		// Append attributes to SOME NODE
		// Append SOME NODE TO componentNode
	}
};
class cGraphicsObject {
public:

 	iState* pState;
	std::vector<cMesh*> vec_meshes;
	std::vector<cPlayerControlComponent*> vec_playerControllers;
	// Whatever other internal graphics stuff you want!
	std::vector<cLight*> vec_lights; // Lights that belong to this object..

	// Takes a reference to an existing xml node. (RenderableComponent) Then appends nodes to it containing all of it's information
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