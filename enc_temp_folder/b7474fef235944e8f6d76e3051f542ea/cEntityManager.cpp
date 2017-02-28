#include "stdafx.h"
#include "cEntityManager.h"
#include "global.h"
class cGameEntity {
public:
	std::vector<std::shared_ptr<cComponent>>
		vec_pComponents; // Components that belong to this entity..
	// Used by the entity manager during the save game process..
	std::string stateNodeID = "";

};
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cEntityManager_Impl : public cEntityManager {
	// Boilerplate
	friend class cEntityManager;

public: 
	//std::vector<cGameEntity*> vec_pEntites;
};
inline const cEntityManager_Impl *cEntityManager::impl() const {
	return static_cast<const cEntityManager_Impl *>(this);
}
inline cEntityManager_Impl *cEntityManager::impl() {
	return static_cast<cEntityManager_Impl *>(this);
}

std::vector<cGameEntity*> vec_gameEntities;
// Redo the load game entities process..
void cEntityManager::loadGameEntitiesFromXML(std::string filename) {
	// TODO: Delete all of the components.. 

	std::cout << "Loading saved game entities data..\n";
	rapidxml::xml_document<> theDoc;
	rapidxml::xml_node<> *root_node;

	std::ifstream theFile(filename); // load the XML data into the buffer
	std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
		std::istreambuf_iterator<char>());
	bufData.push_back('\0');
	theDoc.parse<0>(&bufData[0]);
	root_node = theDoc.first_node("GameEntities");
	for (rapidxml::xml_node<> *cGameEntity_node = root_node->first_node("GameEntity");
		cGameEntity_node; cGameEntity_node = cGameEntity_node->next_sibling("GameEntity")) {
		// Register new state node for the component..
		cGameEntity* gameEntity = new cGameEntity();

		gameEntity->stateNodeID = g_pComponentEngine->registerNewEntity();

		// These components are loaded and share a state! ...
		gameEntity->vec_pComponents = g_pComponentEngine->loadFromXML(cGameEntity_node, gameEntity->stateNodeID);
		vec_gameEntities.push_back(gameEntity);
		//this->impl()->vec_pEntites.push_back(gameEntity);
	}
}


//////////////////////////////////////////////////////////
//////   loadGameFromXML Method - XML file loader   //////
/////////////////////////////////////////////////////////
int cEntityManager::loadGameFromXML(std::string filename) {
	std::cout << "Loading saved cEntity data..\n";
	rapidxml::xml_document<> theDoc;
	rapidxml::xml_node<> *root_node;
	
	std::ifstream theFile(filename); // load the XML data into the buffer
	std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
		std::istreambuf_iterator<char>());
	bufData.push_back('\0');
	theDoc.parse<0>(&bufData[0]);
	root_node = theDoc.first_node("GameAssets");
	rapidxml::xml_node<> *category_node;

	// Load game assets...

	/* Load the Mipmap Textures */
	category_node = root_node->first_node("MipmapTextures");
	GraphicsEngine::cGraphicsEngine::instance()->loadMipmapTextures(category_node);
	

	/* Load the Cubemaps */
	category_node = root_node->first_node("Cubemaps");
	GraphicsEngine::cGraphicsEngine::instance()->loadCubemaps(category_node);

		/* Load the Textures */
	category_node = root_node->first_node("Textures");
	GraphicsEngine::cGraphicsEngine::instance()->loadTextures(category_node);

	category_node = root_node->first_node("FrameBufferObjects");
	GraphicsEngine::cGraphicsEngine::instance()->loadFramebufferObjects(category_node);


	category_node = root_node->first_node("Meshes");
		GraphicsEngine::cGraphicsEngine::instance()->loadMeshes(category_node);

		loadGameEntitiesFromXML("config20.xml");

	return 1;
}



// For each entity request entity xml node from state manager.. state manager will get each all of the entities component nodes 
void cEntityManager::saveGameToXML() {
	// CREATE new XML DOC
	// Append GameEntites node
	rapidxml::xml_document<> theDoc;
	rapidxml::xml_node<> *root_node;

	std::string xmlString;
	xmlString += "<GameEntities>";

	// Iterate over each game entity in vec entites..
	for each(cGameEntity* gameEntity in vec_gameEntities)
	{
		xmlString += g_pComponentEngine->getGameEntityXML(gameEntity->stateNodeID);
	}


	xmlString += "<GameEntities />\0";
	theDoc.parse<0>(&xmlString[0]);


	// Iterate over each game entity in vec entites..
	// APPEND GameEntity to GameEntities..
	//Call method in iStateManager to getXMLData by stateNodeID as parameter. append result node to GameEntity

	// Save file == done...

}


cEntityManager *cEntityManager::instance() {
	if (!s_cEntityManager)
		s_cEntityManager = new cEntityManager();
	return s_cEntityManager;
}
