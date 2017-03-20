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

		// TODO: LOAD GAME ( PUT IN NEW FUNCTION )
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

void cEntityManager::spawnObjectsAtSelectedTile()
{
	std::vector<sAABB> aabbs = g_pWorld->getSelectionAABBs();

	std::string meshName;
	switch (g_pWorld->getActiveSelectionMode())
	{
	case FLOOR:
		meshName = "FloorTile";
		break;
	case WALL_LEFT:
		meshName = "WallZ";
		break;
	case WALL_RIGHT:
		meshName = "WallZ";
		break;
	case WALL_FORWARD:
		meshName = "WallX";
		break;
	case WALL_BACKWARD:
		meshName = "WallX";
		break;
	default:
		break;
	}

	for each(sAABB aabb in aabbs)
	{

		cGameEntity* gameEntity = new cGameEntity();
		gameEntity->stateNodeID = g_pComponentEngine->registerNewEntity(); // Create new entity
		iState* tempStateGraphics = g_pComponentEngine->subcribeToState(gameEntity->stateNodeID); // Get a state for graphics component
		tempStateGraphics->setScale(1.0f);
		g_pGraphicsEngine->addObject(tempStateGraphics, meshName);

		iState* tempStatePhysics = g_pComponentEngine->subcribeToState(gameEntity->stateNodeID);

		g_pPhysicsEngine->addPhysicsObject(aabb.position, tempStatePhysics); // Rigid body contains the basic information about the object's scale, position, and, velocity, etc.. 
		iState* tempStateAI = g_pComponentEngine->subcribeToState(gameEntity->stateNodeID);
		//g_pAIEngine->addAIObject(aabb.position, tempStateAI);
		//vec_gameEntities.push_back(gameEntity); // Needs to be pushed to this vector if you want the objects to be included in the XML file at SaveGame()
												// TODO: Create another state for the AIEngine to use if object is monster etc..
	}
}

// Redo the load game entities process..
void loadGameFromXML(std::string filename) {
	// TODO: Delete all of the components.. 

	std::cout << "Loading saved game entities data..\n";
	rapidxml::xml_document<> theDoc;
	rapidxml::xml_node<> *root_node;

	std::ifstream theFile(filename); // load the XML data into the buffer
	std::vector<char> bufData((std::istreambuf_iterator<char>(theFile)),
		std::istreambuf_iterator<char>());
	bufData.push_back('\0');
	theDoc.parse<0>(&bufData[0]);
	root_node = theDoc.first_node("GameAssets");
	rapidxml::xml_node<> *category_node;

	category_node = root_node->first_node("GameEntities");
	for (rapidxml::xml_node<> *cGameEntity_node = category_node->first_node("GameEntity");
		cGameEntity_node; cGameEntity_node = cGameEntity_node->next_sibling("GameEntity")) {
		// Register new state node for the component..
		cGameEntity* gameEntity = new cGameEntity();

		gameEntity->stateNodeID = g_pComponentEngine->registerNewEntity();

		// These components are loaded and share a state! ...
		gameEntity->vec_pComponents = g_pComponentEngine->loadFromXML(cGameEntity_node, gameEntity->stateNodeID);
		//this->impl()->vec_pEntites.push_back(gameEntity);
	}


}

// For each entity request entity xml node from state manager.. state manager will get each all of the entities component nodes 
void saveGameToXML() {


}


cEntityManager *cEntityManager::instance() {
	if (!s_cEntityManager)
		s_cEntityManager = new cEntityManager();
	return s_cEntityManager;
}
