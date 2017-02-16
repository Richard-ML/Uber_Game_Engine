#include "stdafx.h"
#include "cEntityManager.h"
#include "global.h"
class cGameEntity {
public:
	std::vector<std::shared_ptr<cComponent>>
		vec_pComponents; // Components that belong to this entity..

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

	category_node = root_node->first_node("Meshes");
		GraphicsEngine::cGraphicsEngine::instance()->loadMeshes(category_node);


	category_node = root_node->first_node("GameEntities");
	for (rapidxml::xml_node<> *cGameEntity_node = category_node->first_node("GameEntity");
		cGameEntity_node; cGameEntity_node = cGameEntity_node->next_sibling("GameEntity")) {
		// Register new state node for the component..
		cGameEntity* gameEntity = new cGameEntity();
		
		std::string stateNodeID = g_pComponentEngine->registerNewEntity();

		
		// These components are loaded and share a state! ...
			gameEntity->vec_pComponents = g_pComponentEngine->loadFromXML(cGameEntity_node, stateNodeID);
		//this->impl()->vec_pEntites.push_back(gameEntity);
		
	}


	return 1;
}

cEntityManager *cEntityManager::instance() {
	if (!s_cEntityManager)
		s_cEntityManager = new cEntityManager();
	return s_cEntityManager;
}
