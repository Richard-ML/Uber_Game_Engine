#include "cEntityManager.h"
#include "cComponentManager.h"
#include "global.h"
#include <iostream>
#include <sstream>

// cGameObject Component Based Design Prototype
class cGameObject {
public:
  // Standard Components
  std::vector<std::shared_ptr<cComponent>> vec_m_components;
  cGameObject() {}
  ~cGameObject() {
    // Delete all components
  }
};
// The PIMPL idiom aka Compilation Firewall
// Purpose: Encapsulate private member variables. Reduces make-time,
// compile-time, and the Fragile Binary Interface Problem.
class cEntityManager_Impl : public cEntityManager {
  // Boilerplate
  friend class cEntityManager;

public:
  static std::vector<cGameObject *> vec_Entities;
  static rapidxml::xml_node<> *spiderNode;
};
inline const cEntityManager_Impl *cEntityManager::impl() const {
  return static_cast<const cEntityManager_Impl *>(this);
}
inline cEntityManager_Impl *cEntityManager::impl() {
  return static_cast<cEntityManager_Impl *>(this);
}
// Initialize vectors
std::vector<cGameObject *> cEntityManager_Impl::vec_Entities;
rapidxml::xml_node<> *cEntityManager_Impl::spiderNode;


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

	/* Load the Textures */
	root_node = theDoc.first_node("Textures");
	for (rapidxml::xml_node<> *cTexture_node = root_node->first_node("Texture");
		cTexture_node; cTexture_node = cTexture_node->next_sibling()) {
		g_pTextureManager->loadTextureMipmap(cTexture_node);
	}
	/* Load the Meshes */
	root_node = theDoc.first_node("Meshes");
	// iterate over each game asset and store its data in a cMeshEntry object
	for (rapidxml::xml_node<> *cMeshEntry_node = root_node->first_node("Mesh");
		cMeshEntry_node; cMeshEntry_node = cMeshEntry_node->next_sibling()) {
		// load the mesh buffers
		bool success = false;

		success = g_pMeshManager->loadMeshFileIntoGLBuffer(
			cMeshEntry_node->first_attribute("path")->value(),
			g_pMeshManager->m_MapMeshNameTocMeshEntry
			[cMeshEntry_node->first_attribute("name")->value()],
			std::stof(cMeshEntry_node->first_attribute("scale")->value()),
			g_pXML_Utils->attributeToBool(
				cMeshEntry_node->first_attribute("isStationary")));

		if (!success) {
			std::cout << "We couldn't load a mesh. Please fix the XML File.."
				<< std::endl;
			system("pause");
			return -1;
		}
	}

	/* Load the Entities */
	root_node = theDoc.first_node("GameEntities");
	cEntityManager_Impl::spiderNode = root_node;
	// iterate over each game asset and store its data in a cMeshEntry object
	for (rapidxml::xml_node<> *cEntity_node = root_node->first_node("Entity");
		cEntity_node; cEntity_node = cEntity_node->next_sibling()) {
		cGameObject *tempGameObject = new cGameObject();
		std::string entityName = cEntity_node->first_attribute("name")->value();
		cEntityManager_Impl::vec_Entities.push_back(tempGameObject);

		std::vector<std::shared_ptr<cComponent>> vec_TempComponents =
			cComponentManager::loadFromXML(cEntity_node);
		for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
			vec_TempComponents.begin();
			iter != vec_TempComponents.end(); iter++)
			tempGameObject->vec_m_components.push_back((*iter));
	}

  /*Load the CubeMaps*/
  root_node = theDoc.first_node("CubeMaps");
  for (rapidxml::xml_node<> *cTexture_node = root_node->first_node("CubeMap");
       cTexture_node; cTexture_node = cTexture_node->next_sibling()) {
    // glBindTexture(GL_TEXTURE_CUBE_MAP,
    // g_pTextureManager->loadCubeMap(cTexture_node));
    g_pTextureManager->loadCubeMap(cTexture_node);
  }

  /* Load the Area Info */
  root_node = theDoc.first_node("Cameras");

  for (rapidxml::xml_node<> *cCamera_node = root_node->first_node("Camera");
       cCamera_node; cCamera_node = cCamera_node->next_sibling()) {

    gCamera->loadFromXML(cCamera_node);
  }

  return 1;
}

cEntityManager *cEntityManager::instance() {
  if (!s_cEntityManager)
    s_cEntityManager = new cEntityManager();
  return s_cEntityManager;
}

void cEntityManager::drawEntites() const {
  for (std::vector<cGameObject *>::iterator iter =
           cEntityManager_Impl::vec_Entities.begin();
       iter != cEntityManager_Impl::vec_Entities.end(); iter++) {
    for (std::vector<std::shared_ptr<cComponent>>::iterator iter2 =
             (*iter)->vec_m_components.begin();
         iter2 != (*iter)->vec_m_components.end(); iter2++) {
      (*iter2)->update();
      //(*iter)->draw();
    }
  }
}

void cEntityManager::createSpiderAtPos(float x, float y, float z) {


  cGameObject *tempGameObject = new cGameObject();
  cEntityManager_Impl::vec_Entities.push_back(tempGameObject);

  rapidxml::xml_document<> theDoc;

  for (rapidxml::xml_node<> *cEntity_node =
           cEntityManager_Impl::spiderNode->first_node("Entity");
       cEntity_node; cEntity_node = cEntity_node->next_sibling()) {
    // cGameObject* tempGameObject = new cGameObject();
    std::string entityName = cEntity_node->first_attribute("name")->value();
    cEntityManager_Impl::vec_Entities.push_back(tempGameObject);
    if (entityName == "Spider") {

      rapidxml::xml_attribute<> *att1 =
          cEntity_node->first_node()->first_attribute("offsetX");
      std::string att1Value = std::to_string(x);
      std::string att1Name = "offsetX";
      // allocate memory assigned to document for attribute value
      char *ratt1Value = theDoc.allocate_string(att1Value.c_str());
      // search for the attribute at the given node
      rapidxml::xml_attribute<> *attr1 =
          cEntity_node->first_node()->first_attribute(att1Name.c_str());
      if (attr1 != 0) { // attribute already exists
                        // only change value of existing attribute
        attr1->value(ratt1Value);
      } else { // attribute does not exist
               // allocate memory assigned to document for attribute name
        char *rAttName = theDoc.allocate_string(att1Name.c_str());
        // create new a new attribute with the given name and value
        attr1 = theDoc.allocate_attribute(rAttName, ratt1Value);
        // append attribute to node
        cEntity_node->first_node()->append_attribute(attr1);
      }

      rapidxml::xml_attribute<> *att2 =
          cEntity_node->first_node()->first_attribute("offsetY");
      std::string att2Value = std::to_string(y);
      std::string att2Name = "offsetY";
      // allocate memory assigned to document for attribute value
      char *ratt2Value = theDoc.allocate_string(att2Value.c_str());
      // search for the attribute at the given node
      rapidxml::xml_attribute<> *attr2 =
          cEntity_node->first_node()->first_attribute(att2Name.c_str());
      if (attr2 != 0) { // attribute already exists
                        // only change value of existing attribute
        attr2->value(ratt2Value);
      } else { // attribute does not exist
               // allocate memory assigned to document for attribute name
        char *rAttName = theDoc.allocate_string(att2Name.c_str());
        // create new a new attribute with the given name and value
        attr2 = theDoc.allocate_attribute(rAttName, ratt2Value);
        // append attribute to node
        cEntity_node->first_node()->append_attribute(attr2);
      }

      rapidxml::xml_attribute<> *att3 =
          cEntity_node->first_node()->first_attribute("offsetZ");
      std::string att3Value = std::to_string(z);
      std::string att3Name = "offsetZ";
      // allocate memory assigned to document for attribute value
      char *ratt3Value = theDoc.allocate_string(att3Value.c_str());
      // search for the attribute at the given node
      rapidxml::xml_attribute<> *attr3 =
          cEntity_node->first_node()->first_attribute(att3Name.c_str());
      if (attr3 != 0) { // attribute already exists
                        // only change value of existing attribute
        attr3->value(ratt3Value);
      } else { // attribute does not exist
               // allocate memory assigned to document for attribute name
        char *rAttName = theDoc.allocate_string(att3Name.c_str());
        // create new a new attribute with the given name and value
        attr3 = theDoc.allocate_attribute(rAttName, ratt3Value);
        // append attribute to node
        cEntity_node->first_node()->append_attribute(attr3);
      }

      std::vector<std::shared_ptr<cComponent>> vec_TempComponents =
          cComponentManager::loadFromXML(cEntity_node);

      for (std::vector<std::shared_ptr<cComponent>>::iterator iter =
               vec_TempComponents.begin();
           iter != vec_TempComponents.end(); iter++)
        tempGameObject->vec_m_components.push_back((*iter));
    }
  }
  // std::vector<std::shared_ptr<cComponent>>  vec_TempComponents =
  // cComponentManager::loadFromXML(cEntityManager_Impl::spiderNode->first_node());
  // for (std::vector<std::shared_ptr<cComponent>> ::iterator iter =
  // vec_TempComponents.begin(); iter != vec_TempComponents.end(); iter++)
  //	tempGameObject->vec_m_components.push_back((*iter));
}
