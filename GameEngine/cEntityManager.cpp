#include "stdafx.h"
#include "cEntityManager.h"
#include "global.h"

// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cEntityManager_Impl : public cEntityManager {
	// Boilerplate
	friend class cEntityManager;
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
	/*Load the CubeMaps*/
	category_node = root_node->first_node("CubeMaps");
	for (rapidxml::xml_node<> *cTexture_node = category_node->first_node("CubeMap");
		cTexture_node; cTexture_node = cTexture_node->next_sibling()) {
		GraphicsEngine::cGraphicsEngine::instance()->loadCubeMap(cTexture_node);
	}

	return 1;
}

cEntityManager *cEntityManager::instance() {
	if (!s_cEntityManager)
		s_cEntityManager = new cEntityManager();
	return s_cEntityManager;
}
