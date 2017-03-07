#include "stdafx.h"
#include "cEntityManager.h"
#include "global.h"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>
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




// Hardcoded method to generate random builds and place 500 Dalek randomly in the scene
void generateObjects() {

	bool tempBuildingTiles[28][28];
	for(int nc = 0; nc < 27; nc++)
		for (int nc2 = 0; nc2 < 27; nc2++)
			tempBuildingTiles[nc][nc2] = false;
	
	// Simple loop used to create 300 buildings
	for (int nc = 0; nc < 300; nc++)
	{
		TRY_AGAIN:
		int blockOffsetX = rand() % 5; // rand 0 to 4
		int blockOffsetZ = rand() % 5; // rand 0 to 4
		int tileX = (rand() % 4) + 1; // rand 1 to 4
		int tileZ = (rand() % 4) + 1; // rand 1 to 4
		// Could result in an endless loop though this is so unlikely I am not worried about it. xD
		// MIGHT come back and create a vector of available locations and pop them randomly. 
		// 4x4 tiles in city block && 5x5 city blocks
		if (tempBuildingTiles[tileX + 5 * blockOffsetX][tileZ + 5 * blockOffsetZ] == 1)
			goto TRY_AGAIN;
		

		tempBuildingTiles[tileX + 5 * blockOffsetX][tileZ + 5 * blockOffsetZ] = 1;

		//cGameEntity* gameEntity = new cGameEntity();

		iState* tempState = g_pComponentEngine->subcribeToState(g_pComponentEngine->registerNewEntity());
		tempState->setPosition(glm::vec3((float)((100 * tileX) + (500 * blockOffsetX)),0.0f, (float)((100 * tileZ) + (500 * blockOffsetZ))));
		tempState->setScale(1.0f); // Forgot to do this. Wasted lots of time! :S
		std::string meshName = "Building_" + std::to_string((rand() % 3) + 1);

		g_pGraphicsEngine->addObject(tempState, meshName);


		//vec_gameEntities.push_back(gameEntity); // This data will not be saved to XML
	}


	std::vector<iState*> vec_states;
	for (int nc = 0; nc < 1000; nc++)
	{
		std::string stateNodeID = g_pComponentEngine->registerNewEntity();
		iState* tempState = g_pComponentEngine->subcribeToState(stateNodeID);
		tempState->setScale(1.0f);
		g_pGraphicsEngine->addObject(tempState, "Dalek_2");

		// Create another state for the AIEngine to use..
		vec_states.push_back(g_pComponentEngine->subcribeToState(stateNodeID));
	}
	g_pAIEngine->generateAI(vec_states);
}










// Loads game entities..
void cEntityManager::loadGameEntitiesFromXML(int difficulty) {
	std::string filename;
	// TODO: Delete all of the components properly.. 
	if (difficulty == 0)
		filename = "config20.xml";
	//else
		//if (difficulty == 1)
			//filename = "config30.xml";
		//else
			//if (difficulty == 2)
				//filename = "config40.xml";
			else return;
	g_pGraphicsEngine->clearGameObjects();
	g_pComponentEngine->clearStateInfo();
	vec_gameEntities.clear();


	std::cout << "Loading saved game entities data..\nDifficulty == " << difficulty << std::endl;
	rapidxml::xml_document<> theDoc;
	rapidxml::xml_node<> *root_node;

	std::ifstream theFile(filename); // load the XML data into the buffer
	std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
		std::istreambuf_iterator<char>());
	bufData.push_back('\0');
	theDoc.parse<0>(&bufData[0]);

	bool tardis = true;
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
		if (tardis == true) {
			iState* tempState = g_pComponentEngine->subcribeToState(gameEntity->stateNodeID);
			//g_pAIEngine->tardis = tempState; // TODO: Pointer to player? Remove this?
			tardis = false;
		}
	}


	// TODO: REMOVE THIS
	generateObjects();
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

	//category_node = root_node->first_node("FrameBufferObjects");
	//GraphicsEngine::cGraphicsEngine::instance()->loadFramebufferObjects(category_node);


	category_node = root_node->first_node("Meshes");
		GraphicsEngine::cGraphicsEngine::instance()->loadMeshes(category_node);




	return 1;
}



// For each entity request entity xml node from state manager.. state manager will get each all of the entities component nodes 
void cEntityManager::saveGameToXML(int difficulty) {

	std::string filename;
	// TODO: Delete all of the components properly.. 
	if (difficulty == 0)
		filename = "config20.xml";
	else
		return;
		//if (difficulty == 1)
		//	filename = "config30.xml";
		//else
		//	if (difficulty == 2)
		//		filename = "config40.xml";
		//	else return;


	std::string xmlString;
	xmlString += "<GameEntities>";

	// Iterate over each game entity in vec entities..
	for each(cGameEntity* gameEntity in vec_gameEntities)
		xmlString.append( g_pComponentEngine->getGameEntityXML(gameEntity->stateNodeID));
	xmlString += "</GameEntities>";

	std::cout << xmlString;

	// Save to file
	std::ofstream file_stored;
	file_stored.open(filename, std::ofstream::out | std::ofstream::trunc);
	file_stored << xmlString;
	file_stored.close();

	// Save file == done...
}


cEntityManager *cEntityManager::instance() {
	if (!s_cEntityManager)
		s_cEntityManager = new cEntityManager();
	return s_cEntityManager;
}
